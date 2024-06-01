//! @file bitflag.hpp
#pragma once

#include <cstdint>

//! expands to the `r`th least significant bit of `b`
#define bitflag_get(b, r) (((b) >> (r)) & 1)
//! expands to the value of `b` with its `r`th least significant bit set to `v`
#define bitflag_set(b, r, v) ((v) ? (1 << (r)) | (b) : (~(1 << (r))) & (b))

typedef uint64_t Bitflag;
