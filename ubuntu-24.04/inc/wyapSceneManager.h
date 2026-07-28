///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_SCENE_MANAGER_H
#define WYAP_SCENE_MANAGER_H

#include <wyapDefs.h>

NS_WYAP_BEG

class Scene;
class Document;
class Application;

class SceneManager
{
protected:
    SceneManager() {}
    virtual ~SceneManager() {}
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    // Create a scene for the document.
    virtual Scene* newScene(Document* pDoc) = 0;
    // Delete the scene associated with the document.
    virtual bool deleteScene(Document* pDoc) = 0;

public:
    // Get the scene associated with the document.
    virtual Scene* getScene(Document* pDoc) = 0;

    friend class Application;
};

NS_WYAP_END

#endif // WYAP_SCENE_MANAGER_H
