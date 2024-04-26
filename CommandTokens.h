#pragma once
#include "Tokenizer.h"

namespace shell
{
  class CommandTokens
  {
  public:
    [[nodiscard]] static bool hasArguments(const Tokenizer& tokenizer);
    [[nodiscard]] static bool hasFlags(const Tokenizer& tokenizer);
    [[nodiscard]] static bool expectedQtyArguments(const Tokenizer& tokenizer, size_t min, size_t max);
    [[nodiscard]] static bool expectedQtyFlags(const Tokenizer& tokenizer, size_t min, size_t max);
  };
} // namespace shell
