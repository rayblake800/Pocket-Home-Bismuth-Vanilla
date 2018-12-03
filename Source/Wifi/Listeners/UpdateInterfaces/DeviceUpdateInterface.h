#pragma once

namespace Wifi
{
    class DeviceUpdateInterface
    {
    public:
        DeviceUpdateInterface() { }

        virtual ~DeviceUpdateInterface() { }

    private:
        virtual void wirelessEnabled() = 0;

        virtual void wirelessDisabled() = 0;
    };
}
