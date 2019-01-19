#pragma once
/**
 * @file  Icon_RequestID.h
 *
 * @brief  Defines a type used to identify pending icon requests.
 */

namespace Icon
{
    /**
     * @brief  Identifies pending icon requests so that they can be cancelled.
     */
    typedef unsigned int RequestID;

    /**
     * @brief  Checks if a RequestID value represents an invalid request ID.
     *
     * @param request  The value to check.
     *
     * @return         Whether the request value does not represent a valid
     *                 request.
     */
    static inline bool isNullRequest(const RequestID request)
    {
        return request == 0;
    }
}
