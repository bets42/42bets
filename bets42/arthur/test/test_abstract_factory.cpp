#include "bets42/arthur/abstract_factory.hpp"
#include <gtest/gtest.h>
#include <string>

using namespace bets42::arthur;

namespace {

    /* FRUIT */
    struct Fruit
    {
        virtual ~Fruit() {}
        virtual std::string name() const = 0;
    };

    struct Apple : public Fruit
    {
        virtual ~Apple() {}
        virtual std::string name() const { return "Apple"; }
    };

    struct Banana : public Fruit
    {
        virtual ~Banana() {}
        virtual std::string name() const { return "Banana"; }
    };  
 
    typedef static_abstract_factory<Fruit> fruit_factory_type;

    struct RegistrarApple : public Fruit,
                            private fruit_factory_type::registrant<RegistrarApple>
    {
        virtual ~RegistrarApple() {}
        virtual std::string name() const { return "Apple"; }
        static std::string registrant_key() { return "apple"; }
    };

    struct RegistrarBanana : public Fruit,
                             private fruit_factory_type::registrant<RegistrarBanana>
    {
        virtual ~RegistrarBanana() {}
        virtual std::string name() const { return "Banana"; }
        static std::string registrant_key() { return "banana"; }
    }; 

    /* LUNCH BOX */
    struct LunchBox 
    {
        virtual ~LunchBox() {}
        virtual const Fruit& fruit() const = 0;
    };

    template <typename TFruit>
    struct LunchBoxImpl : public LunchBox
    {
        virtual ~LunchBoxImpl() {}
        virtual const Fruit& fruit() const { return fruit_; }
        TFruit fruit_;
    };

    template <typename TFruit>
    struct RegistrarLunchBoxImpl : public LunchBox,
                                   private static_abstract_factory<LunchBox>::registrant<RegistrarLunchBoxImpl<TFruit>>
    {
        virtual ~RegistrarLunchBoxImpl() {}
        static std::string registrant_key() { return "lunch_box_with_" + TFruit::registrant_key(); }
        virtual const Fruit& fruit() const { return fruit_; }
        TFruit fruit_;
    };
 
    /* DRINK */
    struct Drink
    {
        virtual ~Drink() {}
        virtual unsigned volume() const = 0;
    };

    struct CokeZero : public Drink
    {
        explicit CokeZero(const unsigned volume) 
            : volume_(volume) {}

        unsigned volume() const { return volume_; }

        unsigned volume_;
    };

    struct RegistrarCokeZero : public Drink,
                               private static_abstract_factory<Drink, unsigned>::registrant<RegistrarCokeZero>
    {
        explicit RegistrarCokeZero(const unsigned volume) 
            : volume_(volume) {}

        static std::string registrant_key() { return "coke_zero"; }

        unsigned volume() const { return volume_; }

        unsigned volume_;
    }; 

} //namespace annonymous

TEST(TestAbstractFactory, constructor)
{
    EXPECT_NO_THROW(bets42::arthur::abstract_factory<Fruit>());
    EXPECT_NO_THROW(bets42::arthur::abstract_factory<LunchBox>());
    EXPECT_NO_THROW(bets42::arthur::abstract_factory<LunchBoxImpl<Fruit>>());
    EXPECT_NO_THROW(bets42::arthur::abstract_factory<LunchBoxImpl<Apple>>());
}

TEST(TestAbstractFactory, register_type)
{
    //non-template types
    bets42::arthur::abstract_factory<Fruit> fruit_factory;
    EXPECT_TRUE(fruit_factory.register_type<Apple>("apple"));
    EXPECT_TRUE(fruit_factory.register_type<Apple>("apple2"));
    EXPECT_TRUE(fruit_factory.register_type<Banana>("banana"));
    EXPECT_FALSE(fruit_factory.register_type<Apple>("apple")); //duplicate

    //template types
    bets42::arthur::abstract_factory<LunchBox> lunch_factory;
    EXPECT_TRUE(lunch_factory.register_type<LunchBoxImpl<Apple>>("apple"));
    EXPECT_TRUE(lunch_factory.register_type<LunchBoxImpl<Apple>>("apple2"));
    EXPECT_TRUE(lunch_factory.register_type<LunchBoxImpl<Banana>>("banana"));
    EXPECT_FALSE(lunch_factory.register_type<LunchBoxImpl<Apple>>("apple")); //duplicate

    //with args
    bets42::arthur::abstract_factory<Drink, unsigned> drink_factory;
    EXPECT_TRUE(drink_factory.register_type<CokeZero>("coke_zero"));
}

TEST(TestAbstractFactory, create)
{
    //non-template types
    bets42::arthur::abstract_factory<Fruit> fruit_factory;
    fruit_factory.register_type<Apple>("apple");
    fruit_factory.register_type<Apple>("apple2");
    fruit_factory.register_type<Banana>("banana");

    EXPECT_EQ(fruit_factory.create("apple")->name(), "Apple");
    EXPECT_EQ(fruit_factory.create("apple2")->name(), "Apple");
    EXPECT_EQ(fruit_factory.create("banana")->name(), "Banana");

    //template types
    bets42::arthur::abstract_factory<LunchBox> lunch_factory;
    lunch_factory.register_type<LunchBoxImpl<Apple>>("apple");
    lunch_factory.register_type<LunchBoxImpl<Apple>>("apple2");
    lunch_factory.register_type<LunchBoxImpl<Banana>>("banana");

    EXPECT_EQ(lunch_factory.create("apple")->fruit().name(), "Apple");
    EXPECT_EQ(lunch_factory.create("apple2")->fruit().name(), "Apple");
    EXPECT_EQ(lunch_factory.create("banana")->fruit().name(), "Banana");

    //with args
    bets42::arthur::abstract_factory<Drink, unsigned> drink_factory;
    drink_factory.register_type<CokeZero>("coke_zero");
    EXPECT_EQ(drink_factory.create("coke_zero", 330)->volume(), 330);
}

TEST(TestAbstractFactory, static_constructor)
{
    EXPECT_NO_THROW(bets42::arthur::static_abstract_factory<Fruit>());
    EXPECT_NO_THROW(bets42::arthur::static_abstract_factory<LunchBox>());
    EXPECT_NO_THROW(bets42::arthur::static_abstract_factory<LunchBoxImpl<Fruit>>());
    EXPECT_NO_THROW(bets42::arthur::static_abstract_factory<LunchBoxImpl<Apple>>());
}

TEST(TestAbstractFactory, static_createType)
{
    //non-template types
    typedef bets42::arthur::static_abstract_factory<Fruit> fruit_factory;
    EXPECT_EQ(fruit_factory::create("apple")->name(), "Apple");
    EXPECT_EQ(fruit_factory::create("banana")->name(), "Banana");

    //template types
    typedef bets42::arthur::static_abstract_factory<LunchBox> lunch_factory;
    EXPECT_EQ(lunch_factory::create("lunch_box_with_apple")->fruit().name(), "Apple");
    EXPECT_EQ(lunch_factory::create("lunch_box_with_banana")->fruit().name(), "Banana");

    //with args
    typedef bets42::arthur::static_abstract_factory<Drink, unsigned> drink_factory;
    EXPECT_EQ(drink_factory::create("coke_zero", 330)->volume(), 330);
}
   
