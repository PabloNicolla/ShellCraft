#pragma once
#include "Tokenizer.h"

namespace shell
{
  class CommandTokens
  {
  public:
    [[nodiscard]] static bool noArguments(const Tokenizer& tokenizer);
    [[nodiscard]] static bool noFlags(const Tokenizer& tokenizer);
    [[nodiscard]] static bool expectedQtyArguments(const Tokenizer& tokenizer, size_t min, size_t max);
  };
} // namespace shell
