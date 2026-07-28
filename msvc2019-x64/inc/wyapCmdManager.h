///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_CMD_MANAGER_H
#define WYAP_CMD_MANAGER_H

#include <string>
#include <vector>

#include <wyErrors.h>
#include <wyapDefs.h>
#include <wyapCommand.h>
#include <wyapCmdExecution.h>

NS_WYAP_BEG

class CmdManagerReactor;
class CmdManagerImpl;
class SystemInternals;

// Command manager.
// Handles synchronous execution, asynchronous posting, and abortion of
// commands, as well as management of command reactors.
class WYAP_EXPORT CmdManager
{
private:
    CmdManager();
    ~CmdManager();
    CmdManager(const CmdManager&) = delete;
    CmdManager& operator=(const CmdManager&) = delete;
    CmdManager(CmdManager&&) = delete;
    CmdManager& operator=(CmdManager&&) = delete;
    friend class SystemInternals;

public:
    // Execute a command synchronously.
    // The call blocks until an immediate command completes,
    // or until an interactive command has started. After this
    // call returns, the interactive command continues its interaction.
    // If a modal command is already running, another modal command cannot be
    // started, but a transparent command is still allowed.
    //
    // @param commandName Name of the command to execute.
    //
    // @return
    //   - Ok: an immediate command completed successfully, or an
    //         interactive command started successfully.
    //   - SystemBusy: synchronous execution is not allowed because
    //         DocumentManager, EnvironmentManager, CommandManager,
    //         or the asynchronous request scheduler is busy.
    //   - CommandNotFound: no command is registered with the given name.
    //   - ModalCommandInProgress: another modal command is currently running,
    //         so a new modal command cannot be started.
    //   - CommandRejected: the command was rejected during start.
    //   - CommandCanceled: the command was canceled during start.
    //   - CommandStartFailed: the command failed during start.
    //   - CommandFailed: an immediate command returned a non-zero value from run().
    wy::ErrorStatus executeCommand(const std::string& commandName);

    // Post a command for asynchronous execution.
    // The command is enqueued and will be executed later when the asynchronous
    // request scheduler processes pending requests.
    //
    // @param commandName Name of the command to post.
    //
    // @return
    //   - Ok: the command was enqueued successfully.
    wy::ErrorStatus postCommand(const std::string& commandName);

    // Abort the currently running modal command.
    //
    // @param abortCause Reason for aborting the command.
    //
    // @return
    //   - Ok: the current modal command was aborted.
    //   - SystemBusy: the current modal command cannot be aborted because
    //         DocumentManager, EnvironmentManager, CommandManager,
    //         or the asynchronous request scheduler is busy.
    //   - NoCurrentModalCommand: there is no current modal command running.
    wy::ErrorStatus abortCurrentModalCommand(
        wyap::CmdExecution::AbortCause abortCause);

    // Get the current modal command.
    // Returns nullptr if there is no modal command currently executing.
    Command* getCurrentModalCommand() const;

    // Get the current modal command execution.
    // Returns nullptr if there is no modal command currently executing.
    CmdExecution* getCurrentModalCmdExecution() const;

    // Get the list of recently executed command names.
    // Commands with the WYAP_CMD_NOHISTORY flag are not included in the recent command list.
    std::vector<std::string> getRecentCommands() const;

    // Add a reactor.
    // @param pReactor Pointer to the reactor to add.
    // @return
    //   - Ok: reactor was added successfully.
    //   - CommandManagerBusy: command manager is busy and adding reactors is not allowed now.
    //   - NullInputPointer: pReactor is nullptr.
    //   - DuplicateKey: pReactor has already been added.
    wy::ErrorStatus addReactor(CmdManagerReactor* pReactor);

    // Remove a reactor.
    // @param pReactor Pointer to the reactor to remove.
    // @return
    //   - Ok: reactor was removed successfully.
    //   - CommandManagerBusy: command manager is busy and removing reactors is not allowed now.
    //   - NullInputPointer: pReactor is nullptr.
    //   - KeyNotFound: pReactor is not in the reactor list.
    wy::ErrorStatus removeReactor(CmdManagerReactor* pReactor);

private:
    CmdManagerImpl* _pImpl;
};

// Command manager reactor.
class WYAP_EXPORT CmdManagerReactor
{
public:
    CmdManagerReactor() = default;
    virtual ~CmdManagerReactor() = default;

    // Called right before a command starts executing.
    virtual void onCommandWillStart(wyap::Command* pCmd) {}

    // Called right after a command starts.
    virtual void onCommandStarted(wyap::Command* pCmd) {}

    // Called when a command failed to start.
    // @param startResult Result returned by onStart() of the command execution.
    virtual void onCommandStartFailed(
        wyap::Command* pCmd,
        wyap::CmdExecution::StartResult startResult) {}

    // Called when a command finishes normally (neither aborted nor failed).
    virtual void onCommandEnded(wyap::Command* pCmd) {}

    // Called when a command is aborted.
    // @param abortCause Reason why the command was aborted.
    virtual void onCommandAborted(
        wyap::Command* pCmd,
        wyap::CmdExecution::AbortCause abortCause) {}
};

NS_WYAP_END

#endif // WYAP_CMD_MANAGER_H