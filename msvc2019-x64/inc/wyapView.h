///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_VIEW_H
#define WYAP_VIEW_H

#include <list>
#include <set>
#include <cassert>
#include <wydbElementId.h>
#include <wydbDatabase.h>
#include <wyapDefs.h>

NS_WYAP_BEG

class Document;
class ViewImpl;

// View.
class WYAP_EXPORT View : public wydb::DatabaseReactor
{
protected:
    explicit View(Document* pDoc);
    virtual ~View();
    View(const View&) = delete;
    View& operator=(const View&) = delete;
    View(View&&) = delete;
    View& operator=(View&&) = delete;
    friend class SystemInternals;

public:
    // Get the associated document.
    Document* getDocument() const;

    // Initialize the view.
    virtual void initialize() = 0;

    // Database change notification.
    virtual void onDatabaseChanged(
        const wydb::Database* pDb,
        const wydb::Transaction* pTransaction,
        const wydb::DatabaseChangeInfo& changeInfo) override {}

private:
    ViewImpl* _pImpl;
};

NS_WYAP_END

#endif // WYAP_VIEW_H