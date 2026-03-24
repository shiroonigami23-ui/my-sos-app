# My SOS App

[![Android Release](https://github.com/shiroonigami23-ui/my-sos-app/actions/workflows/android-release.yml/badge.svg)](https://github.com/shiroonigami23-ui/my-sos-app/actions/workflows/android-release.yml)

Offline-first SOS toolkit with:
- ESP32 firmware (`firmware/`)
- PWA controller (`pwa/`)
- Android app (`android-app/`) with **C-native SOS core** via JNI

## What Was Improved
- Added a complete Android app project.
- Implemented core SOS payload generation/scoring in C (`sos_core.c`).
- Connected Kotlin UI to C native logic through JNI.
- Added GitHub Actions release pipeline that builds and uploads APK on tag push.

## Android App Features
- Builds structured SOS messages with timestamp and coordinates.
- Calculates a severity score (0-100) in native C code.
- Opens SMS app with prefilled emergency payload.

## Build Android APK Locally
```bash
cd android-app
./gradlew assembleRelease
```

Release output:
`android-app/app/build/outputs/apk/release/app-release.apk`

## Existing Components
- `firmware/esp32_sos_firmware.ino` for ESP-NOW mesh signaling.
- `pwa/index.html` for Web Serial browser control.
