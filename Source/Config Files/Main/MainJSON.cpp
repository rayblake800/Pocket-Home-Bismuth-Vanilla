#include "MainJSON.h"
#include "MainConfigKeys.h"
    
/*
 * Get the set of all basic (non-array, non-object) properties tracked by
 * this ConfigJSON.
 */
const std::vector<ConfigKey>& MainJSON::getConfigKeys() const
{
    return MainConfigKeys::allKeys();
}

