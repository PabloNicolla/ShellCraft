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

  const User* FileSystemEnv::getUser() const
  {
    return m_user;
  }

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
        std::clog << "ERROR: failed to create user directory\n\n";
      }
      m_root = std::make_unique<Directory>(m_user, m_user->getUsername(), "");
    }
    else
    {
      std::ifstream file{ m_rootPath / ".config_file_tree" };
      std::string line{};

      std::getline(file, line);
      std::istringstream rootData{ line };
      m_root = std::make_unique<Directory>(rootData);

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

        auto parent = searchSystemObject(sysObj->getParentPath());
        if (parent->getType() == SystemObjectType::directory)
        {
          if (!dynamic_cast<Directory*>(parent)->addChildren(std::move(sysObj)))
          {
            std::clog << "ERROR: failed to add children to directory\n\n";
          }
        }
        else
        {
          std::clog << "ERROR: trying to add an object to a non-directory-object\n\n";
        }
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
        file << '\n';
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
    Directory* currDir = m_root.get();
    for (const auto& dirName : pr.getParentPath())
    {
      if (const auto sysObj = currDir->searchChildren(dirName);
        sysObj && sysObj->getType() == SystemObjectType::directory)
      {
        currDir = dynamic_cast<Directory*>(sysObj);
      }
      else
      {
        return nullptr;
      }
    }
    if (path.empty())
    {
      return m_root.get();
    }
    return currDir->searchChildren(pr.getObjectName());
  }

  Directory* FileSystemEnv::getRoot()
  {
    return m_root.get();
  }
} // namespace fs
