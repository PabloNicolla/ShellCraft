#include <algorithm>
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

  void Command::setCommands(const std::unordered_map<std::string, std::unique_ptr<Command>(*)()>* const commands)
  {
    m_commands = commands;
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
    if (const auto so = m_env->searchSystemObject(cmdPathResolution(path)); !so)
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

    if (const auto so = m_env->searchSystemObject(cmdPathResolution(parentPath));
      so && so->getType() == fs::SystemObjectType::directory)
    {
      return dynamic_cast<fs::Directory*>(so);
    }
    std::cout << "path is invalid\n";
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

  void CommandExit::help() const
  {
    std::cout << "exit - will shut down the app\n";
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
    if (CommandTokens::hasArguments(tokenizer))
    {
      std::cout << "exit does not take arguments\n";
      return false;
    }
    if (CommandTokens::hasFlags(tokenizer))
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

  void CommandLogout::help() const
  {
    std::cout << "logout - will log user out and send to initial menu\n";
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
    if (CommandTokens::hasArguments(tokenizer))
    {
      std::cout << "logout does not take arguments\n";
      return false;
    }
    if (CommandTokens::hasFlags(tokenizer))
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

  void CommandLs::help() const
  {
    std::cout << "ls - print directory contents\n";
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
    if (CommandTokens::hasFlags(tokenizer))
    {
      std::cout << "ls does not have any flags implemented\n";
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

  void CommandCd::help() const
  {
    std::cout << "cd - change working directory\n";
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
    if (CommandTokens::hasFlags(tokenizer))
    {
      std::cout << "cd does not have any flags implemented\n";
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

// Cat
namespace shell
{
  ShellFlag CommandCat::execute(const Tokenizer& tokenizer)
  {
    if (validateTokens(tokenizer))
    {
      m_fileTarget->printFile();
    }
    return ShellFlag::run;
  }

  void CommandCat::help() const
  {
    std::cout << "cat - concatenate files and print on the standard output\n";
  }

  std::vector<ResourceTypes> CommandCat::requiredResources()
  {
    return { ResourceTypes::root, ResourceTypes::workingDir, ResourceTypes::env };
  }

  std::unique_ptr<Command> CommandCat::factory()
  {
    return std::make_unique<CommandCat>();
  }

  bool CommandCat::validateTokens(const Tokenizer& tokenizer)
  {
    if (!CommandTokens::expectedQtyArguments(tokenizer, 1, 1))
    {
      std::cout << "invalid number of arguments\n";
      return false;
    }
    if (CommandTokens::hasFlags(tokenizer))
    {
      std::cout << "cat does not have any flags implemented\n";
      return false;
    }

    const fs::PathResolver pr{ tokenizer.getArguments()[0] };
    if (const auto so = ensureFoundSysObj(pr); !so)
    {
      std::cout << "Invalid path\n";
    }
    else if (so->getType() != fs::SystemObjectType::file)
    {
      std::cout << "touch only works with file objects\n";
    }
    else
    {
      m_fileTarget = dynamic_cast<fs::File*>(so);
      return true;
    }
    return false;
  }
} // namespace shell

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

  void CommandClear::help() const
  {
    std::cout << "clear - clear the terminal\n";
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
    if (CommandTokens::hasArguments(tokenizer))
    {
      std::cout << "clear does not take arguments\n";
      return false;
    }
    if (CommandTokens::hasFlags(tokenizer))
    {
      std::cout << "clear does not take flags\n";
      return false;
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

  void CommandMkdir::help() const
  {
    std::cout << "mkdir - create a new directory\n";
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
    if (CommandTokens::hasFlags(tokenizer))
    {
      std::cout << "mkdir does not have any flags implemented\n";
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

  void CommandRmdir::help() const
  {
    std::cout << "rmdir - remove an empty directory\n";
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
    if (CommandTokens::hasFlags(tokenizer))
    {
      std::cout << "rmdir does not have any flags implemented\n";
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
        fs::File::adaptParentPath(parentPath);
        (void)fs::Utils::createFile(
          "./fs/home/" + m_env->getUser()->getUsername() + "/" + parentPath + "_" + m_fileName);
      }
    }
    return ShellFlag::run;
  }

  void CommandTouch::help() const
  {
    std::cout << "touch - create file\n";
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
    if (CommandTokens::hasFlags(tokenizer))
    {
      std::cout << "touch does not have any flags implemented\n";
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

  void CommandEcho::help() const
  {
    std::cout << "echo - display a line of text\n"
      << "you can use echo with `>>` to send the line to a file\n";
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
    if (CommandTokens::hasFlags(tokenizer))
    {
      std::cout << "echo command does not have any flags implemented\n";
      return false;
    }
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

// rm
namespace shell
{
  ShellFlag CommandRm::execute(const Tokenizer& tokenizer)
  {
    if (validateTokens(tokenizer))
    {
      m_pDirectory->removeChild(m_sysObjName);
    }
    return ShellFlag::run;
  }

  void CommandRm::help() const
  {
    std::cout << "rm - deletes a file\n"
      << "flags [ -r ]: deletes non-empty directories\n";
  }

  std::vector<ResourceTypes> CommandRm::requiredResources()
  {
    return { ResourceTypes::root, ResourceTypes::workingDir, ResourceTypes::env };
  }

  std::unique_ptr<Command> CommandRm::factory()
  {
    return std::make_unique<CommandRm>();
  }

  bool CommandRm::validateTokens(const Tokenizer& tokenizer)
  {
    if (!CommandTokens::expectedQtyArguments(tokenizer, 1, 1))
    {
      std::cout << "invalid number of arguments\n";
      return false;
    }
    if (!CommandTokens::expectedQtyFlags(tokenizer, 0, 1))
    {
      std::cout << "invalid number of flags\n";
      return false;
    }

    const fs::PathResolver pr{ tokenizer.getArguments()[0] };
    const fs::SystemObject* target{};
    m_sysObjName = pr.getObjectName();
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
      if (k == m_sysObjName)
      {
        target = v.get();
        break;
      }
    }
    if (!target)
    {
      std::cout << "invalid path\n";
      return false;
    }

    if (!tokenizer.getFlags().empty())
    {
      const Flags flag = stringToFlag(tokenizer.getFlags()[0]);
      if (flag == invalid)
      {
        std::cout << "invalid flag\n";
        return false;
      }
      if (flag == r) m_objType = target->getType();
    }
    else
    {
      if (target->getType() != fs::SystemObjectType::file)
      {
        std::cout << "cannot delete directories\n"
          << "use rm -r to delete directories\n";
        return false;
      }
      m_objType = fs::SystemObjectType::file;
    }
    return true;
  }

  CommandRm::Flags CommandRm::stringToFlag(const std::string& str) const
  {
    Flags output = invalid;
    if (str == "-r") { output = r; }
    return output;
  }
} // namespace shell

// Help
namespace shell
{
  ShellFlag CommandHelp::execute(const Tokenizer& tokenizer)
  {
    if (validateTokens(tokenizer))
    {
      if (tokenizer.getArguments().empty())
      {
        for (const auto& [k, v] : *m_commands)
        {
          std::cout << k << "\n";
        }
      }
      else
      {
        m_commands->at(tokenizer.getArguments()[0])()->help();
      }
    }
    return ShellFlag::run;
  }

  void CommandHelp::help() const
  {
    std::cout << "help - display additional information\n";
  }

  std::vector<ResourceTypes> CommandHelp::requiredResources()
  {
    return { ResourceTypes::additionalCmd };
  }

  std::unique_ptr<Command> CommandHelp::factory()
  {
    return std::make_unique<CommandHelp>();
  }

  bool CommandHelp::validateTokens(const Tokenizer& tokenizer) const
  {
    if (!CommandTokens::expectedQtyArguments(tokenizer, 0, 1))
    {
      std::cout << "Invalid arguments\n";
      return false;
    }
    if (CommandTokens::hasFlags(tokenizer))
    {
      std::cout << "help command does not take flags\n";
      return false;
    }

    if (tokenizer.getArguments().size() == 1)
    {
      if (m_commands->find(tokenizer.getArguments()[0]) == m_commands->end())
      {
        std::cout << "argument is not valid\n";
        return false;
      }
    }
    return true;
  }
} // namespace shell
