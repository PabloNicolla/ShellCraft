#pragma once

#include <ctime>
#include <string>

namespace fileSystem
{
  class Metadata
  {
    std::string m_name;
    std::time_t m_created;  // Timestamp for file creation
    std::time_t m_modified; // Timestamp for last modification
    int m_permissions;      // Represented as an octal number, e.g., 0755

    Metadata(const std::string& name)
    {
    }
  };
} // namespace fileSystem
