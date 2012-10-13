#ifndef BETS42_ARTHUR_BASIC_LOCKABLE_HPP
#define BETS42_ARTHUR_BASIC_LOCKABLE_HPP

// Implements the BasicLockable concept
//
// For example:
// typedef basic_lockable_type<std::queue<int>, std::mutex> Queue;
// std::lock_guard<Queue> lock(queue_);

#include <utility>

namespace bets42 { namespace arthur {

    template <typename TMutex>
    struct basic_lockable : private TMutex
    {
        virtual ~basic_lockable() {}
        using TMutex::lock;
        using TMutex::unlock;
    };

    template <typename TType, typename TMutex>
    struct basic_lockable_type : public TType
                               , private TMutex
    {
        template <typename... TArgs>
        explicit basic_lockable_type(TArgs&&... args)
            : TType(std::forward<TArgs...>(args)...) {}

        virtual ~basic_lockable_type() {}

        using TMutex::lock;
        using TMutex::unlock;
    };

}}

#endif  // BETS42_ARTHUR_BASIC_LOCKABLE_HPP
