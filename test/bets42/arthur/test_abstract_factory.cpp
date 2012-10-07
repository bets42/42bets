#include <bets42/arthur/abstract_factory.hpp>
#include <gtest/gtest.h>
#include <string>

using namespace bets42::arthur;

namespace
{
	// no-arg ctor
	struct Fruit
	{
		virtual ~Fruit() {}
		virtual std::string name() const = 0;
	};

	typedef abstract_factory<Fruit>         FruitFactory;
	typedef static_abstract_factory<Fruit>  StaticFruitFactory;

	struct Apple : public Fruit
			, private StaticFruitFactory::registrant<Apple>
	{
		virtual ~Apple() {}
		std::string name() const
		{
			return "Apple";
		}

		static std::string registrant_key()
		{
			return "apple";
		}
	};

	struct Banana : public Fruit
			, private StaticFruitFactory::registrant<Banana>
	{
		virtual ~Banana() {}
		std::string name() const
		{
			return "Banana";
		}

		static std::string registrant_key()
		{
			return "banana";
		}
	};

	// with-arg ctor
	class Song
	{
		public:
			explicit Song(const unsigned bpm)
				: bpm_(bpm) {}
			virtual ~Song() {}

			unsigned bpm() const
			{
				return bpm_;
			}

		private:
			const unsigned bpm_;
	};

	typedef abstract_factory<Song, const unsigned>          SongFactory;
	typedef static_abstract_factory<Song, const unsigned>   StaticSongFactory;

	struct DrumAndBassSong : public Song
			, private StaticSongFactory::registrant<DrumAndBassSong>
	{
		explicit DrumAndBassSong(const unsigned bpm)
			: Song(bpm) {}
		virtual ~DrumAndBassSong() {}

		static std::string registrant_key()
		{
			return "dnb";
		}
	};

	struct TechnoSong : public Song
			, private StaticSongFactory::registrant<TechnoSong>
	{
		explicit TechnoSong(const unsigned bpm)
			: Song(bpm) {}
		virtual ~TechnoSong() {}

		static std::string registrant_key()
		{
			return "techno";
		}
	};

}  // namespace annonymous

class abstract_factory_test_fixture : public testing::Test {};

TEST_F(abstract_factory_test_fixture, constructor)
{
	EXPECT_NO_THROW(FruitFactory());
	EXPECT_NO_THROW(SongFactory());
}

TEST_F(abstract_factory_test_fixture, register_type)
{
	FruitFactory fruitf;
	EXPECT_TRUE(fruitf.register_type<Apple>("apple"));
	EXPECT_TRUE(fruitf.register_type<Apple>("apple2"));
	EXPECT_TRUE(fruitf.register_type<Banana>("banana"));
	EXPECT_FALSE(fruitf.register_type<Apple>("apple"));  // duplicate

	SongFactory songf;
	EXPECT_TRUE(songf.register_type<DrumAndBassSong>("dnb"));
	EXPECT_TRUE(songf.register_type<DrumAndBassSong>("2dnb"));
	EXPECT_TRUE(songf.register_type<TechnoSong>("techno"));
	EXPECT_FALSE(songf.register_type<DrumAndBassSong>("2dnb"));  // duplicate
}

TEST_F(abstract_factory_test_fixture, create)
{
	// non-static
	FruitFactory fruitf;
	EXPECT_TRUE(fruitf.register_type<Apple>("apple"));
	EXPECT_TRUE(fruitf.register_type<Apple>("apple2"));
	EXPECT_TRUE(fruitf.register_type<Banana>("banana"));

	EXPECT_EQ(fruitf.create("apple")->name(), "Apple");
	EXPECT_EQ(fruitf.create("apple2")->name(), "Apple");
	EXPECT_EQ(fruitf.create("banana")->name(), "Banana");

	SongFactory songf;
	EXPECT_TRUE(songf.register_type<DrumAndBassSong>("dnb"));
	EXPECT_TRUE(songf.register_type<DrumAndBassSong>("2dnb"));
	EXPECT_TRUE(songf.register_type<TechnoSong>("techno"));

	EXPECT_EQ(songf.create("dnb", 174)->bpm(), 174);
	EXPECT_EQ(songf.create("2dnb", 168)->bpm(), 168);
	EXPECT_EQ(songf.create("techno", 130)->bpm(), 130);

	// static
	EXPECT_EQ(StaticFruitFactory::create("apple")->name(), "Apple");
	EXPECT_EQ(StaticFruitFactory::create("banana")->name(), "Banana");

	EXPECT_EQ(StaticSongFactory::create("dnb", 172)->bpm(), 172);
	EXPECT_EQ(StaticSongFactory::create("techno", 133)->bpm(), 133);
}
