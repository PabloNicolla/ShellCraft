#pragma once
#include "SystemObject.h"
#include "User.h"

namespace fs
{
  class File : public SystemObject
  {
    std::string m_content;

  public:
    File(const User* user, const std::string& name, const std::string& parentPath);
    [[nodiscard]] const std::string& getContent() const;
    void setContent(const std::string& content);
  };
} // namespace fs
