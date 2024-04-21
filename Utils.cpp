#include <algorithm>

#include "Utils.h"

namespace fileSystem
{
  std::string Utils::trim(const std::string& str, const std::string& whitespace)
  {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
    {
      return "";
    }
    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;
    return str.substr(strBegin, strRange);
  }

  bool Utils::areAllCharactersAlpha(const std::string& str)
  {
    return std::all_of(str.begin(), str.end(), [](const char& c)
    {
      return std::isalpha(c);
    });
  }
} // namespace fileSystem
