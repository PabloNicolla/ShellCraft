#pragma once
#include "FileSystemEnv.h"
#include "UserManager.h"


namespace fs
{
  enum class AppState { running, exiting };

  class FileSystemManager
  {
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
  };
} // namespace fs
