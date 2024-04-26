#include <filesystem>
#include <iostream>

#include "File.h"


namespace fs
{
  File::File(const User* user, const std::string& name, const std::string& parentPath)
  {
    setParentPath(parentPath);
    setType(SystemObjectType::file);
    setName(name);
    setOwner(user->getUsername());
  }

  File::File(std::istringstream& ifs)
  {
    ifs >> *this;
  }

  const std::string& File::getContent() const
  {
    return m_content;
  }

  void File::setContent(const std::string& content)
  {
    m_content = content;
  }

  void File::writeToFile(const std::string& content) const
  {
    const std::filesystem::path path{
      "./fs/home/" + (getParentPath().empty() ? "" : getParentPath() + '/') + getName()
    };
    std::fstream(path, std::ios::out | std::ios::trunc) << content << '\n';
  }

  void File::appendToFile(const std::string& content) const
  {
    const std::filesystem::path path{
      "./fs/home/" + (getParentPath().empty() ? "" : getParentPath() + '/') + getName()
    };
    std::fstream(path, std::ios::out | std::ios::app) << content << '\n';
  }

  void File::printFile() const
  {
    const std::filesystem::path path{
      "./fs/home/" + (getParentPath().empty() ? "" : getParentPath() + '/') + getName()
    };
    std::ifstream file{ path };
    std::string line;
    while (getline(file, line))
    {
      std::cout << line << '\n';
    }
  }

  size_t File::getSize() const
  {
    return 0;
  }
} // namespace fs
