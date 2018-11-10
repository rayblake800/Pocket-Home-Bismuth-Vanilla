#pragma once
#include "DesktopEntry/DesktopEntry.h"
#include "DesktopEntry/UpdateInterface.h"
#include "ResourceHandler.h"

/**
 * @file DesktopEntry/UpdateListener.h"
 *
 * @brief  Receives notifications when desktop entry data is updated.
 */
class DesktopEntry::UpdateListener : public UpdateInterface,
    public ResourceHandler<LoadingThread>
{
protected:
    UpdateListener();

public:
    virtual ~UpdateListener() { }
}
