///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_APPLICATION_H
#define WYAP_APPLICATION_H

#include <memory>
#include <string>

#include <wydbDatabase.h>
#include <wyapDefs.h>
#include <wyapDocument.h>
#include <wyapDocManager.h>
#include <wyapSelManager.h>
#include <wyapGizmoManager.h>
#include <wyapCmdStack.h>
#include <wyapCmdManager.h>
#include <wyapViewManager.h>
#include <wyapSceneManager.h>
#include <wyapClipboard.h>
#include <wyapEnvManager.h>
#include <wyapView.h>
#include <wyapScene.h>
#include <wyapAsyncRequest.h>

NS_WYAP_BEG

class ApplicationImpl;
class SystemInternals;

class WYAP_EXPORT Application : 
    public wyap::DocManagerReactor,
    public wyap::CmdManagerReactor,
    public wyap::EnvManagerReactor,
    public wydb::DatabaseReactor
{
protected:
    Application();
    virtual ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;
    friend class SystemInternals;

public:
    // Get active document.
    Document* getActiveDocument() const;

    // Get active database.
    wydb::Database* getActiveDatabase() const;

    // Get document manager.
    DocManager* getDocManager() const;

    // Get environment manager.
    EnvManager* getEnvManager() const;

    // Get command stack.
    CmdStack* getCmdStack() const;

    // Get command manager.
    CmdManager* getCmdManager() const;

    // Get scene manager.
    SceneManager* getSceneManager() const;

    // Get view manager.
    ViewManager* getViewManager() const;

    // Get selection manager.
    SelManager* getSelManager() const;

    // Get gizmo manager.
    GizmoManager* getGizmoManager() const;

    // Get clipboard.
    Clipboard* getClipboard() const;

    // The following pure virtual functions must be implemented by concrete subclasses.
    virtual wydb::Database* newDatabase() = 0;
    virtual wyap::Document* newDocument() = 0;
    virtual Scene* newScene(Document* pDoc) = 0;
    virtual View* newView(Document* pDoc) = 0;
    virtual std::unique_ptr<GatewayEnvironment> newDefaultGatewayEnvironment() = 0;
    virtual std::unique_ptr<DocumentEnvironment> newDefaultDocumentEnvironment() = 0;

public:
    // document manager reactor callback functions.
    virtual void onDocumentCreated(Document* pNewDoc) override;
    virtual void onDocumentToBeDestroyed(Document* pDocToDestroy) override;
    virtual void onDocumentDestroyed(const std::string& fileName) override;
    virtual void onDocumentToBeDeactivated(Document* pDocToDeactivate) override;
    virtual void onDocumentToBeActivated(Document* pDocToActivate) override;
    virtual void onDocumentActivated(Document* pActivatedDoc) override;
    virtual void onDocumentStatusChanged(Document* pDoc, unsigned int oldStatus) override;
    virtual void onDocumentTitleUpdated(Document* pDoc) override;

    virtual void onDatabaseChanged(
        const wydb::Database* pDatabase,
        const wydb::Transaction* pTransaction,
        const wydb::DatabaseChangeInfo& info) override;

    // async request callbacks
    virtual void onAsyncRequestAccepted(const AsyncRequest* pReq);
    virtual void onAsyncRequestDropped(const AsyncRequest* pReq);
    virtual void onAsyncRequestSucceeded(const AsyncRequest* pReq);
    virtual void onAsyncRequestFailed(
        const AsyncRequest* pReq,
        wy::ErrorStatus error,
        std::string detail);

private:
    ApplicationImpl* _pImpl;
};

WYAP_EXPORT Application* getApplication();

NS_WYAP_END

#endif // WYAP_APPLICATION_H