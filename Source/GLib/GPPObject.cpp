#include "GPPObject.h"

GPPObject::GPPObject()
{
}

GPPObject::GPPObject(GPPObject& toCopy)
{
}

GPPObject::GPPObject(gpointer toAssign)
{
}

GPPObject::~GPPObject()
{
}

bool GPPObject::isVoid()
{
}

bool GPPObject::operator==(const GPPObject& rhs) const
{
}

bool GPPObject::operator==(gpointer rhs)
{
}

void GPPObject::operator=(GPPObject& toCopy)
{
}

void GPPObject::operator=(gpointer toAssign)
{
}

GType GPPObject::getType()
{
}

void GPPObject::freezeNotify()
{
}

void GPPObject::thawNotify()
{
}

gpointer GPPObject::getGObject()
{
}

void GPPObject::removeData()
{
}
