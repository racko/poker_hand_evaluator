#pragma once

#include "card.h"
#include <array>
#include <iosfwd>

enum Rank { HighCard, OnePair, TwoPair, ThreeOfAKind, Straight, Flush, FullHouse, FourOfAKind, StraightFlush };

struct Hand {
    Rank r;
    std::array<Karte, 5> cards;

    Hand() = default;
    Hand(Rank rank, const std::array<Karte, 5>& c) : r{rank}, cards{c} {}
};

std::ostream& operator<<(std::ostream& s, const Rank rank);

std::ostream& operator<<(std::ostream& s, const Hand hand);

bool operator==(const Hand& lhs, const Hand& rhs);

int compare(const Hand& lhs, const Hand& rhs);
