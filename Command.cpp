#include <iostream>
#include "Command.h"

namespace shell
{
  Command::Command(Command&& other) noexcept
  {
    m_root = other.m_root;
    m_workingDir = other.m_workingDir;
  }

  Command& Command::operator=(Command&& other) noexcept
  {
    m_root = other.m_root;
    m_workingDir = other.m_workingDir;
    return *this;
  }

  void Command::setRoot(fs::Directory* const root)
  {
    m_root = root;
  }

  void Command::setWorkingDir(fs::Directory* const workingDir)
  {
    m_workingDir = workingDir;
  }

  ShellFlag CommandExit::execute()
  {
    return ShellFlag::exit;
  }

  ShellFlag CommandExit::help()
  {
    return ShellFlag::run;
  }

  std::vector<ResourceTypes> CommandExit::requiredResources()
  {
    return {};
  }

  /*constexpr*/
  std::unique_ptr<Command> CommandExit::factory()
  {
    return std::make_unique<CommandExit>();
  }

  ShellFlag CommandLogout::execute()
  {
    return ShellFlag::logout;
  }

  ShellFlag CommandLogout::help()
  {
    return ShellFlag::run;
  }

  std::vector<ResourceTypes> CommandLogout::requiredResources()
  {
    return {};
  }

  std::unique_ptr<Command> CommandLogout::factory()
  {
    return std::make_unique<CommandLogout>();
  }

  ShellFlag CommandLs::execute()
  {
    std::cout << "ls\n\n";
    return ShellFlag::run;
  }

  ShellFlag CommandLs::help()
  {
    return ShellFlag::run;
  }

  std::vector<ResourceTypes> CommandLs::requiredResources()
  {
    return { ResourceTypes::root, ResourceTypes::workingDir };
  }

  std::unique_ptr<Command> CommandLs::factory()
  {
    return std::make_unique<CommandLs>();
  }
} // namespace shell
