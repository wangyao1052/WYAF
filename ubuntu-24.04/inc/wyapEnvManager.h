///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_ENV_MANAGER_H
#define WYAP_ENV_MANAGER_H

#include <memory>

#include <wyErrors.h>
#include <wyapEnvironment.h>
#include <wyapEnums.h>

NS_WYAP_BEG

class EnvManagerReactor;
class EnvManagerImpl;
class SystemInternals;

// Environment manager.
class WYAP_EXPORT EnvManager
{
private:
    EnvManager();
    ~EnvManager();
    EnvManager(const EnvManager&) = delete;
    EnvManager& operator=(const EnvManager&) = delete;
    EnvManager(EnvManager&&) = delete;
    EnvManager& operator=(EnvManager&&) = delete;
    friend class SystemInternals;

public:
    // Get the active environment.
    // Returns nullptr if there is no active environment
    // (for example, during transition between environments).
    Environment* getActiveEnvironment() const;

    // Get the top environment on the environment stack.
    // The active environment, when it exists, is always the top environment.
    Environment* getTopEnvironment() const;

    // Enter an environment.
    // Ownership of pEnvironment is transferred to EnvManager.
    //
    // @param pEnvironment Environment instance to enter.
    // @param mode Execution mode:
    //   - Sync: perform transition immediately.
    //   - Async: enqueue an exit request; transition is performed later when
    //         the async request scheduler processes pending requests.
    //
    // @return
    //   - Ok:
    //       - Sync: transition completed successfully.
    //       - Async: request was enqueued successfully.
    //   - NullEnvironmentPointer: pEnvironment is nullptr.
    //   - SystemBusy: Sync mode is not allowed because the system is processing another operation
    //         (Document/Environment/Command manager or async scheduler is busy).
    //   - InvalidEnvironmentTransition: environment state is invalid, or environment type/level 
    //         transition is not allowed.
    wy::ErrorStatus enterEnvironment(
        std::unique_ptr<Environment> environment,
        ExecutionMode mode);

    // Exit active environment.
    //
    // @param exitCode Exit behavior: commit or discard changes.
    // @param mode Execution mode:
    //   - Sync: perform transition immediately.
    //   - Async: enqueue an exit request; transition is performed later when
    //         the async request scheduler processes pending requests.
    //
    // @return
    //   - Ok:
    //       - Sync: transition completed successfully.
    //       - Async: request was enqueued successfully.
    //   - NoActiveEnvironment: there is no active environment.
    //   - SystemBusy: Sync mode is not allowed because the system is processing another operation
    //         (Document/Environment/Command manager or async scheduler is busy).
    wy::ErrorStatus exitActiveEnvironment(
        Environment::ExitCode exitCode,
        ExecutionMode mode);

    // Add a reactor.
    // @param pReactor Pointer to the reactor to add.
    // @return
    //   - Ok: reactor was added successfully.
    //   - EnvironmentManagerBusy: environment manager is busy and adding
    //     reactors is not allowed now.
    //   - NullInputPointer: pReactor is nullptr.
    //   - DuplicateKey: pReactor has already been added.
    wy::ErrorStatus addReactor(EnvManagerReactor* pReactor);

    // Remove a reactor.
    // @param pReactor Pointer to the reactor to remove.
    // @return
    //   - Ok: reactor was removed successfully.
    //   - EnvironmentManagerBusy: environment manager is busy and removing
    //     reactors is not allowed now.
    //   - NullInputPointer: pReactor is nullptr.
    //   - KeyNotFound: pReactor is not in the reactor list.
    wy::ErrorStatus removeReactor(EnvManagerReactor* pReactor);

private:
    EnvManagerImpl* _pImpl;
};

// Environment manager reactor.
class WYAP_EXPORT EnvManagerReactor
{
public:
    EnvManagerReactor() = default;
    virtual ~EnvManagerReactor() = default;

    // Called right before entering an environment.
    virtual void onEnvironmentToBeEntered(Environment* pEnvironment) {}
    // Called right after entering an environment.
    virtual void onEnvironmentEntered(Environment* pEnvironment) {}

    // Called right before exiting an environment.
    virtual void onEnvironmentToBeExited(Environment* pEnvironment) {}
    // Called right after exiting an environment.
    virtual void onEnvironmentExited(
        Environment* pEnvironment,
        Environment::ExitCode exitCode) {}

    // Called right before suspending an environment.
    virtual void onEnvironmentToBeSuspended(Environment* pEnvironment) {}
    // Called right after suspending an environment.
    virtual void onEnvironmentSuspended(Environment* pEnvironment) {}

    // Called right before resuming an environment.
    virtual void onEnvironmentToBeResumed(Environment* pEnvironment) {}
    // Called right after resuming an environment.
    virtual void onEnvironmentResumed(Environment* pEnvironment) {}
};

NS_WYAP_END

#endif // WYAP_ENV_MANAGER_H
