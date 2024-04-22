#include <sstream>

#include "PathResolver.h"

namespace fs
{
  PathResolver::PathResolver(const std::string& path)
  {
    std::istringstream iss{ path };
    std::string token{};
    while (std::getline(iss, token, '/'))
    {
      if (iss.eof())
      {
        m_objectName = token;
      }
      else
      {
        m_parentPath.push_back(token);
      }
    }
  }

  const std::vector<std::string>& PathResolver::getParentPath() const
  {
    return m_parentPath;
  }

  const std::string& PathResolver::getObjectName() const
  {
    return m_objectName;
  }
} // namespace fs
