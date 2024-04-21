#pragma once
#include <memory>

#include "Directory.h"
#include "User.h"

namespace fs
{
  class FileSystemEnv
  {
    const User* m_user{};
    std::unique_ptr<Directory> m_root{};

  public:
    explicit FileSystemEnv(const User* user);
    void loadUserEnv();
  };
} // namespace fs
