#include <iostream>
#include "Command.h"

#include "CommandTokens.h"

// Command
namespace shell
{
  void Command::setEnv(fs::FileSystemEnv* env)
  {
    m_env = env;
  }

  std::string& Command::cmdPathResolution(std::string& path) const
  {
    if (path[0] == '/')
    {
      path.erase(0, 1);
    }
    else if (m_root != m_workingDir)
    {
      if (m_workingDir->getParentPath().empty())
      {
        path = m_workingDir->getName() + "/" + path;
      }
      path = m_workingDir->getParentPath() + "/" + m_workingDir->getName() + "/" + path;
    }
    return path;
  }

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
} // namespace shell

// Exit
namespace shell
{
  ShellFlag CommandExit::execute(const Tokenizer& tokenizer)
  {
    if (!validateTokens(tokenizer))
    {
      return ShellFlag::run;
    }
    return ShellFlag::exit;
  }

  ShellFlag CommandExit::help()
  {
    std::cout << "exit - will shut down the app\n";
    return ShellFlag::run;
  }

  std::vector<ResourceTypes> CommandExit::requiredResources()
  {
    return {};
  }

  std::unique_ptr<Command> CommandExit::factory()
  {
    return std::make_unique<CommandExit>();
  }

  bool CommandExit::validateTokens(const Tokenizer& tokenizer) const
  {
    if (!CommandTokens::noArguments(tokenizer))
    {
      std::cout << "exit does not take arguments\n";
    }
    if (!CommandTokens::noFlags(tokenizer))
    {
      std::cout << "exit does not take flags\n";
    }
    return true;
  }
} // namespace shell

// Logout
namespace shell
{
  ShellFlag CommandLogout::execute(const Tokenizer& tokenizer)
  {
    if (!validateTokens(tokenizer))
    {
      return ShellFlag::run;
    }
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

  bool CommandLogout::validateTokens(const Tokenizer& tokenizer) const
  {
    if (!CommandTokens::noArguments(tokenizer))
    {
      std::cout << "logout does not take arguments\n";
    }
    if (!CommandTokens::noFlags(tokenizer))
    {
      std::cout << "logout does not take flags\n";
    }
    return true;
  }
} // namespace shell

// Ls
namespace shell
{
  ShellFlag CommandLs::execute(const Tokenizer& tokenizer)
  {
    if (validateTokens(tokenizer))
    {
      for (const auto& [k, v] : m_target->getChildren())
      {
        std::cout << v->getName() << '\t';
      }
    }
    std::cout << "\n";
    return ShellFlag::run;
  }

  ShellFlag CommandLs::help()
  {
    return ShellFlag::run;
  }

  std::vector<ResourceTypes> CommandLs::requiredResources()
  {
    return { ResourceTypes::root, ResourceTypes::workingDir, ResourceTypes::env };
  }

  std::unique_ptr<Command> CommandLs::factory()
  {
    return std::make_unique<CommandLs>();
  }

  bool CommandLs::validateTokens(const Tokenizer& tokenizer)
  {
    if (!CommandTokens::expectedQtyArguments(tokenizer, 0, 1))
    {
      std::cout << "invalid number of arguments\n";
      return false;
    }
    if (tokenizer.getArguments().empty())
    {
      m_target = m_workingDir;
    }
    else
    {
      std::string path = tokenizer.getArguments()[0];
      const auto so = m_env->searchSystemObject(cmdPathResolution(path));
      if (!so)
      {
        std::cout << "path is invalid\n";
        return false;
      }
      if (so->getType() == fs::SystemObjectType::directory)
      {
        m_target = dynamic_cast<fs::Directory*>(so);
      }
      else
      {
        std::cout << "ls only works in directory objects\n";
        return false;
      }
    }
    return true;
  }
} // namespace shell
