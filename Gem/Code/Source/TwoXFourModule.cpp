
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include <TwoXFourSystemComponent.h>

namespace TwoXFour
{
    class TwoXFourModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(TwoXFourModule, "{F95BE78B-6231-45D1-9F55-78E679E60BED}", AZ::Module);
        AZ_CLASS_ALLOCATOR(TwoXFourModule, AZ::SystemAllocator, 0);

        TwoXFourModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                TwoXFourSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<TwoXFourSystemComponent>(),
            };
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(TwoXFour_281f033c8f9a4e99a3cfd2f0d379a07e, TwoXFour::TwoXFourModule)
