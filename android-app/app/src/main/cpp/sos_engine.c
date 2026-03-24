#include "sos_engine.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

static const unsigned int CRC32_POLY = 0xEDB88320u;

static void utc_now(char *buf, size_t buf_len) {
    time_t now = time(NULL);
    struct tm *utc = gmtime(&now);
    if (utc != NULL) {
        strftime(buf, buf_len, "%Y-%m-%dT%H:%M:%SZ", utc);
    } else if (buf_len > 0) {
        strncpy(buf, "unknown-time", buf_len - 1);
        buf[buf_len - 1] = '\0';
    }
}

unsigned int crc32_text(const char *text) {
    unsigned int crc = 0xFFFFFFFFu;
    size_t i = 0;
    if (text == NULL) {
        return 0;
    }
    while (text[i] != '\0') {
        unsigned int byte = (unsigned int)(unsigned char)text[i];
        crc ^= byte;
        for (int bit = 0; bit < 8; bit++) {
            unsigned int mask = (unsigned int)-(int)(crc & 1u);
            crc = (crc >> 1u) ^ (CRC32_POLY & mask);
        }
        i++;
    }
    return ~crc;
}

int severity_score(const char *payload, double distance_km) {
    int score = 35;
    if (payload != NULL) {
        if (strstr(payload, "SOS") != NULL) score += 25;
        if (strstr(payload, "Lat:") != NULL && strstr(payload, "Lon:") != NULL) score += 15;
        if (strstr(payload, "CRITICAL") != NULL) score += 15;
    }
    if (distance_km > 5.0) score += 5;
    if (distance_km > 15.0) score += 5;
    if (score > 100) score = 100;
    return score;
}

void build_sos_payload(
    const char *name,
    double latitude,
    double longitude,
    double distance_km,
    char *out,
    size_t out_len
) {
    char ts[32];
    utc_now(ts, sizeof(ts));
    const char *safe_name = (name == NULL || name[0] == '\0') ? "Unknown User" : name;
    const char *risk = distance_km > 15.0 ? "CRITICAL" : (distance_km > 5.0 ? "HIGH" : "ELEVATED");

    snprintf(
        out,
        out_len,
        "SOS ALERT\nUser: %s\nTime(UTC): %s\nLat: %.6f\nLon: %.6f\nDistanceFromBaseKm: %.2f\nRisk: %s\nStatus: Needs immediate assistance.",
        safe_name,
        ts,
        latitude,
        longitude,
        distance_km,
        risk
    );
}
