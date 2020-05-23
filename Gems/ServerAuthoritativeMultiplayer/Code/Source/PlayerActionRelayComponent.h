#pragma once
#include <AzCore/Component/Component.h>
#include <ServerAuthoritativeMultiplayer/PlayerControlsRequestBus.h>
#include <AZFramework/Network/NetBindable.h>
#include <ServerAuthoritativeMultiplayer/LocalPlayerBodyNotificationBus.h>


namespace ServerAuthoritativeMultiplayer
{
    class PlayerActionRelayComponent
        : public AZ::Component
        , public PlayerControlsRequestBus::Handler
        , public AzFramework::NetBindable
        , public LocalPlayerBodyNotificationBus::Handler
    {
    public:
        AZ_COMPONENT(PlayerActionRelayComponent, "{a7cb9754-69fb-4ea6-ae2b-f45b3b229975}", NetBindable)

        static void Reflect(AZ::ReflectContext* reflection);

        void Activate() override;
        void Deactivate() override;

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& prov)
        {
            prov.push_back(AZ_CRC("PlayerActionRelayService"));
        }

        // LocalPlayerBodyNotificationBus implementation
        void OnClientInputConnected() override;

        // NetBindable overrides
        GridMate::ReplicaChunkPtr GetNetworkBinding() override;
        void SetNetworkBinding(GridMate::ReplicaChunkPtr chunk) override;
        void UnbindFromNetwork() override;

    
        // PlayerControlsRequestBus
        void MoveForwardBackward(float amount) override;
        void StrafeLeftRight(float amount) override;
        void Yaw(float amount) override;
        void Pitch(float amount) override;
        void Jump(bool state) override;

    private:
        bool MoveForwardBackwardRpc(float amount, const GridMate::RpcContext& rc);
        bool StrafeLeftRightRpc(float amount, const GridMate::RpcContext& rc);
        bool YawRpc(float amount, const GridMate::RpcContext& rc);
        bool PitchRpc(float amount, const GridMate::RpcContext& rc);
        bool JumpRpc(bool state, const GridMate::RpcContext& rc);

        class Chunk;
        GridMate::ReplicaChunkPtr m_chunk;

    };
}