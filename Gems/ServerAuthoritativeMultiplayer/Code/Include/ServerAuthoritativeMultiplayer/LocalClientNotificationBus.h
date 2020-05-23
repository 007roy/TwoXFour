#pragma once
#include <AzCore/EBus/EBus.h>
#include <GridMate/Session/Session.h>
#include <AZCore/Component/EntityId.h>

namespace ServerAuthoritativeMultiplayer
{
    class LocalClientNotification
        : public AZ::EBusTraits
    {
    public:
        static const AZ::EBusHandlerPolicy HandlerPoicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

        virtual void AttachToBody(GridMate::MemberIDCompact playerId, const AZ::EntityId& body) = 0;
    };

    using LocalClientNotificationBus = AZ::EBus<LocalClientNotification>;
}
