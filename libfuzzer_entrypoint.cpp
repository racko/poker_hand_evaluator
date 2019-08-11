#include <cstdint>                // for uint8_t
#include <array>                   // for array
#include <iostream>                // for size_t, char_traits, cout, ostream
#include <utility>                 // for swap
#include "NumRange.h"              // for NumRange
#include "card.h"                  // for Karte
#include "deck.h"                  // for make_sorted_deck, run
#include "polymorphic_iterator.h"  // for ContiguousIterator

namespace {

const std::array<Karte, 52> Deck{make_sorted_deck()};

struct NullLogger : std::ostream {};

NullLogger null_logger;
auto& logger = std::cout;

} // namespace

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data, std::size_t size) {
    if (size < 7 || data[0] > 51 || data[1] > 50 || data[2] > 49 || data[3] > 48 || data[4] > 47 || data[5] > 46 ||
        data[6] > 45)
        return 0;

    std::array<Karte, 52> random_deck{Deck};
    for (const auto i : NumRange<std::size_t>(0, 7)) {
        std::swap(random_deck[i], random_deck[i + data[i]]);
    }

    ContiguousIterator<const Karte> cards{random_deck.cbegin()};
    ContiguousIterator<const Karte> stop{random_deck.cbegin() + 7};

    run(cards, stop, logger);

    return 0;
}
