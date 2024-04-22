#pragma once
#include "CommandProcessor.h"

namespace shell
{
  enum class CommandNames
  {
    ls, cd, pwd, touch, mkdir, rmdir, echo, exit, cat, logout, clear, history, user, help, man
  };

  class Command
  {
    virtual ~Command();
    virtual ShellFlag execute() = 0;
    virtual ShellFlag help() = 0;
    virtual void requiredResources() = 0;
  };
} // namespace shell
