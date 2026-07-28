///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_ID_MAPPING_H
#define WYDB_ID_MAPPING_H

#include <memory>

#include <wydbDefs.h>
#include <wydbElementId.h>

NS_WYDB_BEG

class Database;
class IdMappingImpl;

class WYDB_EXPORT IdMapping
{
public:
    IdMapping();
    ~IdMapping();
    IdMapping(const IdMapping& other);
    IdMapping& operator=(const IdMapping& other);
    IdMapping(IdMapping&& other) noexcept;
    IdMapping& operator=(IdMapping&& other) noexcept;

    Database* getSourceDatabase() const;
    void setSourceDatabase(wydb::Database* pSrcDb);

    Database* getDestDatabase() const;
    void setDestDatabase(wydb::Database* pDestDb);

    bool hasKey(const wydb::ElementId& id) const;
    bool find(const wydb::ElementId& key, wydb::ElementId& value) const;

    void insert(const wydb::ElementId& key, const wydb::ElementId& value);
    bool erase(const wydb::ElementId& key);
    void clear();

private:
    IdMappingImpl* _pImpl;
};

NS_WYDB_END

#endif // WYDB_ID_MAPPING_H