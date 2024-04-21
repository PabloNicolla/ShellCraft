#pragma once
#include <string>

namespace fs
{
  class User
  {
    std::string m_username;
    std::string m_password;

  public:
    User(std::string username, std::string password);
    [[nodiscard]] const std::string& getUsername() const;
    [[nodiscard]] const std::string& getPassword() const;
    void setPassword(const std::string& password);
  };
} // namespace fs
