#include "Directory.h"

namespace fs
{
  Directory::Directory(const User* user, const std::string& name, const std::string& parentPath)
  {
    setName(name);
    setOwner(user->getUsername());
    setParentPath(parentPath);
    setType(SystemObjectType::directory);
  }

  Directory::Directory(std::istringstream& ifs)
  {
    ifs >> *this;
  }

  size_t Directory::getSize() const
  {
    return 1;
  }

  const std::unordered_map<std::string, std::unique_ptr<SystemObject>>& Directory::getChildren() const
  {
    return m_children;
  }
} // namespace fs
