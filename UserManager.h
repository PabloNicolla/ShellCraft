#pragma once
#include <vector>

#include "User.h"

namespace fileSystem
{
  class UserManager
  {
    std::vector<User> m_users{};

  public:
    UserManager();
    void authenticate();
    void registerUser();

  private:
    void loadUsers();
  };
} // namespace fileSystem
