#pragma once
#include <filesystem>
#include <optional>
#include <string>

namespace fs
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
    [[nodiscard]] std::optional<int> static getNumber();
    [[nodiscard]] std::optional<std::string> static getLine();
    [[nodiscard]] bool static promptToExitLoop();
    [[nodiscard]] bool static checkIfDirectoryExists(const std::filesystem::path& dir);
    [[nodiscard]] bool static createDirectory(const std::filesystem::path& dir);
    void static bufferSafetyCheck();
  };
} // namespace fs
