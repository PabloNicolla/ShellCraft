#pragma once

#include "UserManager.h"

namespace fileSystem
{
  class FileSystemManager
  {
    UserManager m_userManager{};

  public:
    void run();
  };
} // namespace fileSystem
