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
    void clear();
    [[nodiscard]] bool process(const std::string& tLine);
    [[nodiscard]] const std::string& getCommandName() const;
    [[nodiscard]] const std::vector<std::string>& getFlags() const;
    [[nodiscard]] const std::vector<std::string>& getArguments() const;
  };
} // namespace shell
