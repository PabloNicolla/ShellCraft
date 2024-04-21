#pragma once
#include <vector>

#include "User.h"

namespace fileSystem
{
  constexpr int c_maxUsernameLength{ 25 };
  constexpr int c_minUsernameLength{ 3 };
  constexpr int c_maxPasswordLength{ 50 };
  constexpr int c_minPasswordLength{ 3 };

  class UserManager
  {
    std::vector<User> m_users{};

  public:
    UserManager();
    void authenticate();
    void registerUser();
    [[nodiscard]] bool usernameExists(std::string_view username) const;

  private:
    void loadUsers();
    [[nodiscard]] User getUser();
  };
} // namespace fileSystem
