#include "SystemObject.h"

namespace fs
{
  const std::string& SystemObject::getName() const
  {
    return m_name;
  }

  void SystemObject::setName(const std::string& name)
  {
    m_name = name;
  }

  const std::string& SystemObject::getOwner() const
  {
    return m_owner;
  }

  void SystemObject::setOwner(const std::string& owner)
  {
    m_owner = owner;
  }

  std::time_t SystemObject::getCreated() const
  {
    return m_created;
  }

  void SystemObject::setCreated(const std::time_t created)
  {
    m_created = created;
  }

  std::time_t SystemObject::getModified() const
  {
    return m_modified;
  }

  void SystemObject::setModified(const std::time_t modified)
  {
    m_modified = modified;
  }

  const std::string& SystemObject::getParentPath() const
  {
    return m_parentPath;
  }

  void SystemObject::setParentPath(const std::string& parentPath)
  {
    m_parentPath = parentPath;
  }

  size_t SystemObject::getSize() const
  {
    return m_size;
  }

  int SystemObject::getPermissions() const
  {
    return m_permissions;
  }

  void SystemObject::setPermissions(const int permissions)
  {
    m_permissions = permissions;
  }

  SystemObjectType SystemObject::getType() const
  {
    return m_type;
  }

  void SystemObject::setType(const SystemObjectType& type)
  {
    m_type = type;
  }
} // namespace fs
