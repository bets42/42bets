#include <bets42/arthur/memory.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <string>

using namespace bets42::arthur;

class memory_test_fixture : public testing::Test {};

TEST_F(memory_test_fixture, make_unique)
{
	const std::string str("let's go out for some beer larryness");
	const std::unique_ptr<std::string> str_ptr(bets42::arthur::make_unique<std::string>(str));
	EXPECT_EQ(*str_ptr, str);
}
