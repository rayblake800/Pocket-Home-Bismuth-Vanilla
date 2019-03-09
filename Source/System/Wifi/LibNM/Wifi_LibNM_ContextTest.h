#pragma once
/**
 * @file  LibNM/ContextTest
 *
 * @brief  A debugging macro used to find inappropriate LibNM data 
 *         access outside of the global default main context. In release builds,
 *         these checks will be removed.
 * 
 * LibNM::Object and LibNM::Settings classes should call this at the beginning 
 * of most methods.
 */
#ifdef JUCE_DEBUG
  #define ASSERT_NM_CONTEXT                                               \
      if(!g_main_context_is_owner(g_main_context_default()))              \
      {                                                                   \
          DBG("LibNM: Accessed LibNM data outside of the LibNM thread!"); \
          jassertfalse;                                                   \
    }
#else
  #define ASSERT_NM_CONTEXT
#endif
