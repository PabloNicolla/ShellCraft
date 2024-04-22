#pragma once
#include "SystemObject.h"
#include "User.h"

namespace fs
{
  class File final : public SystemObject
  {
    std::string m_content;

  public:
    File(const User* user, const std::string& name, const std::string& parentPath);
    [[nodiscard]] const std::string& getContent() const;
    void setContent(const std::string& content);
    [[nodiscard]] size_t getSize() const override;
  };
} // namespace fs
