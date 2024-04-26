#pragma once
#include <vector>
#include "Directory.h"
#include "File.h"
#include "FileSystemEnv.h"
#include "Flags.h"
#include "PathResolver.h"
#include "Tokenizer.h"

namespace shell
{
  class Command
  {
  protected:
    fs::Directory* m_root{};
    fs::Directory* m_workingDir{};
    fs::FileSystemEnv* m_env{};
    const std::unordered_map<std::string, std::unique_ptr<Command>(*)()>* m_commands{};

  public:
    Command() = default;
    virtual ~Command() = default;
    Command(const Command&) = delete;
    Command(Command&& other) noexcept;
    Command& operator=(const Command&) = delete;
    Command& operator=(Command&& other) noexcept;

    virtual ShellFlag execute(const Tokenizer& tokenizer) = 0;
    virtual void help() const = 0;
    virtual std::vector<ResourceTypes> requiredResources() = 0;
    void setRoot(fs::Directory* root);
    void setWorkingDir(fs::Directory* workingDir);
    void setEnv(fs::FileSystemEnv* env);
    void setCommands(const std::unordered_map<std::string, std::unique_ptr<Command>(*)()>* commands);
    [[nodiscard]] fs::Directory* getWorkingDir() const;
    [[nodiscard]] std::string& cmdPathResolution(std::string& path) const;
    [[nodiscard]] fs::Directory* ensureFoundDirectory(const Tokenizer& tokenizer) const;
    [[nodiscard]] fs::Directory* ensureFoundParentDir(const fs::PathResolver& pr) const;
    [[nodiscard]] fs::SystemObject* ensureFoundSysObj(const fs::PathResolver& pr) const;
  };


  class CommandExit final : public Command
  {
  public:
    CommandExit() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    void help() const override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer) const;
  };

  class CommandLogout final : public Command
  {
  public:
    CommandLogout() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    void help() const override;
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
    void help() const override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer);
  };

  class CommandCd final : public Command
  {
    fs::Directory* m_target{};

  public:
    CommandCd() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    void help() const override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer);
  };

  class CommandCat final : public Command
  {
    fs::File* m_fileTarget{};

  public:
    CommandCat() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    void help() const override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer);
  };

  class CommandClear final : public Command
  {
  public:
    CommandClear() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    void help() const override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer) const;
  };

  class CommandMkdir final : public Command
  {
    fs::Directory* m_pDirectory{};
    std::string m_dirName{};

  public:
    CommandMkdir() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    void help() const override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer);
  };

  class CommandRmdir final : public Command
  {
    fs::Directory* m_pDirectory{};
    std::string m_dirName{};

  public:
    CommandRmdir() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    void help() const override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer);
  };

  class CommandTouch final : public Command
  {
    fs::Directory* m_pDirectory{};
    std::string m_fileName{};

  public:
    CommandTouch() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    void help() const override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer);
  };

  class CommandEcho final : public Command
  {
    fs::File* m_fileTarget{};

  public:
    CommandEcho() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    void help() const override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer);
  };

  class CommandRm final : public Command
  {
    fs::Directory* m_pDirectory{};
    fs::SystemObjectType m_objType{};
    std::string m_sysObjName{};

  public:
    enum Flags { invalid = -1, r };

    CommandRm() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    void help() const override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer);
    [[nodiscard]] Flags stringToFlag(const std::string& str) const;
  };

  class CommandHelp final : public Command
  {
  public:
    CommandHelp() = default;
    [[nodiscard]] ShellFlag execute(const Tokenizer& tokenizer) override;
    void help() const override;
    [[nodiscard]] std::vector<ResourceTypes> requiredResources() override;
    [[nodiscard]] static std::unique_ptr<Command> factory();
    [[nodiscard]] bool validateTokens(const Tokenizer& tokenizer) const;
  };
} // namespace shell
