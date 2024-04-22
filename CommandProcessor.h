#pragma once
#include "Directory.h"
#include "FileSystemManager.h"

namespace shell
{
  enum class ShellFlag { exit, run, logout };

  class CommandProcessor
  {
    fs::Directory* m_workingDir;
    fs::FileSystemManager* m_fileSystemManager;

  public:
    explicit CommandProcessor(fs::FileSystemManager* fsm);
    fs::AppState run();
  };
} // namespace shell
