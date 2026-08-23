///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_TRANSACTION_H
#define WYDB_TRANSACTION_H

#include <map>
#include <memory>

#include <wydbDefs.h>
#include <wyErrors.h>
#include <wydbChainUpdateFeedback.h>

NS_WYDB_BEG

class Element;
class ElementId;
class TransactionReactor;

// Transaction status.
enum class TransactionStatus
{
    // Initial value. The transaction has not been started yet.
    Initialized = 0,
    // Internal status. The transaction is in the process of starting.
    Starting = 1,
    // The transaction has started.
    Started = 2,
    // Internal status. The transaction is in the process of committing.
    Committing = 3,
    // The transaction has been committed.
    Committed = 4,
    // Internal status. The transaction is in the process of aborting.
    Aborting = 5,
    // The transaction has been aborted.
    Aborted = 6,
    // An error occurred while processing the transaction.
    Error = 7,
};

enum class ChainUpdateScope : unsigned short
{
    // Execute chain update only within the set of elements directly involved
    // in the current transaction. Do not cascade to external dependent elements.
    Local   = 1,
    // Execute chain update and cascade through dependent elements.
    Cascade = 2,
};

struct TransactionOption
{
    // Scope used when executing chain update during transaction commit.
    ChainUpdateScope chainUpdateScope = ChainUpdateScope::Cascade;
};

class WYDB_EXPORT Transaction
{
public:
    // Get name.
    virtual const std::string& getName() const = 0;

    // Whether this is a transaction group.
    virtual bool isGroup() const = 0;

    // Get transaction status.
    virtual TransactionStatus getStatus() const = 0;

    // Get the database for this transaction.
    virtual Database* getDatabase() const = 0;

    // Get an element for read access by id.
    // @param id Element id.
    // @param pElement Output element pointer.
    // @return
    //   - Ok: succeeded.
    //   - NullElementId: id is null.
    //   - ElementNotFound: the element was not found.
    virtual wy::ErrorStatus getElementForRead(const ElementId& id, const Element*& pElement) const = 0;
    
    // Get an element by id for read access.
    // Returns the element pointer on success; otherwise returns nullptr.
    virtual const Element* getElementForRead(const ElementId& id) const = 0;

    // Get an element for write access by id.
    // @param id Element id.
    // @param pElement Output element pointer.
    // @return
    //   - Ok: succeeded.
    //   - NullElementId: id is null.
    //   - ElementNotFound: the element was not found.
    //   - TransactionGroupNotSupported: this operation is not supported by transaction groups.
    //   - ElementErased: the element has been erased.
    //   - NewElementNotAddedToTransaction: the element is newly created but has not
    //     been added to the transaction.
    virtual wy::ErrorStatus getElementForWrite(const ElementId& id, Element*& pElement) = 0;

    // Get an element by id for write access.
    // Returns the element pointer on success; otherwise returns nullptr.
    virtual Element* getElementForWrite(const ElementId& id) = 0;

    // Add a newly created element to the transaction.
    // @param pElement Input element pointer.
    // @return
    //   - Ok: succeeded.
    //   - NullElementPointer: pElement is nullptr.
    //   - TransactionGroupNotSupported: this operation is not supported by transaction groups.
    //   - NotNewlyCreated: pElement is not a newly created element.
    virtual wy::ErrorStatus addNewlyCreatedElement(Element* pElement) = 0;

protected:
    Transaction();
    virtual ~Transaction();
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
    Transaction(Transaction&&) = delete;
    Transaction& operator=(Transaction&&) = delete;
};

class WYDB_EXPORT TransactionManager
{
public:
    // Get the database to which this transaction manager belongs.
    virtual Database* getDatabase() const = 0;

    // Start a transaction.
    // @param name Transaction name.
    // @param option Transaction options.
    // @param pTransaction Output transaction pointer.
    // @return
    //   - Ok: succeeded.
    //   - TransactionManagerBusy: the transaction manager is busy now.
    //   - NotCurrentlyAllowed: a new transaction cannot be started while the
    //     current active transaction is a transaction leaf.
    virtual wy::ErrorStatus startTransaction(
        const std::string& name,
        const TransactionOption& option,
        Transaction*& pTransaction) = 0;

    // Start a transaction.
    // Returns the newly started transaction on success; otherwise returns nullptr.
    virtual Transaction* startTransaction(
        const std::string& name = "",
        const TransactionOption& option = TransactionOption()) = 0;

    // Start a transaction group.
    // A transaction group can be nested inside another transaction group.
    // @param name Transaction group name.
    // @param pTransaction Output transaction pointer.
    // @return
    //   - Ok: succeeded.
    //   - TransactionManagerBusy: the transaction manager is busy now.
    //   - NotCurrentlyAllowed: a new transaction cannot be started while the
    //     current active transaction is a transaction leaf.
    virtual wy::ErrorStatus startTransactionGroup(
        const std::string& name,
        Transaction*& pTransaction) = 0;

    // Start a transaction group.
    // Returns the newly started transaction group on success; otherwise returns nullptr.
    virtual Transaction* startTransactionGroup(const std::string& name = "") = 0;

    // Commit the current transaction.
    // @return
    //   - Ok: succeeded.
    //   - TransactionManagerBusy: the transaction manager is busy now.
    //   - NoActiveTransaction: there is no active transaction.
    virtual wy::ErrorStatus endTransaction() = 0;

    // Abort the current transaction.
    // @return
    //   - Ok: succeeded.
    //   - TransactionManagerBusy: the transaction manager is busy.
    //   - NoActiveTransaction: there is no active transaction.
    virtual wy::ErrorStatus abortTransaction() = 0;

    // Merge all child transactions of the current transaction group into one.
    // @return
    //   - Ok: succeeded.
    //   - TransactionManagerBusy: the transaction manager is busy.
    //   - NoActiveTransaction: there is no active transaction.
    //   - TransactionLeafNotSupported: the current active transaction is a
    //     transaction leaf and does not support merge.
    virtual wy::ErrorStatus mergeTransaction() = 0;

    // Get the current active transaction.
    // Returns nullptr if there is no active transaction.
    virtual Transaction* getActiveTransaction() const = 0;

    // Undo the most recent committed transaction.
    // @return
    //   - Ok: succeeded.
    //   - TransactionManagerBusy: the transaction manager is busy.
    //   - NotCurrentlyAllowed: undo is not allowed in the current state.
    //   - NotAvailable: there is no transaction available to undo.
    virtual wy::ErrorStatus undo() = 0;

    // Redo the most recently undone transaction.
    // @return
    //   - Ok: succeeded.
    //   - TransactionManagerBusy: the transaction manager is busy.
    //   - NotCurrentlyAllowed: redo is not allowed in the current state.
    //   - NotAvailable: there is no transaction available to redo.
    virtual wy::ErrorStatus redo() = 0;

    // Whether undo is currently available.
    virtual bool canUndo() const = 0;
    // Whether redo is currently available.
    virtual bool canRedo() const = 0;

    // Get the number of undo records currently available.
    virtual std::size_t getUndoRecordCount() const = 0;

    // Get an undo record by index.
    // @param index Zero-based undo record index.
    // @return
    //   - Pointer to the undo record if index is valid.
    //   - nullptr if index is out of range.
    virtual const Transaction* getUndoRecord(std::size_t index) const = 0;

    // Get the number of redo records currently available.
    virtual std::size_t getRedoRecordCount() const = 0;

    // Get a redo record by index.
    // @param index Zero-based redo record index.
    // @return
    //   - Pointer to the redo record if index is valid.
    //   - nullptr if index is out of range.
    virtual const Transaction* getRedoRecord(std::size_t index) const = 0;

    // Clear all undo and redo records.
    //
    // @return
    //   - Ok: undo and redo records were cleared successfully.
    //   - NotCurrentlyAllowed: clearing undo and redo records is not allowed now.
    virtual wy::ErrorStatus clearUndoRedoRecords() = 0;

    // Get chain update feedback for the given element.
    virtual ChainUpdateFeedbackSPtr getChainUpdateFeedback(const ElementId& id) const = 0;

    // Add a transaction reactor.
    virtual wy::ErrorStatus addReactor(TransactionReactor* pReactor) const = 0;
    // Remove a transaction reactor.
    virtual wy::ErrorStatus removeReactor(TransactionReactor* pReactor) const = 0;

protected:
    TransactionManager();
    virtual ~TransactionManager();
    TransactionManager(const TransactionManager&) = delete;
    TransactionManager& operator=(const TransactionManager&) = delete;
    TransactionManager(TransactionManager&&) = delete;
    TransactionManager& operator=(TransactionManager&&) = delete;
};

class WYDB_EXPORT TransactionReactor
{
public:
    TransactionReactor() = default;
    virtual ~TransactionReactor() = default;
    TransactionReactor(const TransactionReactor&) = delete;
    TransactionReactor& operator=(const TransactionReactor&) = delete;
    TransactionReactor(TransactionReactor&&) = delete;
    TransactionReactor& operator=(TransactionReactor&&) = delete;

    // Called right after a transaction has been started.
    virtual void onTransactionStarted(Transaction* pTrans) {}
    // Called right after a transaction has been committed.
    virtual void onTransactionEnded(Transaction* pTrans) {}
    // Called right after a transaction has been aborted.
    virtual void onTransactionAborted(Transaction* pTrans) {}
    // Called right after a transaction has been undone.
    virtual void onTransactionUndone(Transaction* pTrans) {}
    // Called right after a transaction has been redone.
    virtual void onTransactionRedone(Transaction* pTrans) {}
};

NS_WYDB_END

#endif // WYDB_TRANSACTION_H