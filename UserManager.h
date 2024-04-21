#pragma once
#include <optional>
#include <string>
#include <vector>

#include "User.h"

namespace fs
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
    [[nodiscard]] std::optional<const User*> authenticate() const;
    void registerUser();
    [[nodiscard]] bool usernameExists(std::string_view username) const;
    [[nodiscard]] std::optional<const User*> getUser(std::string_view username) const;

  private:
    void loadUsers();
    [[nodiscard]] std::optional<User> createUser() const;
    [[nodiscard]] std::optional<std::string> getUsername() const;
    [[nodiscard]] std::optional<std::string> getPassword() const;
  };
} // namespace fs
