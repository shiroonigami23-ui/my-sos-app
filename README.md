My Offline SOS Messenger
A personal project to build and learn about creating an offline, mesh-network-based emergency SOS device using ESP32 and a Progressive Web App (PWA) for control.
Project Overview
This project is an exploration into creating a reliable, internet-independent communication system for emergency situations. The core idea is to use ESP32 microcontrollers to form a mesh network with ESP-NOW, allowing SOS signals to be broadcast and relayed between devices without any reliance on Wi-Fi routers or cellular service.
A simple Progressive Web App (PWA) connects to the ESP32 device via the Web Serial API, acting as a control panel to trigger alerts and monitor incoming messages from the network.
Core Technologies:
Hardware: ESP32 Development Board
Communication Protocol: ESP-NOW (for offline mesh networking)
Control Interface: Progressive Web App (PWA) using HTML, CSS, and JavaScript
Device Communication: Web Serial API (for browser-to-hardware connection)
Firmware: C++/Arduino
Project Goals
Learn: Gain hands-on experience with embedded systems, mesh networking, and hardware-browser communication.
Build: Create a functional proof-of-concept for an offline SOS device.
Test: Document the process, test the range and reliability, and identify areas for improvement.
Share: Maintain a clean, well-documented codebase on GitHub.
Getting Started
Prerequisites
Hardware:
2 x ESP32 Development Boards (you need at least two to test the mesh)
2 x Push Buttons
USB Cables
Software:
Arduino IDE or PlatformIO for flashing the firmware.
A modern web browser that supports the Web Serial API (e.g., Google Chrome, Microsoft Edge).
1. Hardware Setup
Connect a push button to GPIO 0 and GND on each ESP32 board. The firmware uses an internal pull-up resistor.
2. Firmware Setup
Clone this repository to your local machine.
Open the firmware/esp32_sos_firmware.ino file in the Arduino IDE.
Install the ESP32 board definitions if you haven't already.
Select your ESP32 board and COM port under the Tools menu.
Upload the firmware to both of your ESP32 devices.
3. Web App Usage
Open the pwa/index.html file in your web browser.
Connect one of the ESP32s to your computer via USB.
Click the "Connect to Device" button on the web page and select the correct serial port.
The status should change to "Connected". You can now send an SOS signal from the web app or by pressing the physical button on the connected ESP32.
Open the Serial Monitor in the Arduino IDE for the other ESP32 to see the broadcasted message received over the mesh network.
Development Roadmap
This project will be developed iteratively. Key milestones include:
[x] Phase 1: Basic Proof of Concept
[x] ESP-NOW broadcast firmware
[x] Web Serial PWA for control
[ ] Phase 2: Enhancing the Signal
[ ] Integrate a GPS module (e.g., NEO-6M) to include coordinates in the SOS message.
[ ] Implement a message relay system (meshing) where nodes re-broadcast messages to extend range.
[ ] Phase 3: Low Power & Reliability
[ ] Add deep-sleep modes to conserve battery.
[ ] Implement message acknowledgments to confirm delivery.
[ ] Phase 4: Alternative Comms
[ ] Explore LoRa (SX127x) as a longer-range alternative to ESP-NOW.
Contributing
This is a personal learning project, but suggestions and contributions are welcome! Please feel free to open an issue to discuss ideas or submit a pull request.
License
This project is licensed under the MIT License. See the LICENSE file for details.
