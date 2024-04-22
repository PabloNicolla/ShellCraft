#include <iostream>
#include "FileSystemManager.h"

#include "CommandProcessor.h"
#include "Flags.h"
#include "Utils.h"


namespace fs
{
  enum Action { authenticate, registerUser, exit };

  [[nodiscard]] int getAction();

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
            startFileSystem();
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

  void FileSystemManager::startFileSystem()
  {
    if (!Utils::checkIfDirectoryExists("./fs"))
    {
      if (!Utils::createDirectory("./fs"))
      {
        std::clog << "ERROR: failed to create fs directory" << std::endl;  // NOLINT(performance-avoid-endl)
        std::terminate();
      }
    }
    if (!Utils::checkIfDirectoryExists("./fs/home"))
    {
      if (!Utils::createDirectory("./fs/home"))
      {
        std::clog << "ERROR: failed to create home directory" << std::endl;  // NOLINT(performance-avoid-endl)
        std::terminate();
      }
    }
    auto& mainEnv = m_environments[0];
    mainEnv.loadUserEnv();
    shell::CommandProcessor::run();
  }

  [[nodiscard]] int getAction()
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
        std::cout << "Choose user selected.\n";
        break;
      case registerUser:
        std::cout << "Create user selected.\n";
        break;
      case exit:
        std::cout << "Exit selected. Goodbye!\n";
        break;
      default:
        isValid = false;
        std::cout << "Invalid choice. Please try again.\n";
      }
    }
    Utils::bufferSafetyCheck();
    return choice;
  }
} // namespace fs
