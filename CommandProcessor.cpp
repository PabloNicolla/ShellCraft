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
      std::cout << m_workingDir->getParentPath() << "/" << m_workingDir->getName() << " >";
      std::getline(std::cin, terminalLine);
      terminalLine.append(" ");
      if (!tokenizer.process(terminalLine))
      {
        continue;
      }
      auto cmd = findCommand(tokenizer);
      if (!cmd)
      {
        continue;
      }
      provideResources(cmd.get());
      state = cmd->execute();
      if (state != ShellFlag::run)
      {
        // ask
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
