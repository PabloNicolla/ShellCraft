#include <iostream>
#include "CommandProcessor.h"
#include "Flags.h"
#include "Tokenizer.h"

namespace shell
{
  CommandProcessor::CommandProcessor(fs::FileSystemManager* fsm) : m_workingDir{ fsm->getMainEnv()->getRoot() },
    m_root{ fsm->getMainEnv()->getRoot() }, m_fileSystemManager{ fsm }
  {
    m_commands["exit"] = CommandExit::factory;
    m_commands["logout"] = CommandLogout::factory;
    m_commands["ls"] = CommandLs::factory;
    m_commands["cd"] = CommandCd::factory;
    m_commands["clear"] = CommandClear::factory;
    m_commands["mkdir"] = CommandMkdir::factory;
    m_commands["rmdir"] = CommandRmdir::factory;
    m_commands["touch"] = CommandTouch::factory;
    m_commands["echo"] = CommandEcho::factory;
  }

  fs::AppState CommandProcessor::run()
  {
    std::cout << "\x1B[2J\x1B[H";
    std::string terminalLine;
    Tokenizer tokenizer;
    auto state = ShellFlag::run;
    while (state != ShellFlag::exit and state != ShellFlag::logout)
    {
      tokenizer.clear();
      if (m_root != m_workingDir)
      {
        std::cout << "/" << m_root->getName()
          << (m_workingDir->getParentPath().empty() ? "" : "/");
      }
      std::cout << m_workingDir->getParentPath() << "/" << m_workingDir->getName() << " >";
      std::getline(std::cin, terminalLine);
      terminalLine.append(" ");
      if (!tokenizer.process(terminalLine))
      {
        std::cout << "Invalid syntax\n\n";
        continue;
      }
      auto cmd = findCommand(tokenizer);
      if (!cmd)
      {
        std::cout << "Command not found\n\n";
        continue;
      }
      provideResources(cmd.get());
      state = cmd->execute(tokenizer);
      if (cmd->getWorkingDir())
      {
        m_workingDir = cmd->getWorkingDir();
      }
      if (state != ShellFlag::run)
      {
        std::cout << "Are you sure? (y/N): ";
        std::getline(std::cin, terminalLine);

        if (!(terminalLine == "y" || terminalLine == "Y"))
        {
          state = ShellFlag::run;
        }
      }
    }
    return state == ShellFlag::exit ? fs::AppState::exiting : fs::AppState::running;
  }

  void CommandProcessor::provideResources(Command* cmd) const
  {
    for (const auto& requiredResource : cmd->requiredResources())
    {
      if (requiredResource == ResourceTypes::root)
      {
        cmd->setRoot(m_root);
      }
      else if (requiredResource == ResourceTypes::workingDir)
      {
        cmd->setWorkingDir(m_workingDir);
      }
      else if (requiredResource == ResourceTypes::env)
      {
        cmd->setEnv(m_fileSystemManager->getMainEnv());
      }
    }
  }

  std::unique_ptr<Command> CommandProcessor::findCommand(const Tokenizer& tokenizer) const
  {
    if (const auto it = m_commands.find(tokenizer.getCommandName()); it != m_commands.end())
    {
      return it->second();
    }
    return nullptr;
  }
} // namespace shell
