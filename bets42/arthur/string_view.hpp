#ifndef BETS42_ARTHUR_STRING_VIEW_HPP
#define BETS42_ARTHUR_STRING_VIEW_HPP

// TODO:
// 1. implement move semantics for class (the 'big 5')
// 2. test npos

#include <array>
#include <iterator>
#include <ostream>
#include <string>

namespace bets42
{
	namespace arthur
	{
		class string_view
		{
			public:
				typedef std::array<char, 0>                     typedef_type;
				typedef typedef_type::value_type                value_type;
				typedef typedef_type::size_type                 size_type;
				typedef typedef_type::reference                 reference;
				typedef typedef_type::const_reference           const_reference;
				typedef typedef_type::pointer                   pointer;
				typedef typedef_type::const_pointer             const_pointer;
				typedef typedef_type::iterator                  iterator;
				typedef typedef_type::const_iterator            const_iterator;
				typedef typedef_type::reverse_iterator          reverse_iterator;
				typedef typedef_type::const_reverse_iterator    const_reverse_iterator;

				static const auto npos = std::string::npos;

			public:
				explicit string_view(const std::string& str);
				string_view(const char* const begin, const char* const end);
				string_view(const char* const str, const size_type len);
				explicit string_view(const char* const str);
				string_view();

				// copy assignment
				string_view& operator=(const string_view& str);
				string_view& operator=(const std::string& str);
				string_view& operator=(const char* const str);

				// iterators
				const_iterator begin() const;
				const_iterator end() const;

				// element access
				const_reference operator[](const size_type pos) const;
				const_reference at(const size_type pos) const;

				const_reference front() const;
				const_reference back() const;

				const_pointer c_str() const;
				const_pointer data() const;

				// size
				bool empty() const;
				size_type size() const;
				size_type length() const;

				// operators
				int compare(const string_view& str) const;
				int compare(const std::string& str) const;

				// conversion
				std::string as_string() const;

			private:
				const_iterator begin_;
				const_iterator end_;
		};

		std::ostream& operator<<(std::ostream& stream, const string_view& str);

		bool operator==(const string_view& lhs, const string_view& rhs);
		bool operator==(const string_view& lhs, const std::string& rhs);
		bool operator==(const string_view& lhs, const char* const rhs);

		bool operator!=(const string_view& lhs, const string_view& rhs);
		bool operator!=(const string_view& lhs, const std::string& rhs);
		bool operator!=(const string_view& lhs, const char* const rhs);

		bool operator<(const string_view& lhs, const string_view& rhs);
		bool operator<(const string_view& lhs, const std::string& rhs);
		bool operator<(const string_view& lhs, const char* const rhs);

		bool operator<=(const string_view& lhs, const string_view& rhs);
		bool operator<=(const string_view& lhs, const std::string& rhs);
		bool operator<=(const string_view& lhs, const char* const rhs);

		bool operator>(const string_view& lhs, const string_view& rhs);
		bool operator>(const string_view& lhs, const std::string& rhs);
		bool operator>(const string_view& lhs, const char* const rhs);

		bool operator>=(const string_view& lhs, const string_view& rhs);
		bool operator>=(const string_view& lhs, const std::string& rhs);
		bool operator>=(const string_view& lhs, const char* const rhs);
	}
}

#endif  // BETS42_ARTHUR_STRING_VIEW_HPP
