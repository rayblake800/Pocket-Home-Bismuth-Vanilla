#pragma once
#include <gio/gio.h>

/**
 * @file  GLib/SmartPointers/SharedContextPtr.h
 *
 * @brief  A managed GMainContext* that can be copied or set to new values, 
 *         automatically referencing or unreferencing its GMainContext as 
 *         necessary.
 */
namespace GLib
{
    class SharedContextPtr
    {
    public:
        /**
         * @brief 
         */
        SharedContextPtr();

        /**
         * @brief 
         *
         * @param context
         */
        SharedContextPtr(GMainContext* context);

        /**
         * @brief 
         *
         * @param context
         */
        SharedContextPtr(const SharedContextPtr& context);

        /**
         * @brief 
         */
        virtual ~SharedContextPtr();

        /**
         * @brief 
         *
         * @param rhs
         *
         * @return 
         */
        bool operator==(const GMainContext* rhs) const;

        /**
         * @brief 
         *
         * @param rhs
         *
         * @return 
         */
        bool operator==(const SharedContextPtr& rhs) const;

        /**
         * @brief 
         *
         * @param rhs
         *
         * @return 
         */
        bool operator!=(const GMainContext* rhs) const;

        /**
         * @brief 
         *
         * @param rhs
         *
         * @return 
         */
        bool operator!=(const SharedContextPtr& rhs) const;

        /**
         * @brief 
         *
         * @param rhs
         *
         * @return 
         */
        SharedContextPtr& operator=(const GMainContext* rhs);

        /**
         * @brief 
         *
         * @param rhs
         *
         * @return 
         */
        SharedContextPtr& operator=(const SharedContextPtr& rhs);

        /**
         * @brief 
         *
         * @return 
         */
        operator GMainContext*();

    private:
        /* The internal GLib context pointer. */
        GMainContext* context = nullptr;
    };
}
