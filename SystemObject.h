#pragma once
#include <ctime>
#include <string>
#include <fstream>

namespace fs
{
  enum class SystemObjectType { file, directory };

  class SystemObject
  {
    std::string m_name{};
    std::string m_owner{};
    std::time_t m_created{};  // Timestamp for file creation
    std::time_t m_modified{}; // Timestamp for last modification
    std::string m_parentPath{};
    size_t m_size{};
    int m_permissions{}; // Represented as an octal number, e.g., 0755
    SystemObjectType m_type;

  public:
    virtual ~SystemObject() = default;

    [[nodiscard]] const std::string& getName() const;
    void setName(const std::string& name);
    [[nodiscard]] const std::string& getOwner() const;
    void setOwner(const std::string& owner);
    [[nodiscard]] std::time_t getCreated() const;
    void setCreated(std::time_t created);
    [[nodiscard]] std::time_t getModified() const;
    void setModified(std::time_t modified);
    [[nodiscard]] const std::string& getParentPath() const;
    void setParentPath(const std::string& parentPath);
    [[nodiscard]] virtual size_t getSize() const = 0;
    [[nodiscard]] int getPermissions() const;
    void setPermissions(int permissions);
    [[nodiscard]] SystemObjectType getType() const;
    void setType(const SystemObjectType& type);

    std::ofstream& saveToFile(std::ofstream& ofs) const;
    friend std::ofstream& operator<<(std::ofstream& ofs, const SystemObject& so);

    std::istringstream& readFromFile(std::istringstream& ifs);
    friend std::istringstream& operator>>(std::istringstream& ifs, SystemObject& so);
  };
} // namespace fs
