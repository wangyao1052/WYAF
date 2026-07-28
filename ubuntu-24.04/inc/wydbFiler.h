///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_FILER_H
#define WYDB_FILER_H

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include <wyVector2.h>
#include <wyVector3.h>
#include <wydbDefs.h>
#include <wydbDatabase.h>
#include <wydbFileFormat.h>
#include <wydbIdMapping.h>

NS_WYDB_BEG

class ElementId;

enum class FilerType
{
    Text   = 1,
    Binary = 2,
    Memory = 3,
};

// Abstract base class for reading serialized data.
class InFiler
{
protected:
    InFiler() = default;
    virtual ~InFiler() = default;
    InFiler(const InFiler&) = delete;
    InFiler& operator=(const InFiler&) = delete;
    InFiler(InFiler&&) = delete;
    InFiler& operator=(InFiler&&) = delete;

public:
    // Get this filer's type.
    virtual FilerType getFilerType() const = 0;

    // Get the file version used by this filer.
    virtual FileVersion getFileVersion() const = 0;

    // Get the current persistent status of this filer.
    virtual wy::ErrorStatus getFilerStatus() const = 0;

    // Read unsigned integer values.
    virtual InFiler& operator>>(std::uint8_t&) = 0;
    virtual InFiler& operator>>(std::uint16_t&) = 0;
    virtual InFiler& operator>>(std::uint32_t&) = 0;
    virtual InFiler& operator>>(std::uint64_t&) = 0;

    // Read signed integer values.
    virtual InFiler& operator>>(std::int8_t&) = 0;
    virtual InFiler& operator>>(std::int16_t&) = 0;
    virtual InFiler& operator>>(std::int32_t&) = 0;
    virtual InFiler& operator>>(std::int64_t&) = 0;

    // Read floating-point, boolean, and string values.
    virtual InFiler& operator>>(float&) = 0;
    virtual InFiler& operator>>(double&) = 0;
    virtual InFiler& operator>>(bool&) = 0;
    virtual InFiler& operator>>(std::string&) = 0;

    // Read framework value types.
    virtual InFiler& operator>>(ElementId&) = 0;
    virtual InFiler& operator>>(wy::Vector2&) = 0;
    virtual InFiler& operator>>(wy::Vector3&) = 0;

    // Read numBytes raw bytes into the caller-provided destination buffer.
    // pDest must point to a writable buffer of at least numBytes bytes.
    // The caller retains ownership of the buffer.
    virtual InFiler& readBytes(void* pDest, std::uint64_t numBytes) = 0;
};

// Abstract base class for writing serialized data.
class OutFiler
{
protected:
    OutFiler() = default;
    virtual ~OutFiler() = default;
    OutFiler(const OutFiler&) = delete;
    OutFiler& operator=(const OutFiler&) = delete;
    OutFiler(OutFiler&&) = delete;
    OutFiler& operator=(OutFiler&&) = delete;

public:
    // Get the runtime filer type.
    virtual FilerType getFilerType() const = 0;

    // Get the file version used by this filer.
    virtual FileVersion getFileVersion() const = 0;

    // Get the current persistent status of this filer.
    virtual wy::ErrorStatus getFilerStatus() const = 0;

    // Get the precision used when writing floating-point values.
    virtual int getPrecision() const = 0;
    // Set the precision used when writing floating-point values.
    virtual void setPrecision(int precision) = 0;

    // Write unsigned integer values.
    virtual OutFiler& operator<<(std::uint8_t) = 0;
    virtual OutFiler& operator<<(std::uint16_t) = 0;
    virtual OutFiler& operator<<(std::uint32_t) = 0;
    virtual OutFiler& operator<<(std::uint64_t) = 0;

    // Write signed integer values.
    virtual OutFiler& operator<<(std::int8_t) = 0;
    virtual OutFiler& operator<<(std::int16_t) = 0;
    virtual OutFiler& operator<<(std::int32_t) = 0;
    virtual OutFiler& operator<<(std::int64_t) = 0;

    // Write floating-point, boolean, and string values.
    virtual OutFiler& operator<<(float) = 0;
    virtual OutFiler& operator<<(double) = 0;
    virtual OutFiler& operator<<(bool) = 0;
    virtual OutFiler& operator<<(const std::string&) = 0;

    // Write framework value types.
    virtual OutFiler& operator<<(const ElementId&) = 0;
    virtual OutFiler& operator<<(const wy::Vector2&) = 0;
    virtual OutFiler& operator<<(const wy::Vector3&) = 0;

    // Write raw bytes from the specified source buffer.
    virtual OutFiler& writeBytes(const void* pSrc, std::uint64_t numBytes) = 0;
};

class MemoryFiler : public wydb::InFiler, public wydb::OutFiler
{
public:
    static WYDB_EXPORT MemoryFiler* create(const Database* pDb);

    // Get the current persistent status of this filer.
    virtual wy::ErrorStatus getFilerStatus() const override = 0;

    // Get the size of the memory buffer, in bytes.
    virtual std::size_t getSize() const = 0;

    // Get the current cursor position, in bytes from the beginning.
    virtual std::size_t getPosition() const = 0;

    // Move the current cursor position to the specified byte offset from the beginning.
    virtual wy::ErrorStatus setPosition(std::size_t position) = 0;

    // Get the current allocated capacity of the memory buffer, in bytes.
    virtual std::size_t getCapacity() const = 0;

    // Reserve storage for at least capacity bytes.
    virtual void reserveCapacity(std::size_t capacity) = 0;

    // Get the id mapping currently held by this filer.
    virtual const IdMapping& getIdMapping() const = 0;

    // Set the id mapping by copy.
    virtual void setIdMapping(const IdMapping& idMap) = 0;

    // Set the id mapping by move.
    virtual void setIdMapping(IdMapping&& idMap) = 0;

    // Release the id mapping currently held by this filer.
    // After the call, this filer's id mapping is empty,
    // and the returned mapping can be reused by another filer.
    virtual IdMapping releaseIdMapping() = 0;

protected:
    MemoryFiler() = default;
    MemoryFiler(const MemoryFiler&) = delete;
    MemoryFiler& operator=(const MemoryFiler&) = delete;
    MemoryFiler(MemoryFiler&&) = delete;
    MemoryFiler& operator=(MemoryFiler&&) = delete;
public:
    virtual ~MemoryFiler() = default;
};

NS_WYDB_END

#endif // WYDB_FILER_H
