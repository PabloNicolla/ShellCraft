#pragma once
#include <string>

namespace fileSystem
{
  class Utils
  {
  private:
    Utils() = default;
    ~Utils() = default;

  public:
    Utils(const Utils&) = delete;
    Utils& operator=(const Utils&) = delete;
    Utils(Utils&&) = delete;
    Utils& operator=(Utils&&) = delete;

    [[nodiscard]] std::string static trim(const std::string& str, const std::string& whitespace = " \t");
    [[nodiscard]] bool static areAllCharactersAlpha(const std::string& str);
  };
} // namespace fileSystem
