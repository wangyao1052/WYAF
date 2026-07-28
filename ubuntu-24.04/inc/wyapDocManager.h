///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_DOC_MANAGER_H
#define WYAP_DOC_MANAGER_H

#include <vector>
#include <string>

#include <wyErrors.h>
#include <wydbDatabase.h>
#include <wyapDefs.h>
#include <wyapEnums.h>
#include <wyapDocument.h>

NS_WYAP_BEG

class DocManagerReactor;
class DocManagerImpl;

// Document manager.
class WYAP_EXPORT DocManager
{
private:
    DocManager();
    ~DocManager();
    DocManager(const DocManager&) = delete;
    DocManager& operator=(const DocManager&) = delete;
    DocManager(DocManager&&) = delete;
    DocManager& operator=(DocManager&&) = delete;
    friend class SystemInternals;

public:
    // Get all documents.
    std::vector<Document*> getDocuments() const;

    // Get the active document.
    Document* getActiveDocument() const;

    // Activate a document.
    //
    // @param pDoc Document to activate.
    // @param mode Execution mode:
    //   - Sync: perform document activation immediately.
    //   - Async: enqueue an activation request; actual activation is performed
    //     later when the asynchronous request scheduler processes pending requests.
    //
    // @return
    //   - Ok:
    //       - Sync: the document was activated successfully, or it is already
    //         the active document.
    //       - Async: the activation request was enqueued successfully.
    //   - NullDocumentPointer: pDoc is nullptr.
    //   - DocumentNotManaged: pDoc is not managed by the document manager.
    //   - SystemBusy: Sync mode is not allowed because DocumentManager,
    //     EnvironmentManager, CommandManager, or the asynchronous request
    //     scheduler is busy.
    //   - NoActiveEnvironment: there is no active environment.
    //   - NotAllowedInCurrentEnvironment: document activation is not allowed
    //     in the current environment.
    wy::ErrorStatus activateDocument(
        Document* pDoc,
        ExecutionMode mode);

    // Create a new document.
    //
    // @param fileName File name to assign to the new document.
    // @param pNewDoc Receives the created document on success. Set to nullptr on failure.
    //
    // @return
    //   - Ok: the document was created successfully.
    //   - DocumentManagerBusy: the document manager is busy and creating
    //     a new document is not allowed now.
    //   - InvalidFileName: fileName is not a valid document file name.
    //   - DuplicateFileName: a newly created document with the same file name
    //     already exists.
    wy::ErrorStatus newDocument(
        const std::string& u8FileName,
        Document*& pNewDoc);

    // Create a new document.
    // Returns nullptr on failure.
    Document* newDocument(const std::string& u8FileName);

    // Close a document.
    //
    // @param pDoc Document to close.
    //
    // @return
    //   - Ok: the document was closed successfully.
    //   - DocumentManagerBusy: the document manager is busy and closing
    //     a document is not allowed now.
    //   - NullDocumentPointer: pDoc is null.
    //   - DocumentNotManaged: pDoc is not managed by the document manager.
    //   - DocumentActive: pDoc is the active document. Use closeActiveDocument() instead.
    wy::ErrorStatus closeDocument(Document* pDoc);

    // Close the active document.
    //
    // @param mode Execution mode:
    //   - Sync: perform active-document closing immediately.
    //   - Async: enqueue a close request; actual closing is performed
    //     later when the asynchronous request scheduler processes pending requests.
    //
    // @return
    //   - Ok:
    //       - Sync: the active document was closed successfully. Another
    //         document became active if one was available; otherwise, the
    //         current document environment will be exited.
    //       - Async: the close request was enqueued successfully.
    //   - NoActiveDocument: there is no active document.
    //   - SystemBusy: Sync mode is not allowed because DocumentManager,
    //     EnvironmentManager, CommandManager, or the asynchronous request
    //     scheduler is busy.
    //   - NoActiveEnvironment: there is no active environment.
    //   - NotAllowedInCurrentEnvironment: closing the active document is not
    //     allowed in the current environment.
    wy::ErrorStatus closeActiveDocument(ExecutionMode mode);

    // Open a document.
    //
    // @param fileFullPath Full path to the document file.
    // @param pDoc Receives the opened document on success. Set to nullptr on failure.
    //
    // @return
    //   - Ok: the document was opened successfully.
    //   - DocumentManagerBusy: opening a document is not allowed because
    //     the document manager is busy.
    //   - InvalidFilePath: fileFullPath is invalid.
    //   - FileNotFound: the specified file does not exist.
    //   - CantOpenFile: the file exists but cannot be opened.
    //   - BadFile: the file content is invalid or does not match the
    //     expected document file format.
    //   - FileVersionTooNew: the file version is newer than the version
    //     supported by the application.
    //
    // @note If the document is already open, the existing document is returned.
    wy::ErrorStatus openDocument(
        const std::string& u8FilePath,
        const wydb::Database::ReadFileOption& option,
        Document*& pDoc);

    // Open a document.
    // Returns the opened document on success, or nullptr on failure.
    Document* openDocument(
        const std::string& u8FilePath,
        const wydb::Database::ReadFileOption& option);

    // Save a document to a file.
    //
    // @param pDoc Document to save.
    // @param fileName Full path of the output file.
    //
    // @return
    //   - Ok: the document was saved successfully.
    //   - DocumentManagerBusy: saving a document is not allowed because
    //     the document manager is busy.
    //   - NullDocumentPointer: pDoc is nullptr.
    //   - DocumentNotManaged: pDoc is not managed by the document manager.
    //   - InvalidFilePath: invalid file path.
    //   - CantOpenFile: the output file cannot be opened.
    wy::ErrorStatus saveDocument(
        Document* pDoc,
        const std::string& u8FilePath,
        const wydb::Database::WriteFileOption& option);

    // Add a reactor.
    // @param pReactor Pointer to the reactor to add.
    // @return
    //   - Ok: reactor was added successfully.
    //   - DocumentManagerBusy: the document manager is busy and adding
    //     reactors is not allowed now.
    //   - NullInputPointer: pReactor is nullptr.
    //   - DuplicateKey: pReactor has already been added.
    wy::ErrorStatus addReactor(DocManagerReactor* pReactor);

    // Remove a reactor.
    // @param pReactor Pointer to the reactor to remove.
    // @return
    //   - Ok: reactor was removed successfully.
    //   - DocumentManagerBusy: the document manager is busy and removing
    //     reactors is not allowed now.
    //   - NullInputPointer: pReactor is nullptr.
    //   - KeyNotFound: pReactor is not in the reactor list.
    wy::ErrorStatus removeReactor(DocManagerReactor* pReactor);

private:
    DocManagerImpl* _pImpl;
};

// Document manager reactor.
class WYAP_EXPORT DocManagerReactor
{
public:
    DocManagerReactor() = default;
    virtual ~DocManagerReactor() = default;

    // Called right after a document is created.
    virtual void onDocumentCreated(Document* pNewDoc) {}

    // Called right before a document is about to be destroyed.
    virtual void onDocumentToBeDestroyed(Document* pDocToDestroy) {}
    // Called right after a document is destroyed.
    virtual void onDocumentDestroyed(const std::string& fileName) {}

    // Called right before a document is about to be deactivated.
    virtual void onDocumentToBeDeactivated(Document* pDocToDeactivate) {}
    // Called right after a document is deactivated.
    virtual void onDocumentDeactivated(Document* pDeactivatedDoc) {}

    // Called right before a document is about to be activated.
    virtual void onDocumentToBeActivated(Document* pDocToActivate) {}
    // Called right after a document is activated.
    virtual void onDocumentActivated(Document* pActivatedDoc) {}

    // Called right after a document's status flags have changed.
    virtual void onDocumentStatusChanged(Document* pDoc, unsigned int oldStatus) {}

    // Called right after a document title is updated.
    virtual void onDocumentTitleUpdated(Document* pDoc) {}
};

WYAP_EXPORT DocManager* getDocManager();

NS_WYAP_END

#endif // WYAP_DOC_MANAGER_H