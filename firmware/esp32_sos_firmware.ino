/*
 * =================================================================
 * My Offline SOS Messenger - FULL FEATURE FIRMWARE (Phase 2)
 * =================================================================
 *
 * This firmware includes:
 * 1.  GPS Integration: Reads location data from a GPS module.
 * 2.  Mesh Relay Logic: Re-broadcasts received messages to extend network range.
 * 3.  JSON Data Format: Sends data as a structured JSON string for easy parsing.
 *
 * HARDWARE REQUIRED:
 * - ESP32 Development Board
 * - Push Button on GPIO 0
 * - GPS Module (e.g., NEO-6M, GY-NEO6MV2)
 *
 * WIRING FOR GPS MODULE:
 * GPS VCC   ->   ESP32 3.3V
 * GPS GND   ->   ESP32 GND
 * GPS TX    ->   ESP32 GPIO 16 (RX2)
 * GPS RX    ->   ESP32 GPIO 17 (TX2)
 *
 * 
 *
 * LIBRARIES NEEDED:
 * - TinyGPS++: Install via Arduino IDE's Library Manager.
 * - ArduinoJson: Install via Arduino IDE's Library Manager.
 *
*/

#include <esp_now.h>
#include <WiFi.h>
#include <TinyGPS++.h>
#include <ArduinoJson.h>

// --- Pin Definitions ---
const int SOS_BUTTON_PIN = 0;
#define GPS_RX_PIN 16 // ESP32 RX2
#define GPS_TX_PIN 17 // ESP32 TX2

// --- GPS Setup ---
TinyGPSPlus gps;
HardwareSerial gpsSerial(2); // Use hardware serial 2 for GPS

// --- ESP-NOW Setup ---
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// --- Mesh Relay Logic ---
const int MAX_SEEN_MESSAGES = 10;
unsigned long seenMessageIds[MAX_SEEN_MESSAGES];
int nextMessageIdIndex = 0;

// Data structure for messages
typedef struct struct_message {
    unsigned long id;
    char sender[18];
    char message[100];
    double latitude;
    double longitude;
} struct_message;

struct_message myData;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Last Packet Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Check if we've already seen and relayed this message
bool hasSeenMessage(unsigned long id) {
    for (int i = 0; i < MAX_SEEN_MESSAGES; i++) {
        if (seenMessageIds[i] == id) {
            return true;
        }
    }
    return false;
}

// Add a message ID to our list of seen messages
void addSeenMessage(unsigned long id) {
    seenMessageIds[nextMessageIdIndex] = id;
    nextMessageIdIndex = (nextMessageIdIndex + 1) % MAX_SEEN_MESSAGES;
}

// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    memcpy(&myData, incomingData, sizeof(myData));
    
    // Check if this is a new message we haven't relayed yet
    if (!hasSeenMessage(myData.id)) {
        addSeenMessage(myData.id);

        // Print to local serial for the connected Web App
        JsonDocument doc;
        doc["id"] = myData.id;
        doc["sender"] = myData.sender;
        doc["message"] = myData.message;
        doc["lat"] = myData.latitude;
        doc["lon"] = myData.longitude;
        serializeJson(doc, Serial);
        Serial.println();

        // Relay the message to the rest of the mesh
        Serial.printf("Relaying message %lu from %s...\n", myData.id, myData.sender);
        esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    } else {
        Serial.printf("Ignoring duplicate message %lu.\n", myData.id);
    }
}

void setup() {
    Serial.begin(115200);
    gpsSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

    pinMode(SOS_BUTTON_PIN, INPUT_PULLUP);

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    Serial.println("=========================================");
    Serial.println("SOS Device Initialized (Full-Feature)");
    Serial.print("My MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.println("GPS & Mesh Relay Enabled.");
    Serial.println("=========================================");
}

// Function to get the latest GPS data
void updateGPS() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }
}

void sendSOS() {
    Serial.println("SOS triggered! Reading GPS...");
    updateGPS(); // Get the very latest GPS fix

    // Prepare data
    myData.id = esp_random(); // Generate a unique ID for this message
    String macAddress = WiFi.macAddress();
    macAddress.toCharArray(myData.sender, macAddress.length() + 1);
    strcpy(myData.message, "HELP! Emergency Signal.");

    if (gps.location.isValid()) {
        myData.latitude = gps.location.lat();
        myData.longitude = gps.location.lng();
        Serial.printf("Location: %f, %f\n", myData.latitude, myData.longitude);
    } else {
        myData.latitude = 0.0;
        myData.longitude = 0.0;
        Serial.println("Location: Not available.");
    }

    // Add this message to our own 'seen' list so we don't relay our own message
    addSeenMessage(myData.id);

    // Broadcast the message
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}


void loop() {
    // Check for button press
    if (digitalRead(SOS_BUTTON_PIN) == LOW) {
        sendSOS();
        delay(2000); // Debounce and prevent spamming
    }

    // Check for command from Web App
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        if (command == "SOS") {
            sendSOS();
        }
    }
    
    // Continuously update GPS data in the background
    updateGPS();
}    myData.message_type = 1; // SOS Type

    // 2. --- INTEGRATE CELLULAR (SIM800L) ---
    // This is where you would add code to send an SMS as a fallback.
    // Example: sendSmsViaSIM800L("+1234567890", sosMessage);


    // 3. --- SEND VIA ESP-NOW MESH ---
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    if (result == ESP_OK) {
        Serial.println("SOS broadcast sent successfully!");
    } else {
        Serial.println("Error sending SOS broadcast.");
    }
}
