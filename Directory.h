#pragma once
#include <unordered_map>
#include "SystemObject.h"
#include "User.h"

namespace fs
{
  class Directory : public SystemObject
  {
    std::unordered_map<std::string, SystemObject> m_children{};

  public:
    Directory(const User* user, const std::string& name, const std::string& parentPath);
  };
} // namespace fs
