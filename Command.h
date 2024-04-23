#pragma once
#include "Directory.h"
#include "Flags.h"

namespace shell
{
  class Command
  {
    fs::Directory* m_root{};
    fs::Directory* m_workingDir{};

  public:
    Command() = default;
    virtual ~Command() = default;
    Command(const Command&) = delete;
    Command(Command&& other) noexcept;
    Command& operator=(const Command&) = delete;
    Command& operator=(Command&& other) noexcept;

    virtual ShellFlag execute() = 0;
    virtual ShellFlag help() = 0;
    virtual std::vector<ResourceTypes> requiredResources() = 0;
    void setRoot(fs::Directory* root);
    void setWorkingDir(fs::Directory* workingDir);
  };


  class CommandExit final : public Command
  {
  public:
    CommandExit() = default;
    ShellFlag execute() override;
    ShellFlag help() override;
    std::vector<ResourceTypes> requiredResources() override;
    static /*constexpr*/ std::unique_ptr<Command> factory();
  };

  class CommandLogout final : public Command
  {
  public:
    CommandLogout() = default;
    ShellFlag execute() override;
    ShellFlag help() override;
    std::vector<ResourceTypes> requiredResources() override;
    static std::unique_ptr<Command> factory();
  };

  class CommandLs final : public Command
  {
  public:
    CommandLs() = default;
    ShellFlag execute() override;
    ShellFlag help() override;
    std::vector<ResourceTypes> requiredResources() override;
    static std::unique_ptr<Command> factory();
  };

  class CommandCd final : public Command
  {
  public:
    CommandCd() = default;
    ShellFlag execute() override;
    ShellFlag help() override;
    std::vector<ResourceTypes> requiredResources() override;
    static Command* factory();
  };

  class CommandCat final : public Command
  {
  public:
    CommandCat() = default;
    ShellFlag execute() override;
    ShellFlag help() override;
    std::vector<ResourceTypes> requiredResources() override;
    static Command* factory();
  };

  class CommandClear final : public Command
  {
  public:
    CommandClear() = default;
    ShellFlag execute() override;
    ShellFlag help() override;
    std::vector<ResourceTypes> requiredResources() override;
    static Command* factory();
  };

  class CommandMkdir final : public Command
  {
  public:
    CommandMkdir() = default;
    ShellFlag execute() override;
    ShellFlag help() override;
    std::vector<ResourceTypes> requiredResources() override;
    static Command* factory();
  };
} // namespace shell
