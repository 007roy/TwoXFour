#include "ServerAuthPlayerComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <GridMate/Replica/ReplicaFunctions.h>
#include <ServerAuthoritativeMultiplayer/LocalClientNotificationBus.h>
#include <AzCore/RTTI/BehaviorContext.h>

using namespace AZ;
using namespace GridMate;
using namespace ServerAuthoritativeMultiplayer;

class LocalClientNotificationBusHandler
    : public LocalClientNotificationBus::Handler
    , public BehaviorEBusHandler
{
    public:
        AZ_EBUS_BEHAVIOR_BINDER(LocalClientNotificationBusHandler, "{278be197-b4dc-42f8-9674-aeb0d10e8560}", 
        AZ::SystemAllocator, AttachToBody);
        void AttachToBody(GridMate::MemberIDCompact playerId, const AZ::EntityId& body) override
        {
            Call(FN_AttachToBody, playerId, body);
        }
};

class ServerAuthPlayerComponent::Chunk : public ReplicaChunkBase
{
public:
    GM_CLASS_ALLOCATOR(Chunk);
    Chunk() : m_owningPlayer("Owning Player") {}

    static const char* GetChunkName()
    {
        return "ServerAuthPlayerComponent::Chunk";
    }
    bool IsReplicaMigratable() override { return true; }

    DataSet<MemberIDCompact> m_owningPlayer;
};

void ServerAuthPlayerComponent::Reflect(AZ::ReflectContext* reflection)
{
    auto& descTable = ReplicaChunkDescriptorTable::Get();
    if (!descTable.FindReplicaChunkDescriptor(ReplicaChunkClassId(Chunk::GetChunkName())))
    {
        descTable.RegisterChunkType<Chunk>();
    }

    if (auto serializationContext = azrtti_cast<AZ::SerializeContext*>(reflection))
    {
        serializationContext->Class<ServerAuthPlayerComponent, Component>()
            ->Version(1);

        if (auto editContext = serializationContext->GetEditContext())
        {
            editContext->Class<ServerAuthPlayerComponent>("Server Auth Player Component", "Server Authoritative")
                ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                ->Attribute(AZ::Edit::Attributes::Category, "Server Authoritative Multiplayer")
                ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game"));
        }
    }

    if(auto* bc = azrtti_cast<AZ::BehaviorContext*>(reflection))
    {
        bc->EBus<LocalClientNotificationBus>("Local Client Notification Bus")
        ->Attribute(AZ::Script::Attributes::Category, "Server Authoritative Multiplayer")
        ->Handler<LocalClientNotificationBusHandler>();
    }
}

void ServerAuthPlayerComponent::SetAssociatedPlayerId(GridMate::MemberIDCompact player)
{
    if (Chunk* chunk = static_cast<Chunk*>(m_chunk.get()))
        chunk->m_owningPlayer.Set(player);
}

ReplicaChunkPtr ServerAuthPlayerComponent::GetNetworkBinding()
{
    m_chunk = GridMate::CreateReplicaChunk<Chunk>();
    m_chunk->SetHandler(this);
    return m_chunk;
}

void ServerAuthPlayerComponent::SetNetworkBinding(GridMate::ReplicaChunkPtr chunk)
{
    m_chunk = chunk;
    m_chunk->SetHandler(this);
}

void ServerAuthPlayerComponent::UnbindFromNetwork()
{
    if (m_chunk)
    {
        m_chunk->SetHandler(nullptr);
        m_chunk = nullptr;
    }
}

void ServerAuthPlayerComponent::Activate()
{
    if (IsDedicated())
    {
        ServerPlayerBodyBus::Handler::BusConnect(GetEntityId());
    }
    else
    {
        BroadcastNewBody();
    }
}

void ServerAuthPlayerComponent::Deactivate()
{
    if (IsDedicated())
    {
        ServerPlayerBodyBus::Handler::BusDisconnect();
    }
}

void ServerAuthPlayerComponent::BroadcastNewBody()
{
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
    {
        const MemberIDCompact playerId = c->m_owningPlayer.Get();
        AZ_Printf("SAM", "BroadcastNewBody %u", playerId);

        if (playerId != 0)
        {
            LocalClientNotificationBus::Broadcast(&LocalClientNotificationBus::Events::AttachToBody,playerId, GetEntityId());
        }
    }
}