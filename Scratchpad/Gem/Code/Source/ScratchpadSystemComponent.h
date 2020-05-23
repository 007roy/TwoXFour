
#pragma once

#include <AzCore/Component/Component.h>

#include <Scratchpad/ScratchpadBus.h>

namespace Scratchpad
{
    class ScratchpadSystemComponent
        : public AZ::Component
        , protected ScratchpadRequestBus::Handler
    {
    public:
        AZ_COMPONENT(ScratchpadSystemComponent, "{E863CA71-5432-4A3D-8B40-3E2B951AD7F0}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

    protected:
        ////////////////////////////////////////////////////////////////////////
        // ScratchpadRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };

} // namespace Scratchpad
