/*
ranxoshi256.h - Portable, single-file, PRNG library implementing the xoshiro256** algorithm

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring
rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software.
If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

// Repo: https://github.com/BareRose/ranxoshi256
// This module was shrunk, split, and renamed to serve the purposes of tic-tac-toe, still under CC-0 1.0

#ifndef RANXOSHI_HEADER_
#define RANXOSHI_HEADER_

#define RANXOSHI_SEED_SIZE 32

#include <stdint.h>

typedef struct {
  uint64_t bytes[4]; // PRNG state
} ranxoshi_t;

// Pastes the given seed of 32 bytes into the generator's state in an endian-proof way
// This allows for consistent results across machines with differing architectures
void ranxoshi_seed(ranxoshi_t state[static 1], const unsigned char seed[static RANXOSHI_SEED_SIZE]);

// Returns a random uint64 (raw output of the generator)
uint64_t ranxoshi_rand(ranxoshi_t state[static 1]);

#endif // RANXOSHI_HEADER_
