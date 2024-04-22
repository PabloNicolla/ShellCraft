#pragma once

#include "FileSystemEnv.h"
#include "UserManager.h"

namespace fs
{
  class FileSystemManager
  {
    UserManager m_userManager{};
    std::vector<FileSystemEnv> m_environments{};

  public:
    void run();

  private:
    void startFileSystem();
    void static createEssentialFiles();
  };
} // namespace fs
