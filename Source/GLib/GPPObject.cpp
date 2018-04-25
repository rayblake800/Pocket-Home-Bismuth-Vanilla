#include "GPPObject.h"

GPPObject();

GPPObject(GPPObject& toCopy);

GPPObject(gpointer toAssign);

~GPPObject();

bool isValid();
bool operator==(const GPPObject& rhs) const;

bool operator==(gpointer rhs);

void operator=(GPPObject& toCopy);

void operator=(gobject toAssign);

GType getType();


void freezeNotify();

void thawNotify();

gpointer getGObject();

void removeData();