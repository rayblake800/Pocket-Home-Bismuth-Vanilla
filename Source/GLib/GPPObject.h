#pragma once
#include <glib.h>

class GPPObject
{
protected:
    GPPObject();
    
    GPPObject(GPPObject& toCopy);
    
    GPPObject(gpointer toAssign);
    
public:
    ~GPPObject();
    
    bool isVoid();
    
protected:
    bool operator==(const GPPObject& rhs) const;
    
    bool operator==(gpointer rhs);
    
    void operator=(GPPObject& toCopy);
    
    void operator=(gpointer toAssign);
    
    virtual GType getType();
    
    <typename T> T* getProperty(const char* property)
    {
        gpointer object = getGObject();
        if(object != nullptr)
        {
            T* pValue = nullptr;
            g_object_get(object, property, &pValue);
            g_object_unref(object);
            return pValue;
        }
        return nullptr;
    }
    
    <typename T> void setProperty(const char* property, T value)
    {
        gpointer object = getGObject();
        if(object != nullptr)
        {
            g_object_set(object, property, value, nullptr);
            g_object_unref(object);
        }
    }
    
    void freezeNotify();
    
    void thawNotify();
    
    gpointer getGObject();
    
    void removeData();
    
    GWeakRef* objectRef = nullptr;
    gpointer objectData = nullptr;
};
