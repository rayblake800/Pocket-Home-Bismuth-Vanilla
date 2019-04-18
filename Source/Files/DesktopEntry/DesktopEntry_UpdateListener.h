#pragma once
/**
 * @file  DesktopEntry/UpdateListener.h"
 *
 * @brief  Receives notifications when desktop entry data is updated.
 */

#include "DesktopEntry_UpdateInterface.h"
#include "SharedResource_Handler.h"

namespace DesktopEntry { class UpdateListener; }
namespace DesktopEntry { class LoadingThread; }

/**
 * @brief  Listens for updates to the DesktopEntry resource's cache of saved
 *         desktop entry data.
 *
 *  Whenever the desktop entry loader updates its cached desktop entry data, it
 * notifies all UpdateListeners of all added, changed, or removed entries.
 * UpdateListener classes must implement DesktopEntry::UpdateInterface's
 * functions to handle these notifications.
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
