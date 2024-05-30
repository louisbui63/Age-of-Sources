//! @file errors.hh
#pragma once

//! a general error return type
typedef enum {
  //! self explanatory
  SUCCESS = 0,
  //! function call couldn't succeed because of insufficiant memory in the heap
  OUT_OF_MEMORY,
  //! function was called with invalid parameters leading to access out of the
  //! allowed range
  INDEX_OUT_OF_RANGE,
  //! asset doesn't exist, i.e. it is not loaded in the asset manager and there
  //! is no file corresponding to the requested asset
  ASSET_NOT_FOUND,
  //! self explanatory
  ASSERTION_FAILED,
  //! self explanatory
  COULD_NOT_MIX_SOUND,
  //! the length of a `Bitflag` is to short to accomodate one more component
  OUT_OF_COMPONENTS
} Error;
