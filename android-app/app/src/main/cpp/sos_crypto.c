#include "sos_crypto.h"

#include <stdio.h>

static const char HEX[] = "0123456789ABCDEF";

void hex_digest(const unsigned char *input, size_t len, char *out, size_t out_len) {
    size_t need = (len * 2) + 1;
    size_t i = 0;
    if (out_len < need) {
        if (out_len > 0) out[0] = '\0';
        return;
    }
    for (i = 0; i < len; i++) {
        unsigned char b = input[i];
        out[i * 2] = HEX[(b >> 4) & 0x0F];
        out[(i * 2) + 1] = HEX[b & 0x0F];
    }
    out[len * 2] = '\0';
}

void xor_obfuscate(unsigned char *buf, size_t len, unsigned char key) {
    size_t i;
    for (i = 0; i < len; i++) {
        buf[i] ^= key;
    }
}
