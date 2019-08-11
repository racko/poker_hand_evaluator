#include "determine_hand.h"

#include "NumRange.h"
#include "hand.h"
#include <cassert>
#include <numeric>

namespace {
template <class ForwardIt, class Size, class UnaryPredicate>
constexpr ForwardIt search_n(ForwardIt first, ForwardIt last, Size count, UnaryPredicate p) {
    if (count <= 0) {
        return first;
    }
    for (; first != last; ++first) {
        if (!p(*first)) {
            continue;
        }

        ForwardIt candidate = first;
        Size cur_count = 0;

        while (true) {
            ++cur_count;
            if (cur_count >= count) {
                // success
                return candidate;
            }
            ++first;
            if (first == last) {
                // exhausted the list
                return last;
            }
            if (!p(*first)) {
                // too few in a row
                break;
            }
        }
    }
    return last;
}

// Returns Five if there is no straight
template <typename UnaryPredicate>
Wert ComputeStraightHighCard(UnaryPredicate p) {
    NumRange<int> values(0, 13);
    const auto foo = ::search_n(values.rbegin(), values.rend(), 5, p);
    return foo == values.rend() ? Five : Wert(*--foo.base());
}
} // namespace

Hand DetermineHand(const bool (&Karten)[4][13]) {
    NumRange<int> colors{0, 4};
    const auto flush_color =
        std::find_if(colors.begin(),
                     colors.end(),
                     [&Karten](const int i) {
                         return std::accumulate(std::begin(Karten[i]),
                                                std::end(Karten[i]),
                                                0,
                                                [](const int acc, const bool v) { return acc + v; }) >= 5;
                     }) -
        colors.begin();

    if (flush_color < 4) { // Flush vorhanden --> Check auf Straight Flush
        const Wert high = ComputeStraightHighCard([& cards = Karten[flush_color]](const int k) { return cards[k]; });

        if (high > Five) { // Straight Flush vorhanden --> BestHand auf diesen setzen
            Hand BestHand;

            BestHand.r = StraightFlush;
            for (int k = high; k > high - 5; --k) {
                BestHand.cards[high - k].col = Suit(flush_color);
                BestHand.cards[high - k].val = Wert(k);
            }
            return BestHand;
        } else if (Karten[flush_color][0] && Karten[flush_color][1] && Karten[flush_color][2] &&
                   Karten[flush_color][3] && Karten[flush_color][12]) { // Straight Flush, Ace to 5 vorhanden --> setzen
            Hand BestHand;

            BestHand.r = StraightFlush;
            BestHand.cards[4].col = Suit(flush_color);
            BestHand.cards[4].val = Wert(12);
            for (int k = 3; k > -1; --k) {
                BestHand.cards[3 - k].col = Suit(flush_color);
                BestHand.cards[3 - k].val = Wert(k);
            }
            return BestHand;
        } else { // Kein Straight Flush vorhanden --> BestHand auf den Flush setzen
            Hand BestHand;

            BestHand.r = Flush;
            int l = 0;
            int k = 12;
            do {
                if (Karten[flush_color][k]) {
                    BestHand.cards[l].col = Suit(flush_color);
                    BestHand.cards[l].val = Wert(k);
                    l++;
                }
                k--;
            } while (l < 5 && k > -1);
            return BestHand;
        }
    } else { // Kein Flush vorhanden --> Check auf Straight
        int CoCoaK[13] = {};

        for (int k = 0; k < 4; k++) {
            for (int i = 0; i < 13; i++) {
                CoCoaK[i] += Karten[k][i]; // count of cards of a kind
            }
        }
        const Wert high = ComputeStraightHighCard([&CoCoaK](const int k) { return CoCoaK[k]; });

        if (high > Five) { // Straight vorhanden --> BestHand auf diesen setzen
            Hand BestHand;

            BestHand.r = Straight;
            for (int i = high; i > high - 5; --i) {
                BestHand.cards[high - i].col =
                    Suit(0); // Farbe irrelevant --> vorerst zu faul Funktion zum Erkennen zu schreiben
                BestHand.cards[high - i].val = Wert(i);
            }
            return BestHand;
        } else if (CoCoaK[0] > 0 && CoCoaK[1] > 0 && CoCoaK[2] > 0 && CoCoaK[3] > 0 &&
                   CoCoaK[12] > 0) { // Straight, Ace to 5 vorhanden --> setzen
            Hand BestHand;

            BestHand.r = Straight;
            BestHand.cards[4].col = Suit(0); // Farbe irrelevant --> vorerst zu faul Funktion zum Erkennen zu schreiben
            BestHand.cards[4].val = Wert(12);
            for (int k = 3; k > -1; --k) {
                BestHand.cards[3 - k].col =
                    Suit(0); // Farbe irrelevant --> vorerst zu faul Funktion zum Erkennen zu schreiben
                BestHand.cards[3 - k].val = Wert(k);
            }
            return BestHand;
        } else { // Kein Straight vorhanden --> Check auf x Cards of a Kind
            const int max = *std::max_element(std::begin(CoCoaK), std::end(CoCoaK));
            switch (max) {
            case 1: // High Card
            {
                Hand BestHand;

                BestHand.r = HighCard;
                int i = 12;
                int k = 0;
                do {
                    if (CoCoaK[i]) {
                        BestHand.cards[k].col =
                            Suit(0); // Farbe irrelevant --> vorerst zu faul Funktion zum Erkennen zu schreiben
                        BestHand.cards[k].val = Wert(i);
                        k++;
                    }
                    i--;
                } while (k < 5 && i > -1);
                return BestHand;
            }
            case 2: // One Pair oder Two Pair?
            {
                const auto CoP = std::count_if(
                    std::begin(CoCoaK), std::end(CoCoaK), [](const int count) { return count == 2; }); // count of pairs
                Hand BestHand;

                if (CoP == 1) {
                    BestHand.r = OnePair;
                } else {
                    BestHand.r = TwoPair;
                }
                int i = 12;
                int k = 0;
                int l = 0;
                do {
                    if (CoCoaK[i] == 2 && (CoP == 1 && k == 0 || CoP > 1 && k < 4)) {
                        for (int c = k; c < k + 2; c++) {
                            BestHand.cards[c].col = Suit(0); // Farbe irrelevant --> vorerst zu faul
                                                             // Funktion zum Erkennen zu schreiben
                            BestHand.cards[c].val = Wert(i);
                        }
                        k = k + 2;
                    } else if (CoCoaK[i] == 1 && (CoP == 1 && l < 3 || CoP > 1 && l == 0)) {
                        BestHand.cards[CoP == 1 ? 2 + l : 4].col = Suit(0);
                        BestHand.cards[CoP == 1 ? 2 + l : 4].val = Wert(i);
                        l++;
                    }
                    i--;

                } while ((CoP == 1 && (!(k >= 1) || !(l >= 3)) && !(i <= -1)) ||
                         (CoP > 1 && (!(k >= 3) || !(l >= 1)) && !(i <= -1)));
                return BestHand;
            }
            case 3: // Three of a Kind oder Full House?
            {
                const auto c =
                    std::count_if(std::begin(CoCoaK), std::end(CoCoaK), [](const int count) { return count > 1; });
                if (c > 1) { // Full House vorhanden -->
                    Hand BestHand;

                    BestHand.r = FullHouse;
                    int i = 12;
                    bool found = false;
                    bool foundkicker = false;
                    do {
                        if (CoCoaK[i] == 3 && !found) {
                            for (int k = 0; k < 3; k++) {
                                BestHand.cards[k].col = Suit(0); // Farbe irrelevant --> vorerst zu faul
                                                                 // Funktion zum Erkennen zu schreiben
                                BestHand.cards[k].val = Wert(i);
                            }
                            found = true;
                        } else if (CoCoaK[i] >= 2 && !foundkicker) {
                            for (int k = 3; k < 5; k++) {
                                BestHand.cards[k].col = Suit(0); // Farbe irrelevant --> vorerst zu faul
                                                                 // Funktion zum Erkennen zu schreiben
                                BestHand.cards[k].val = Wert(i);
                            }
                            foundkicker = true;
                        }
                        i--;
                    } while ((!found || !foundkicker) && i > -1);
                    return BestHand;
                } else { // kein Full House
                    Hand BestHand;

                    BestHand.r = ThreeOfAKind;
                    int i = 12;
                    bool found = false;
                    int l = 0;
                    do {
                        if (CoCoaK[i] == 3 && !found) {
                            for (int k = 0; k < 3; k++) {
                                BestHand.cards[k].col = Suit(0); // Farbe irrelevant --> vorerst zu faul
                                                                 // Funktion zum Erkennen zu schreiben
                                BestHand.cards[k].val = Wert(i);
                            }
                            found = true;
                        } else if (CoCoaK[i] == 1 && l < 2) {
                            BestHand.cards[3 + l].col = Suit(0);
                            BestHand.cards[3 + l].val = Wert(i);
                            l++;
                        }
                        i--;

                    } while ((!found || l < 2) && i > -1);
                    return BestHand;
                }
            }
            default: {
                assert(max == 4); // Four of a Kind
                Hand BestHand;

                BestHand.r = FourOfAKind;
                int i = 12;
                bool found = false;
                bool foundkicker = false;
                do {
                    if (CoCoaK[i] == 4 && !found) {
                        for (int k = 0; k < 4; k++) {
                            BestHand.cards[k].col = Suit(k);
                            BestHand.cards[k].val = Wert(i);
                            found = true;
                        }
                    } else if (CoCoaK[i] == 1 && !foundkicker) {
                        BestHand.cards[4].col =
                            Suit(0); // Farbe irrelevant --> vorerst zu faul Funktion zum Erkennen zu schreiben
                        BestHand.cards[4].val = Wert(i);
                        foundkicker = true;
                    }
                    i--;
                } while ((!found || !foundkicker) && i > -1);
                return BestHand;
            }
            }
        }
    }
}

