#pragma once
#include <fstream>
#include <memory>
#include <unordered_map>
#include "SystemObject.h"
#include "User.h"

namespace fs
{
  class Directory final : public SystemObject
  {
    std::unordered_map<std::string, std::unique_ptr<SystemObject>> m_children{};

  public:
    Directory(const User* user, const std::string& name, const std::string& parentPath);
    explicit Directory(std::istringstream& ifs);
    [[nodiscard]] size_t getSize() const override;
    [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<SystemObject>>& getChildren() const;
  };
} // namespace fs
