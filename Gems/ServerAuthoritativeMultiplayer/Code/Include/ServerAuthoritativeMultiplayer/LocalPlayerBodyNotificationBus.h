#pragma once
#include <AzCore/Component/Component.h>

namespace ServerAuthoritativeMultiplayer
{
    class LocalPlayerBodyNotification
        : public AZ::ComponentBus
    {
    public:
        virtual void OnClientInputConnected() = 0;
    };

    using LocalPlayerBodyNotificationBus = AZ::EBus<LocalPlayerBodyNotification>;
}
