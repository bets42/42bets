#ifndef BETS42_ARTHUR_ABSTRACT_FACTORY_HPP
#define BETS42_ARTHUR_ABSTRACT_FACTORY_HPP

#include "bets42/arthur/exception.hpp"
#include "bets42/arthur/memory.hpp"
#include <functional>
#include <iterator>
#include <string>
#include <unordered_map>

namespace {

    template <typename T, typename... TArgs>
    std::unique_ptr<T> create_impl(TArgs... args) { 
        return bets42::arthur::make_unique<T>(args...); 
    }

    //http://stackoverflow.com/questions/6420985/how-to-force-a-static-member-to-be-initialized
    template <typename V, V> struct force_static_init_trick {};

} //namespace annonymous 

namespace bets42 { namespace arthur {

    template <typename T, typename... TArgs>
    class abstract_factory
    {
        public:
            typedef std::unordered_map<std::string, std::function<std::unique_ptr<T> (TArgs...)>> factory_map;
            typedef typename factory_map::key_type key_type;

        public:
            template <typename TDerived>
            bool register_type(const key_type& key)
            {
                const auto factory(create_impl<TDerived, TArgs...>);
                return factories_.insert(typename factory_map::value_type(key, factory)).second;
            }

            std::unique_ptr<T> create(const key_type& key, TArgs... args) const
            {
                const auto iter(factories_.find(key));

                if(iter == std::end(factories_)) {
                    throw exception(__FILE__, __LINE__) << "Can't create unknown type " << key;
                }

                return iter->second(args...);
            }

        private:
            factory_map factories_;
    };

    template <typename T, typename... TArgs>
    class static_abstract_factory
    {
        private:
            typedef abstract_factory<T, TArgs...> factory_type;

            static factory_type factory_;
            typedef force_static_init_trick<factory_type&, factory_> force_static_init_trick_factory;

        public:
            static std::unique_ptr<T> create(const typename factory_type::key_type& key, TArgs... args)
            {
                return factory_.create(key, args...);
            }

            template <typename TDerived>
            class registrar
            {
                protected:
                    virtual ~registrar() {}

                private:
                    struct registrar_impl
                    {
                        registrar_impl() {
                            factory_.template register_type<TDerived>(TDerived::registrar_key());
                        }
                    };
                    
                    static registrar_impl impl_;
                    typedef force_static_init_trick<registrar_impl&, impl_> force_static_init_trick_impl;
            };
    };

    template <typename T, typename... TArgs>
    typename static_abstract_factory<T, TArgs...>::factory_type static_abstract_factory<T, TArgs...>::factory_;

    template <typename T, typename... TArgs>
    template <typename TDerived>
    typename static_abstract_factory<T, TArgs...>::template registrar<TDerived>::registrar_impl static_abstract_factory<T, TArgs...>::registrar<TDerived>::impl_;

}} //namespace bets42::arthur

#endif //BETS42_ARTHUR_ABSTRACT_FACTORY_HPP
