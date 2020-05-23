#include "PlayerActionRelayComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <GridMate/Replica/ReplicaFunctions.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <ServerAuthoritativeMultiplayer/ServerPlayerControlsNotificationBus.h>

using namespace AZ;
using namespace GridMate;
using namespace ServerAuthoritativeMultiplayer;

class PlayerActionRelayComponent::Chunk : public ReplicaChunkBase
{
public:
    GM_CLASS_ALLOCATOR(Chunk);
    Chunk() = default;

    static const char* GetChunkName()
    {
        return "PlayerActionRelayComponent::Chunk";
    }
    bool IsReplicaMigratable() override { return true; }

    GridMate::Rpc<RpcArg<float>>::BindInterface<PlayerActionRelayComponent, &PlayerActionRelayComponent::MoveForwardBackwardRpc>
    m_forwardBackwardRpc = {"ForwardBackward"};

    GridMate::Rpc<RpcArg<float>>::BindInterface<PlayerActionRelayComponent, &PlayerActionRelayComponent::StrafeLeftRightRpc>
    m_strafeLeftRightRpc = {"StrafeLeftRight"};

    GridMate::Rpc<RpcArg<float>>::BindInterface<PlayerActionRelayComponent, &PlayerActionRelayComponent::YawRpc>
    m_yawRpc = {"Yaw"};

    GridMate::Rpc<RpcArg<float>>::BindInterface<PlayerActionRelayComponent, &PlayerActionRelayComponent::PitchRpc>
    m_pitchRpc = {"Pitch"};

    GridMate::Rpc<RpcArg<bool>>::BindInterface<PlayerActionRelayComponent, &PlayerActionRelayComponent::JumpRpc>
    m_jumpRpc = {"Jump"};

};

class ServerPlayerControlsNotificationBusHandler: public AZ::BehaviorEBusHandler
        ,public ServerPlayerControlsNotificationBus::Handler     
{
    public:
        AZ_EBUS_BEHAVIOR_BINDER(ServerPlayerControlsNotificationBusHandler, "{61cda5f3-48af-470b-9d86-1ed8df94bc66}", AZ::SystemAllocator,
         MoveForwardBackward, StrafeLeftRight, Yaw, Pitch, Jump);

        void MoveForwardBackward(float amount) override
        {
            Call(FN_MoveForwardBackward, amount);
        }

        void StrafeLeftRight(float amount) override
        {
            Call(FN_StrafeLeftRight, amount);
        }

        void Yaw(float amount) override
        {
            Call(FN_Yaw, amount);
        }

        void Pitch(float amount) override
        {
            Call(FN_Pitch, amount);
        }

        void Jump(bool state) override
        {
            Call(FN_Jump, state);
        }
};

void PlayerActionRelayComponent::Reflect(AZ::ReflectContext* reflection)
{
    auto& descTable = ReplicaChunkDescriptorTable::Get();
    if (!descTable.FindReplicaChunkDescriptor(ReplicaChunkClassId(Chunk::GetChunkName())))
    {
        descTable.RegisterChunkType<Chunk>();
    }

    if (auto serializationContext = azrtti_cast<AZ::SerializeContext*>(reflection))
    {
        serializationContext->Class<PlayerActionRelayComponent, Component>()
            ->Version(1);

        if (auto editContext = serializationContext->GetEditContext())
        {
            editContext->Class<PlayerActionRelayComponent>("Player Action Relay Component", "[Relays input controls to server]")
                ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                ->Attribute(AZ::Edit::Attributes::Category, "Server Authoritative Multiplayer")
                ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game"));
        }
    }

    if(auto* bc = azrtti_cast<AZ::BehaviorContext*>(reflection))
    {
        bc->EBus<PlayerControlsRequestBus>("Player Controls Request Bus")
            ->Attribute(AZ::Script::Attributes::Category, "Server Authoritative Multiplayer")
            ->Event("Move Forward Backward", &PlayerControlsRequestBus::Events::MoveForwardBackward)
            ->Event("Strafe Left Right", &PlayerControlsRequestBus::Events::StrafeLeftRight)
            ->Event("Yaw", &PlayerControlsRequestBus::Events::Yaw)
            ->Event("Pitch", &PlayerControlsRequestBus::Events::Pitch)
            ->Event("Jump", &PlayerControlsRequestBus::Events::Jump) ;
            

//todo change to locaplayerbodyrequestbus
        bc->EBus<LocalPlayerBodyNotificationBus>("Local Player Body Notification Bus")
            ->Attribute(AZ::Script::Attributes::Category,"Server Authoritative Multiplayer")
            ->Event("OnClientInputConnected", &LocalPlayerBodyNotificationBus::Events::OnClientInputConnected);
            
        bc->EBus<ServerPlayerControlsNotificationBus>("Server Player Controls Notification Bus")
            ->Attribute(AZ::Script::Attributes::Category, "Server Authoritative Multiplayer")
            ->Handler<ServerPlayerControlsNotificationBusHandler>();  

        
    }
}

void PlayerActionRelayComponent::Activate()
{
#if !defined(DEDICATED_SERVER)
    LocalPlayerBodyNotificationBus::Handler::BusConnect(GetEntityId());
    AZ_Printf("SAM", "Relay bus connected %llu", (AZ::u64)GetEntityId());
#endif
}

void PlayerActionRelayComponent::Deactivate()
{
#if !defined(DEDICATED_SERVER)
    LocalPlayerBodyNotificationBus::Handler::BusDisconnect();
    if (PlayerControlsRequestBus::Handler::BusIsConnected())
        PlayerControlsRequestBus::Handler::BusDisconnect();
#endif
}

void PlayerActionRelayComponent::OnClientInputConnected()
{
    PlayerControlsRequestBus::Handler::BusConnect();
    AZ_Printf("SAM", "PlayerControlsRequestBus connected  %llu", (AZ::u64)GetEntityId());
}

ReplicaChunkPtr PlayerActionRelayComponent::GetNetworkBinding()
{
    AZ_Printf("SAM","get network binding  %llu", (AZ::u64)GetEntityId());
    m_chunk = GridMate::CreateReplicaChunk<Chunk>();
    m_chunk->SetHandler(this);
    return m_chunk;
}

void PlayerActionRelayComponent::SetNetworkBinding(GridMate::ReplicaChunkPtr chunk)
{
    AZ_Printf("SAM","set network binding  %llu", (AZ::u64)GetEntityId());
    m_chunk = chunk;
    m_chunk->SetHandler(this);
}

void PlayerActionRelayComponent::UnbindFromNetwork()
{
    if (m_chunk)
    {
        m_chunk->SetHandler(nullptr);
        m_chunk = nullptr;
    }
}

void PlayerActionRelayComponent::MoveForwardBackward(float amount)
{
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
           c->m_forwardBackwardRpc(amount);
}

void PlayerActionRelayComponent::StrafeLeftRight(float amount)
{   
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
        c->m_strafeLeftRightRpc(amount);
}

void PlayerActionRelayComponent::Yaw(float amount)
{
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
        c->m_yawRpc(amount);
}

void PlayerActionRelayComponent::Pitch(float amount)
{
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
        c->m_pitchRpc(amount);
}

void PlayerActionRelayComponent::Jump(bool state)
{
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
        c->m_jumpRpc(state);
}

bool PlayerActionRelayComponent::MoveForwardBackwardRpc(float amount, const GridMate::RpcContext& rc)
{
    ServerPlayerControlsNotificationBus::Event(GetEntityId(),&ServerPlayerControlsNotificationBus::Events::MoveForwardBackward, amount);
    return true; // pass back to clients
}

bool PlayerActionRelayComponent::StrafeLeftRightRpc(float amount, const GridMate::RpcContext& rc)
{
    ServerPlayerControlsNotificationBus::Event(GetEntityId(),&ServerPlayerControlsNotificationBus::Events::StrafeLeftRight, amount);
    return true; // pass back to clients
}

bool PlayerActionRelayComponent::YawRpc(float amount, const GridMate::RpcContext& rc)
{
    ServerPlayerControlsNotificationBus::Event(GetEntityId(),&ServerPlayerControlsNotificationBus::Events::Yaw, amount);
    return true; // pass back to clients
}

bool PlayerActionRelayComponent::PitchRpc(float amount, const GridMate::RpcContext& rc)
{
    ServerPlayerControlsNotificationBus::Event(GetEntityId(),&ServerPlayerControlsNotificationBus::Events::Pitch, amount);
    return true; // pass back to clients
}

bool PlayerActionRelayComponent::JumpRpc(bool state, const GridMate::RpcContext& rc)
{
    ServerPlayerControlsNotificationBus::Event(GetEntityId(),&ServerPlayerControlsNotificationBus::Events::Jump, state);
    return true; // pass back to clients
}
