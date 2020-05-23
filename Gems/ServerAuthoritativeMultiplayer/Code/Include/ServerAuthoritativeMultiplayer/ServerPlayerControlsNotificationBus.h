#pragma once
#include "PlayerControlsRequestBus.h"

namespace ServerAuthoritativeMultiplayer
{
    class ServerPlayerControlsNotification
        : public PlayerControlsRequest
    {
    public:
        // EBusTraits overrides
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Multiple;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::EntityId;
    };

    using ServerPlayerControlsNotificationBus = AZ::EBus<ServerPlayerControlsNotification>;
}
