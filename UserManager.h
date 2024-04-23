#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <vector>
#include "User.h"

namespace fs
{
  class UserManager
  {
  private:
    std::vector<User> m_users{};

  public:
    UserManager();
    ~UserManager();

    void registerUser();
    void saveUsers();
    [[nodiscard]] std::optional<const User*> authenticate() const;
    [[nodiscard]] bool usernameExists(std::string_view username) const;
    [[nodiscard]] std::optional<const User*> getUser(std::string_view username) const;
    [[nodiscard]] bool requestPassword(const std::string& username) const;

    static constexpr int maxUsernameLength{ 25 };
    static constexpr int minUsernameLength{ 3 };
    static constexpr int maxPasswordLength{ 50 };
    static constexpr int minPasswordLength{ 3 };
    static const std::filesystem::path userManagerPath;

  private:
    void loadUsers();
    void static createEssentialFiles();
    [[nodiscard]] std::optional<User> createUser() const;
    [[nodiscard]] static std::optional<std::string> getUsername();
    [[nodiscard]] static std::optional<std::string> getPassword();
  };
} // namespace fs
