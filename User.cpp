#include "User.h"

namespace fs
{
  User::User(std::string username, std::string password) :
    m_username{ std::move(username) }, m_password{ std::move(password) }
  {
  }

  const std::string& User::getUsername() const
  {
    return m_username;
  }

  const std::string& User::getPassword() const
  {
    return m_password;
  }

  void User::setPassword(const std::string& password)
  {
    m_password = password;
  }
} // namespace fs
