#include <fstream>
#include <iostream>
#include <queue>

#include "FileSystemEnv.h"
#include "Utils.h"

namespace fs
{
  FileSystemEnv::FileSystemEnv(const User* user) : m_user{ user }, m_rootPath{ "./fs/home/" + m_user->getUsername() }
  {
  }

  FileSystemEnv::~FileSystemEnv()
  {
    saveUserEnv();
  }

  FileSystemEnv::FileSystemEnv(FileSystemEnv&& other) noexcept
  {
    m_root = std::move(other.m_root);
    m_user = other.m_user;
  }

  FileSystemEnv& FileSystemEnv::operator=(FileSystemEnv&& other) noexcept
  {
    m_root = std::move(other.m_root);
    m_user = other.m_user;
    return *this;
  }

  void FileSystemEnv::loadUserEnv()
  {
    

    if (!Utils::checkIfDirectoryExists(m_rootPath))
    {
      if (!Utils::createDirectory(m_rootPath))
      {
        std::clog << "ERROR: failed to create user directory" << std::endl; // NOLINT(performance-avoid-endl)
        std::terminate();
      }
      m_root = std::make_unique<Directory>(m_user, m_user->getUsername(), "");
    }
    else
    {
      std::ifstream file{ m_rootPath + ".config_file_tree" };

      while (file)
      {
        if (file.peek() == static_cast<int>(SystemObjectType::directory))
        {
          
        }
        else
        {
          
        }
      }
    }
  }

  void FileSystemEnv::saveUserEnv() const
  {
    std::ofstream file{ ".config_file_tree" };
    std::queue<const Directory*> directories;

    directories.push(m_root.get());
    file << *m_root;

    while (!directories.empty())
    {
      auto dir = directories.front();
      directories.pop();
      for (const auto& kv : dir->getChildren())
      {
        file << *kv.second;
        if (kv.second->getType() == SystemObjectType::directory)
        {
          directories.push(dynamic_cast<const Directory*>(kv.second.get()));
        }
      }
    }
  }
} // namespace fs
