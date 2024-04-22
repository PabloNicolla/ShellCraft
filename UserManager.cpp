#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include "UserManager.h"
#include "Utils.h"

namespace fs
{
  UserManager::UserManager()
  {
    loadUsers();
  }

  UserManager::~UserManager()
  {
    saveUsers();
  }

  std::optional<const User*> UserManager::authenticate() const
  {
    while (true)
    {
      const auto username = getUsername();
      if (!username)
      {
        return {};
      }
      if (const auto foundUser = getUser(username.value()))
      {
        if (foundUser.value()->getPassword() == getPassword())
        {
          return { foundUser.value() };
        }
        std::cout << "Wrong password\n";
      }
      else
      {
        std::cout << "User with username: " << username.value() << " could not be found.\n";
      }
    }
  }

  void UserManager::registerUser()
  {
    if (const auto user = createUser())
    {
      m_users.push_back(user.value());
    }
  }

  void UserManager::saveUsers()
  {
    createEssentialFiles();
    std::ofstream file{ c_userManagerPath / "users.txt" };
    std::for_each(m_users.begin(), m_users.end(), [&](const User& user)
    {
      file << user << "\n";
    });
  }

  bool UserManager::usernameExists(const std::string_view username) const
  {
    return std::any_of(m_users.begin(), m_users.end(), [&](const User& user)
    {
      return user.getUsername() == username;
    });
  }

  std::optional<const User*> UserManager::getUser(const std::string_view username) const
  {
    const auto userIt = std::find_if(m_users.begin(), m_users.end(), [&](const User& user)
    {
      if (user.getUsername() == username)
      {
        return true;
      }
      return false;
    });

    if (userIt == m_users.end())
    {
      return {};
    }
    return &*userIt;
  }

  void UserManager::loadUsers()
  {
    if (Utils::checkIfDirectoryExists(c_userManagerPath))
    {
      std::ifstream file{ c_userManagerPath / "users.txt" };
      std::string line{};
      while (std::getline(file, line))
      {
        std::istringstream iss{ line };
        m_users.emplace_back(iss);
      }
    }
  }

  std::optional<User> UserManager::createUser() const
  {
    std::string username{};
    std::string password{};
    bool isValid{ false };

    while (!isValid)
    {
      if (const auto userInput = getUsername(); !userInput)
      {
        return {};
      }
      else if (usernameExists(userInput.value()))
      {
        std::cout << "Invalid username.\n"
          << "Username is already in use\n";
      }
      else
      {
        username = userInput.value();
        isValid = true;
      }
    }

    if (const auto userInput = getPassword(); !userInput)
    {
      return {};
    }
    else
    {
      password = userInput.value();
    }

    return User{ username, password };
  }

  std::optional<std::string> UserManager::getUsername()
  {
    std::string username{};
    bool isValid{ false };

    while (!isValid)
    {
      std::cout << "Please enter username: (empty input to cancel)\n"
        << "> ";
      if (auto line = Utils::getLine())
      {
        username = Utils::trim(line.value());
        if (username.size() < c_minUsernameLength || username.size() >= c_maxUsernameLength)
        {
          std::cout << "Invalid username.\n"
            << "Username cannot have less than " << 3 << " or more than "
            << c_maxUsernameLength << " characters" << "\n";
        }
        else if (!Utils::areAllCharactersAlpha(username))
        {
          std::cout << "Invalid username.\n"
            << "Username must contain only valid characters\n";
        }
        else
        {
          isValid = true;
        }
      }
      if (!isValid && Utils::promptToExitLoop())
      {
        return {};
      }
    }

    Utils::bufferSafetyCheck();
    return { username };
  }

  std::optional<std::string> UserManager::getPassword()
  {
    std::string password{};
    bool isValid{ false };

    while (!isValid)
    {
      std::cout << "Please enter password:\n"
        << "> ";
      if (auto line = Utils::getLine())
      {
        password = Utils::trim(line.value());
        isValid = true;
      }
      else if (Utils::promptToExitLoop())
      {
        return {};
      }
    }

    Utils::bufferSafetyCheck();
    return { password };
  }

  void UserManager::createEssentialFiles()
  {
    if (!Utils::checkIfDirectoryExists("./fs"))
    {
      if (!Utils::createDirectory("./fs"))
      {
        std::clog << "ERROR: failed to create fs directory\n\n";
      }
    }
    if (!Utils::checkIfDirectoryExists(c_userManagerPath))
    {
      if (!Utils::createDirectory(c_userManagerPath))
      {
        std::clog << "ERROR: UserManager::saveUsers() failed to create directory\n\n";
      }
    }
  }
} // namespace fs
