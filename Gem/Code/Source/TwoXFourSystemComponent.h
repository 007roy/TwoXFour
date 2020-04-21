#pragma once

#include <AzCore/Component/Component.h>

#include <TwoXFour/TwoXFourBus.h>

namespace TwoXFour
{
    class TwoXFourSystemComponent
        : public AZ::Component
        , protected TwoXFourRequestBus::Handler
    {
    public:
        AZ_COMPONENT(TwoXFourSystemComponent, "{599565C3-8749-4D0D-B6DE-890385DA00CA}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

    protected:
        ////////////////////////////////////////////////////////////////////////
        // TwoXFourRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
