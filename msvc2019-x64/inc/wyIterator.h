///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY_ITERATOR_H
#define WY_ITERATOR_H

#include <memory>
#include <cassert>

#include <wyDefs.h>

NS_WY_BEG

template<typename TYPE>
class IteratorImpl
{
public:
    virtual ~IteratorImpl() = default;

    // Returns true if the iterator has reached the end position.
    virtual bool isDone() const = 0;

    // Advance the iterator to the next position.
    virtual void moveNext() = 0;

    // Get the current item.
    // The behavior is undefined if the iterator is at the end position.
    virtual TYPE current() const = 0;
};

// Move-only iterator wrapper.
template<typename TYPE>
class Iterator
{
public:
    // Construct an iterator from the specified implementation object.
    // @param pImpl Iterator implementation object.
    explicit Iterator(std::unique_ptr<IteratorImpl<TYPE>> pImpl)
        : _pImpl(std::move(pImpl))
    {
    }

    ~Iterator() = default;

    Iterator(const Iterator&) = delete;
    Iterator& operator=(const Iterator&) = delete;

    Iterator(Iterator&&) noexcept = default;
    Iterator& operator=(Iterator&&) noexcept = default;

    // Returns true if the iterator has reached the end position.
    bool isDone() const
    {
        assert(_pImpl);
        return _pImpl->isDone();
    }

    // Advance the iterator to the next position.
    void moveNext()
    {
        assert(_pImpl);
        _pImpl->moveNext();
    }

    // Get the current item.
    // The behavior is undefined if the iterator is at the end position.
    TYPE current() const
    {
        assert(_pImpl);
        return _pImpl->current();
    }

private:
    std::unique_ptr<IteratorImpl<TYPE>> _pImpl;
};

NS_WY_END

#endif // WY_ITERATOR_H