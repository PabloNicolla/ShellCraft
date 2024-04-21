#include <iomanip>
#include <iostream>
#include <limits>
#include "FileSystemManager.h"
#include "Flags.h"
#include "Utils.h"


namespace fileSystem
{
  [[nodiscard]] int getAction();

  void FileSystemManager::run()
  {
    auto currentAppState = AppState::running;
    while (currentAppState == AppState::running)
    {
      switch (getAction())
      {
      case 1:
        {
          if (const auto user = m_userManager.authenticate())
          {
            //
          }
        }
        break;
      case 2:
        m_userManager.registerUser();
        break;
      case 3:
        currentAppState = AppState::exiting;
        break;
      default:
        std::clog << "Error FileSystemManager::run()\n\n";
      }
    }
  }

  [[nodiscard]] int getAction()
  {
    std::optional<int> choice;
    bool isValid{ false };

    while (!isValid)
    {
      std::cout << "Please choose an option:\n"
        << "1 - Choose user (default)\n"
        << "2 - Create new user\n"
        << "3 - Exit\n"
        << "> ";

      choice = Utils::getNumber();

      if (choice)
      {
        isValid = true;
        switch (choice.value())
        {
        case 1:
          std::cout << "Choose user selected.\n";
          break;
        case 2:
          std::cout << "Create user selected.\n";
          break;
        case 3:
          std::cout << "Exit selected. Goodbye!\n";
          break;
        default:
          isValid = false;
          std::cout << "Invalid choice. Please try again.\n";
        }
      }
    }
    if (std::cin.rdbuf()->in_avail() > 0)
    {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::clog << "Ignoring input: FileSystemManager.cpp GetUserInput()\n\n"; // TODO(pablo)
    }
    return choice.value();
  }
} // namespace fileSystem
