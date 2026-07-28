///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_CHAIN_UPDATE_H
#define WYDB_CHAIN_UPDATE_H

#include <cstdint>
#include <initializer_list>
#include <utility>
#include <set>
#include <memory>

#include <wydbElementId.h>
#include <wydbTypedValue.h>

NS_WYDB_BEG

// Completion - virtual data piece type; by convention,
// it is the sink node of the dependency graph, but this is not enforced.
enum class ElementDataPieceType : std::uint32_t
{
    None             = 0x00000000U,
    Completion       = 0x00000001U,
    Shape            = 0x00000002U,
    Transform        = 0x00000004U,
    Appearance       = 0x00000008U,
    Hierarchy        = 0x00000010U,
    Visibility       = 0x00000020U,
    UserDefined      = 0x00010000U,
    Parameter        = 0x00020000U,
    Property         = 0x00040000U,
    UserDefinedExt1  = 0x00100000U,
    UserDefinedExt2  = 0x00200000U,
    UserDefinedExt3  = 0x00400000U,
    UserDefinedExt4  = 0x00800000U,
    UserDefinedExt5  = 0x01000000U,
    UserDefinedExt6  = 0x02000000U,
    UserDefinedExt7  = 0x04000000U,
    UserDefinedExt8  = 0x08000000U,
    UserDefinedExt9  = 0x10000000U,
    UserDefinedExt10 = 0x20000000U,
    UserDefinedExt11 = 0x40000000U,
    UserDefinedExt12 = 0x80000000U,
};

// Represents a data piece of an element.
// A data piece is identified by an element id, a piece type, and an optional extra value.
// The extra value is used by piece types greater than or equal to UserDefined.
class ElementDataPiece
{
private:
    ElementDataPiece(
        const ElementId& id,
        ElementDataPieceType type,
        const TypedValue& extra = TypedValue::kNone)
        : _id(id)
        , _extra(extra)
        , _type(type)
    {
    }

    ElementDataPiece(
        const ElementId& id,
        ElementDataPieceType type,
        TypedValue&& extra)
        : _id(id)
        , _extra(std::move(extra))
        , _type(type)
    {
    }

public:
    ElementDataPiece(const ElementDataPiece&) = default;
    ElementDataPiece& operator=(const ElementDataPiece&) = default;
    ElementDataPiece(ElementDataPiece&&) noexcept = default;
    ElementDataPiece& operator=(ElementDataPiece&&) noexcept = default;
    ~ElementDataPiece() = default;

    // Creates the virtual completion piece.
    static ElementDataPiece completion(const ElementId& id)
    {
        return ElementDataPiece(id, ElementDataPieceType::Completion);
    }

    // Creates a shape piece.
    static ElementDataPiece shape(const ElementId& id)
    {
        return ElementDataPiece(id, ElementDataPieceType::Shape);
    }

    // Creates a transform piece.
    static ElementDataPiece transform(const ElementId& id)
    {
        return ElementDataPiece(id, ElementDataPieceType::Transform);
    }

    // Creates an appearance piece.
    static ElementDataPiece appearance(const ElementId& id)
    {
        return ElementDataPiece(id, ElementDataPieceType::Appearance);
    }

    // Creates a hierarchy piece.
    static ElementDataPiece hierarchy(const ElementId& id)
    {
        return ElementDataPiece(id, ElementDataPieceType::Hierarchy);
    }

    // Creates a visibility piece.
    static ElementDataPiece visibility(const ElementId& id)
    {
        return ElementDataPiece(id, ElementDataPieceType::Visibility);
    }

    static ElementDataPiece create(
        const ElementId& id,
        ElementDataPieceType type,
        const wydb::TypedValue& extraData)
    {
        switch (type)
        {
        case ElementDataPieceType::Completion: return completion(id);
        case ElementDataPieceType::Shape:      return shape(id);
        case ElementDataPieceType::Transform:  return transform(id);
        case ElementDataPieceType::Appearance: return appearance(id);
        case ElementDataPieceType::Hierarchy:  return hierarchy(id);
        case ElementDataPieceType::Visibility: return visibility(id);
        default:
            return ElementDataPiece(id, type, extraData);
        }
    }

    static ElementDataPiece create(
        const ElementId& id,
        ElementDataPieceType type,
        wydb::TypedValue&& extraData)
    {
        switch (type)
        {
        case ElementDataPieceType::Completion: return completion(id);
        case ElementDataPieceType::Shape:      return shape(id);
        case ElementDataPieceType::Transform:  return transform(id);
        case ElementDataPieceType::Appearance: return appearance(id);
        case ElementDataPieceType::Hierarchy:  return hierarchy(id);
        case ElementDataPieceType::Visibility: return visibility(id);
        default:
            return ElementDataPiece(id, type, std::move(extraData));
        }
    }

    // Creates a parameter piece with an extra value.
    static ElementDataPiece parameter(const ElementId& id, const TypedValue& extra)
    {
        return ElementDataPiece(id, ElementDataPieceType::Parameter, extra);
    }

    // Creates a parameter piece with an extra value.
    static ElementDataPiece parameter(const ElementId& id, TypedValue&& extra)
    {
        return ElementDataPiece(id, ElementDataPieceType::Parameter, std::move(extra));
    }

    // Creates a property piece with an extra value.
    static ElementDataPiece property(const ElementId& id, const TypedValue& extra)
    {
        return ElementDataPiece(id, ElementDataPieceType::Property, extra);
    }

    // Creates a property piece with an extra value.
    static ElementDataPiece property(const ElementId& id, TypedValue&& extra)
    {
        return ElementDataPiece(id, ElementDataPieceType::Property, std::move(extra));
    }

    // Returns the element id.
    inline const ElementId& getId() const { return _id; }

    // Returns the piece type.
    inline ElementDataPieceType getType() const { return _type; }

    // Returns the extra value.
    inline const TypedValue& getExtra() const { return _extra; }

    friend inline bool operator<(const ElementDataPiece& lhs, const ElementDataPiece& rhs)
    {
        if (lhs._id < rhs._id) return true;
        else if (lhs._id > rhs._id) return false;
        else if (lhs._type < rhs._type) return true;
        else if (lhs._type > rhs._type) return false;
        else return lhs._extra < rhs._extra;
    }

private:
    ElementId _id;
    TypedValue _extra;
    ElementDataPieceType _type;
};

// Collects the data pieces associated with a specific element and the data pieces they depend on.
// The dependencies may include data pieces from other elements.
// The resulting dependency graph determines the execution order of chain updates.
// By default, a collected data piece does not trigger a chain update when it becomes dirty
// unless explicitly enabled.
// By convention, the completion data piece is the sink node of the dependency graph
// and usually has chain-update triggering enabled, but this is not enforced.
class WYDB_EXPORT ElementDataPieceCollector
{
public:
    // Returns the element id associated with this collector.
    virtual ElementId getId() const = 0;

    // Adds a data piece without any dependency information.
    //
    // @param dataPiece Data piece to be added.
    //
    // @return
    //   - Ok: the data piece was added successfully.
    //   - ElementIdMismatch: the specified data piece has a different element id.
    virtual wy::ErrorStatus append(
        const ElementDataPiece& dataPiece) = 0;

    // Adds a single dependency for the specified data piece.
    // Repeated append calls for the same data piece are merged incrementally.
    //
    // @param dataPiece Data piece to be added or updated.
    // @param dependency Dependency of the specified data piece.
    //
    // @return
    //   - Ok: the dependency was added successfully.
    //   - ElementIdMismatch: the specified data piece has a different element id.
    virtual wy::ErrorStatus append(
        const ElementDataPiece& dataPiece,
        const ElementDataPiece& dependency) = 0;

    // Adds a set of dependencies for the specified data piece.
    // Repeated append calls for the same data piece are merged incrementally.
    //
    // @param dataPiece Data piece to be added or updated.
    // @param dependencies Dependencies of the specified data piece.
    //
    // @return
    //   - Ok: the dependencies were added successfully.
    //   - ElementIdMismatch: the specified data piece has a different element id.
    virtual wy::ErrorStatus append(
        const ElementDataPiece& dataPiece,
        const std::set<ElementDataPiece>& dependencies) = 0;

    // Convenience overload for appending a dependency list.
    // Repeated append calls for the same data piece are merged incrementally.
    //
    // @param dataPiece Data piece to be added or updated.
    // @param dependencies Dependencies of the specified data piece.
    //
    // @return
    //   - Ok: the dependencies were added successfully.
    //   - ElementIdMismatch: the specified data piece has a different element id.
    virtual inline wy::ErrorStatus append(
        ElementDataPiece dataPiece,
        std::initializer_list<ElementDataPiece> dependencies)
    {
        return this->append(dataPiece, std::set<ElementDataPiece>(dependencies));
    }

    // Returns whether the specified data piece is contained in this collector.
    virtual bool contains(const ElementDataPiece& dataPiece) const = 0;

    // Returns whether dirtying the specified data piece will trigger chain update.
    // If the specified data piece is not contained in this collector, false is returned.
    virtual bool isChainUpdateEnabledOnDirty(
        const ElementDataPiece& dataPiece) const = 0;

    // Sets whether dirtying the specified data piece will trigger chain update.
    //
    // @param dataPiece Data piece to be configured.
    // @param enabled Whether chain update should be triggered when the data piece becomes dirty.
    //
    // @return
    //   - Ok: the configuration was updated successfully.
    //   - KeyNotFound: the specified data piece is not contained in this collector.
    virtual wy::ErrorStatus setChainUpdateEnabledOnDirty(
        const ElementDataPiece& dataPiece,
        bool enabled) = 0;
};

class Transaction;

// Chain update callback.
class WYDB_EXPORT ChainUpdateCallback
{
public:
    ChainUpdateCallback() = default;
    virtual ~ChainUpdateCallback() = default;
    ChainUpdateCallback(const ChainUpdateCallback&) = delete;
    ChainUpdateCallback& operator=(const ChainUpdateCallback&) = delete;
    ChainUpdateCallback(ChainUpdateCallback&&) = delete;
    ChainUpdateCallback& operator=(ChainUpdateCallback&&) = delete;

    // Called after one chain update cycle has completed.
    virtual void operator()(Transaction* pTrans) = 0;
};

using ChainUpdateCallbackSPtr = std::shared_ptr<ChainUpdateCallback>;

// Chain update callback manager.
class WYDB_EXPORT ChainUpdateCallbackManager
{
public:
    // Add a chain update callback.
    virtual void append(ChainUpdateCallbackSPtr callback) = 0;

protected:
    ChainUpdateCallbackManager() = default;
    virtual ~ChainUpdateCallbackManager() = default;
    ChainUpdateCallbackManager(const ChainUpdateCallbackManager&) = delete;
    ChainUpdateCallbackManager& operator=(const ChainUpdateCallbackManager&) = delete;
    ChainUpdateCallbackManager(ChainUpdateCallbackManager&&) = delete;
    ChainUpdateCallbackManager& operator=(ChainUpdateCallbackManager&&) = delete;
};

NS_WYDB_END

#endif // WYDB_CHAIN_UPDATE_H
