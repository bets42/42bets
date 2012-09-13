#ifndef BETS42_DEEPTHGT_EXCEPTION_HPP
#define BETS42_DEEPTHGT_EXCEPTION_HPP

#include "bets42/arthur/exception.hpp"
#include <string>

namespace bets42 { namespace deepthgt {

    struct Exception : public arthur::streaming_exception<Exception>
    {
        Exception(const std::string& file, const long line)
            : arthur::streaming_exception<Exception>(file, line) {}
    }; 

}} //namespace bets42::deepthgt

#endif //BETS42_DEEPTHGT_EXCEPTION_HPP
