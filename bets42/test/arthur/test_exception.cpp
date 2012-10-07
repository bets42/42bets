#include <bets42/arthur/exception.hpp>
#include <gtest/gtest.h>
#include <string>

using namespace bets42::arthur;

class exception_test_fixture : public testing::Test
{
	public:
		exception_test_fixture()
			: file_("exception_test_fixture.cpp")
			, line_(100) {}

		const std::string& file() const
		{
			return file_;
		}
		long line() const
		{
			return line_;
		}

	private:
		const std::string   file_;
		const long          line_;
};

TEST_F(exception_test_fixture, ctor_with_file_and_line)
{
	EXPECT_NO_THROW(exception(__FILE__, __LINE__));
	EXPECT_NO_THROW(exception(file(), line()));
}

TEST_F(exception_test_fixture, ctor_copy)
{
	const exception copy(file(), line());
	const exception ex(copy);
	EXPECT_STREQ(copy.what(), ex.what());
}

TEST_F(exception_test_fixture, operator_stream)
{
	bool result(false);

	try
	{
		//testing that stream doesn't cast exception
		//down to a base class given operator<< is
		//implemented in the base class
		exception ex(file(), line());
		ex << "exception message";
		throw ex;
	}
	catch(const exception&)
	{
		result = true;
	}
	catch(...)
	{
	}

	EXPECT_TRUE(result);
}

TEST_F(exception_test_fixture, operator_what)
{
	std::string result;

	try
	{
		throw exception(file(), line()) << "exception message";
	}
	catch(const exception& e)
	{
		result += e.what();
	}
	catch(...)
	{
	}

	EXPECT_EQ(result, "exception_test_fixture.cpp[100]: exception message");
}

