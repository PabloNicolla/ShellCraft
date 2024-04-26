#pragma once
#include "FileSystemEnv.h"
#include "Flags.h"
#include "UserManager.h"


namespace fs
{
  class FileSystemManager
  {
  private:
    UserManager m_userManager{};
    std::vector<FileSystemEnv> m_environments{};

  public:
    void run();
    [[nodiscard]] FileSystemEnv* getMainEnv();
    [[nodiscard]] FileSystemEnv* getEnv(const std::string& user);
    [[nodiscard]] bool requestAuthentication(const std::string& user) const;

  private:
    AppState startFileSystem();
    void static createEssentialFiles();
    [[nodiscard]] int static getAction();
  };
} // namespace fs
