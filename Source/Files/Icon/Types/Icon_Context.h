#pragma once
/**
 * @file  Icon_Context.h
 *
 * @brief  Describes the purpose of an icon directory.
 */

namespace Icon { enum class Context; }

/**
 * Defines the purpose of an icon directory.
 */
enum class Icon::Context
{
    /* Icons used in menus and dialogs for user interaction: */
    actions,
    /* Animated images used to represent background processing: */
    animations,
    /* Icons representing individual applications: */
    applications,
    /* Icons representing application categories: */
    categories,
    /* Icons representing hardware devices: */
    devices,
    /* Icons for tags and file properties (e.g. "read-only", "photos"): */
    emblems,
    /* Icons for emoticons used in text/messenger applications: */
    emotes,
    /* Different national flags, etc.: */
    international,
    //Icons for specific data types (e.g. "audio", "image")
    mimeTypes,
    /* Icons representing file locations: */
    places,
    /* Icons for presenting statuses to the user, such as error icons: */
    status,
    /* Icon directory has a missing or unexpected context value: */
    unknown
};
