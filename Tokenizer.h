#pragma once
#include <string>
#include <vector>

namespace shell
{
  class Tokenizer
  {
    std::string m_commandName{};
    std::vector<std::string> m_flags{};
    std::vector<std::string> m_arguments{};

  public:
    [[nodiscard]] bool process(const std::string& tLine);
    void clear();
  };

} // namespace shell


