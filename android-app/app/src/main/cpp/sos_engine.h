#ifndef SOS_ENGINE_H
#define SOS_ENGINE_H

#include <stddef.h>

void build_sos_payload(
    const char *name,
    double latitude,
    double longitude,
    double distance_km,
    char *out,
    size_t out_len
);

int severity_score(const char *payload, double distance_km);
unsigned int crc32_text(const char *text);

#endif
