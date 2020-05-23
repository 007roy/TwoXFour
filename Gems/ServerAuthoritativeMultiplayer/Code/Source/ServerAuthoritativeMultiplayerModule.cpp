
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include <ServerAuthoritativeMultiplayerSystemComponent.h>
#include <PlayerActionRelayComponent.h>
#include <ServerAuthPlayerComponent.h>
#include <ServerPlayerSpawnerComponent.h>

namespace ServerAuthoritativeMultiplayer
{
    class ServerAuthoritativeMultiplayerModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(ServerAuthoritativeMultiplayerModule, "{625FAEAC-63F2-4F4C-82D3-E8A84FD59E05}", AZ::Module);
        AZ_CLASS_ALLOCATOR(ServerAuthoritativeMultiplayerModule, AZ::SystemAllocator, 0);

        ServerAuthoritativeMultiplayerModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                ServerAuthoritativeMultiplayerSystemComponent::CreateDescriptor(),
                PlayerActionRelayComponent::CreateDescriptor(),
                ServerAuthPlayerComponent::CreateDescriptor(),
                ServerPlayerSpawnerComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<ServerAuthoritativeMultiplayerSystemComponent>(),
            };
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(ServerAuthoritativeMultiplayer_4cb27a4eb78649c7b21e829b0792a3dd, ServerAuthoritativeMultiplayer::ServerAuthoritativeMultiplayerModule)
