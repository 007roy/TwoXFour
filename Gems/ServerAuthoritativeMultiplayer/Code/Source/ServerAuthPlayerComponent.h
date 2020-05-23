#pragma once
#include <AzCore/Component/Component.h>
#include <AzFramework/Network/NetBindable.h>
#include <ServerAuthoritativeMultiplayer/ServerPlayerBodyBus.h>

namespace ServerAuthoritativeMultiplayer
{
    class ServerAuthPlayerComponent
        : public AZ::Component
        , public AzFramework::NetBindable
        , public ServerPlayerBodyBus::Handler
    {
    public:
        AZ_COMPONENT(ServerAuthPlayerComponent, "{307c08b3-e25a-43b9-a27a-06d17fef7927}", NetBindable);

        static void Reflect(AZ::ReflectContext* reflection);

        void Activate() override;
        void Deactivate() override;

        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
        {
            required.push_back(AZ_CRC("PlayerActionRelayService"));
        }
        
        GridMate::ReplicaChunkPtr GetNetworkBinding() override;
        void SetNetworkBinding(GridMate::ReplicaChunkPtr chunk) override;
        void UnbindFromNetwork() override;

    protected:
        // ServerPlayerBodyBus
        void SetAssociatedPlayerId(GridMate::MemberIDCompact player) override;

    private:
        class Chunk;
        GridMate::ReplicaChunkPtr m_chunk;

        void BroadcastNewBody();

#if defined(DEDICATED_SERVER)
        constexpr bool IsDedicated() const { return true; }
#else
        constexpr bool IsDedicated() const { return false; }
#endif
    };
}   