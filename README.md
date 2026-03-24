# My SOS App

[![Android Release](https://github.com/shiroonigami23-ui/my-sos-app/actions/workflows/android-release.yml/badge.svg)](https://github.com/shiroonigami23-ui/my-sos-app/actions/workflows/android-release.yml)

Offline-first SOS toolkit with:
- ESP32 firmware (`firmware/`)
- PWA controller (`pwa/`)
- Android app (`android-app/`) with **C-native SOS engine** via JNI

## What Was Improved
- Added a complete Android app project.
- Implemented multi-file C engine:
  - `jni_bridge.c`
  - `sos_engine.c`
  - `geo_math.c`
- Connected Kotlin UI to C native logic through JNI for payload generation, severity scoring, CRC digesting, and geo-distance estimation.
- Added GitHub Actions release pipeline that builds and uploads APK on tag push.

## Android App Features
- Builds structured SOS messages with timestamp and coordinates.
- Calculates severity score (0-100), payload CRC digest, and distance-from-base in native C code.
- Opens SMS app with prefilled emergency payload.

## Build Android APK Locally
```bash
cd android-app
./gradlew assembleRelease
```

Release output:
`android-app/app/build/outputs/apk/release/app-release.apk`

## Signed Release APK (GitHub Actions)
To produce a signed APK, set these repo secrets:
- `ANDROID_KEYSTORE_BASE64`
- `ANDROID_KEYSTORE_PASSWORD`
- `ANDROID_KEY_ALIAS`
- `ANDROID_KEY_PASSWORD`

If secrets are missing, workflow still builds and uploads unsigned release APK.

## Existing Components
- `firmware/esp32_sos_firmware.ino` for ESP-NOW mesh signaling.
- `pwa/index.html` for Web Serial browser control.
