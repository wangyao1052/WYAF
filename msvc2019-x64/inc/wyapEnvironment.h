///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_ENVIRONMENT_H
#define WYAP_ENVIRONMENT_H

#include <string>

#include <wydbDatabase.h>
#include <wyapDefs.h>
#include <wyapEnvironmentId.h>
#include <wyapCmdManager.h>
#include <wyapDocManager.h>

NS_WYAP_BEG

class EnvironmentImpl;
class GatewayEnvironment;
class DocumentEnvironment;
class TaskEnvironment;
class EnvManagerImpl;
class SystemInternals;

class WYAP_EXPORT Environment : public CmdManagerReactor
{
private:
    Environment();
public:
    virtual ~Environment();
    Environment(const Environment&) = delete;
    Environment& operator=(const Environment&) = delete;
    Environment(Environment&&) = delete;
    Environment& operator=(Environment&&) = delete;
    friend class GatewayEnvironment;
    friend class DocumentEnvironment;
    friend class TaskEnvironment;
    friend class EnvManagerImpl;
    friend class SystemInternals;

    // Environment type.
    // Environment nesting must follow this order: Gateway->Document->Task.
    // Additional nesting inside Task environments is allowed,
    // and its validity must be ensured by the library caller.
    enum class Type : unsigned short
    {
        Gateway   = 1,
        Document  = 2,
        Task      = 3,
    };

    // Get environment type.
    virtual Type getType() const = 0;

    // Get environment id.
    EnvironmentId getId() const;

    // Get environment name.
    const std::string& getName() const;
    // Set environment name.
    void setName(const std::string& name);

    // Environment lifecycle status.
    // None means the environment instance has been created
    // but has not yet been managed by EnvManager.
    enum class Status : unsigned short
    {
        None      = 0,
        Active    = 1,
        Suspended = 2,
        Exited    = 3,
    };

    // Get environment status.
    Status getStatus() const;

    // Exit code used when leaving an environment.
    enum class ExitCode
    {
        Ok     = 0, // confirm (commit changes)
        Cancel = 1, // cancel (discard changes)
    };

protected:
    // Called during environment entering, before status changes to Active.
    // Base implementation subscribes to CmdManager notifications.
    virtual void onEnter();

    // Called during environment exiting, before status changes to Exited.
    // Base implementation unsubscribes from CmdManager notifications.
    virtual void onExit(ExitCode exitCode);

    // Called during environment suspending, before status changes to Suspended.
    // Base implementation unsubscribes from CmdManager notifications.
    virtual void onSuspend();

    // Called during environment resuming, before status changes to Active.
    // Base implementation subscribes to CmdManager notifications.
    virtual void onResume();

private:
    EnvironmentImpl* _pImpl;
};

class WYAP_EXPORT GatewayEnvironment
    : public Environment
    , public DocManagerReactor
{
protected:
    GatewayEnvironment();
public:
    virtual ~GatewayEnvironment();
    GatewayEnvironment(const GatewayEnvironment&) = delete;
    GatewayEnvironment& operator=(const GatewayEnvironment&) = delete;
    GatewayEnvironment(GatewayEnvironment&&) = delete;
    GatewayEnvironment& operator=(GatewayEnvironment&&) = delete;

    virtual Type getType() const override
    {
        return Type::Gateway;
    }

    // Document manager reactor functions.
    virtual void onDocumentDeactivated(Document* pDeactivatedDoc) override;
    virtual void onDocumentActivated(Document* pActivatedDoc) override;

protected:
    // Called during gateway-environment entering, before status becomes Active.
    // Calls Environment::onEnter(), then subscribes to DocManager notifications.
    virtual void onEnter() override;

    // Called during gateway-environment exiting, before status becomes Exited.
    // Unsubscribes from DocManager notifications, then calls Environment::onExit(exitCode).
    virtual void onExit(ExitCode exitCode) override;
};

class WYAP_EXPORT DocumentEnvironment
    : public Environment
    , public DocManagerReactor
{
protected:
    DocumentEnvironment();
public:
    virtual ~DocumentEnvironment();
    DocumentEnvironment(const DocumentEnvironment&) = delete;
    DocumentEnvironment& operator=(const DocumentEnvironment&) = delete;
    DocumentEnvironment(DocumentEnvironment&&) = delete;
    DocumentEnvironment& operator=(DocumentEnvironment&&) = delete;

    virtual Type getType() const override
    {
        return Type::Document;
    }

protected:
    // Called during document-environment entering, before status becomes Active.
    // Calls Environment::onEnter(), then subscribes to DocManager notifications.
    virtual void onEnter() override;

    // Called during document-environment exiting, before status becomes Exited.
    // Unsubscribes from DocManager notifications, then calls Environment::onExit(exitCode).
    virtual void onExit(ExitCode exitCode) override;

    // Called during document-environment suspending, before status becomes Suspended.
    // Unsubscribes from DocManager notifications, then calls Environment::onSuspend().
    virtual void onSuspend() override;

    // Called during document-environment resuming, before status becomes Active.
    // Calls Environment::onResume(), then subscribes to DocManager notifications.
    virtual void onResume() override;
};

class WYAP_EXPORT TaskEnvironment : public Environment
{
protected:
    TaskEnvironment();
public:
    virtual ~TaskEnvironment();
    TaskEnvironment(const TaskEnvironment&) = delete;
    TaskEnvironment& operator=(const TaskEnvironment&) = delete;
    TaskEnvironment(TaskEnvironment&&) = delete;
    TaskEnvironment& operator=(TaskEnvironment&&) = delete;

    virtual Type getType() const override
    {
        return Type::Task;
    }
};

NS_WYAP_END

#endif // WYAP_ENVIRONMENT_H