#include "gtest/gtest.h"
#include "card.h"
#include "determine_hand.h"
#include "hand.h"
#include <array>
#include <cassert>

TEST(DetermineHand, StraightFlushFiveHigh) {
    bool cards[4][13] = {};
    // should probably change to 7 cards ...
    cards[Spades][Ace] = true;
    cards[Spades][Deuce] = true;
    cards[Spades][Three] = true;
    cards[Spades][Four] = true;
    cards[Spades][Five] = true;
    const auto actual = DetermineHand(cards);
    const auto expected = Hand{StraightFlush,
                          std::array<Karte, 5>{
                              {{Spades, Five}, {Spades, Four}, {Spades, Three}, {Spades, Deuce}, {Spades, Ace}}}};
    ASSERT_EQ(actual, expected);
}

TEST(DetermineHand, StraightFlushSixHigh) {
    bool cards[4][13] = {};
    // should probably change to 7 cards ...
    cards[Spades][Deuce] = true;
    cards[Spades][Three] = true;
    cards[Spades][Four] = true;
    cards[Spades][Five] = true;
    cards[Spades][Six] = true;
    const auto actual = DetermineHand(cards);
    const auto expected = Hand{StraightFlush,
                          std::array<Karte, 5>{
                              {{Spades, Six}, {Spades, Five}, {Spades, Four}, {Spades, Three}, {Spades, Deuce}}}};
    ASSERT_EQ(actual, expected);
}

TEST(DetermineHand, RoyalFlush) {
    bool cards[4][13] = {};
    // should probably change to 7 cards ...
    cards[Spades][Ten] = true;
    cards[Spades][Jack] = true;
    cards[Spades][Queen] = true;
    cards[Spades][King] = true;
    cards[Spades][Ace] = true;
    const auto actual = DetermineHand(cards);
    const auto expected = Hand{StraightFlush,
                          std::array<Karte, 5>{
                              {{Spades, Ace}, {Spades, King}, {Spades, Queen}, {Spades, Jack}, {Spades, Ten}}}};
    ASSERT_EQ(actual, expected);
}

TEST(DetermineHand, StraightFiveHigh) {
    bool cards[4][13] = {};
    // should probably change to 7 cards ...
    cards[Spades][Ace] = true;
    cards[Hearts][Deuce] = true;
    cards[Diamonds][Three] = true;
    cards[Cross][Four] = true;
    cards[Spades][Five] = true;
    const auto actual = DetermineHand(cards);
    const auto expected = Hand{Straight,
                          std::array<Karte, 5>{
                              {{Spades, Five}, {Spades, Four}, {Spades, Three}, {Spades, Deuce}, {Spades, Ace}}}};
    ASSERT_EQ(actual, expected);
}

TEST(DetermineHand, StraightSixHigh) {
    bool cards[4][13] = {};
    // should probably change to 7 cards ...
    cards[Spades][Deuce] = true;
    cards[Hearts][Three] = true;
    cards[Diamonds][Four] = true;
    cards[Cross][Five] = true;
    cards[Spades][Six] = true;
    const auto actual = DetermineHand(cards);
    const auto expected = Hand{Straight,
                          std::array<Karte, 5>{
                              {{Spades, Six}, {Spades, Five}, {Spades, Four}, {Spades, Three}, {Spades, Deuce}}}};
    ASSERT_EQ(actual, expected);
}

TEST(DetermineHand, StraightAceHigh) {
    bool cards[4][13] = {};
    // should probably change to 7 cards ...
    cards[Spades][Ten] = true;
    cards[Hearts][Jack] = true;
    cards[Diamonds][Queen] = true;
    cards[Cross][King] = true;
    cards[Spades][Ace] = true;
    const auto actual = DetermineHand(cards);
    const auto expected = Hand{Straight,
                          std::array<Karte, 5>{
                              {{Spades, Ace}, {Spades, King}, {Spades, Queen}, {Spades, Jack}, {Spades, Ten}}}};
    ASSERT_EQ(actual, expected);
}

