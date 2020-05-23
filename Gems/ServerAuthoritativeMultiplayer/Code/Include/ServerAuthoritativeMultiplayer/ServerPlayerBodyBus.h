#pragma once
#include <AzCore/Component/Component.h>
#include <GridMate/Session/Session.h>

namespace ServerAuthoritativeMultiplayer
{
    class ServerPlayerBodyInterface
        : public AZ::ComponentBus
    {
    public:
        virtual void SetAssociatedPlayerId(GridMate::MemberIDCompact player) = 0;
    };

    using ServerPlayerBodyBus = AZ::EBus<ServerPlayerBodyInterface>;
}
