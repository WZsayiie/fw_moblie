//Don't change this file manually, it was generated by automation tool.

#pragma once

#include <memory>
#include <string>
#include <vector>

enum class CQJSONType {
    Null,
    Bool,
    Number,
    String,
    Array,
    Object,
};

struct CQJSONValue {
    
    static std::shared_ptr<CQJSONValue> fromString(const std::string &string);
    static std::shared_ptr<CQJSONValue> fromFile(const std::string &path);
    static std::shared_ptr<CQJSONValue> value();

    virtual std::string toString() = 0;
    virtual bool toFile(const std::string &path) = 0;
    
    virtual CQJSONType type() = 0;
    
    virtual bool boolValue() = 0;
    virtual int intValue() = 0;
    virtual int64_t int64Value() = 0;
    virtual float floatValue() = 0;
    virtual double doubleValue() = 0;
    virtual std::string stringValue() = 0;

    virtual int arraySize() = 0;
    virtual void addItem(std::shared_ptr<CQJSONValue> item) = 0;
    virtual void insertItem(std::shared_ptr<CQJSONValue> item, int index) = 0;
    virtual void removeItemAt(int index) = 0;
    virtual std::shared_ptr<CQJSONValue> itemAt(int index) = 0;
    
    virtual std::vector<std::string> objectNames() = 0;
    virtual void setObject(std::shared_ptr<CQJSONValue> object, const std::string &name) = 0;
    virtual void removeObjectForName(const std::string &name) = 0;
    virtual std::shared_ptr<CQJSONValue> objectForName(const std::string &name) = 0;
};
