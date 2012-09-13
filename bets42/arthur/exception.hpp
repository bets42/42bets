#ifndef BETS42_ARTHUR_STREAMING_EXCEPTION_HPP
#define BETS42_ARTHUR_STREAMING_EXCEPTION_HPP

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

namespace {

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
    {
        os << '[';
        for(const auto val : vec) {
            os << val << ",";
        }
        os << ']';
        return os;
    }    

} //namespace annonymous
         
namespace bets42 { namespace arthur {

    template <typename TDerived>
    class streaming_exception : public std::runtime_error
    {
        public:
            streaming_exception(const std::string& file, const long line)
                : std::runtime_error("")
            {
                stream_ << file << "[" << line << "]: ";
            }

            streaming_exception(const streaming_exception& other)
                : std::runtime_error("")
            {   
                stream_ << other.stream_.rdbuf();
            }

            streaming_exception(streaming_exception&& other)
                : std::runtime_error(""),
                  stream_(std::move(other.stream_)) {}

            streaming_exception& operator=(streaming_exception&& other)
            {
                if(this != std::addressof(other))
                {
                    stream_ = std::move(other.stream_);
                }
                return *this;
            }

            template <typename T>
            TDerived& operator<<(const T& t)
            {
                stream_ << t;
                return static_cast<TDerived&>(*this);
            }

            virtual const char* what() const throw()
            {
                what_ = stream_.str();
                return what_.c_str();
            }

        private:
           std::stringstream    stream_;
           mutable std::string  what_;
    };

    struct exception : public streaming_exception<exception>
    {
        exception(const std::string& file, const long line)
            : streaming_exception<exception>(file, line) {}
    };

}} //namespace bets42::arthur

#endif //BETS42_ARTHUR_STREAMING_EXCEPTION_HPP
