#pragma once
#include <AzCore/Component/Component.h>
#include <GridMate/Session/Session.h>
#include <AzFramework/Entity/EntityContextBus.h>

namespace ServerAuthoritativeMultiplayer
{
    class ServerPlayerSpawnerComponent
        : public AZ::Component
        , public GridMate::SessionEventBus::Handler
        , AzFramework::SliceInstantiationResultBus::MultiHandler
    {
    public:
        AZ_COMPONENT(ServerPlayerSpawnerComponent, "{478f2a95-565b-4e2d-9500-f546991f6606}")

        static void Reflect(AZ::ReflectContext* reflection);

        void Activate() override;
        void Deactivate() override;
    protected:
        // GridMate::SessionEventBus interface
        void OnMemberJoined(GridMate::GridSession* session, GridMate::GridMember* member) override;

        // SiceInstantiationResultBus
        void OnSliceInstantiated(const AZ::Data::AssetId&, const AZ::SliceComponent::SliceInstanceAddress&) override;

    private:
        AZStd::unordered_map<AzFramework::SliceInstantiationTicket, GridMate::MemberIDCompact> m_joiningPlayers;
        AZ::u8 m_playerCount = 0;

        void SpawnPlayer(GridMate::MemberIDCompact playerId);
    };
}