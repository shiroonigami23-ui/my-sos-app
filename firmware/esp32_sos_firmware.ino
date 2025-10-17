/*
 * EmergencySOS - ESP32 Firmware
 *
 * This firmware creates a simple mesh network using ESP-NOW to send and receive
 * emergency SOS signals. It's designed to work offline, without any internet connection.
 *
 * Hardware Required:
 * - ESP32 Development Board
 * - A push button connected to GPIO 0 (or any other pin, just update the code)
 *
 * How it works:
 * 1. The ESP32 initializes ESP-NOW, a fast, connectionless communication protocol.
 * 2. It listens for incoming messages from other devices in the mesh.
 * 3. When the physical button on the device is pressed, it broadcasts an SOS
 * message containing its unique MAC address to all other devices in range.
 * 4. Any device that receives a message prints it to the Serial Monitor.
 * 5. This firmware is the core of the system described in your production pack.
 * You can later integrate GPS, fall detection (MPU6050), and cellular (SIM800L)
 * modules by adding the relevant code in the indicated sections.
*/

#include <esp_now.h>
#include <WiFi.h>

// Pin for the SOS button. GPIO 0 is convenient as it's often the "BOOT" button.
const int SOS_BUTTON_PIN = 0;

// Define a structure for the data to be sent over ESP-NOW
typedef struct struct_message {
  char device_id[18]; // To store the MAC address
  char message[100];
  int message_type; // 1 for SOS, 2 for acknowledgement, etc.
} struct_message;

// Create a message structure
struct_message myData;

// Broadcast address (sends to all devices in the network)
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Variable to store peer info
esp_now_peer_info_t peerInfo;

// Callback function for when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback function for when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println("-----------------");
  Serial.print("Message received from: ");
  Serial.println(myData.device_id);
  Serial.print("Message: ");
  Serial.println(myData.message);
  Serial.println("-----------------");

  // You could add logic here to re-broadcast the message
  // to extend the range of the mesh network.
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  // Register the receive callback
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Configure the SOS button pin
  pinMode(SOS_BUTTON_PIN, INPUT_PULLUP);

  Serial.println("=====================================");
  Serial.println("ESP32 SOS Device Initialized");
  Serial.print("My MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.println("Press the button on Pin 0 to send an SOS broadcast.");
  Serial.println("=====================================");
}

void loop() {
  // Check if the SOS button is pressed
  // We use digitalRead and check for LOW because INPUT_PULLUP means the
  // pin is HIGH by default and goes LOW when pressed.
  if (digitalRead(SOS_BUTTON_PIN) == LOW) {
    Serial.println("SOS Button Pressed! Sending broadcast...");
    sendSOS();
    // Wait for a moment to avoid sending multiple messages for a single press
    delay(1000);
  }

  // You can listen for commands from the Web App here
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "SOS") {
        Serial.println("SOS command received from Web App! Sending broadcast...");
        sendSOS();
    }
  }
}

void sendSOS() {
    // 1. --- INTEGRATE GPS ---
    // Here you would add code to read from a GPS module like a NEO-6M.
    // For now, we'll use a placeholder message.
    // Example: String gps_coords = getGPSCoordinates();

    // Set the data to be sent
    String macAddress = WiFi.macAddress();
    macAddress.toCharArray(myData.device_id, macAddress.length() + 1);

    // Construct the message
    String sosMessage = "HELP! Emergency at my location. "; // + gps_coords;
    sosMessage.toCharArray(myData.message, sosMessage.length() + 1);
    myData.message_type = 1; // SOS Type

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
