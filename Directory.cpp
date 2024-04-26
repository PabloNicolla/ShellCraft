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

  SystemObject* Directory::searchChildren(const std::string& key)
  {
    if (const auto it = m_children.find(key); it != m_children.end())
    {
      return &*it->second;
    }
    return nullptr;
  }

  bool Directory::addChildren(std::unique_ptr<SystemObject> so)
  {
    if (searchChildren(so->getName()))
    {
      return false;
    }
    m_children[so->getName()] = std::move(so);
    return true;
  }

  void Directory::removeChild(const std::string& key)
  {
    m_children[key]->cascadeDelete();
    m_children.erase(key);
  }

  void Directory::cascadeDelete()
  {
    for (auto& [k, v] : m_children)
    {
      v->cascadeDelete();
    }
  }
} // namespace fs
