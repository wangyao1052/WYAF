///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_CLIP_DATA_H
#define WYAP_CLIP_DATA_H

#include <string>
#include <vector>

#include <wydbElementId.h>
#include <wyapDefs.h>

NS_WYAP_BEG

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable: 4251)
#endif

class WYAP_EXPORT ClipData
{
public:
    ClipData() = default;
    virtual ~ClipData() = default;
    ClipData(const ClipData&) = delete;
    ClipData& operator=(const ClipData&) = delete;
    ClipData(ClipData&&) = delete;
    ClipData& operator=(ClipData&&) = delete;

    // Get the description of this clip data.
    const std::string& getDescription() const
    {
        return _description;
    }

    // Set the description of this clip data.
    void setDescription(const std::string& description)
    {
        _description = description;
    }

private:
    std::string _description;
};

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

class ElementsClipDataImpl;

class WYAP_EXPORT ElementsClipData : public ClipData
{
public:
    ElementsClipData();
    ~ElementsClipData();
    ElementsClipData(const ElementsClipData&) = delete;
    ElementsClipData& operator=(const ElementsClipData&) = delete;
    ElementsClipData(ElementsClipData&&) = delete;
    ElementsClipData& operator=(ElementsClipData&&) = delete;
    friend class SystemInternals;

public:
    // Summary information for one source element stored in this clip data.
    struct Item
    {
        // Id of the source element in the original database.
        wydb::ElementId sourceId = wydb::ElementId::kNull;
        // Runtime class name of the source element.
        std::string className;
    };

    // Get summary items for the elements stored in this clip data.
    std::vector<Item> getClipItems() const;

private:
    ElementsClipDataImpl* _pImpl;
};

NS_WYAP_END

#endif // WYAP_CLIP_DATA_H