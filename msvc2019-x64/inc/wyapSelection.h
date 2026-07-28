///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_SELECTION_H
#define WYAP_SELECTION_H

#include <string>
#include <vector>
#include <list>
#include <memory>

#include <wyVector3.h>
#include <wyIterator.h>
#include <wydbElementId.h>
#include <wyapDefs.h>

NS_WYAP_BEG

class SelectionSetImpl;

class WYAP_EXPORT SelectionData
{
public:
    SelectionData() = default;
    virtual ~SelectionData() = default;
    SelectionData(const SelectionData&) = default;
    SelectionData& operator=(const SelectionData&) = default;
    SelectionData(SelectionData&&) = default;
    SelectionData& operator=(SelectionData&&) = default;

    // Create a deep copy of this selection data object.
    // The caller takes ownership of the returned object.
    virtual SelectionData* clone() const = 0;
};

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable: 4251)
#endif

class WYAP_EXPORT Selection
{
public:
    // Construct a whole-element selection for the specified element.
    explicit Selection(const wydb::ElementId& elemId)
        : _selectionType(kWholeElement), _elemId(elemId)
    {
    }

    // Construct a selection for the specified element.
    // subPath identifies a selected sub-part when selectionType is not kWholeElement.
    Selection(
        unsigned int selectionType,
        const wydb::ElementId& elemId,
        const std::string& subPath = "")
        : _selectionType(selectionType)
        , _elemId(elemId)
        , _subPath(subPath)
    {
    }

    Selection(const Selection& rhs)
        : _selectionType(rhs._selectionType)
        , _elemId(rhs._elemId)
        , _subPath(rhs._subPath)
        , _pickPos(rhs._pickPos)
        , _data(rhs._data ? std::shared_ptr<SelectionData>(rhs._data->clone()) : nullptr)
    {
    }

    Selection& operator=(const Selection& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        _selectionType = rhs._selectionType;
        _elemId = rhs._elemId;
        _subPath = rhs._subPath;
        _pickPos = rhs._pickPos;
        _data = rhs._data ? std::shared_ptr<SelectionData>(rhs._data->clone()) : nullptr;

        return *this;
    }

    Selection(Selection&& rhs) noexcept = default;
    Selection& operator=(Selection && rhs) noexcept = default;
    ~Selection() = default;

    // Selection type value identifying the whole element.
    static constexpr unsigned int kWholeElement = 0;

    // Get the selection type.
    // kWholeElement identifies the whole element.
    // Other values are reserved for library users to define custom selection types.
    unsigned int getSelectionType() const
    {
        return _selectionType;
    }

    // Set the selection type.
    void setSelectionType(unsigned int selectionType)
    {
        _selectionType = selectionType;
    }

    // Get the selected element id.
    wydb::ElementId getElementId() const
    {
        return _elemId;
    }

    // Set the selected element id.
    void setElementId(const wydb::ElementId& elemId)
    {
        _elemId = elemId;
    }

    // Get the optional subpath identifying a sub-part within the selected element.
    const std::string& getSubPath() const
    {
        return _subPath;
    }

    // Set the optional subpath identifying a sub-part within the selected element.
    void setSubPath(const std::string& subPath)
    {
        _subPath = subPath;
    }

    // Get the pick position.
    wy::Vector3 getPickPosition() const
    {
        return _pickPos;
    }

    // Set the pick position.
    void setPickPosition(const wy::Vector3& pickPos)
    {
        _pickPos = pickPos;
    }

    // Get the optional additional data associated with this selection.
    std::shared_ptr<const SelectionData> getData() const
    {
        return _data;
    }

    // Set the optional additional data associated with this selection.
    void setData(std::shared_ptr<SelectionData> data)
    {
        _data = std::move(data);
    }

    // Ordering ignores pick position and additional data.
    bool operator<(const Selection& rhs) const
    {
        if (_elemId != rhs._elemId)
            return _elemId < rhs._elemId;

        if (_selectionType != rhs._selectionType)
            return _selectionType < rhs._selectionType;

        // subPath is not meaningful for whole-element selections.
        if (_selectionType == kWholeElement)
            return false;

        return _subPath < rhs._subPath;
    }

    // Equality ignores pick position and additional data.
    bool operator==(const Selection& rhs) const
    {
        if (_elemId != rhs._elemId)
            return false;

        if (_selectionType != rhs._selectionType)
            return false;

        // subPath is not meaningful for whole-element selections.
        if (_selectionType == kWholeElement)
            return true;

        return _subPath == rhs._subPath;
    }

    bool operator!=(const Selection& rhs) const
    {
        return !(*this == rhs);
    }

private:
    // Selection type.
    unsigned int _selectionType = kWholeElement;

    // Selected element id.
    wydb::ElementId _elemId;

    // Optional subpath identifying a sub-part within the selected element.
    // Ignored when the selection type is kWholeElement.
    std::string _subPath;

    // Pick position.
    wy::Vector3 _pickPos;

    // Optional additional data associated with this selection.
    std::shared_ptr<SelectionData> _data;
};

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

class WYAP_EXPORT SelectionSet
{
public:
    SelectionSet();
    ~SelectionSet();
    SelectionSet(const SelectionSet&);
    SelectionSet& operator=(const SelectionSet&);
    SelectionSet(SelectionSet&&);
    SelectionSet& operator=(SelectionSet&&);

    // Returns true if the selection set is empty.
    bool isEmpty() const;

    // Get the number of selections in the selection set.
    unsigned int getCount() const;

    // Returns true if the selection set contains the specified selection.
    bool contains(const Selection& sel) const;

    // Returns true if the selection set contains one or more selections
    // associated with the specified element id.
    bool contains(const wydb::ElementId& id) const;

    // Get all selections associated with the specified element id.
    // @param id Element id to query.
    // @param sels Output selections associated with id.
    // @return Number of selections written to sels.
    unsigned int getSelections(
        const wydb::ElementId& id,
        std::list<wyap::Selection>& sels) const;

    // Add the specified selection to the selection set.
    // Returns true if the selection set was changed.
    bool add(const Selection& sel);

    // Remove the specified selection from the selection set.
    // Returns true if the selection set was changed.
    bool remove(const Selection& sel);

    // Remove all selections associated with the specified element id.
    // Returns true if the selection set was changed.
    bool remove(const wydb::ElementId& id);

    // Remove all selections from the selection set.
    void clear();

    // Swap the contents of this selection set with rhs.
    void swap(SelectionSet& rhs);

    // Create a const iterator for traversing the selections.
    wy::Iterator<const Selection&> createIterator() const;

private:
    SelectionSetImpl* _pImpl;
};

NS_WYAP_END

#endif // WYAP_SELECTION_H