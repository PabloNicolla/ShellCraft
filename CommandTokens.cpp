#include "CommandTokens.h"

namespace shell
{
  bool CommandTokens::noArguments(const Tokenizer& tokenizer)
  {
    return tokenizer.getArguments().empty();
  }

  bool CommandTokens::noFlags(const Tokenizer& tokenizer)
  {
    return tokenizer.getFlags().empty();
  }

  bool CommandTokens::expectedQtyArguments(const Tokenizer& tokenizer, const size_t min, const size_t max)
  {
    return min <= tokenizer.getArguments().size() && tokenizer.getArguments().size() <= max;
  }
} // namespace shell
