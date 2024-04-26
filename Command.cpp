#include <iostream>
#include "Command.h"

#include "CommandTokens.h"
#include "File.h"
#include "PathResolver.h"
#include "Utils.h"

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
      else
      {
        path = m_workingDir->getParentPath() + "/" + m_workingDir->getName() + "/" + path;
      }
    }
    return path;
  }

  fs::Directory* Command::ensureFoundDirectory(const Tokenizer& tokenizer) const
  {
    std::string path = tokenizer.getArguments()[0];
    const auto so = m_env->searchSystemObject(cmdPathResolution(path));
    if (!so)
    {
      std::cout << "path is invalid\n";
    }
    else if (so->getType() == fs::SystemObjectType::directory)
    {
      return dynamic_cast<fs::Directory*>(so);
    }
    else
    {
      std::cout << "command only works with directory objects\n";
    }
    return nullptr;
  }

  fs::Directory* Command::ensureFoundParentDir(const fs::PathResolver& pr) const
  {
    std::string parentPath{};
    for (const auto& token : pr.getParentPath())
    {
      parentPath += token + "/";
    }
    if (parentPath.size() > 1)
    {
      parentPath.pop_back();
    }
    const auto so = m_env->searchSystemObject(cmdPathResolution(parentPath));
    if (!so)
    {
      std::cout << "path is invalid\n";
    }
    else if (so->getType() == fs::SystemObjectType::directory)
    {
      return dynamic_cast<fs::Directory*>(so);
    }
    else
    {
      std::cout << "path is invalid\n";
    }
    return nullptr;
  }

  fs::SystemObject* Command::ensureFoundSysObj(const fs::PathResolver& pr) const
  {
    std::string parentPath{};
    for (const auto& token : pr.getParentPath())
    {
      parentPath += token + "/";
    }
    parentPath += pr.getObjectName();
    return m_env->searchSystemObject(cmdPathResolution(parentPath));
  }

  fs::Directory* Command::getWorkingDir() const
  {
    return m_workingDir;
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
      return false;
    }
    if (!CommandTokens::noFlags(tokenizer))
    {
      std::cout << "exit does not take flags\n";
      return false;
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
      return false;
    }
    if (!CommandTokens::noFlags(tokenizer))
    {
      std::cout << "logout does not take flags\n";
      return false;
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
    std::cout << "ls - print directory contents\n";
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
      m_target = ensureFoundDirectory(tokenizer);
      if (!m_target)
      {
        return false;
      }
    }
    return true;
  }
} // namespace shell

// Cd
namespace shell
{
  ShellFlag CommandCd::execute(const Tokenizer& tokenizer)
  {
    if (validateTokens(tokenizer))
    {
      m_workingDir = m_target;
    }
    std::cout << "\n";
    return ShellFlag::run;
  }

  ShellFlag CommandCd::help()
  {
    std::cout << "cd - change working directory\n";
    return ShellFlag::run;
  }

  std::vector<ResourceTypes> CommandCd::requiredResources()
  {
    return { ResourceTypes::root, ResourceTypes::workingDir, ResourceTypes::env };
  }

  std::unique_ptr<Command> CommandCd::factory()
  {
    return std::make_unique<CommandCd>();
  }

  bool CommandCd::validateTokens(const Tokenizer& tokenizer)
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
      m_target = ensureFoundDirectory(tokenizer);
      if (!m_target)
      {
        return false;
      }
    }
    return true;
  }
} // namespace shell

//// Cat
//namespace shell
//{
//  ShellFlag CommandCat::execute(const Tokenizer& tokenizer)
//  {
//  }
//
//  ShellFlag CommandCat::help()
//  {
//  }
//
//  std::vector<ResourceTypes> CommandCat::requiredResources()
//  {
//  }
//
//  std::unique_ptr<Command> CommandCat::factory()
//  {
//  }
//
//  bool CommandCat::validateTokens(const Tokenizer& tokenizer) const
//  {
//  }
//} // namespace shell

// clear
namespace shell
{
  ShellFlag CommandClear::execute(const Tokenizer& tokenizer)
  {
    if (validateTokens(tokenizer))
    {
      std::cout << "\x1B[2J\x1B[H";
    }
    return ShellFlag::run;
  }

  ShellFlag CommandClear::help()
  {
    std::cout << "clear - clear the terminal\n";
    return ShellFlag::run;
  }

  std::vector<ResourceTypes> CommandClear::requiredResources()
  {
    return {};
  }

  std::unique_ptr<Command> CommandClear::factory()
  {
    return std::make_unique<CommandClear>();
  }

  bool CommandClear::validateTokens(const Tokenizer& tokenizer) const
  {
    if (!CommandTokens::noArguments(tokenizer))
    {
      std::cout << "clear does not take arguments\n";
    }
    if (!CommandTokens::noFlags(tokenizer))
    {
      std::cout << "clear does not take flags\n";
    }
    return true;
  }
} // namespace shell

// mkdir
namespace shell
{
  ShellFlag CommandMkdir::execute(const Tokenizer& tokenizer)
  {
    if (validateTokens(tokenizer))
    {
      if (m_pDirectory == m_root)
      {
        (void)m_root->addChildren(std::make_unique<fs::Directory>(m_env->getUser(), m_dirName, ""));
      }
      else
      {
        (void)m_pDirectory->addChildren(std::make_unique<fs::Directory>(m_env->getUser(), m_dirName,
          (m_pDirectory->getParentPath().empty()
             ? ""
             : m_pDirectory->getParentPath() + "/") + m_pDirectory->getName()));
      }
    }
    return ShellFlag::run;
  }

  ShellFlag CommandMkdir::help()
  {
    std::cout << "mkdir - create a new directory\n";
    return ShellFlag::run;
  }

  std::vector<ResourceTypes> CommandMkdir::requiredResources()
  {
    return { ResourceTypes::root, ResourceTypes::workingDir, ResourceTypes::env };
  }

  std::unique_ptr<Command> CommandMkdir::factory()
  {
    return std::make_unique<CommandMkdir>();
  }

  bool CommandMkdir::validateTokens(const Tokenizer& tokenizer)
  {
    if (!CommandTokens::expectedQtyArguments(tokenizer, 1, 1))
    {
      std::cout << "invalid number of arguments\n";
      return false;
    }

    const fs::PathResolver pr{ tokenizer.getArguments()[0] };
    m_dirName = pr.getObjectName();
    m_pDirectory = m_workingDir;

    if (!pr.getParentPath().empty())
    {
      m_pDirectory = ensureFoundParentDir(pr);
      if (!m_pDirectory)
      {
        return false;
      }
    }
    for (const auto& [k, v] : m_pDirectory->getChildren())
    {
      if (k == m_dirName)
      {
        std::cout << "an object is already using this name\n";
        return false;
      }
    }
    return true;
  }
} // namespace shell

// rmdir
namespace shell
{
  ShellFlag CommandRmdir::execute(const Tokenizer& tokenizer)
  {
    if (validateTokens(tokenizer))
    {
      m_pDirectory->removeChild(m_dirName);
    }
    return ShellFlag::run;
  }

  ShellFlag CommandRmdir::help()
  {
    std::cout << "rmdir - remove an empty directory\n";
    return ShellFlag::run;
  }

  std::vector<ResourceTypes> CommandRmdir::requiredResources()
  {
    return { ResourceTypes::root, ResourceTypes::workingDir, ResourceTypes::env };
  }

  std::unique_ptr<Command> CommandRmdir::factory()
  {
    return std::make_unique<CommandRmdir>();
  }

  bool CommandRmdir::validateTokens(const Tokenizer& tokenizer)
  {
    if (!CommandTokens::expectedQtyArguments(tokenizer, 1, 1))
    {
      std::cout << "invalid number of arguments\n";
      return false;
    }

    const fs::PathResolver pr{ tokenizer.getArguments()[0] };
    m_dirName = pr.getObjectName();
    m_pDirectory = m_workingDir;

    if (!pr.getParentPath().empty())
    {
      m_pDirectory = ensureFoundParentDir(pr);
      if (!m_pDirectory)
      {
        return false;
      }
    }
    for (const auto& [k, v] : m_pDirectory->getChildren())
    {
      if (k == m_dirName)
      {
        if (v->getType() != fs::SystemObjectType::directory)
        {
          std::cout << "rmdir can only be used with directories\n";
          return false;
        }
        if (!dynamic_cast<fs::Directory*>(v.get())->getChildren().empty())
        {
          std::cout << "rmdir can only be used with empty directories\n";
          return false;
        }
        return true;
      }
    }
    std::cout << "directory not found\n";
    return false;
  }
} // namespace shell

// touch
namespace shell
{
  ShellFlag CommandTouch::execute(const Tokenizer& tokenizer)
  {
    if (validateTokens(tokenizer))
    {
      if (m_pDirectory == m_root)
      {
        (void)m_root->addChildren(std::make_unique<fs::File>(m_env->getUser(), m_fileName, ""));
        (void)fs::Utils::createFile("./fs/home/" + m_env->getUser()->getUsername() + "/" + m_fileName);
      }
      else
      {
        std::string parentPath = (m_pDirectory->getParentPath().empty()
                                    ? ""
                                    : m_pDirectory->getParentPath() + "/") + m_pDirectory->getName();
        (void)m_pDirectory->addChildren(std::make_unique<fs::File>(m_env->getUser(), m_fileName, parentPath));
        std::replace_if(parentPath.begin(), parentPath.end(), [](const char c)
        {
          if (c == '/')
          {
            return true;
          }
          return false;
        }, '_');
        (void)fs::Utils::createFile(
          "./fs/home/" + m_env->getUser()->getUsername() + "/" + parentPath + "_" + m_fileName);
      }
    }
    return ShellFlag::run;
  }

  ShellFlag CommandTouch::help()
  {
    std::cout << "touch - create file\n";
    return ShellFlag::run;
  }

  std::vector<ResourceTypes> CommandTouch::requiredResources()
  {
    return { ResourceTypes::root, ResourceTypes::workingDir, ResourceTypes::env };
  }

  std::unique_ptr<Command> CommandTouch::factory()
  {
    return std::make_unique<CommandTouch>();
  }

  bool CommandTouch::validateTokens(const Tokenizer& tokenizer)
  {
    if (!CommandTokens::expectedQtyArguments(tokenizer, 1, 1))
    {
      std::cout << "invalid number of arguments\n";
      return false;
    }

    const fs::PathResolver pr{ tokenizer.getArguments()[0] };
    m_fileName = pr.getObjectName();
    m_pDirectory = m_workingDir;

    if (!pr.getParentPath().empty())
    {
      m_pDirectory = ensureFoundParentDir(pr);
      if (!m_pDirectory)
      {
        return false;
      }
    }
    for (const auto& [k, v] : m_pDirectory->getChildren())
    {
      if (k == m_fileName)
      {
        std::cout << "an object is already using this name\n";
        return false;
      }
    }
    for (const auto c : m_fileName)
    {
      if (c == '_')
      {
        std::cout << "'_' is not allowed in file's name\n";
        return false;
      }
    }
    return true;
  }
} // namespace shell

// echo
namespace shell
{
  ShellFlag CommandEcho::execute(const Tokenizer& tokenizer)
  {
    if (validateTokens(tokenizer))
    {
      if (m_fileTarget)
      {
        m_fileTarget->appendToFile(tokenizer.getArguments()[0]);
      }
      else
      {
        std::cout << (tokenizer.getArguments().empty() ? "" : tokenizer.getArguments()[0]) << "\n";
      }
    }
    return ShellFlag::run;
  }

  ShellFlag CommandEcho::help()
  {
    std::cout << "echo - display a line of text\n"
      << "you can use echo with `>>` to send the line to a file\n";
    return ShellFlag::run;
  }

  std::vector<ResourceTypes> CommandEcho::requiredResources()
  {
    return { ResourceTypes::root, ResourceTypes::workingDir, ResourceTypes::env };
  }

  std::unique_ptr<Command> CommandEcho::factory()
  {
    return std::make_unique<CommandEcho>();
  }

  bool CommandEcho::validateTokens(const Tokenizer& tokenizer)
  {
    if (tokenizer.getArguments().size() <= 1)
    {
      return true;
    }
    if (tokenizer.getArguments().size() == 3 && tokenizer.getArguments()[1] == ">>")
    {
      const fs::PathResolver pr{ tokenizer.getArguments()[2] };
      if (const auto so = ensureFoundSysObj(pr); !so)
      {
        std::cout << "Invalid path\n";
      }
      else if (so->getType() != fs::SystemObjectType::file)
      {
        std::cout << "touch can only redirect to file objects\n";
      }
      else
      {
        m_fileTarget = dynamic_cast<fs::File*>(so);
        return true;
      }
    }
    std::cout << "Invalid arguments\n";
    return false;
  }
} // namespace shell
