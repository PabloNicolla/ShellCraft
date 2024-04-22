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

  size_t File::getSize() const
  {
    return 1;
  }
} // namespace fs
