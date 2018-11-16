#pragma once
#include "DesktopEntry/DesktopEntry.h"
#include "DesktopEntry/UpdateInterface.h"
#include "SharedResource/Handler.h"

/**
 * @file DesktopEntry/UpdateListener.h"
 *
 * @brief  Receives notifications when desktop entry data is updated.
 *
 * Whenever the desktop entry loader updates its cached desktop entry data, it
 * notifies all UpdateListeners of all added, changed, or removed entries.
 * UpdateListener classes must implement DesktopEntry::UpdateInterface's 
 * methods to handle these notifications.
 *
 * @see DesktopEntry/UpdateInterface.h
 */
class DesktopEntry::UpdateListener : public UpdateInterface,
    public SharedResource::Handler<LoadingThread>
{
protected:
    UpdateListener();

public:
    virtual ~UpdateListener() { }
};
