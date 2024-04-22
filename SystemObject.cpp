#include <sstream>
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

  std::ofstream& SystemObject::saveToFile(std::ofstream& ofs) const
  {
    ofs << static_cast<int>(m_type) << "|"
      << m_name << "|"
      << m_owner << "|"
      << m_created << "|"
      << m_modified << "|"
      << m_parentPath << "|"
      << m_size << "|"
      << m_permissions;
    return ofs;
  }

  std::istringstream& SystemObject::readFromFile(std::istringstream& ifs)
  {
    std::string token;

    // Read type
    std::getline(ifs, token, '|');
    m_type = static_cast<SystemObjectType>(std::stoi(token));

    // Read name
    std::getline(ifs, m_name, '|');

    // Read owner
    std::getline(ifs, m_owner, '|');

    // Read created
    std::getline(ifs, token, '|');
    m_created = static_cast<std::time_t>(std::stol(token));

    // Read modified
    std::getline(ifs, token, '|');
    m_modified = static_cast<std::time_t>(std::stol(token));

    // Read parentPath
    std::getline(ifs, m_parentPath, '|');

    // Read size
    std::getline(ifs, token, '|');
    m_size = std::stoull(token);

    // Read permissions
    std::getline(ifs, token, '|');
    m_permissions = std::stoi(token);
    return ifs;
  }

  std::istringstream& operator>>(std::istringstream& ifs, SystemObject& so)
  {
    return so.readFromFile(ifs);
  }

  std::ofstream& operator<<(std::ofstream& ofs, const SystemObject& so)
  {
    return so.saveToFile(ofs);
  }
} // namespace fs
