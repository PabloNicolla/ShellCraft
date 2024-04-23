#pragma once
#include <filesystem>
#include <memory>
#include "Directory.h"
#include "User.h"


namespace fs
{
  class FileSystemEnv
  {
  private:
    const User* m_user{};
    std::unique_ptr<Directory> m_root{};
    std::filesystem::path m_rootPath{};
    static const char* m_usersHomePath;

  public:
    explicit FileSystemEnv(const User* user);
    ~FileSystemEnv();
    FileSystemEnv(const FileSystemEnv&) = delete;
    FileSystemEnv& operator=(const FileSystemEnv&) = delete;
    FileSystemEnv(FileSystemEnv&& other) noexcept;
    FileSystemEnv& operator=(FileSystemEnv&& other) noexcept;

    void loadUserEnv();
    void saveUserEnv() const;
    [[nodiscard]] SystemObject* searchSystemObject(const std::string& path);
    [[nodiscard]] Directory* getRoot();
  };
} // namespace fs
