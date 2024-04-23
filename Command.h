#pragma once
#include <vector>
#include "Directory.h"
#include "FileSystemEnv.h"
#include "Flags.h"
#include "Tokenizer.h"

namespace shell
{
  class Command
  {
  protected:
    fs::Directory* m_root{};
    fs::Directory* m_workingDir{};
    fs::FileSystemEnv* m_env{};

  public:
    Command() = default;
    virtual ~Command() = default;
    Command(const Command&) = delete;
    Command(Command&& other) noexcept;
    Command& operator=(const Command&) = delete;
    Command& operator=(Command&& other) noexcept;

    virtual ShellFlag execute(const Tokenizer& tokenizer) = 0;
    virtual ShellFlag help() = 0;
    virtual std::vector<ResourceTypes> requiredResources() = 0;
    void setRoot(fs::Directory* root);
    void setWorkingDir(fs::Directory* workingDir);
    void setEnv(fs::FileSystemEnv* env);
    [[nodiscard]] std::string& cmdPathResolution(std::string& path) const;
  };


  class CommandExit final : public Command
  {
  public:
    CommandExit() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    [[nodiscard]] ShellFlag help() override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer) const;
  };

  class CommandLogout final : public Command
  {
  public:
    CommandLogout() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    [[nodiscard]] ShellFlag help() override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer) const;
  };

  class CommandLs final : public Command
  {
    fs::Directory* m_target{};
  public:
    CommandLs() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    [[nodiscard]] ShellFlag help() override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer);
  };

  class CommandCd final : public Command
  {
  public:
    CommandCd() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    [[nodiscard]] ShellFlag help() override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer) const;
  };

  class CommandCat final : public Command
  {
  public:
    CommandCat() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    [[nodiscard]] ShellFlag help() override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer) const;
  };

  class CommandClear final : public Command
  {
  public:
    CommandClear() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    [[nodiscard]] ShellFlag help() override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer) const;
  };

  class CommandMkdir final : public Command
  {
  public:
    CommandMkdir() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    [[nodiscard]] ShellFlag help() override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer) const;
  };
} // namespace shell
