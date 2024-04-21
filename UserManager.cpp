#include <algorithm>
#include <iostream>
#include <string>
#include "UserManager.h"
#include "Utils.h"

namespace fileSystem
{
  UserManager::UserManager()
  {
    loadUsers();
  }

  void UserManager::authenticate()
  {
  }

  void UserManager::registerUser()
  {
    User a = getUser();
    m_users.push_back(a);
  }

  bool UserManager::usernameExists(const std::string_view username) const
  {
    return std::any_of(m_users.begin(), m_users.end(), [&](const User& user)
    {
      return user.getUsername() == username;
    });
  }

  void UserManager::loadUsers()
  {
  }

  User UserManager::getUser()
  {
    std::string username{};
    std::string password{};
    bool isValid{ false };

    while (!isValid)
    {
      std::string buff{};
      std::cout << "Please enter a new and unique username:\n"
        << "> ";
      std::getline(std::cin, buff);

      if (std::cin.fail())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::clog << "cin fail: UserManager.cpp GetUserInput()\n\n"; // TODO(pablo)
      }
      else
      {
        buff = Utils::trim(buff);
        if (buff.size() < c_minUsernameLength || buff.size() >= c_maxUsernameLength)
        {
          std::cout << "Invalid username.\n"
            << "Username cannot have less than " << 3 << " or more than "
            << c_maxUsernameLength << " characters" << "\n";
        }
        else if (!Utils::areAllCharactersAlpha(buff))
        {
          std::cout << "Invalid username.\n"
            << "Username must contain only valid characters\n";
        }
        else if (usernameExists(buff))
        {
          std::cout << "Invalid username.\n"
            << "Username is already in use\n";
        }
        else
        {
          isValid = true;
          username = std::move(buff);
        }
      }
    }

    isValid = false;
    while (!isValid)
    {
      std::string buff{};
      std::cout << "Please enter password:\n"
        << "> ";
      std::getline(std::cin, buff);

      if (std::cin.fail())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::clog << "cin fail: UserManager.cpp GetUserInput()\n\n"; // TODO(pablo)
      }
      else
      {
        buff = Utils::trim(buff);
        if (buff.size() < c_minPasswordLength || buff.size() >= c_maxPasswordLength)
        {
          std::cout << "Invalid password.\n"
            << "Password cannot have less than " << 3 << " or more than "
            << c_maxPasswordLength << " characters" << "\n";
        }
        else
        {
          isValid = true;
          password = std::move(buff);
        }
      }
    }

    if (std::cin.rdbuf()->in_avail() > 0)
    {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::clog << "Ignoring input: UserManager.cpp GetUserInput()\n\n"; // TODO(pablo)
    }
    return { username, password };
  }
} // namespace fileSystem
