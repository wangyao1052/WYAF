///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_CMD_EXECUTION_H
#define WYAP_CMD_EXECUTION_H

#include <wyErrors.h>
#include <wyrxObject.h>
#include <wyapDefs.h>
#include <wyapCommand.h>

NS_WYAP_BEG

class CmdExecutionImpl;

// Command execution base class.
class WYAP_EXPORT CmdExecution : public wyrx::Object
{
    WYRX_DECLARE_MEMBERS(CmdExecution, wyap::CmdExecution, wyrx::Object)
private:
    CmdExecution();
protected:
    virtual ~CmdExecution();
    CmdExecution(const CmdExecution&) = delete;
    CmdExecution& operator=(const CmdExecution&) = delete;
    CmdExecution(CmdExecution&&) = delete;
    CmdExecution& operator=(CmdExecution&&) = delete;
    friend class ImmediateCmdExecution;
    friend class InteractiveCmdExecution;
    friend class SystemInternals;

public:
    // Command start result.
    enum class StartResult : unsigned short
    {
        Succeeded = 1, // Command started successfully.
        Failed    = 2, // Command start failed because of an internal error or exception.
        Rejected  = 3, // Command start was rejected because preconditions were not satisfied.
        Canceled  = 4, // Command start was canceled.
    };

    // Command abort cause.
    enum class AbortCause : unsigned short
    {
        UserCancel       = 1,   // Aborted by user action.
        ErrorTerminate   = 2,   // Aborted because an error occurred during execution.
        ForceTerminate   = 3,   // Aborted forcibly by the framework or an external system.
        UserDefinedBegin = 100, // Values from 100 and above are reserved for user-defined causes.
    };

    // Get the owning command.
    Command* getCommand() const;

    // Called by CommandManager when the command is being started.
    // Override this function to perform validation and initialization required
    // for command start. The return value determines whether command start succeeds.
    //
    // @return
    //   - Succeeded: the command started successfully and will enter
    //     its execution phase.
    //   - Rejected: the command start was rejected because preconditions
    //     were not satisfied.
    //   - Failed: the command start failed because of an internal error
    //     or exception.
    //   - Canceled: the command start was canceled.
    //
    // @note
    //   - If the return value is not Succeeded, the command will not enter
    //     its execution phase.
    //   - Any cleanup required for an unsuccessful start must be completed here.
    //   - The default implementation clears the current preselection set unless
    //     the command has the WYAP_CMD_USEPICKFIRST flag.
    virtual StartResult onStart();

    // Called by CommandManager when the command is ending successfully.
    // Override this function to perform finalization and cleanup
    // required for successful command completion, such as releasing
    // resources or committing a transaction.
    virtual void onEnd();

    // Called by CommandManager when the command is being aborted.
    // Override this function to perform finalization and cleanup
    // required for command abortion, such as releasing resources
    // or rolling back a transaction.
    //
    // @param cause Reason why the command is aborted, for example
    //        UserCancel, ErrorTerminate, or ForceTerminate.
    virtual void onAbort(AbortCause cause);

protected:
    CmdExecutionImpl* _pImpl;
};

// Immediate command execution.
// Used for commands that complete during a single call to run().
class WYAP_EXPORT ImmediateCmdExecution : public CmdExecution
{
    WYRX_DECLARE_MEMBERS(ImmediateCmdExecution, wyap::ImmediateCmdExecution, wyap::CmdExecution)
protected:
    ImmediateCmdExecution();
    virtual ~ImmediateCmdExecution();
    ImmediateCmdExecution(const ImmediateCmdExecution&) = delete;
    ImmediateCmdExecution& operator=(const ImmediateCmdExecution&) = delete;
    ImmediateCmdExecution(ImmediateCmdExecution&&) = delete;
    ImmediateCmdExecution& operator=(ImmediateCmdExecution&&) = delete;

public:
    // Called by CommandManager when the command is being executed.
    //
    // @return
    //   - 0: command execution succeeded. The command framework will
    //     automatically end the command.
    //   - non-zero: command execution failed. The command framework will
    //     automatically abort the command.
    virtual int run() { return 0; }
};

// Interactive command execution.
// Used for commands that start successfully, remain active, and process
// GUI input events (for example, mouse and keyboard events). They finish
// later by calling requestEnd() or requestAbort().
// A running modal command may also be aborted externally by CommandManager
// through abortCurrentModalCommand().
// Transparent commands must not use this execution type.
class WYAP_EXPORT InteractiveCmdExecution : public CmdExecution
{
    WYRX_DECLARE_MEMBERS(InteractiveCmdExecution, wyap::InteractiveCmdExecution, wyap::CmdExecution)
protected:
    InteractiveCmdExecution();
    virtual ~InteractiveCmdExecution();
    InteractiveCmdExecution(const InteractiveCmdExecution&) = delete;
    InteractiveCmdExecution& operator=(const InteractiveCmdExecution&) = delete;
    InteractiveCmdExecution(InteractiveCmdExecution&&) = delete;
    InteractiveCmdExecution& operator=(InteractiveCmdExecution&&) = delete;

protected:
    // Request synchronous successful completion of this command.
    // CommandManager immediately performs the successful end flow.
    //
    // @return
    //   - Ok: the command ended successfully.
    //   - SystemBusy: the command cannot be ended because
    //     DocumentManager, EnvironmentManager, CommandManager,
    //     or the asynchronous request scheduler is busy.
    wy::ErrorStatus requestEnd();

    // Request synchronous abortion of this command.
    // CommandManager immediately performs the abort flow.
    //
    // @param cause Reason why the command is being aborted.
    //
    // @return
    //   - Ok: the command was aborted.
    //   - SystemBusy: the command cannot be aborted because
    //     DocumentManager, EnvironmentManager, CommandManager,
    //     or the asynchronous request scheduler is busy.
    wy::ErrorStatus requestAbort(AbortCause cause);
};

NS_WYAP_END

#endif // WYAP_CMD_EXECUTION_H