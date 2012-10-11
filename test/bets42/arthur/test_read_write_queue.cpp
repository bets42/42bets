#include <bets42/arthur/read_write_queue.hpp>
#include <gtest/gtest.h>

using namespace bets42::arthur;

class read_write_queue_test_fixture : public testing::Test {};

TEST_F(read_write_queue_test_fixture, ctor)
{
	EXPECT_NO_THROW(read_write_queue<char>());
}

TEST_F(read_write_queue_test_fixture, push_pop)
{
    read_write_queue<unsigned> queue;

    // push on elements
    queue.push(13);
    queue.push(2);
    queue.push(1986);

    // pop elements
    std::pair<bool, unsigned> result;

    EXPECT_FALSE(result.first);

    result = queue.pop();
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second, 13);

    result = queue.pop();
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second, 2);

    result = queue.pop();
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second, 1986);

    result = queue.pop();
    EXPECT_FALSE(result.first);

    // push elements
    queue.push(10);
    queue.push(10);

    // pop elements
    result = queue.pop();
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second, 10);

    result = queue.pop();
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second, 10);

    result = queue.pop();
    EXPECT_FALSE(result.first);
    EXPECT_FALSE(result.first);
}

TEST_F(read_write_queue_test_fixture, empty)
{
    read_write_queue<unsigned> queue;

    EXPECT_TRUE(queue.empty());
    queue.push(2);
    EXPECT_FALSE(queue.empty());
    queue.push(1986);
    EXPECT_FALSE(queue.empty());
    queue.pop();
    EXPECT_FALSE(queue.empty());
    queue.pop();
    EXPECT_TRUE(queue.empty());
    queue.pop();
    EXPECT_TRUE(queue.empty());
    queue.push(1986);
    EXPECT_FALSE(queue.empty());
}

