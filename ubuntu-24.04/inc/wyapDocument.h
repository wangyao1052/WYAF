///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_DOCUMENT_H
#define WYAP_DOCUMENT_H

#include <string>
#include <set>

#include <wydbDatabase.h>
#include <wyapDefs.h>
#include <wyapDocumentId.h>

NS_WYAP_BEG

class DocumentImpl;

// Document status flags.
enum class DocumentStatus : unsigned int
{
    None = 0x00000000, // No status.
    NewlyCreated = 0x00000001, // Created and not yet saved for the first time.
    Modified = 0x00000002, // Modified and not yet saved.
};

// Document class.
class WYAP_EXPORT Document : public wydb::DatabaseReactor
{
protected:
    Document();
    virtual ~Document();
    Document(const Document&) = delete;
    Document& operator=(const Document&) = delete;
    Document(Document&&) = delete;
    Document& operator=(Document&&) = delete;
    friend class DocManagerImpl;
    friend class SystemInternals;

public:
    // Get the document ID.
    // The ID uniquely identifies the document within the current application
    // session. It is not a persistent identifier. A new ID is assigned after
    // the document is closed and reopened.
    DocumentId getId() const;

    // Get the database owned by the document.
    wydb::Database* getDatabase() const;

    // Get the document file name, including the full path.
    const std::string& getFileName() const;

    // Check whether the document has the specified status flag.
    bool hasStatus(DocumentStatus status) const;

    // Get the document status flags.
    unsigned int getStatus() const;

    // Database reactor callback.
    // Called when the document database changes.
    virtual void onDatabaseChanged(
        const wydb::Database* pDatabase,
        const wydb::Transaction* pTransaction,
        const wydb::DatabaseChangeInfo& changeInfo) override;

private:
    DocumentImpl* _pImpl;
};

NS_WYAP_END

#endif // WYAP_DOCUMENT_H