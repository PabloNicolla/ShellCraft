#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <fstream>
#include <sstream>
#include <string>

namespace fs
{
  class User
  {
  private:
    std::string m_username;
    std::string m_password;

  public:
    User(std::string username, std::string password);
    explicit User(std::istringstream& iss);

    [[nodiscard]] const std::string& getUsername() const;
    [[nodiscard]] const std::string& getPassword() const;
    void setPassword(const std::string& password);
    std::ofstream& saveToFile(std::ofstream& ofs) const;
    std::istringstream& readFromFile(std::istringstream& iss);
    friend std::ofstream& operator<<(std::ofstream& ofs, const User& user);
    friend std::istringstream& operator>>(std::istringstream& iss, User& user);
  };
} // namespace fs
