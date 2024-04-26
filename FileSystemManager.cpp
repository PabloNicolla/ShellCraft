#include <iostream>
#include "FileSystemManager.h"
#include "CommandProcessor.h"
#include "Utils.h"


namespace fs
{
  enum Action { authenticate, registerUser, exit };

  void FileSystemManager::run()
  {
    auto currentAppState = AppState::running;
    while (currentAppState == AppState::running)
    {
      switch (getAction())
      {
      case authenticate:
        {
          if (const auto user = m_userManager.authenticate())
          {
            m_environments.emplace_back(user.value());
            currentAppState = startFileSystem();
          }
        }
        break;
      case registerUser:
        m_userManager.registerUser();
        break;
      case exit:
        currentAppState = AppState::exiting;
        break;
      default:
        std::clog << "Error FileSystemManager::run()\n\n";
      }
    }
  }

  FileSystemEnv* FileSystemManager::getMainEnv()
  {
    if (!m_environments.empty())
    {
      return &m_environments.front();
    }
    return nullptr;
  }

  FileSystemEnv* FileSystemManager::getEnv([[maybe_unused]] const std::string& user)
  {
    return nullptr; // TODO(pablo)
  }

  bool FileSystemManager::requestAuthentication(const std::string& user) const
  {
    return m_userManager.requestPassword(user);
  }

  AppState FileSystemManager::startFileSystem()
  {
    createEssentialFiles();
    m_environments[0].loadUserEnv();
    shell::CommandProcessor cp{ this };
    const auto state = cp.run();
    m_environments.clear();
    return state;
  }

  [[nodiscard]] int FileSystemManager::getAction()
  {
    int choice{ -1 };
    bool isValid{ false };

    while (!isValid)
    {
      std::cout << "Please choose an option:\n"
        << "1 - Choose user\n"
        << "2 - Create new user\n"
        << "3 - Exit\n"
        << "> ";

      choice = Utils::getNumber().value_or(-1) - 1;

      isValid = true;
      switch (choice)
      {
      case authenticate:
        std::cout << "Choose user selected.\n\n";
        break;
      case registerUser:
        std::cout << "Create user selected.\n\n";
        break;
      case exit:
        std::cout << "Exit selected. Goodbye!\n\n";
        break;
      default:
        isValid = false;
        std::cout << "Invalid choice. Please try again.\n\n";
      }
    }
    Utils::bufferSafetyCheck();
    return choice;
  }

  void FileSystemManager::createEssentialFiles()
  {
    if (!Utils::checkIfDirectoryExists("./fs"))
    {
      if (!Utils::createDirectory("./fs"))
      {
        std::clog << "ERROR: failed to create fs directory\n\n";
      }
    }
    if (!Utils::checkIfDirectoryExists("./fs/home"))
    {
      if (!Utils::createDirectory("./fs/home"))
      {
        std::clog << "ERROR: failed to create home directory\n\n";
      }
    }
  }
} // namespace fs
