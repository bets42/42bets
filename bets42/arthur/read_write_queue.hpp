#ifndef BETS42_ARTHUR_READ_WRITE_QUEUE_HPP
#define BETS42_ARTHUR_READ_WRITE_QUEUE_HPP

// Lockfree single producer/single consumer queue
// This queue is based upon an article by Herb Sutter in which he implements
// a lock free queue in the context of a single producer and single consumer.
// It's imperitive this is used in such a manner, using multiple producers or
// consumers will render the class non-thread-safe.
//
// For more inforamtion Google "Herb Sutter writing lock free code - a corrected queue"

//TODO:
// 1. Add support for emplace(Args&&... args) and push(T&&)

#include <atomic>
#include <list>

namespace bets42 { namespace arthur {

    template <typename T>
    class read_write_queue
    {
        public:
            read_write_queue()
            {
                // add a dummy value so we have something to point at
                queue_.emplace_front(T());
                front_ = divider_ = back_ = std::begin(queue_);
            }

            virtual ~read_write_queue() {}

            void push(const T& val)
            {
                // add element and then publish it by incrementing
                // the back iterator
                queue_.emplace_back(val);
                ++back_;

                // erase all elements that have been read, up to divider
                auto tmp(front_);
                queue_.erase(tmp, divider_);
                front_ = divider_;
            }

            std::pair<bool, T> pop()
            {
                // atomically increment divider to mark as read
                const bool has_val(!empty());
                return std::make_pair(has_val, has_val ? *(++divider_) : T());
            }

            bool empty() const
            {
                return divider_ == back_;
            }

        private:
            typedef std::list<T>                    queue_type;
            typedef typename queue_type::iterator   iterator_type;

            queue_type      queue_;
            iterator_type   front_;
            iterator_type   divider_;
            iterator_type   back_;
    };

}}

#endif  // BETS42_ARTHUR_READ_WRITE_QUEUE_HPP
