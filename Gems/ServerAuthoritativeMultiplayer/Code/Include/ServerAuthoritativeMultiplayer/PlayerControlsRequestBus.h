#pragma once
#include <AzCore/EBus/EBus.h>

namespace ServerAuthoritativeMultiplayer
{
    class PlayerControlsRequest
        : public AZ::EBusTraits
    {
    public:
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

        virtual void MoveForwardBackward(float amount) {}
        virtual void StrafeLeftRight(float amount) {}
        virtual void Yaw(float amount) {}
        virtual void Pitch(float amount) {}
        virtual void Jump(bool state) {}
 
    };

    using PlayerControlsRequestBus = AZ::EBus<PlayerControlsRequest>;

}
