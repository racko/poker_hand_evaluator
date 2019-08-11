#pragma once

#include <array>   // for array
#include <iosfwd>  // for ostream
struct Karte;
template <typename T> struct PolymorphicIterator;

void run(PolymorphicIterator<const Karte>& cards, PolymorphicIterator<const Karte>& stop, std::ostream& logger);

std::array<Karte, 52> make_sorted_deck();
