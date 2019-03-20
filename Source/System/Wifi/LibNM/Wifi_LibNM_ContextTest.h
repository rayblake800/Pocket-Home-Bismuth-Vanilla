#pragma once
/**
 * @file  Wifi_LibNM_ContextTest.
 *
 * @brief  A debugging macro used to find inappropriate LibNM data access 
 *         outside of the global default main context.
 */

#ifdef JUCE_DEBUG

/**
 * @brief  Asserts that this check is running within the LibNM thread event
 *         loop.
 *
 *  LibNM::Object classes controlled by the LibNM library should call this at 
 * the beginning of most methods. These assertions will only run within debug
 * builds.
 */
#define ASSERT_NM_CONTEXT                                           \
if(!g_main_context_is_owner(g_main_context_default()))              \
{                                                                   \
    DBG("LibNM: Accessed LibNM data outside of the LibNM thread!"); \
    jassertfalse;                                                   \
}

#else
  #define ASSERT_NM_CONTEXT
#endif
