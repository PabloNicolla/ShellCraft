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
} // namespace fs
