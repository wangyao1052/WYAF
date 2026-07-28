///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_SEL_MANAGER_H
#define WYAP_SEL_MANAGER_H

#include <vector>
#include <set>

#include <wyVector3.h>
#include <wyErrors.h>
#include <wydbElementId.h>
#include <wydbDatabase.h>

#include <wyapDefs.h>
#include <wyapSelection.h>
#include <wyapDocManager.h>

NS_WYAP_BEG

class SelManagerReactor;
class SelManagerImpl;

// Selection set manager.
class WYAP_EXPORT SelManager
{
private:
    SelManager();
    ~SelManager();
    SelManager(const SelManager&) = delete;
    SelManager& operator=(const SelManager&) = delete;
    SelManager(SelManager&&) = delete;
    SelManager& operator=(SelManager&&) = delete;
    friend class SystemInternals;

public:
    // Get the current selection set.
    const SelectionSet& getSelections() const;

    // Begin a selection change session.
    // Changes made after this call are cached until endChange() is called.
    // @return
    //   - Ok: change session began successfully.
    //   - SelectionManagerBusy: selection manager is busy now.
    //   - Error: unexpected internal state was encountered.
    wy::ErrorStatus beginChange();

    // End the current selection change session and commit cached changes.
    // If the committed selection set changes, registered reactors are notified once.
    // @return
    //   - Ok: cached changes were committed successfully.
    //   - NotCurrentlyAllowed: no change session is currently active.
    wy::ErrorStatus endChange();

    // Add a selection.
    // The selection is cached and is not committed until endChange() is called.
    // @param sel Selection to add.
    // @return
    //   - Ok: selection was added to the current pending change state.
    //   - NotCurrentlyAllowed: no change session is currently active.
    //   - DuplicateKey: selection is already present in the current pending change state.
    wy::ErrorStatus addSelection(const Selection& sel);

    // Remove a selection.
    // The removal is cached and is not committed until endChange() is called.
    // @param sel Selection to remove.
    // @return
    //   - Ok: selection was removed from the current pending change state.
    //   - NotCurrentlyAllowed: no change session is currently active.
    //   - KeyNotFound: selection is not present in the current pending change state.
    wy::ErrorStatus removeSelection(const Selection& sel);

    // Remove all selections associated with the specified element id.
    // The removals are cached and are not committed until endChange() is called.
    // @param elemId Element id whose selections are to be removed.
    // @return
    //   - Ok: selections associated with elemId were removed successfully.
    //   - NotCurrentlyAllowed: no change session is currently active.
    //   - KeyNotFound: no selection associated with elemId is present
    //                 in the current pending change state.
    wy::ErrorStatus removeSelections(const wydb::ElementId& elemId);

    // Clear the selection set.
    // The change is cached and is not committed until endChange() is called.
    // @return
    //   - Ok: selection set was cleared successfully.
    //   - NotCurrentlyAllowed: no change session is currently active.
    wy::ErrorStatus clearSelections();

    // Replace the selection set.
    // The change is cached and is not committed until endChange() is called.
    // @param sels New selection set to apply.
    // @return
    //   - Ok: selection set was replaced successfully.
    //   - NotCurrentlyAllowed: no change session is currently active.
    wy::ErrorStatus setSelections(const SelectionSet& sels);

    // Add a reactor.
    // @param pReactor Pointer to the reactor to add.
    // @return
    //   - Ok: reactor was added successfully.
    //   - SelectionManagerBusy: selection manager is busy and adding reactors is not allowed now.
    //   - InvalidInput: pReactor is nullptr.
    //   - DuplicateKey: pReactor has already been added.
    wy::ErrorStatus addReactor(SelManagerReactor* pReactor);

    // Remove a reactor.
    // @param pReactor Pointer to the reactor to remove.
    // @return
    //   - Ok: reactor was removed successfully.
    //   - SelectionManagerBusy: selection manager is busy and removing reactors is not allowed now.
    //   - InvalidInput: pReactor is nullptr.
    //   - KeyNotFound: pReactor has not been added.
    wy::ErrorStatus removeReactor(SelManagerReactor* pReactor);

private:
    SelManagerImpl* _pImpl;
};

// Selection set manager reactor.
class WYAP_EXPORT SelManagerReactor
{
public:
    SelManagerReactor() {}
    virtual ~SelManagerReactor() {}

    // Called when the selection set changes.
    // @param addedSS Selections added by the committed change.
    // @param removedSS Selections removed by the committed change.
    // @param currSS Current committed selection set after the change.
    virtual void onSelectionChanged(
        const wyap::SelectionSet& addedSS,
        const wyap::SelectionSet& removedSS,
        const wyap::SelectionSet& currSS) {}
};

NS_WYAP_END

#endif // WYAP_SEL_MANAGER_H