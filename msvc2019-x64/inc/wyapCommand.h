///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_COMMAND_H
#define WYAP_COMMAND_H

#include <string>
#include <wyapDefs.h>

NS_WYAP_BEG

// Command flags (bitwise combination of the following macros).
// Modal command:
//   - Runs as the main command.
//   - While a modal command is running, another modal command cannot be started.
//   - A transparent command can still be started during a modal command.
//
// Transparent command:
//   - Can be started while a modal command is running.
//   - Can also be started independently when no modal command is running.
//   - Must be an immediate command, not an interactive command.
//   - If started during a modal command, control returns to that modal command
//     after the transparent command finishes.
#define WYAP_CMD_MODAL         0x00000000  // Modal command.
#define WYAP_CMD_TRANSPARENT   0x00000001  // Transparent command.
#define WYAP_CMD_USEPICKFIRST  0x00000002  // Use preselection set (similar to AutoCAD).
#define WYAP_CMD_NOHISTORY     0x00000010  // Do not add to command history.

class Command
{
protected:
    Command();
    virtual ~Command();
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;
    Command(Command&&) = delete;
    Command& operator=(Command&&) = delete;

public:
    // Command name.
    virtual const std::string& getName() const = 0;

    // Command flags (bitwise combination of WYAP_CMD_* macros).
    virtual unsigned int getFlags() const = 0;

    // Whether it is a modal command.
    inline bool isModal() const
    {
        return (this->getFlags() & WYAP_CMD_TRANSPARENT) == WYAP_CMD_MODAL;
    }

    // Whether it is a transparent command.
    inline bool isTransparent() const
    {
        return this->getFlags() & WYAP_CMD_TRANSPARENT;
    }
};

NS_WYAP_END

#endif // WYAP_COMMAND_H