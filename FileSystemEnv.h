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
    std::string m_rootPath{};

  public:
    explicit FileSystemEnv(const User* user);
    ~FileSystemEnv();
    FileSystemEnv(const FileSystemEnv&) = delete;
    FileSystemEnv& operator=(const FileSystemEnv&) = delete;
    FileSystemEnv(FileSystemEnv&& other) noexcept;
    FileSystemEnv& operator=(FileSystemEnv&& other) noexcept;
    void loadUserEnv();
    void saveUserEnv() const;
  };
} // namespace fs
