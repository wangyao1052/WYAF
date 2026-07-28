///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_CMD_STACK_H
#define WYAP_CMD_STACK_H

#include <string>

#include <wyErrors.h>
#include <wyrxClassInfo.h>
#include <wyapDefs.h>
#include <wyapCommand.h>

NS_WYAP_BEG

class CmdStack
{
protected:
    CmdStack();
    virtual ~CmdStack();
    CmdStack(const CmdStack&) = delete;
    CmdStack& operator=(const CmdStack&) = delete;
    CmdStack(CmdStack&&) = delete;
    CmdStack& operator=(CmdStack&&) = delete;

public:
    // Get a command by name.
    // Returns nullptr if no command with the given name exists.
    virtual Command* getCommand(const std::string& commandName) const = 0;

    // Add a command.
    //
    // @param commandName Command name.
    // @param commandFlags Command flags.
    // @param cmdExecutionClassInfo Class information of the command execution.
    //        Must be CmdExecution or a class derived from it.
    //        Used to create an instance of the command execution.
    //
    // @return
    //   - Ok: command was added successfully.
    //   - CommandManagerBusy: the command manager is busy and the command
    //         stack cannot be modified now.
    //   - InvalidCommandName: commandName is empty or otherwise invalid.
    //   - NotCommandExecutionClass: cmdExecutionClassInfo is not CmdExecution
    //         or a class derived from it.
    //   - DuplicateKey: a command with the same name already exists.
    //   - TransparentCommandMustBeImmediate: a transparent command must use
    //         ImmediateCmdExecution.
    virtual wy::ErrorStatus addCommand(
        const std::string& commandName,
        unsigned int commandFlags,
        wyrx::ClassInfo* pCmdExecutionClassInfo) = 0;

    // Remove a command by name.
    //
    // @param commandName Name of the command to remove.
    //
    // @return
    //   - Ok: command was removed successfully.
    //   - CommandManagerBusy: the command manager is busy and the command
    //         stack cannot be modified now.
    //   - KeyNotFound: no command with the given name exists.
    virtual wy::ErrorStatus removeCommand(const std::string& commandName) = 0;
};

NS_WYAP_END

#endif // WYAP_CMD_STACK_H