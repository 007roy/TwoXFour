
#pragma once

#include <AzCore/EBus/EBus.h>
#include <GridMate/Session/Session.h>

namespace ServerAuthoritativeMultiplayer
{
    class ServerAuthoritativeMultiplayerRequests
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////

        // Put your public methods here
        virtual GridMate::MemberIDCompact GetLocal() = 0;
    };

    using ServerAuthoritativeMultiplayerRequestBus = AZ::EBus<ServerAuthoritativeMultiplayerRequests>;

} // namespace ServerAuthoritativeMultiplayer
