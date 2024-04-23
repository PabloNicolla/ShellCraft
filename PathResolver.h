#pragma once
#include <string>
#include <vector>

namespace fs
{
  class PathResolver
  {
  private:
    std::vector<std::string> m_parentPath{};
    std::string m_objectName;

  public:
    explicit PathResolver(const std::string& path);
    [[nodiscard]] const std::vector<std::string>& getParentPath() const;
    [[nodiscard]] const std::string& getObjectName() const;
  };
} // namespace fs
