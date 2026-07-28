///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_GIZMO_H
#define WYAP_GIZMO_H

#include <memory>

#include <wyapDefs.h>

NS_WYAP_BEG

// Base class for interactive gizmos.
class WYAP_EXPORT Gizmo
{
public:
    Gizmo() = default;
    virtual ~Gizmo() = default;
    Gizmo(const Gizmo&) = delete;
    Gizmo& operator=(const Gizmo&) = delete;
    Gizmo(Gizmo&&) = delete;
    Gizmo& operator=(Gizmo&&) = delete;
    friend class GizmoManagerImpl;

    // Update the gizmo to match the current state of its associated object.
    virtual void refresh() = 0;

protected:
    // Called by GizmoManager when the gizmo is being activated.
    virtual void onActivate() = 0;

    // Called by GizmoManager when the gizmo is being deactivated.
    virtual void onDeactivate() = 0;
};

using GizmoSPtr = std::shared_ptr<Gizmo>;

NS_WYAP_END

#endif // WYAP_GIZMO_H
