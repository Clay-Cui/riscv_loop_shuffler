#ifndef NTT_H
#define NTT_H

#include <stdint.h>
#include "params.h"

#define RANDOM_BUFFER_SIZE_NEW 8
#define RANDOM_BUFFER_SIZE 4

#define NO_PROTECTION 0
#define COARSE_SHUFFLING 1
#define GROUP_COARSE_SHUFFLING 2
#define GROUP_SHUFFLING 4
#define FINE_SHUFFLING 8
#define COARSE_MULTIPLICATIVE_MASKING 16
#define FINE_MULTIPLICATIVE_MASKING 32
#define GENERIC_MULTIPLICATIVE_MASKING 64
#define NO_MASKS_IN_STAGE 2

#define NTT_PROTECTION_MODE GROUP_COARSE_SHUFFLING

//extern const int16_t zetas_poly[64];
// extern const int16_t zetas[128];
//extern const int16_t zetas_inv[128];

void ntt(int16_t poly[256]);
void ntt_h(int16_t poly[256]);
void ntt_h2(int16_t poly[256]);
void ntt_h3(int16_t poly[256]);
//void invntt(int16_t poly[256]);
//void basemul(int16_t r[2],
//             const int16_t a[2],
//             const int16_t b[2],
//             int16_t zeta);

#endif
