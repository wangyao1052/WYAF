///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_CLIPBOARD_H
#define WYAP_CLIPBOARD_H

#include <string>
#include <vector>
#include <memory>

#include <wydbElement.h>
#include <wydbDatabase.h>
#include <wydbTransaction.h>
#include <wyapDefs.h>
#include <wyapClipData.h>

NS_WYAP_BEG

class ClipboardImpl;

class WYAP_EXPORT Clipboard
{
private:
    Clipboard();
    ~Clipboard();
    Clipboard(const Clipboard&) = delete;
    Clipboard& operator=(const Clipboard&) = delete;
    Clipboard(Clipboard&&) = delete;
    Clipboard& operator=(Clipboard&&) = delete;
    friend class SystemInternals;

public:
    // Get the current clip data stored in the clipboard.
    std::shared_ptr<const ClipData> getClipData() const;

    // Set the current clip data stored in the clipboard.
    void setClipData(std::shared_ptr<ClipData> clipData);

public:
    // Create built-in elements clip data from the specified elements.
    // @param pSourceDb Source database.
    // @param ids Ids of the source elements to copy.
    // @param outClipData Output built-in elements clip data.
    // @return
    //   - Ok: succeeded.
    //   - NullDatabasePointer: pDb is nullptr.
    //   - ElementNotFound: one or more element ids were not found in pDb.
    //   - FilerError: failed to serialize one or more elements.
    static wy::ErrorStatus newElementsClipData(
        const wydb::Database* pSourceDb,
        const std::vector<wydb::ElementId>& ids,
        std::shared_ptr<ElementsClipData>& outClipData);

    // Create built-in elements clip data from the specified elements.
    // Returns nullptr on failure.
    static std::shared_ptr<ElementsClipData> newElementsClipData(
        const wydb::Database* pSourceDb,
        const std::vector<wydb::ElementId>& ids);

    // Create elements in the target database from built-in elements clip data.
    // @param pTargetDb Target database.
    // @param elementsClipData Source built-in elements clip data.
    // @param newlyCreatedElements Output newly created elements.
    // @return Ok if successful.
    static wy::ErrorStatus createElements(
        wydb::Transaction* pTransaction,
        const ElementsClipData& elementsClipData,
        std::vector<wydb::Element*>& newlyCreatedElements);

private:
    ClipboardImpl* _pImpl;
};

NS_WYAP_END

#endif // WYAP_CLIPBOARD_H