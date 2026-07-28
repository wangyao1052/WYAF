///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_ELEMENT_H
#define WYDB_ELEMENT_H

#include <vector>
#include <string>
#include <set>
#include <memory>
#include <any>
#include <cstdint>

#include <wyrxObject.h>
#include <wydbDefs.h>
#include <wydbBoiler.h>
#include <wyErrors.h>
#include <wydbElementId.h>
#include <wydbFieldId.h>
#include <wydbParameter.h>
#include <wydbChainUpdate.h>

NS_WYDB_BEG

class Database;
class ElementImpl;
class InFiler;
class OutFiler;
class ChainUpdateFeedbackCollector;

class WYDB_EXPORT Element : public wyrx::Object
{
    WYDB_DECLARE_ABSTRACT_MEMBERS(Element, wydb::Element, wyrx::Object)
public:
    // Get the id of this element.
    ElementId getId() const;

    // Get the database that owns this element.
    Database* getDatabase() const;

    // Health state of an element.
    enum class HealthState
    {
        Healthy = 0,
        Warning = 1,
        Error = 2,
    };

    // Get the current health state of this element.
    // The returned state is determined from the feedback reported during chain update.
    HealthState getHealthState() const;

    // Check whether this element is erased.
    bool isErased() const;

    // Erase this element or cancel its erased state.
    // @param erasing
    //   - true: erase this element.
    //   - false: cancel the erased state of this element.
    // @return
    //   - Ok: succeeded.
    //   - ElementNotOpenedForWrite: this element is not opened for write.
    wy::ErrorStatus erase(bool erasing = true);

    // Check whether this element is hidden.
    bool isHidden() const;

    // Hide this element or cancel its hidden state.
    // @param hiding
    //   - true: hide this element.
    //   - false: cancel the hidden state of this element.
    // @return
    //   - Ok: succeeded.
    //   - ElementNotOpenedForWrite: this element is not opened for write.
    wy::ErrorStatus hide(bool hiding = true);

    // Upgrade this element for write access.
    // @return
    //   - Ok: succeeded, or this element is already opened for write.
    //   - NoActiveTransaction: there is no active transaction.
    wy::ErrorStatus upgradeForWrite();

    // Get the parent element id.
    // Returns ElementId::kNull if this element has no parent.
    virtual ElementId getParent() const
    {
        return ElementId::kNull;
    }

    // Get the child element ids.
    virtual std::vector<ElementId> getChildren() const
    {
        return std::vector<ElementId>();
    }

    // Mark the element's Completion data piece as dirty to schedule dependency-driven
    // chain updates, which are executed when the transaction is committed.
    // In a parametric 3D CAD system, derived elements typically override
    // onChainUpdate() to regenerate geometry and perform other model-update logic.
    // 
    // @return
    //   - Ok: succeeded.
    //   - ElementNotOpenedForWrite: this element is not opened for write.
    wy::ErrorStatus regenerate();

public:
    // Get all parameter definitions of this element.
    std::vector<const ParameterDefinition*> listParameters() const;

    // Get the value of the specified parameter.
    // @param className Class name that defines the parameter. This is required
    //   because parameter names may be duplicated across different classes
    //   in the inheritance hierarchy.
    // @param paramName Parameter name.
    // @return Parameter value if found; otherwise nullptr.
    virtual ParameterValueUPtr getParameterValue(
        const std::string& className,
        const std::string& paramName) const;

    // Set the value of the specified parameter.
    // @param className Class name that defines the parameter. This is required
    //   because parameter names may be duplicated across different classes
    //   in the inheritance hierarchy.
    // @param paramName Parameter name.
    // @param paramValue Parameter value to set.
    // @return
    //   - Ok: succeeded.
    //   - ParameterNotFound: the specified parameter was not found.
    //   - InvalidInput: paramValue is invalid for the specified parameter,
    //     including type mismatch and out-of-range values.
    // Derived classes should generally follow this return-value convention when
    // overriding this method, but may return more specific error codes when appropriate.
    virtual wy::ErrorStatus setParameterValue(
        const std::string& className,
        const std::string& paramName,
        const ParameterValue& paramValue);

    // Write this element to the filer.
    virtual wy::ErrorStatus writeToFiler(OutFiler& filer) const;

    // Read this element from the filer.
    virtual wy::ErrorStatus readFromFiler(InFiler& filer);

protected:
    // Get the value of the specified field.
    // This method is used by the transaction system to record the value
    // of this element's field identified by fieldId.
    // Each field supported by the concrete element type must have a
    // globally unique registered FieldId.
    virtual bool getFieldValue(FieldId fieldId, std::any& value);

    // Set the value of the specified field.
    // This method is used by the transaction system to restore the value
    // of this element's field identified by fieldId.
    // Each field supported by the concrete element type must have a
    // globally unique registered FieldId.
    virtual bool setFieldValue(FieldId fieldId, const std::any& value);

    // Report the elements on which this element depends.
    virtual void reportDependencies(std::set<ElementId>& dependencies) const
    {}

    // Called when one or more dependent elements are erased.
    // Returns true if this element changes in response.
    virtual bool onDependenciesErased(
        const std::set<ElementId>& erasedDependencies)
    {
        return false;
    }

    // Report all chain-update data pieces of this element and their dependencies.
    virtual void reportChainUpdateDataPieces(ElementDataPieceCollector& dps) const;

    // Perform chain update for the specified dirty data piece.
    virtual void onChainUpdate(
        const ElementDataPiece& dirtyDataPiece,
        ChainUpdateFeedbackCollector& feedbackCollector,
        ChainUpdateCallbackManager& callbackManager);

protected:
    // Prepare for a field change and mark the related data piece dirty.
    wy::ErrorStatus prepareForFieldChange(
        FieldId fieldId,
        wydb::ElementDataPieceType dataPieceType = wydb::ElementDataPieceType::Shape,
        const wydb::TypedValue& extra = wydb::TypedValue::kNone);

    // Mark the specified data piece as dirty.
    // Dirty data pieces are used to schedule dependency-driven chain updates,
    // which are executed when the transaction is committed.
    // @param dataPiece Data piece to mark dirty.
    // @return
    //   - Ok: succeeded.
    //   - ElementIdMismatch: dataPiece refers to a different element.
    //   - ElementNotOpenedForWrite: this element is not opened for write.
    wy::ErrorStatus markDataPieceDirty(const ElementDataPiece& dataPiece);

protected:
    ElementImpl* _pImpl;
    friend class TransactionLeaf;
    friend class DatabaseImpl;
    friend class SystemInternals;
};

// Delete an element instance created by the library.
WYDB_EXPORT void deleteElement(const Element* pElem);

NS_WYDB_END

#endif // WYDB_ELEMENT_H