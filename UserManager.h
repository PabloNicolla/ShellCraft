#pragma once
#include <filesystem>
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
  const std::filesystem::path c_userManagerPath{ "./fs/userManager" };

  class UserManager
  {
    std::vector<User> m_users{};

  public:
    UserManager();
    ~UserManager();

    [[nodiscard]] std::optional<const User*> authenticate() const;
    void registerUser();
    void saveUsers();
    [[nodiscard]] bool usernameExists(std::string_view username) const;
    [[nodiscard]] std::optional<const User*> getUser(std::string_view username) const;
    [[nodiscard]] bool requestPassword(const std::string& username) const;

  private:
    void loadUsers();
    [[nodiscard]] std::optional<User> createUser() const;
    [[nodiscard]] static std::optional<std::string> getUsername();
    [[nodiscard]] static std::optional<std::string> getPassword();
    void static createEssentialFiles();
  };
} // namespace fs
