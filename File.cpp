#include <algorithm>
#include <filesystem>
#include <iostream>

#include "File.h"


namespace fs
{
  File::File(const User* user, const std::string& name, const std::string& parentPath)
  {
    setParentPath(parentPath);
    setType(SystemObjectType::file);
    setName(name);
    setOwner(user->getUsername());
  }

  File::File(std::istringstream& ifs)
  {
    ifs >> *this;
  }

  const std::string& File::getContent() const
  {
    return m_content;
  }

  void File::setContent(const std::string& content)
  {
    m_content = content;
  }

  void File::writeToFile(const std::string& content) const
  {
    std::string parentPath = getParentPath().empty() ? "" : getParentPath() + '/';
    adaptParentPath(parentPath);
    const std::filesystem::path path{
      "./fs/home/" + getOwner() + '/' + parentPath + getName()
    };
    std::fstream(path, std::ios::out | std::ios::trunc) << content << '\n';
  }

  void File::appendToFile(const std::string& content) const
  {
    std::string parentPath = getParentPath().empty() ? "" : getParentPath() + '/';
    adaptParentPath(parentPath);
    const std::filesystem::path path{
      "./fs/home/" + getOwner() + '/' + parentPath + getName()
    };
    std::fstream(path, std::ios::out | std::ios::app) << content << '\n';
  }

  void File::printFile() const
  {
    std::string parentPath = getParentPath().empty() ? "" : getParentPath() + '/';
    adaptParentPath(parentPath);
    const std::filesystem::path path{
      "./fs/home/" + getOwner() + '/' + parentPath + getName()
    };

    std::ifstream file{ path };
    std::string line;
    while (getline(file, line))
    {
      std::cout << line << '\n';
    }
  }

  void File::cascadeDelete()
  {
    std::string parentPath = getParentPath().empty() ? "" : getParentPath() + '/';
    adaptParentPath(parentPath);
    const std::filesystem::path path{
      "./fs/home/" + getOwner() + '/' + parentPath + getName()
    };
    std::filesystem::remove(path);
  }

  size_t File::getSize() const
  {
    return 0;
  }

  std::string& File::adaptParentPath(std::string& parentPath)
  {
    replace_if(parentPath.begin(), parentPath.end(), [](const char c)
    {
      return c == '/';
    }, '_');
    return parentPath;
  }
} // namespace fs
