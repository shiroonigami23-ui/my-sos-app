#ifndef SOS_CRYPTO_H
#define SOS_CRYPTO_H

#include <stddef.h>

void hex_digest(const unsigned char *input, size_t len, char *out, size_t out_len);
void xor_obfuscate(unsigned char *buf, size_t len, unsigned char key);

#endif
