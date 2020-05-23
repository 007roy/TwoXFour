
#include <ServerAuthoritativeMultiplayerSystemComponent.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <INetwork.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace ServerAuthoritativeMultiplayer
{
    void ServerAuthoritativeMultiplayerSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<ServerAuthoritativeMultiplayerSystemComponent, AZ::Component>()
                ->Version(0)
                ;

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<ServerAuthoritativeMultiplayerSystemComponent>("Server Authoritative Multiplayer", "[Description of functionality provided by this System Component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }

        if(auto* bc = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            bc->EBus<ServerAuthoritativeMultiplayerRequestBus>("Server Authoritative Multiplayer Request Bus")
            ->Attribute(AZ::Script::Attributes::Category,"Server Authoritative Multiplayer")
            ->Event("Get Local ID", &ServerAuthoritativeMultiplayerRequestBus::Events::GetLocal);
        }
    }

    GridMate::MemberIDCompact ServerAuthoritativeMultiplayerSystemComponent::GetLocal()
    {
        return m_selfId;
    }

    void ServerAuthoritativeMultiplayerSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("ServerAuthoritativeMultiplayerService"));
    }

    void ServerAuthoritativeMultiplayerSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("ServerAuthoritativeMultiplayerService"));
    }

    void ServerAuthoritativeMultiplayerSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        AZ_UNUSED(required);
    }

    void ServerAuthoritativeMultiplayerSystemComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        AZ_UNUSED(dependent);
    }

    void ServerAuthoritativeMultiplayerSystemComponent::Init()
    {
    }

    void ServerAuthoritativeMultiplayerSystemComponent::Activate()
    {
        ServerAuthoritativeMultiplayerRequestBus::Handler::BusConnect();
        CrySystemEventBus::Handler::BusConnect();
    }

    void ServerAuthoritativeMultiplayerSystemComponent::Deactivate()
    {
        ServerAuthoritativeMultiplayerRequestBus::Handler::BusDisconnect();
        if (GridMate::SessionEventBus::Handler::BusIsConnected())
        {
            GridMate::SessionEventBus::Handler::BusDisconnect();
        }
    }

    void ServerAuthoritativeMultiplayerSystemComponent::OnCrySystemInitialized(ISystem& system, const SSystemInitParams& param)
    {
        GridMate::SessionEventBus::Handler::BusConnect(system.GetINetwork()->GetGridMate());
        CrySystemEventBus::Handler::BusDisconnect();
    }

    void ServerAuthoritativeMultiplayerSystemComponent::OnSessionJoined(GridMate::GridSession* session)
    {
        m_selfId = session->GetMyMember()->GetIdCompact();
    }

} // namespace ServerAuthoritativeMultiplayer
