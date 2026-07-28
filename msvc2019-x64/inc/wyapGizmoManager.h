///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_GIZMO_MANAGER_H
#define WYAP_GIZMO_MANAGER_H

#include <vector>
#include <set>
#include <memory>

#include <wyErrors.h>
#include <wydbElement.h>
#include <wyapDefs.h>
#include <wyapGizmo.h>
#include <wyapDocManager.h>

NS_WYAP_BEG

class Gizmo;
class GizmoManagerReactor;
class GizmoManagerImpl;

// Interactive gizmo manager.
class WYAP_EXPORT GizmoManager
{
private:
    GizmoManager();
    ~GizmoManager();
    GizmoManager(const GizmoManager&) = delete;
    GizmoManager& operator=(const GizmoManager&) = delete;
    GizmoManager(GizmoManager&&) = delete;
    GizmoManager& operator=(GizmoManager&&) = delete;
    friend class SystemInternals;

public:
    // Return whether the manager contains any gizmos.
    bool hasGizmos() const;

    // Return a snapshot of all gizmos currently managed by this manager.
    std::vector<GizmoSPtr> getGizmos() const;

    // Create an iterator over the committed gizmo collection.
    // @note
    //   - The iterator directly references this manager's collection.
    //   - This manager must remain valid for the lifetime of the iterator.
    //   - Do not commit gizmo collection changes while using the iterator.
    wy::Iterator<GizmoSPtr> createIterator() const;

    // Get the currently active gizmo.
    // A null pointer is returned if no gizmo is active.
    GizmoSPtr getActiveGizmo() const;

    // Set the active gizmo.
    // The previously active gizmo, if any, is deactivated before the new
    // gizmo is activated. Passing nullptr deactivates the current gizmo
    // without activating another one.
    //
    // @param pGizmo Gizmo to activate, or nullptr to deactivate
    //        the currently active gizmo. A non-null gizmo must belong
    //        to the committed gizmo collection.
    //
    // @return
    //   - Ok: the requested activation state was applied successfully.
    //   - GizmoManagerBusy: the manager is not idle or an activation
    //     change is already in progress.
    //   - InvalidInput: pGizmo is not managed by this manager.
    wy::ErrorStatus setActiveGizmo(GizmoSPtr pGizmo);


    // Begin a gizmo collection change session.
    // Any existing pending changes are discarded. Changes made after this
    // call remain pending until endChange() is called.
    //
    // Calling this function during an active change session restarts the
    // session and discards its pending changes.
    //
    // @return
    //   - Ok: the change session began successfully.
    //   - GizmoManagerBusy: a change is being committed or an activation
    //     callback is being processed.
    wy::ErrorStatus beginChange();

    // End the current change session and commit all pending changes.
    // If the active gizmo is pending removal, it is deactivated before
    // the collection is updated. Registered reactors are notified once
    // when the committed collection changes.
    //
    // @return
    //   - Ok: the pending changes were committed successfully.
    //   - NotCurrentlyAllowed: no change session is currently active.
    wy::ErrorStatus endChange();

    // Add a gizmo.
    // The gizmo is cached and is not committed until endChange() is called.
    // @param pGizmo Gizmo to add.
    // @return
    //   - Ok: gizmo was added to the current pending change state.
    //   - NotCurrentlyAllowed: no change session is currently active.
    //   - InvalidInput: pGizmo is nullptr.
    //   - DuplicateKey: pGizmo is already present in the current pending change state.
    wy::ErrorStatus addGizmo(GizmoSPtr pGizmo);

    // Remove a gizmo from the pending collection.
    // If the gizmo is pending addition, the addition is canceled. The
    // resulting change is not committed until endChange() is called.
    //
    // @param pGizmo Gizmo to remove.
    //
    // @return
    //   - Ok: the pending collection no longer contains pGizmo.
    //   - NotCurrentlyAllowed: no change session is currently active.
    //   - InvalidInput: pGizmo is nullptr.
    //   - KeyNotFound: the pending collection does not contain pGizmo.
    wy::ErrorStatus removeGizmo(GizmoSPtr pGizmo);

    // Remove all gizmos from the pending collection.
    // The change is not committed until endChange() is called.
    //
    // @return
    //   - Ok: the pending collection is empty.
    //   - NotCurrentlyAllowed: no change session is currently active.
    wy::ErrorStatus clearGizmos();

    // Replace the pending gizmo collection.
    // The change is not committed until endChange() is called.
    //
    // @param gizmos New gizmo collection. The collection must not
    //        contain nullptr.
    //
    // @return
    //   - Ok: the pending collection was replaced successfully.
    //   - NotCurrentlyAllowed: no change session is currently active.
    //   - InvalidInput: gizmos contains nullptr.
    wy::ErrorStatus setGizmos(const std::set<GizmoSPtr>& gizmos);

    // Register a reactor.
    // The manager does not take ownership of the reactor. The reactor must
    // remain valid until it is unregistered.
    //
    // @param pReactor Reactor to register.
    //
    // @return
    //   - Ok: the reactor was registered successfully.
    //   - GizmoManagerBusy: reactor registration is currently not allowed.
    //   - InvalidInput: pReactor is nullptr.
    //   - DuplicateKey: pReactor is already registered.
    wy::ErrorStatus addReactor(GizmoManagerReactor* pReactor);

    // Unregister a reactor.
    //
    // @param pReactor Reactor to unregister.
    //
    // @return
    //   - Ok: the reactor was unregistered successfully.
    //   - GizmoManagerBusy: reactor removal is currently not allowed.
    //   - InvalidInput: pReactor is nullptr.
    //   - KeyNotFound: pReactor is not registered.
    wy::ErrorStatus removeReactor(GizmoManagerReactor* pReactor);

private:
    GizmoManagerImpl* _pImpl;
};

// Receives notifications about gizmo collection and activation changes.
//
// @note
//   - The manager does not own registered reactors.
//   - Do not modify the manager from within an activation callback.
class WYAP_EXPORT GizmoManagerReactor
{
public:
    GizmoManagerReactor() = default;
    virtual ~GizmoManagerReactor() = default;

    // Called after pending collection changes have been committed.
    //
    // @param addedGizmos Gizmos added by the committed change.
    // @param removedGizmos Gizmos removed by the committed change.
    // @param currGizmos Complete committed collection after the change.
    virtual void onGizmoChanged(
        const std::set<GizmoSPtr>& addedGizmos,
        const std::set<GizmoSPtr>& removedGizmos,
        const std::set<GizmoSPtr>& currGizmos)
    {
    }

    // Called immediately before the active gizmo is deactivated.
    //
    // @param pGizmo Gizmo that is about to be deactivated.
    virtual void onGizmoToBeDeactivated(GizmoSPtr pGizmo)
    {
    }

    // Called immediately before a gizmo is activated.
    //
    // @param pGizmo Gizmo that is about to be activated.
    virtual void onGizmoToBeActivated(GizmoSPtr pGizmo)
    {
    }

    // Called after a gizmo has been activated.
    //
    // @param pGizmo Gizmo that was activated.
    virtual void onGizmoActivated(GizmoSPtr pGizmo)
    {
    }
};

NS_WYAP_END

#endif // WYAP_GIZMO_MANAGER_H
