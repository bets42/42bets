#include "string_view.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <string>

using namespace bets42::arthur;

string_view::string_view(const std::string& str)
	: begin_(str.data())
	, end_(str.data() + str.length()) {}

string_view::string_view(const char* const begin, const char* const end)
	: begin_(begin)
	, end_(end)
{
	if(begin_ > end_)
	{
		throw std::out_of_range("reversed string");
	}
}

string_view::string_view(const char* const str, const size_type len)
	: begin_(str)
	, end_(str + len) {}

string_view::string_view()
	: begin_(nullptr)
	, end_(nullptr) {}

string_view::string_view(const char* const str)
	: begin_(str)
	, end_(str + std::strlen(str)) {}

string_view& string_view::operator=(const string_view& str)
{
	if(this != std::addressof(str))
	{
		begin_ = str.begin_;
		end_ = str.end_;
	}

	return *this;
}

string_view& string_view::operator=(const std::string& str)
{
	begin_ = str.data();
	end_ = begin_ + str.length();
	return *this;
}

string_view& string_view::operator=(const char* const str)
{
	begin_ = str;
	end_ = begin_ + std::strlen(str);
	return *this;
}

string_view::const_iterator string_view::begin() const
{
	return begin_;
}
string_view::const_iterator string_view::end() const
{
	return end_;
}

string_view::const_reference string_view::operator[](const size_type pos) const
{
	return begin_[pos];
}

string_view::const_reference string_view::at(const size_type pos) const
{
	if(length() == 0 || pos >= length())
	{
		throw std::out_of_range("at index is longer than string_view length}");
	}

	return (*this)[pos];
}

string_view::const_reference string_view::front() const
{
	return (*this)[0];
}

string_view::const_reference string_view::back() const
{
	return (*this)[length() - 1];
}

string_view::const_pointer string_view::c_str() const
{
	return &(*begin_);
}

string_view::const_pointer string_view::data() const
{
	return c_str();
}

bool string_view::empty() const
{
	return length() == 0;
}

string_view::size_type string_view::size() const
{
	return length();
}

string_view::size_type string_view::length() const
{
	return static_cast<size_type>(std::distance(begin_, end_));
}

int string_view::compare(const string_view& str) const
{
	return std::strncmp(data(), str.data(), std::max(length(), str.length()));
}

int string_view::compare(const std::string& str) const
{
	return std::strncmp(data(), str.data(), std::max(length(), str.length()));
}

std::string string_view::as_string() const
{
	return std::string(begin(), end());
}

std::ostream& bets42::arthur::operator<<(std::ostream& stream, const string_view& str)
{
	static const auto max(std::numeric_limits<std::streamsize>::max());

	if(str.length() > max)
	{
		stream.write(str.data(), max);
		stream.write(str.data() + max, static_cast<std::streamsize>(str.length() - max));
	}
	else
	{
		stream.write(str.data(), static_cast<std::streamsize>(str.length()));
	}

	return stream;
}

bool bets42::arthur::operator==(const string_view& lhs, const string_view& rhs)
{
	return lhs.compare(rhs) == 0;
}
bool bets42::arthur::operator==(const string_view& lhs, const std::string& rhs)
{
	return lhs.compare(rhs) == 0;
}
bool bets42::arthur::operator==(const string_view& lhs, const char* const rhs)
{
	return std::strncmp(lhs.data(), rhs, lhs.length()) == 0;
}

bool bets42::arthur::operator!=(const string_view& lhs, const string_view& rhs)
{
	return !(lhs == rhs);
}
bool bets42::arthur::operator!=(const string_view& lhs, const std::string& rhs)
{
	return !(lhs == rhs);
}
bool bets42::arthur::operator!=(const string_view& lhs, const char* const rhs)
{
	return !(lhs == rhs);
}

bool bets42::arthur::operator<(const string_view& lhs, const string_view& rhs)
{
	return lhs.compare(rhs) < 0;
}
bool bets42::arthur::operator<(const string_view& lhs, const std::string& rhs)
{
	return lhs.compare(rhs) < 0;
}
bool bets42::arthur::operator<(const string_view& lhs, const char* const rhs)
{
	return std::strncmp(lhs.data(), rhs, lhs.length()) < 0;
}

bool bets42::arthur::operator<=(const string_view& lhs, const string_view& rhs)
{
	return !(lhs > rhs);
}
bool bets42::arthur::operator<=(const string_view& lhs, const std::string& rhs)
{
	return !(lhs > rhs);
}
bool bets42::arthur::operator<=(const string_view& lhs, const char* const rhs)
{
	return !(lhs > rhs);
}

bool bets42::arthur::operator>(const string_view& lhs, const string_view& rhs)
{
	return lhs.compare(rhs) > 0;
}
bool bets42::arthur::operator>(const string_view& lhs, const std::string& rhs)
{
	return lhs.compare(rhs) > 0;
}
bool bets42::arthur::operator>(const string_view& lhs, const char* const rhs)
{
	return std::strncmp(lhs.data(), rhs, lhs.length()) > 0;
}

bool bets42::arthur::operator>=(const string_view& lhs, const string_view& rhs)
{
	return !(lhs < rhs);
}
bool bets42::arthur::operator>=(const string_view& lhs, const std::string& rhs)
{
	return !(lhs < rhs);
}
bool bets42::arthur::operator>=(const string_view& lhs, const char* const rhs)
{
	return !(lhs < rhs);
}
