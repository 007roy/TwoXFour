
#include <AzTest/AzTest.h>

class ServerAuthoritativeMultiplayerTest
    : public ::testing::Test
{
protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

TEST_F(ServerAuthoritativeMultiplayerTest, SanityTest)
{
    ASSERT_TRUE(true);
}

AZ_UNIT_TEST_HOOK();
