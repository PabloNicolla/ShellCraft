#include <iostream>

#include "CommandProcessor.h"

#include "Command.h"
#include "Tokenizer.h"

namespace shell
{
  CommandProcessor::CommandProcessor(fs::FileSystemManager* fsm) : m_workingDir{ fsm->getMainEnv()->getRoot() }, m_fileSystemManager{ fsm }
  {
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
      std::getline(std::cin, terminalLine);
      terminalLine.append(" ");
      if (!tokenizer.process(terminalLine))
      {
        continue;
      }
      Command* cmd = findCommand(tokenizer);
      if (!cmd)
      {
        continue;
      }
      provideResources(cmd);
      state = cmd.execute();
      if (state != ShellFlag::run)
      {
        // ask
      }
    }
    return state == ShellFlag::exit ? fs::AppState::exiting : fs::AppState::running;
  }
} // namespace shell
