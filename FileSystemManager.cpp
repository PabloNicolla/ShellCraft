#include <iomanip>
#include <iostream>
#include <limits>
#include "FileSystemManager.h"
#include "Flags.h"

[[nodiscard]] int getUserInput();


namespace fileSystem
{

  void FileSystemManager::run()
  {
    auto currentAppState = AppState::running;
    while (currentAppState == AppState::running)
    {
      switch (getUserInput())
      {
      case 1:
        this->m_userManager.authenticate();
        break;
      case 2:
        this->m_userManager.registerUser();
        break;
      case 3:
        currentAppState = AppState::exiting;
        break;
      default:
        std::clog << "Error FileSystemManager::run()\n\n";
      }
    }
  }
} // namespace fileSystem


[[nodiscard]] int getUserInput()
{
  int choice{};
  bool isValid{ false };

  while (!isValid)
  {
    std::cout << "Please choose an option:\n";
    std::cout << "1 - Choose user (default)\n";
    std::cout << "2 - Create new user\n";
    std::cout << "3 - Exit\n";
    std::cout << "> ";
    std::cin >> choice;

    char nextChar;
    while (std::cin.get(nextChar))
    {
      if (nextChar == '\n') break;
      if (!isspace(nextChar))
      {
        std::cin.setstate(std::ios_base::failbit);
        break;
      }
    }

    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input. Please enter a number.\n";
    }
    else
    {
      switch (choice)
      {
      case 1:
        std::cout << "Choose user selected.\n";
        isValid = true;
        break;
      case 2:
        std::cout << "Create user selected.\n";
        isValid = true;
        break;
      case 3:
        std::cout << "Exit selected. Goodbye!\n";
        isValid = true;
        break;
      default:
        std::cout << "Invalid choice. Please try again.\n";
      }
    }
  }
  if (std::cin.rdbuf()->in_avail() > 0)
  {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::clog << "Ignoring input : FileSystemManager.cpp GetUserInput()\n\n"; // TODO(pablo)
  }
  return choice;
}