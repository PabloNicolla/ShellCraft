#include "CommandTokens.h"

namespace shell
{
  bool CommandTokens::hasArguments(const Tokenizer& tokenizer)
  {
    return !tokenizer.getArguments().empty();
  }

  bool CommandTokens::hasFlags(const Tokenizer& tokenizer)
  {
    return !tokenizer.getFlags().empty();
  }

  bool CommandTokens::expectedQtyArguments(const Tokenizer& tokenizer, const size_t min, const size_t max)
  {
    return min <= tokenizer.getArguments().size() && tokenizer.getArguments().size() <= max;
  }

  bool CommandTokens::expectedQtyFlags(const Tokenizer& tokenizer, const size_t min, const size_t max)
  {
    return min <= tokenizer.getFlags().size() && tokenizer.getFlags().size() <= max;
  }
} // namespace shell
