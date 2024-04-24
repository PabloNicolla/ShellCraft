#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include "Utils.h"

namespace fs
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

  std::optional<int> Utils::getNumber()
  {
    int number;
    std::cin >> number;

    char nextChar;
    while (std::cin.get(nextChar))
    {
      if (nextChar == '\n') break;
      if (!isspace(nextChar))
      {
        std::cin.setstate(std::ios_base::failbit);
        break;
      }
    }

    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input. Please enter a number.\n";
      return {};
    }
    return { number };
  }

  std::optional<std::string> Utils::getLine()
  {
    std::string buff{};
    std::getline(std::cin, buff);

    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::clog << "ERROR: Utils::getLine() \n\n";
      return {};
    }
    return { buff };
  }

  void Utils::bufferSafetyCheck()
  {
    if (std::cin.rdbuf()->in_avail() > 0)
    {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::clog << "ERROR: Utils::bufferSafetyCheck() \n\n";
    }
  }

  bool Utils::promptToExitLoop()
  {
    std::cout << "Do you want to cancel this operation? (y/N): ";
    std::string buff;
    std::getline(std::cin, buff);

    if (buff == "y" || buff == "Y")
    {
      return true;
    }
    return false;
  }

  bool Utils::checkIfDirectoryExists(const std::filesystem::path& dir)
  {
    return std::filesystem::exists(dir);
  }

  bool Utils::createDirectory(const std::filesystem::path& dir)
  {
    return std::filesystem::create_directory(dir);
  }

  bool Utils::createFile(const std::filesystem::path& file)
  {
    std::ofstream f{ file };
    f << "";
    return true;
  }

  bool Utils::removeFile(const std::filesystem::path& file)
  {
    return std::filesystem::remove(file);
  }
} // namespace fs
