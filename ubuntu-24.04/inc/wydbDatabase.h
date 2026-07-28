///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_DATABASE_H
#define WYDB_DATABASE_H

#include <string>
#include <set>
#include <memory>

#include <wyErrors.h>
#include <wyIterator.h>
#include <wydbDefs.h>
#include <wydbElementId.h>
#include <wydbElement.h>
#include <wydbFileFormat.h>
#include <wydbChainUpdate.h>

NS_WYDB_BEG

class Element;
class DatabaseImpl;
class TransactionManager;
class Transaction;
class DatabaseReactor;
class DatabaseChangeDetailsImpl;

class WYDB_EXPORT Database
{
protected:
    explicit Database(const FileFormatConfig& fileFormatConfig);
    virtual ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&&) = delete;
    Database& operator=(Database&&) = delete;
    friend class SystemInternals;

public:
    // Get an element by id.
    // @param id Element id.
    // @param pElem Output element pointer.
    // @return
    //   - Ok: succeeded.
    //   - NullElementId: id is null.
    //   - ElementNotFound: the element was not found.
    wy::ErrorStatus getElement(const ElementId& id, const Element*& pElem) const;

    // Get an element by id.
    // @param id Element id.
    // @return Element pointer on success; otherwise nullptr.
    inline const Element* getElement(const ElementId& id) const
    {
        const Element* pElem(nullptr);
        this->getElement(id, pElem);
        return pElem;
    }

    // Create an iterator for traversing all element ids in the database.
    // @return Iterator of element ids.
    wy::Iterator<ElementId> createIterator() const;

    // Get the transaction manager.
    // @return Transaction manager pointer.
    TransactionManager* getTransactionManager() const;

    // Add a database reactor.
    // @param pReactor Pointer to the reactor to add.
    // @return
    //   - Ok: reactor was added successfully.
    //   - TransactionManagerBusy: the transaction system is busy and adding
    //     database reactors is not allowed now.
    //   - NullInputPointer: pReactor is nullptr.
    //   - DuplicateKey: pReactor has already been added.
    wy::ErrorStatus addReactor(DatabaseReactor* pReactor) const;

    // Remove a database reactor.
    // @param pReactor Pointer to the reactor to remove.
    // @return
    //   - Ok: reactor was removed successfully.
    //   - TransactionManagerBusy: the transaction system is busy and removing
    //     database reactors is not allowed now.
    //   - NullInputPointer: pReactor is nullptr.
    //   - KeyNotFound: pReactor has not been added.
    wy::ErrorStatus removeReactor(DatabaseReactor* pReactor) const;

    // Writes the database to the specified file.
    // Returns Ok if successful.
    struct WriteFileOption
    {
        FileType fileType = FileType::Binary;
        FileVersion fileVersion = FileVersion(0, 0);
        int precision = 12;
    };
    wy::ErrorStatus writeFile(
        const std::string& u8FileFullPath,
        const WriteFileOption& option) const;

    // Reads the specified file into the database.
    // Returns Ok if successful.
    struct ReadFileOption
    {
        FileType fileType = FileType::Binary;
    };
    wy::ErrorStatus readFile(
        const std::string& u8FileFullPath,
        const ReadFileOption& option);

    // Get file version.
    // @return File version.
    FileVersion getFileVersion() const;

private:
    DatabaseImpl* _pImpl;
};

// Cause of a database change notification.
enum class DatabaseChangeCause
{
    EndTransaction = 1,
    AbortTransaction = 2,
    Undo = 3,
    Redo = 4,
};

class WYDB_EXPORT DatabaseChangeDetails
{
public:
    DatabaseChangeDetails();
    ~DatabaseChangeDetails();
    DatabaseChangeDetails(const DatabaseChangeDetails&) = delete;
    DatabaseChangeDetails& operator=(const DatabaseChangeDetails&) = delete;
    DatabaseChangeDetails(DatabaseChangeDetails&&) = delete;
    DatabaseChangeDetails& operator=(DatabaseChangeDetails&&) = delete;
    friend class SystemInternals;

    // Check whether the specified data piece of the element is dirty.
    // @param id Element id.
    // @param dataPieceType Data piece type.
    // @return True if the specified data piece is dirty; otherwise false.
    bool isDataPieceDirty(
        const wydb::ElementId& id,
        wydb::ElementDataPieceType dataPieceType) const;

private:
    DatabaseChangeDetailsImpl* _pImpl;
};

struct DatabaseChangeInfo
{
    // Cause of this database change.
    DatabaseChangeCause cause;
    // Ids of added elements.
    std::set<ElementId> addedIds;
    // Ids of modified elements.
    std::set<ElementId> modifiedIds;
    // Ids of erased elements.
    std::set<ElementId> erasedIds;
    // Additional change details.
    DatabaseChangeDetails details;
};

class WYDB_EXPORT DatabaseReactor
{
public:
    DatabaseReactor() = default;
    virtual ~DatabaseReactor() = default;
    DatabaseReactor(const DatabaseReactor&) = delete;
    DatabaseReactor& operator=(const DatabaseReactor&) = delete;
    DatabaseReactor(DatabaseReactor&&) = delete;
    DatabaseReactor& operator=(DatabaseReactor&&) = delete;
    friend class SystemInternals;

public:
    // Called after the database has changed.
    // @param pDatabase Database pointer.
    // @param pTransaction Transaction pointer associated with the change.
    // @param info Change information.
    virtual void onDatabaseChanged(
        const wydb::Database* pDatabase,
        const wydb::Transaction* pTransaction,
        const DatabaseChangeInfo& info) {};
};

// Delete a database created by the library.
// @param pDb Database pointer.
WYDB_EXPORT void deleteDatabase(const Database* pDb);

NS_WYDB_END

#endif // WYDB_DATABASE_H

