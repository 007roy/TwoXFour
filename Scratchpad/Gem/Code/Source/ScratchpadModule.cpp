
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include <ScratchpadSystemComponent.h>

namespace Scratchpad
{
    class ScratchpadModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(ScratchpadModule, "{489340FE-E749-4B44-81D8-EF353A0639F2}", AZ::Module);
        AZ_CLASS_ALLOCATOR(ScratchpadModule, AZ::SystemAllocator, 0);

        ScratchpadModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                ScratchpadSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<ScratchpadSystemComponent>(),
            };
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(Scratchpad_1a3b17fc63a642509e233d36f5e263ba, Scratchpad::ScratchpadModule)
