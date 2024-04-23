#pragma once
#include "Command.h"
#include "Directory.h"
#include "FileSystemManager.h"
#include "Tokenizer.h"

namespace shell
{
  class CommandProcessor
  {
    fs::Directory* m_workingDir;
    fs::Directory* m_root;
    [[maybe_unused]] fs::FileSystemManager* m_fileSystemManager;
    std::unordered_map<std::string, std::unique_ptr<Command>(*)()> m_commands{};

  public:
    explicit CommandProcessor(fs::FileSystemManager* fsm);
    fs::AppState run();
    void provideResources(Command* cmd) const;
    [[nodiscard]] std::unique_ptr<Command> findCommand(const Tokenizer& tokenizer) const;
  };
} // namespace shell
