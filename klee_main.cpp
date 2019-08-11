#include "NumRange.h"             // for NumRange
#include "card.h"                 // for Karte
#include "deck.h"                 // for make_sorted_deck, run
#include "polymorphic_iterator.h" // for ContiguousIterator
#include <array>                  // for array
#include <cstdint>                // for uint8_t
#include <iostream>               // for size_t, char_traits, cout, ostream
#include <klee/klee.h>
#include <utility> // for swap

namespace {
const std::array<Karte, 52> Deck{make_sorted_deck()};
struct NullLogger : std::ostream {};

NullLogger null_logger;
auto& logger = std::cout;
} // namespace

int main() {
    std::uint8_t cards[7];
    klee_make_symbolic(cards, sizeof(cards), "cards");

    klee_assume(cards[0] >= 0);
    klee_assume(cards[0] < 52);

    klee_assume(cards[1] >= 1);
    klee_assume(cards[1] < 52);

    klee_assume(cards[2] >= 2);
    klee_assume(cards[2] < 52);

    klee_assume(cards[3] >= 3);
    klee_assume(cards[3] < 52);

    klee_assume(cards[4] >= 4);
    klee_assume(cards[4] < 52);

    klee_assume(cards[5] >= 5);
    klee_assume(cards[5] < 52);

    klee_assume(cards[6] >= 6);
    klee_assume(cards[6] < 52);

    std::array<Karte, 52> random_deck{Deck};
    for (const auto i : NumRange<std::size_t>(0, 7)) {
        std::swap(random_deck[i], random_deck[i + cards[i]]);
    }

    ContiguousIterator<const Karte> start{random_deck.cbegin()};
    ContiguousIterator<const Karte> stop{random_deck.cbegin() + 7};

    run(start, stop, logger);

    return 0;
}
