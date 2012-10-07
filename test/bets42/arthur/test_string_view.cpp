#include <bets42/arthur/string_view.hpp>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <sstream>

using namespace bets42::arthur;

class string_view_test_fixture : public testing::Test
{
	protected:
		string_view_test_fixture()
			: str_("spud-gun-fun")
			, str_empty_("")
			, arr_(str_.data())
			, arr_end_(arr_ + str_.length())
			, arr_len_(str_.length()) {}

	protected:
		const std::string   str_;
		const std::string   str_empty_;
		const char*         arr_;
		const char*         arr_end_;
		const std::size_t   arr_len_;
};

TEST_F(string_view_test_fixture, ctor_with_std_string)
{
	EXPECT_NO_THROW(string_view(str_)); //l-value
	EXPECT_NO_THROW(string_view("spud-gun-fun")); //r-value
}

TEST_F(string_view_test_fixture, ctor_with_start_end)
{
	EXPECT_NO_THROW(string_view(str_.data(), str_.data() + str_.length()));
	EXPECT_NO_THROW(string_view(arr_, arr_end_));
	EXPECT_THROW(string_view(arr_end_, arr_), std::out_of_range);
}

TEST_F(string_view_test_fixture, ctor_wth_char_and_len)
{
	EXPECT_NO_THROW(string_view(str_.data(), str_.length()));
	EXPECT_NO_THROW(string_view(arr_, arr_len_));
}

TEST_F(string_view_test_fixture, ctor_default)
{
	EXPECT_NO_THROW(string_view());
}

TEST_F(string_view_test_fixture, operator_copy)
{
	const std::string copy("carrot-gun-fun");
	//string_view
	string_view sv_orig(str_);
	sv_orig = string_view(copy);
	EXPECT_EQ(sv_orig, copy);
	//string
	string_view str_orig(str_);
	str_orig = copy;
	EXPECT_EQ(str_orig, copy);
	//char*
	string_view char_orig(str_);
	char_orig = copy.data();
	EXPECT_EQ(char_orig, copy);
}

TEST_F(string_view_test_fixture, begin)
{
	const char match(*(str_.begin()));
	EXPECT_EQ(match, 's');
	EXPECT_EQ(*(string_view(str_).begin()), match);
	EXPECT_EQ(*(string_view(arr_, arr_end_).begin()), match);
	EXPECT_EQ(*(string_view(arr_, arr_len_).begin()), match);
}

TEST_F(string_view_test_fixture, end)
{
	const char match(*(str_.end() - 1));
	EXPECT_EQ(match, 'n');
	EXPECT_EQ(*(string_view(str_).end() - 1), match);
	EXPECT_EQ(*(string_view(arr_, arr_end_).end() - 1), match);
	EXPECT_EQ(*(string_view(arr_, arr_len_).end() - 1), match);
}

TEST_F(string_view_test_fixture, operator_at)
{
	//default ctor
	EXPECT_NO_THROW(string_view()[0]);
	EXPECT_NO_THROW(string_view()[3]);
	//str ctor
	EXPECT_EQ(string_view(str_)[0], str_[0]);
	EXPECT_EQ(string_view(str_)[3], str_[3]);
	//begin/end ctor
	EXPECT_EQ(string_view(arr_, arr_end_)[0], arr_[0]);
	EXPECT_EQ(string_view(arr_, arr_end_)[3], arr_[3]);
	//char/len ctor
	EXPECT_EQ(string_view(arr_, arr_len_)[0], arr_[0]);
	EXPECT_EQ(string_view(arr_, arr_len_)[3], arr_[3]);
}

TEST_F(string_view_test_fixture, at)
{
	//default ctor
	EXPECT_THROW(string_view().at(0), std::out_of_range);
	EXPECT_THROW(string_view().at(3), std::out_of_range);
	//str ctor
	EXPECT_EQ(string_view(str_).at(0), str_.at(0));
	EXPECT_EQ(string_view(str_).at(3), str_.at(3));
	//begin/end ctor
	EXPECT_EQ(string_view(arr_, arr_end_).at(0), arr_[0]);
	EXPECT_EQ(string_view(arr_, arr_end_).at(3), arr_[3]);
	//char/len ctor
	EXPECT_EQ(string_view(arr_, arr_len_).at(0), arr_[0]);
	EXPECT_EQ(string_view(arr_, arr_len_).at(3), arr_[3]);
	EXPECT_THROW(string_view(arr_, arr_len_).at(arr_len_), std::out_of_range);
}

TEST_F(string_view_test_fixture, front)
{
	EXPECT_EQ(string_view(str_).front(), str_.front());
	EXPECT_EQ(string_view(arr_, arr_end_).front(), arr_[0]);
	EXPECT_EQ(string_view(arr_, arr_len_).front(), arr_[0]);
}

TEST_F(string_view_test_fixture, back)
{
	EXPECT_EQ(string_view(str_).back(), str_.back());
	EXPECT_EQ(string_view(arr_, arr_end_).back(), *(arr_end_ - 1));
	EXPECT_EQ(string_view(arr_, arr_len_).back(), arr_[arr_len_ - 1]);
}

TEST_F(string_view_test_fixture, c_str)
{
	EXPECT_EQ(string_view().c_str(), static_cast<const char*>(nullptr));
	EXPECT_EQ(string_view(str_).c_str(), str_.data());
	EXPECT_EQ(string_view(arr_, arr_end_).c_str(), arr_);
	EXPECT_EQ(string_view(arr_, arr_len_).c_str(), arr_);
}

TEST_F(string_view_test_fixture, data)
{
	EXPECT_EQ(string_view().data(), static_cast<const char*>(nullptr));
	EXPECT_EQ(string_view(str_).data(), str_.data());
	EXPECT_EQ(string_view(arr_, arr_end_).data(), arr_);
	EXPECT_EQ(string_view(arr_, arr_len_).data(), arr_);
}

TEST_F(string_view_test_fixture, empty)
{
	//default ctor
	EXPECT_TRUE(string_view().empty());
	//str ctor
	EXPECT_TRUE(string_view(str_empty_).empty());
	EXPECT_FALSE(string_view(str_).empty());
	//begin/end ctor
	EXPECT_TRUE(string_view(arr_, arr_).empty());
	EXPECT_FALSE(string_view(arr_, arr_end_).empty());
	//char/len ctor
	EXPECT_TRUE(string_view(arr_, std::size_t(0)).empty());
	EXPECT_FALSE(string_view(arr_, arr_len_).empty());
}

TEST_F(string_view_test_fixture, length)
{
	//default ctor
	EXPECT_EQ(string_view().length(), 0);
	//str ctor
	EXPECT_EQ(string_view(str_empty_).length(), str_empty_.length());
	EXPECT_EQ(string_view(str_).length(), str_.length());
	//begin/end ctor
	EXPECT_EQ(string_view(arr_, arr_).length(), 0);
	EXPECT_EQ(string_view(arr_, arr_end_).length(), arr_len_);
	//char/len ctor
	EXPECT_EQ(string_view(arr_, std::size_t(0)).length(), 0);
	EXPECT_EQ(string_view(arr_, arr_len_).length(), arr_len_);
}

TEST_F(string_view_test_fixture, size)
{
	//default ctor
	EXPECT_EQ(string_view().size(), 0);
	//str ctor
	EXPECT_EQ(string_view(str_empty_).size(), str_empty_.size());
	EXPECT_EQ(string_view(str_).size(), str_.size());
	//begin/end ctor
	EXPECT_EQ(string_view(arr_, arr_).size(), 0);
	EXPECT_EQ(string_view(arr_, arr_end_).size(), arr_len_);
	//char/len ctor
	EXPECT_EQ(string_view(arr_, std::size_t(0)).size(), 0);
	EXPECT_EQ(string_view(arr_, arr_len_).size(), arr_len_);
}

TEST_F(string_view_test_fixture, compare_with_string_view)
{
	const std::string less_than_str("carrot-gun-fun");
	const string_view less_than(less_than_str);

	const std::string greater_than_str("turnip-gun-fun");
	const string_view greater_than(greater_than_str);

	//str ctor
	EXPECT_LT(string_view(str_).compare(greater_than), 0);
	EXPECT_EQ(string_view(str_).compare(str_), 0);
	EXPECT_GT(string_view(str_).compare(less_than), 0);
	//begin/end ctor
	EXPECT_LT(string_view(arr_, arr_end_).compare(greater_than), 0);
	EXPECT_EQ(string_view(arr_, arr_end_).compare(str_), 0);
	EXPECT_GT(string_view(arr_, arr_end_).compare(less_than), 0);
	//char/len ctor
	EXPECT_LT(string_view(arr_, arr_len_).compare(greater_than), 0);
	EXPECT_EQ(string_view(arr_, arr_len_).compare(str_), 0);
	EXPECT_GT(string_view(arr_, arr_len_).compare(less_than), 0);
}

TEST_F(string_view_test_fixture, compare_with_std_string)
{
	const std::string less_than("carrot-gun-fun");
	const std::string greater_than("turnip-gun-fun");

	//str ctor
	EXPECT_LT(string_view(str_).compare(greater_than), 0);
	EXPECT_EQ(string_view(str_).compare(str_), 0);
	EXPECT_GT(string_view(str_).compare(less_than), 0);
	//begin/end ctor
	EXPECT_LT(string_view(arr_, arr_end_).compare(greater_than), 0);
	EXPECT_EQ(string_view(arr_, arr_end_).compare(str_), 0);
	EXPECT_GT(string_view(arr_, arr_end_).compare(less_than), 0);
	//char/len ctor
	EXPECT_LT(string_view(arr_, arr_len_).compare(greater_than), 0);
	EXPECT_EQ(string_view(arr_, arr_len_).compare(str_), 0);
	EXPECT_GT(string_view(arr_, arr_len_).compare(less_than), 0);
}

TEST_F(string_view_test_fixture, as_string)
{
	EXPECT_EQ(string_view(str_).as_string(), str_);
	EXPECT_EQ(string_view(arr_, arr_end_).as_string(), str_);
	EXPECT_EQ(string_view(arr_, arr_len_).as_string(), str_);
}

TEST_F(string_view_test_fixture, operator_stream)
{
	std::stringstream stream;
	stream << string_view(str_);
	EXPECT_EQ(stream.str(), str_);
}

TEST_F(string_view_test_fixture, operator_equal)
{
	const string_view comparator(str_);

	const std::string not_equal_str("parsnip-gun-fun");
	const string_view not_equal(not_equal_str);

	//string_view
	EXPECT_TRUE(comparator == comparator);
	EXPECT_FALSE(comparator == not_equal);
	//string
	EXPECT_TRUE(comparator == str_);
	EXPECT_FALSE(comparator == not_equal_str);
	//char*
	EXPECT_TRUE(comparator == str_.data());
	EXPECT_FALSE(comparator == not_equal_str.data());
}

TEST_F(string_view_test_fixture, operator_not_equal)
{
	const string_view comparator(str_);

	const std::string not_equal_str("parsnip-gun-fun");
	const string_view not_equal(not_equal_str);

	//string_view
	EXPECT_FALSE(comparator != comparator);
	EXPECT_TRUE(comparator != not_equal);
	//string
	EXPECT_FALSE(comparator != str_);
	EXPECT_TRUE(comparator != not_equal_str);
	//char*
	EXPECT_FALSE(comparator != str_.data());
	EXPECT_TRUE(comparator != not_equal_str.data());
}

TEST_F(string_view_test_fixture, operator_less_than)
{
	const string_view comparator(str_);

	const std::string less_than_str("carrot-gun-fun");
	const string_view less_than(less_than_str);

	const std::string greater_than_str("turnip-gun-fun");
	const string_view greater_than(greater_than_str);

	//string_view
	EXPECT_FALSE(comparator < comparator);
	EXPECT_FALSE(comparator < less_than);
	EXPECT_TRUE(comparator < greater_than);
	//string
	EXPECT_FALSE(comparator < str_);
	EXPECT_FALSE(comparator < less_than_str);
	EXPECT_TRUE(comparator < greater_than_str);
	//char*
	EXPECT_FALSE(comparator < str_.data());
	EXPECT_FALSE(comparator < less_than_str.data());
	EXPECT_TRUE(comparator < greater_than_str.data());
}

TEST_F(string_view_test_fixture, operator_less_than_equal)
{
	const string_view comparator(str_);

	const std::string less_than_str("carrot-gun-fun");
	const string_view less_than(less_than_str);

	const std::string greater_than_str("turnip-gun-fun");
	const string_view greater_than(greater_than_str);

	//string_view
	EXPECT_TRUE(comparator <= comparator);
	EXPECT_FALSE(comparator <= less_than);
	EXPECT_TRUE(comparator <= greater_than);
	//string
	EXPECT_TRUE(comparator <= str_);
	EXPECT_FALSE(comparator <= less_than_str);
	EXPECT_TRUE(comparator <= greater_than_str);
	//char*
	EXPECT_TRUE(comparator <= str_.data());
	EXPECT_FALSE(comparator <= less_than_str.data());
	EXPECT_TRUE(comparator <= greater_than_str.data());
}

TEST_F(string_view_test_fixture, operator_greater_than)
{
	const string_view comparator(str_);

	const std::string less_than_str("carrot-gun-fun");
	const string_view less_than(less_than_str);

	const std::string greater_than_str("turnip-gun-fun");
	const string_view greater_than(greater_than_str);

	//string_view
	EXPECT_FALSE(comparator > comparator);
	EXPECT_TRUE(comparator > less_than);
	EXPECT_FALSE(comparator > greater_than);
	//string
	EXPECT_FALSE(comparator > str_);
	EXPECT_TRUE(comparator > less_than_str);
	EXPECT_FALSE(comparator > greater_than_str);
	//char*
	EXPECT_FALSE(comparator > str_.data());
	EXPECT_TRUE(comparator > less_than_str.data());
	EXPECT_FALSE(comparator > greater_than_str.data());
}

TEST_F(string_view_test_fixture, operator_greater_than_equal)
{
	const string_view comparator(str_);

	const std::string less_than_str("carrot-gun-fun");
	const string_view less_than(less_than_str);

	const std::string greater_than_str("turnip-gun-fun");
	const string_view greater_than(greater_than_str);

	//string_view
	EXPECT_TRUE(comparator >= comparator);
	EXPECT_TRUE(comparator >= less_than);
	EXPECT_FALSE(comparator >= greater_than);
	//string
	EXPECT_TRUE(comparator >= str_);
	EXPECT_TRUE(comparator >= less_than_str);
	EXPECT_FALSE(comparator >= greater_than_str);
	//char*
	EXPECT_TRUE(comparator >= str_.data());
	EXPECT_TRUE(comparator >= less_than_str.data());
	EXPECT_FALSE(comparator >= greater_than_str.data());
}

