#pragma once

#include <stdint.h>

#define bitflag_get(b, r) (((b) >> (r)) & 1)
#define bitflag_set(b, r, v) ((v) ? (1 << (r)) | (b) : (~(1 << (r))) & (b))

typedef uint64_t Bitflag;
