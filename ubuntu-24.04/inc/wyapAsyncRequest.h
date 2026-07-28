///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2026 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_ASYNC_REQUEST_H
#define WYAP_ASYNC_REQUEST_H

#include <string>

#include <wyapDefs.h>
#include <wyapDocumentId.h>
#include <wyapEnvironmentId.h>
#include <wyapEnvironment.h>

NS_WYAP_BEG

// Context data for an async request.
// Created when accepted and reused in later callbacks.
class AsyncRequest
{
public:
    virtual ~AsyncRequest() = default;

    enum class Type : unsigned short
    {
        ExecuteCommand        = 1,
        ActivateDocument      = 2,
        CloseActiveDocument   = 3,
        EnterEnvironment      = 4,
        ExitActiveEnvironment = 5,
    };

    virtual Type getType() const = 0;
};

class ExecuteCommandAsyncRequest : public AsyncRequest
{
public:
    explicit ExecuteCommandAsyncRequest(const std::string& commandName) 
        : _commandName(commandName) {}

    virtual Type getType() const override
    {
        return Type::ExecuteCommand;
    }

    inline const std::string& getCommandName() const
    {
        return _commandName;
    }

private:
    std::string _commandName;
};

class ActivateDocumentAsyncRequest : public AsyncRequest
{
public:
    explicit ActivateDocumentAsyncRequest(DocumentId docId) 
        : _docId(docId) {}

    virtual Type getType() const override
    {
        return Type::ActivateDocument;
    }

    inline DocumentId getDocumentId() const
    {
        return _docId;
    }

private:
    DocumentId _docId;
};

class CloseActiveDocumentAsyncRequest : public AsyncRequest
{
public:
    explicit CloseActiveDocumentAsyncRequest(DocumentId docId)
        : _docId(docId) {}

    virtual Type getType() const override
    {
        return Type::CloseActiveDocument;
    }

    inline DocumentId getDocumentId() const
    {
        return _docId;
    }

private:
    DocumentId _docId;
};

class EnterEnvironmentAsyncRequest : public AsyncRequest
{
public:
    explicit EnterEnvironmentAsyncRequest(EnvironmentId environmentId)
        : _environmentId(environmentId) {}

    virtual Type getType() const override
    {
        return Type::EnterEnvironment;
    }

    inline EnvironmentId getEnvironmentId() const
    {
        return _environmentId;
    }

private:
    EnvironmentId _environmentId;
};

class ExitActiveEnvironmentAsyncRequest : public AsyncRequest
{
public:
    ExitActiveEnvironmentAsyncRequest(
        EnvironmentId environmentId,
        Environment::ExitCode exitCode)
        : _environmentId(environmentId)
        , _exitCode(exitCode) {}

    virtual Type getType() const override
    {
        return Type::ExitActiveEnvironment;
    }

    inline EnvironmentId getEnvironmentId() const
    {
        return _environmentId;
    }

    inline Environment::ExitCode getExitCode() const
    {
        return _exitCode;
    }

private:
    EnvironmentId _environmentId;
    Environment::ExitCode _exitCode;
};

NS_WYAP_END

#endif // WYAP_ASYNC_REQUEST_H