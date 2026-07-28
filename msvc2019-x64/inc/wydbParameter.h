///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_PARAMETER_H
#define WYDB_PARAMETER_H

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

#include <wyErrors.h>
#include <wyVector2.h>
#include <wyVector3.h>
#include <wyrxClassExtension.h>
#include <wydbDefs.h>
#include <wydbElementId.h>

NS_WYDB_BEG

class ParameterValue;
class AnyParameterValue;
typedef std::unique_ptr<ParameterValue> ParameterValueUPtr;

class ParameterDefinition
{
protected:
    ParameterDefinition() = default;
    virtual ~ParameterDefinition() = default;
    ParameterDefinition(const ParameterDefinition&) = default;
    ParameterDefinition(ParameterDefinition&&) = default;
    ParameterDefinition& operator=(const ParameterDefinition&) = default;
    ParameterDefinition& operator=(ParameterDefinition&&) = default;

public:
    virtual const std::string& getClassName() const = 0;
    virtual const std::string& getName() const = 0;
    virtual const std::string& getDescription() const = 0;
    virtual bool isVisible() const = 0;
    virtual bool isReadonly() const = 0;
};

class WYDB_EXPORT ParameterValue
{
public:
    virtual ~ParameterValue() {}

    enum class Type
    {
        Integer   = 1,
        Double    = 2,
        Boolean   = 3,
        String    = 4,
        ElementId = 5,
        Vector2   = 6,
        Vector3   = 7,
        Any       = 8,
    };

    static ParameterValueUPtr createInteger(int value);
    static ParameterValueUPtr createDouble(double value);
    static ParameterValueUPtr createBoolean(bool value);
    static ParameterValueUPtr createString(const std::string& value);
    static ParameterValueUPtr createElementId(const ElementId& value);
    static ParameterValueUPtr createVector2(const wy::Vector2& value);
    static ParameterValueUPtr createVector3(const wy::Vector3& value);
    template<typename T>
    static ParameterValueUPtr createAny(T&& value);

    virtual Type getType() const = 0;

    bool isInteger() const { return Type::Integer == this->getType(); }
    bool isDouble() const { return Type::Double == this->getType(); }
    bool isBoolean() const { return Type::Boolean == this->getType(); }
    bool isString() const { return Type::String == this->getType(); }
    bool isElementId() const { return Type::ElementId == this->getType(); }
    bool isVector2() const { return Type::Vector2 == this->getType(); }
    bool isVector3() const { return Type::Vector3 == this->getType(); }
    bool isAny() const { return Type::Any == this->getType(); }

    virtual int asInteger() const { return 0; }
    virtual double asDouble() const { return 0.0; }
    virtual bool asBoolean() const { return false; }
    virtual std::string asString() const { return std::string(""); }
    virtual ElementId asElementId() const { return ElementId::kNull; }
    virtual wy::Vector2 asVector2() const { return wy::Vector2::kZero; }
    virtual wy::Vector3 asVector3() const { return wy::Vector3::kZero; }

    virtual bool setValue(int value) { return false; }
    virtual bool setValue(double value) { return false; }
    virtual bool setValue(bool value) { return false; }
    virtual bool setValue(const std::string& value) { return false; }
    virtual bool setValue(const ElementId& value) { return false; }
    virtual bool setValue(const wy::Vector2& value) { return false; }
    virtual bool setValue(const wy::Vector3& value) { return false; }

    virtual ParameterValueUPtr clone() const = 0;
    virtual bool equals(const ParameterValue& other) const = 0;
};

class IntegerParameterValue : public ParameterValue
{
public:
    explicit IntegerParameterValue(int value) : _value(value) {}

    virtual Type getType() const override
    {
        return Type::Integer;
    }

    virtual int asInteger() const override { return _value; }
    int getValue() const { return _value; }
    virtual bool setValue(int value) override
    {
        _value = value;
        return true; 
    }

    virtual ParameterValueUPtr clone() const override
    {
        return std::make_unique<IntegerParameterValue>(_value);
    }

    virtual bool equals(const ParameterValue& other) const override
    {
        return other.getType() == Type::Integer && _value == other.asInteger();
    }

private:
    int _value;
};

class DoubleParameterValue : public ParameterValue
{
public:
    explicit DoubleParameterValue(double value) : _value(value) {}

    virtual Type getType() const override
    {
        return Type::Double;
    }

    virtual double asDouble() const override { return _value; }
    double getValue() const { return _value; }
    virtual bool setValue(double value) override
    {
        _value = value;
        return true;
    }

    virtual ParameterValueUPtr clone() const override
    {
        return std::make_unique<DoubleParameterValue>(_value);
    }

    virtual bool equals(const ParameterValue& other) const override
    {
        return other.getType() == Type::Double && _value == other.asDouble();
    }

private:
    double _value;
};

class BooleanParameterValue : public ParameterValue
{
public:
    explicit BooleanParameterValue(bool value) : _value(value) {}

    virtual Type getType() const override
    {
        return Type::Boolean;
    }

    virtual bool asBoolean() const override { return _value; }
    bool getValue() const { return _value; }
    virtual bool setValue(bool value) override
    {
        _value = value;
        return true;
    }

    virtual ParameterValueUPtr clone() const override
    {
        return std::make_unique<BooleanParameterValue>(_value);
    }

    virtual bool equals(const ParameterValue& other) const override
    {
        return other.getType() == Type::Boolean && _value == other.asBoolean();
    }

private:
    bool _value;
};

class StringParameterValue : public ParameterValue
{
public:
    explicit StringParameterValue(const std::string& value) : _value(value) {}

    virtual Type getType() const override
    {
        return Type::String;
    }

    virtual std::string asString() const override { return _value; }
    const std::string& getValue() const { return _value; }
    virtual bool setValue(const std::string& value) override
    {
        _value = value;
        return true;
    }

    virtual ParameterValueUPtr clone() const override
    {
        return std::make_unique<StringParameterValue>(_value);
    }

    virtual bool equals(const ParameterValue& other) const override
    {
        return other.getType() == Type::String && _value == other.asString();
    }

private:
    std::string _value;
};

class ElementIdParameterValue : public ParameterValue
{
public:
    explicit ElementIdParameterValue(const ElementId& value) : _value(value) {}

    virtual Type getType() const override
    {
        return Type::ElementId;
    }

    virtual ElementId asElementId() const override { return _value; }
    ElementId getValue() const { return _value; }
    virtual bool setValue(const ElementId& value) override
    {
        _value = value;
        return true;
    }

    virtual ParameterValueUPtr clone() const override
    {
        return std::make_unique<ElementIdParameterValue>(_value);
    }

    virtual bool equals(const ParameterValue& other) const override
    {
        return other.getType() == Type::ElementId && _value == other.asElementId();
    }

private:
    ElementId _value;
};

class Vector2ParameterValue : public ParameterValue
{
public:
    explicit Vector2ParameterValue(const wy::Vector2& value) : _value(value) {}

    virtual Type getType() const override
    {
        return Type::Vector2;
    }

    virtual wy::Vector2 asVector2() const override { return _value; }
    const wy::Vector2& getValue() const { return _value; }
    virtual bool setValue(const wy::Vector2& value) override
    {
        _value = value;
        return true;
    }

    virtual ParameterValueUPtr clone() const override
    {
        return std::make_unique<Vector2ParameterValue>(_value);
    }

    virtual bool equals(const ParameterValue& other) const override
    {
        return other.getType() == Type::Vector2 && _value == other.asVector2();
    }

private:
    wy::Vector2 _value;
};

class Vector3ParameterValue : public ParameterValue
{
public:
    explicit Vector3ParameterValue(const wy::Vector3& value) : _value(value) {}

    virtual Type getType() const override
    {
        return Type::Vector3;
    }

    virtual wy::Vector3 asVector3() const override { return _value; }
    const wy::Vector3& getValue() const { return _value; }
    virtual bool setValue(const wy::Vector3& value) override
    {
        _value = value;
        return true;
    }

    virtual ParameterValueUPtr clone() const override
    {
        return std::make_unique<Vector3ParameterValue>(_value);
    }

    virtual bool equals(const ParameterValue& other) const override
    {
        return other.getType() == Type::Vector3 && _value == other.asVector3();
    }

private:
    wy::Vector3 _value;
};

class AnyParameterValue : public ParameterValue
{
private:
    template<typename T, typename = void>
    struct IsEqualityComparable : std::false_type
    {
    };

    template<typename T>
    struct IsEqualityComparable<
        T,
        decltype(void(std::declval<const T&>() == std::declval<const T&>()))>
        : std::true_type
    {
    };

    template<typename T>
    struct IsBlockedType
        : std::integral_constant<
        bool,
        std::is_same<typename std::decay<T>::type, AnyParameterValue>::value ||
        std::is_base_of<ParameterValue, typename std::decay<T>::type>::value>
    {
    };

public:
    AnyParameterValue() {}

    template<
        typename T,
        typename std::enable_if<!IsBlockedType<T>::value, int>::type = 0>
        explicit AnyParameterValue(const T& value)
    {
        this->set(value);
    }

    template<
        typename T,
        typename std::enable_if<!IsBlockedType<T>::value, int>::type = 0>
        explicit AnyParameterValue(T&& value)
    {
        this->set(std::forward<T>(value));
    }

    AnyParameterValue(const AnyParameterValue& rhs)
        : _data(rhs._data ? rhs._data->clone() : std::unique_ptr<Data>())
    {
    }

    AnyParameterValue& operator=(const AnyParameterValue& rhs)
    {
        if (this != &rhs)
        {
            _data = rhs._data ? rhs._data->clone() : std::unique_ptr<Data>();
        }
        return *this;
    }

    AnyParameterValue(AnyParameterValue&& rhs) noexcept = default;
    AnyParameterValue& operator=(AnyParameterValue&& rhs) noexcept = default;

    virtual Type getType() const override
    {
        return Type::Any;
    }

    virtual ParameterValueUPtr clone() const override
    {
        return std::make_unique<AnyParameterValue>(*this);
    }

    virtual bool equals(const ParameterValue& other) const override
    {
        const AnyParameterValue* otherAny =
            dynamic_cast<const AnyParameterValue*>(&other);
        if (otherAny == nullptr)
        {
            return false;
        }

        if (_data == otherAny->_data)
        {
            return true;
        }

        if (!_data || !otherAny->_data)
        {
            return false;
        }

        return _data->equals(*otherAny->_data);
    }

    bool isNull() const
    {
        return !_data;
    }

    void clear()
    {
        _data.reset();
    }

    const std::type_info& getValueTypeInfo() const
    {
        return _data ? _data->type() : typeid(void);
    }

    template<typename T>
    bool is() const
    {
        return this->getValueTypeInfo() == typeid(T);
    }

    template<typename T>
    const T* tryGet() const
    {
        typedef typename std::decay<T>::type TValue;

        if (!_data || _data->type() != typeid(TValue))
        {
            return nullptr;
        }

        const TypedData<TValue>* p =
            static_cast<const TypedData<TValue>*>(_data.get());
        return &(p->value);
    }

    template<typename T>
    T* tryGet()
    {
        typedef typename std::decay<T>::type TValue;

        if (!_data || _data->type() != typeid(TValue))
        {
            return nullptr;
        }

        TypedData<TValue>* p =
            static_cast<TypedData<TValue>*>(_data.get());
        return &(p->value);
    }

    template<
        typename T,
        typename std::enable_if<!IsBlockedType<T>::value, int>::type = 0>
        void set(const T& value)
    {
        typedef typename std::decay<T>::type TValue;

        static_assert(
            IsEqualityComparable<TValue>::value,
            "AnyParameterValue requires stored type to support operator==.");

        _data.reset(new TypedData<TValue>(value));
    }

    template<
        typename T,
        typename std::enable_if<!IsBlockedType<T>::value, int>::type = 0>
        void set(T&& value)
    {
        typedef typename std::decay<T>::type TValue;

        static_assert(
            IsEqualityComparable<TValue>::value,
            "AnyParameterValue requires stored type to support operator==.");

        _data.reset(new TypedData<TValue>(std::forward<T>(value)));
    }

private:
    class Data
    {
    public:
        virtual ~Data() {}
        virtual std::unique_ptr<Data> clone() const = 0;
        virtual bool equals(const Data& rhs) const = 0;
        virtual const std::type_info& type() const = 0;
    };

    template<typename T>
    class TypedData : public Data
    {
    public:
        explicit TypedData(const T& v) : value(v) {}
        explicit TypedData(T&& v) : value(std::move(v)) {}

        virtual std::unique_ptr<Data> clone() const override
        {
            return std::unique_ptr<Data>(new TypedData<T>(value));
        }

        virtual bool equals(const Data& rhs) const override
        {
            const TypedData<T>* p = dynamic_cast<const TypedData<T>*>(&rhs);
            return p && (value == p->value);
        }

        virtual const std::type_info& type() const override
        {
            return typeid(T);
        }

        T value;
    };

    std::unique_ptr<Data> _data;
};

inline ParameterValueUPtr ParameterValue::createInteger(int value)
{
    return std::make_unique<IntegerParameterValue>(value);
}

inline ParameterValueUPtr ParameterValue::createDouble(double value)
{
    return std::make_unique<DoubleParameterValue>(value);
}

inline ParameterValueUPtr ParameterValue::createBoolean(bool value)
{
    return std::make_unique<BooleanParameterValue>(value);
}

inline ParameterValueUPtr ParameterValue::createString(const std::string& value)
{
    return std::make_unique<StringParameterValue>(value);
}

inline ParameterValueUPtr ParameterValue::createElementId(const ElementId& value)
{
    return std::make_unique<ElementIdParameterValue>(value);
}

inline ParameterValueUPtr ParameterValue::createVector2(const wy::Vector2& value)
{
    return std::make_unique<Vector2ParameterValue>(value);
}

inline ParameterValueUPtr ParameterValue::createVector3(const wy::Vector3& value)
{
    return std::make_unique<Vector3ParameterValue>(value);
}

template<typename T>
inline ParameterValueUPtr ParameterValue::createAny(T&& value)
{
    return std::make_unique<AnyParameterValue>(std::forward<T>(value));
}

class ParameterSchemaExtensionImpl;

struct ParameterDefinitionData
{
    ParameterDefinitionData() : name(), description(), isVisible(true), isReadonly(false) {}

    std::string name;
    std::string description;
    bool isVisible;
    bool isReadonly;
};

class WYDB_EXPORT ParameterSchemaExtension : public wyrx::ClassExtension
{
    WYRX_DECLARE_MEMBERS(ParameterSchemaExtension, wydb::ParameterSchemaExtension, wyrx::ClassExtension)
public:
    ParameterSchemaExtension();
    ~ParameterSchemaExtension();
    ParameterSchemaExtension(const ParameterSchemaExtension&) = delete;
    ParameterSchemaExtension& operator=(const ParameterSchemaExtension&) = delete;
    ParameterSchemaExtension(ParameterSchemaExtension&&) = delete;
    ParameterSchemaExtension& operator=(ParameterSchemaExtension&&) = delete;

    // Get all parameter definitions stored in this schema.
    std::vector<const ParameterDefinition*> getParameterDefinitions() const;

    // Add a parameter definition to this schema.
    wy::ErrorStatus addParameterDefinition(const ParameterDefinitionData& data);

private:
    ParameterSchemaExtensionImpl* _pImpl;
};

NS_WYDB_END

#endif // WYDB_PARAMETER_H