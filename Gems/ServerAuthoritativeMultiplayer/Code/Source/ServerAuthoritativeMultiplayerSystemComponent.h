
#pragma once

#include <AzCore/Component/Component.h>

#include <ServerAuthoritativeMultiplayer/ServerAuthoritativeMultiplayerBus.h>
#include <GridMate/Session/Session.h>
#include <CrySystemBus.h>

namespace ServerAuthoritativeMultiplayer
{
    class ServerAuthoritativeMultiplayerSystemComponent
        : public AZ::Component
        , protected ServerAuthoritativeMultiplayerRequestBus::Handler
        , public GridMate::SessionEventBus::Handler
        , public CrySystemEventBus::Handler
    {
    public:
        AZ_COMPONENT(ServerAuthoritativeMultiplayerSystemComponent, "{525988A8-A5E2-4165-87B3-9C06A93F511C}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

    protected:
        ////////////////////////////////////////////////////////////////////////
        // ServerAuthoritativeMultiplayerRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////
        GridMate::MemberIDCompact GetLocal() override;
        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////

        //CrySystemEventBus
        void OnCrySystemInitialized(ISystem& system, const SSystemInitParams& param) override;

        //GridMate::SessionEVentBus
        void OnSessionJoined(GridMate::GridSession* session) override;

    private:
        GridMate::MemberIDCompact m_selfId = 0;
    };

} // namespace ServerAuthoritativeMultiplayer
