///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_VIEW_MANAGER_H
#define WYAP_VIEW_MANAGER_H

#include <wyapDefs.h>

NS_WYAP_BEG

class View;
class Document;
class Application;

// View manager.
class ViewManager
{
protected:
    ViewManager() {}
    virtual ~ViewManager() {}
    ViewManager(const ViewManager&) = delete;
    ViewManager& operator=(const ViewManager&) = delete;
    ViewManager(ViewManager&&) = delete;
    ViewManager& operator=(ViewManager&&) = delete;

    // Create a view for the document.
    virtual View* newView(Document* pDoc) = 0;
    // Delete the view associated with the document.
    virtual bool deleteView(Document* pDoc) = 0;

public:
    // Get the view associated with the document.
    virtual View* getView(Document* pDoc) = 0;

    friend class Application;
};

NS_WYAP_END

#endif // WYAP_VIEW_MANAGER_H