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
    void startFileSystem();
  };
} // namespace fs
