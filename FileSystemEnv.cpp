#include <iostream>
#include "FileSystemEnv.h"
#include "Utils.h"

namespace fs
{
  FileSystemEnv::FileSystemEnv(const User* user) : m_user{ user }
  {
  }

  void FileSystemEnv::loadUserEnv()
  {
    std::string rootPath = "./fs/home/" + m_user->getUsername();

    if (!Utils::checkIfDirectoryExists(rootPath))
    {
      if (!Utils::createDirectory(rootPath))
      {
        std::clog << "ERROR: failed to create user directory" << std::endl;  // NOLINT(performance-avoid-endl)
        std::terminate();
      }
      m_root = std::make_unique<Directory>();
    }
    else
    {
      
    }

  }
} // namespace fs
