#pragma once

#include <iosfwd>

enum Wert { Deuce, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace, EmptyWert };

enum Suit { Spades, Cross, Hearts, Diamonds, EmptyFarbe };

struct Karte {
    Suit col;
    Wert val;

    Karte() = default;
    Karte(Suit c, Wert v) : col{c}, val{v} {}
};

inline bool operator==(const Karte lhs, const Karte rhs) {
    return lhs.col == rhs.col && lhs.val == rhs.val;
}

inline bool operator!=(const Karte lhs, const Karte rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& s, const Karte card);
std::istream& operator>>(std::istream& s, Karte& card);
