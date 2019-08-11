#include "card.h"

#include <algorithm>
#include <istream>
#include <ostream>

namespace {
const char* Wert_Strings[] = {
    "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};

char Werte_kurz[] = {
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'T',
    'J',
    'Q',
    'K',
    'A',
};

char Farben_kurz[] = {
    's',
    'c',
    'h',
    'd',
};

const char* Farbe_Strings[] = {"Spades", "Cross", "Hearts", "Diamonds"};

Karte char2card(const char* card) {
    const auto value = std::find(std::begin(Werte_kurz), std::end(Werte_kurz), card[0]) - std::begin(Werte_kurz);
    const auto suit = std::find(std::begin(Farben_kurz), std::end(Farben_kurz), card[1]) - std::begin(Farben_kurz);
    return Karte(Suit(suit), Wert(value));
}
} // namespace

std::ostream& operator<<(std::ostream& s, const Karte card) {
    return s << Werte_kurz[card.val] << Farben_kurz[card.col];
}

std::istream& operator>>(std::istream& s, Karte& card) {
    char buffer[3];
    s >> buffer;
    card = char2card(buffer);
    return s;
}
