#include "Tokenizer.h"

namespace shell
{
  bool Tokenizer::process(const std::string& tLine)
  {
    bool isBetweenQuotes{ false };
    bool isReadingToken{ false };
    size_t tokenStart{ 0 };
    size_t tokenEnd{ 0 };
    size_t tokensSaved{ 0 };

    auto saveToken = [&]
    {
      if (tokensSaved == 0)
      {
        m_commandName = tLine.substr(tokenStart, tokenEnd);
      }
      else if (tLine[tokenStart] == '-')
      {
        m_flags.emplace_back(tLine.substr(tokenStart, tokenEnd));
      }
      else
      {
        m_arguments.emplace_back(tLine.substr(tokenStart, tokenEnd));
      }
      ++tokensSaved;
    };

    for (size_t i{ 0 }; i < tLine.size(); ++i)
    {
      if (isBetweenQuotes)
      {
        if (tLine[i] != '"')
        {
          continue;
        }
        isReadingToken = false;
        isBetweenQuotes = false;
        tokenEnd = i;
        saveToken();
        continue;
      }

      if (isReadingToken)
      {
        if (!std::isspace(tLine[i]))
        {
          continue;
        }
        isReadingToken = false;
        tokenEnd = i;
        saveToken();
        continue;
      }

      if (!std::isspace(tLine[i]))
      {
        tokenStart = i;
        isReadingToken = true;
        if (tLine[i] == '"')
        {
          isBetweenQuotes = true;
        }
      }
    }

    if (isBetweenQuotes)
    {
      return false;
    }
    return true;
  }

  void Tokenizer::clear()
  {
    m_arguments.clear();
    m_flags.clear();
    m_commandName = "";
  }

  const std::string& Tokenizer::getCommandName() const
  {
    return m_commandName;
  }

  const std::vector<std::string>& Tokenizer::getFlags() const
  {
    return m_flags;
  }

  const std::vector<std::string>& Tokenizer::getArguments() const
  {
    return m_arguments;
  }
} // namespace shell
