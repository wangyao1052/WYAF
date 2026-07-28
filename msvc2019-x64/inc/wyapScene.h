///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_SCENE_H
#define WYAP_SCENE_H

#include <cassert>
#include <wydbDatabase.h>
#include <wydbTransaction.h>

#include <wyapDefs.h>
#include <wyapSelManager.h>
#include <wyapGizmoManager.h>
#include <wyapEnvManager.h>

NS_WYAP_BEG

class Document;
class SceneImpl;

class WYAP_EXPORT Scene : 
    public wydb::DatabaseReactor,
    public wydb::TransactionReactor,
    public wyap::SelManagerReactor,
    public wyap::GizmoManagerReactor,
    public wyap::EnvManagerReactor
{
protected:
    explicit Scene(Document* pDoc);
    virtual ~Scene();
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;
    friend class SystemInternals;

public:
    // Get the associated document.
    Document* getDocument() const;

    // Database change notification.
    virtual void onDatabaseChanged(
        const wydb::Database* pDb,
        const wydb::Transaction* pTransaction,
        const wydb::DatabaseChangeInfo& changeInfo) override {}

    // Selection change notification.
    virtual void onSelectionChanged(
        const wyap::SelectionSet& addedSS,
        const wyap::SelectionSet& removedSS,
        const wyap::SelectionSet& curSS) override {}

    // Gizmo change notification.
    virtual void onGizmoChanged(
        const std::set<GizmoSPtr>& addedGizmos,
        const std::set<GizmoSPtr>& removedGizmos,
        const std::set<GizmoSPtr>& currGizmos) override {}

private:
    SceneImpl* _pImpl;
};

NS_WYAP_END

#endif // WYAP_SCENE_H
