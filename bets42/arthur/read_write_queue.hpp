#ifndef BETS42_ARTHUR_READ_WRITE_QUEUE_HPP
#define BETS42_ARTHUR_READ_WRITE_QUEUE_HPP

// Lockfree single producer/single consumer queue
// This queue is based upon an article by Herb Sutter in which he implements
// a lock free queue in the context of a single producer and single consumer.
// It's imperitive this is used in such a manner, using multiple producers or
// consumers will render the class non-thread-safe.
//
// For more inforamtion Google "Herb Sutter writing lock free code - a corrected queue"

// FIXME: WARNING - doesn't compile yet!

#include <atomic>
#include <deque>
#include <utility>

namespace bets42 { namespace arthur {

    template <typename T>
    class read_write_queue
    {
        public:
            read_write_queue()
            {
                // add a dummy value so we have something to point at
                queue_.push_back(T());
                front_ = divider_ = back_ = std::begin(queue_);
            }

            virtual ~read_write_queue() {}

            template <typename... TArgs>
            void emplace(TArgs&&... args)
            {
                queue_.emplace_back(args...);
                push_impl();
            }

            void push(T&& val)
            {
                queue_.push_back(val);
                push_impl();
            }

            void push(const T& val)
            {
                queue_.push_back(val);
                push_impl();
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
            void push_impl()
            {
                // an element has been added, now 'publish' it
                ++back_;

                // erase all elements that have been read, up to divider
                auto tmp(front_);
                queue_.erase(tmp, divider_);
                front_ = divider_;
            }

        private:
            typedef std::deque<T> queue_type;
            typedef std::atomic<typename queue_type::iterator> iterator_type;

            queue_type      queue_;
            iterator_type   front_;
            iterator_type   divider_;
            iterator_type   back_;
    };

}}

#endif  // BETS42_ARTHUR_READ_WRITE_QUEUE_HPP
