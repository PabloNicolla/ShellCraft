#include "User.h"

namespace fs
{
  User::User(std::string username, std::string password) :
    m_username{ std::move(username) }, m_password{ std::move(password) }
  {
  }

  User::User(std::istringstream& iss)
  {
    iss >> *this;
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

  std::ofstream& User::saveToFile(std::ofstream& ofs) const
  {
    ofs << m_username << "|" << m_password;
    return ofs;
  }

  std::ofstream& operator<<(std::ofstream& ofs, const User& user)
  {
    return user.saveToFile(ofs);
  }

  std::istringstream& User::readFromFile(std::istringstream& iss)
  {
    std::getline(iss, m_username, '|');
    std::getline(iss, m_password);
    return iss;
  }

  std::istringstream& operator>>(std::istringstream& iss, User& user)
  {
    return user.readFromFile(iss);
  }
} // namespace fs
