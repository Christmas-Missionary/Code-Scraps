// Repo: https://github.com/BareRose/ranxoshi256
// This module was shrunk, split, and renamed to serve the purposes of tic-tac-toe, still under CC-0 1.0

#include "rng.h"
#include <assert.h>
#include <stddef.h>

void ranxoshi_seed(ranxoshi_t state[restrict static 1], const unsigned char seed[static RANXOSHI_SEED_SIZE]) {
  assert(state != NULL && "RNG state is null!");
  assert(seed != NULL && "Seed points to null!");
  // enforce no overlap of pointers somehow to uphold restrict keyword
  for (signed char i = 0; i < 4; i++) {
    state->bytes[i] = ((uint64_t)seed[i * 8L] << 56U) | ((uint64_t)seed[i * 8 + 1] << 48U) |
                      ((uint64_t)seed[i * 8 + 2] << 40U) | ((uint64_t)seed[i * 8 + 3] << 32U) |
                      ((uint64_t)seed[i * 8 + 4] << 24U) | ((uint64_t)seed[i * 8 + 5] << 16U) |
                      ((uint64_t)seed[i * 8 + 6] << 8U) | ((uint64_t)seed[i * 8 + 7]);
  }
}

static inline uint64_t ranxoshi_rotate(const uint64_t bytes, const uint8_t shifter) {
  assert((shifter == 7 || shifter == 45) && "Someone has been tampering with ranxoshi_rand!");
  return (bytes << shifter) | (bytes >> (64U - shifter));
}

uint64_t ranxoshi_rand(ranxoshi_t state[static 1]) {
  assert(state != NULL && "RNG state is null!");
  uint64_t res = ranxoshi_rotate(state->bytes[1] * 5, 7) * 9;
  uint64_t second_byte_shifted = state->bytes[1] << 17U;
  state->bytes[2] ^= state->bytes[0];
  state->bytes[3] ^= state->bytes[1];
  state->bytes[1] ^= state->bytes[2];
  state->bytes[0] ^= state->bytes[3];
  state->bytes[2] ^= second_byte_shifted;
  state->bytes[3] = ranxoshi_rotate(state->bytes[3], 45);
  return res;
}
