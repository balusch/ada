/**
 * @file unicode-inl.h
 * @brief Definitions for unicode operations.
 */
#ifndef ADA_UNICODE_INL_H
#define ADA_UNICODE_INL_H
#include <algorithm>
#include "ada/unicode.h"

/**
 * @namespace ada::unicode
 * @brief Includes the declarations for unicode operations
 */
namespace ada::unicode {
ada_really_inline size_t percent_encode_index(const std::string_view input,
                                              const charset &cs) {
  return std::distance(
          input.begin(),
          std::find_if(input.begin(), input.end(), cs));
}
}  // namespace ada::unicode

#endif  // ADA_UNICODE_INL_H
