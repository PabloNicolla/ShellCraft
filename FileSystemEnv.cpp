#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include "FileSystemEnv.h"
#include "File.h"
#include "PathResolver.h"
#include "Utils.h"

namespace fs
{
  const char* FileSystemEnv::m_usersHomePath{ "fs/home/" };

  FileSystemEnv::FileSystemEnv(const User* user) : m_user{ user }, m_rootPath{ m_usersHomePath + m_user->getUsername() }
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
      std::ifstream file{ m_rootPath / ".config_file_tree" };
      std::string line{};
      while (std::getline(file, line))
      {
        std::istringstream iss{ line };
        std::unique_ptr<SystemObject> sysObj;
        if (line[0] == '0' + static_cast<int>(SystemObjectType::directory))
        {
          sysObj = std::make_unique<Directory>(iss);
        }
        else
        {
          sysObj = std::make_unique<File>(iss);
        }
        (void)sysObj->getParentPath();
        //search dir
        //add to dir children
      }
    }
  }

  void FileSystemEnv::saveUserEnv() const
  {
    std::ofstream file{ m_rootPath / ".config_file_tree" };
    std::queue<const Directory*> directories;

    directories.push(m_root.get());
    file << *m_root;

    while (!directories.empty())
    {
      const auto dir = directories.front();
      directories.pop();
      for (const auto& [key, val] : dir->getChildren())
      {
        file << *val;
        if (val->getType() == SystemObjectType::directory)
        {
          directories.push(dynamic_cast<const Directory*>(val.get()));
        }
      }
    }
  }

  SystemObject* FileSystemEnv::searchSystemObject(const std::string& path)
  {
    const PathResolver pr{ path };

  }
} // namespace fs
