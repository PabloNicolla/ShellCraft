#pragma once
#include <ctime>
#include <fstream>
#include <string>
#include "Flags.h"


namespace fs
{
  class SystemObject
  {
    SystemObjectType m_type;
    std::string m_name{};
    std::string m_owner{};
    std::string m_parentPath{};
    std::time_t m_created{};  // Timestamp for file creation
    std::time_t m_modified{}; // Timestamp for last modification
    size_t m_size{};
    int m_permissions{}; // Represented as an octal number, e.g., 0755

  public:
    virtual ~SystemObject() = default;

    [[nodiscard]] SystemObjectType getType() const;
    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] const std::string& getOwner() const;
    [[nodiscard]] const std::string& getParentPath() const;
    [[nodiscard]] std::time_t getCreated() const;
    [[nodiscard]] std::time_t getModified() const;
    [[nodiscard]] virtual size_t getSize() const = 0;
    [[nodiscard]] int getPermissions() const;

    void setType(const SystemObjectType& type);
    void setName(const std::string& name);
    void setOwner(const std::string& owner);
    void setParentPath(const std::string& parentPath);
    void setCreated(std::time_t created);
    void setModified(std::time_t modified);
    void setPermissions(int permissions);

    std::ofstream& saveToFile(std::ofstream& ofs) const;
    std::istringstream& readFromFile(std::istringstream& ifs);
    friend std::ofstream& operator<<(std::ofstream& ofs, const SystemObject& so);
    friend std::istringstream& operator>>(std::istringstream& ifs, SystemObject& so);
  };
} // namespace fs
