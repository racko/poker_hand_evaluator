#include "deck.h"

#include "NumRange.h" // for NumIterator
#include "card.h"     // for Karte, Wert, operator<<, Suit, Four
#include "determine_hand.h"
#include "hand.h"                 // for Hand, compare, operator<<, HighCard
#include "polymorphic_iterator.h" // for PolymorphicIterator
#include "span.h"                 // for span
#include <algorithm>              // for find, find_if, shuffle, sort, tran...
#include <cassert>                // for assert
#include <iostream>               // for operator<<, basic_ostream, basic_o...
#include <iterator>               // for back_insert_iterator, back_inserter
#include <memory>                 // for unique_ptr, make_unique, allocator...
#include <numeric>                // for accumulate
#include <random>                 // for mt19937_64
#include <utility>                // for pair, make_pair
#include <vector>                 // for vector, vector<>::iterator

std::array<Karte, 52> make_sorted_deck() {
    std::array<Karte, 52> deck;

    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 13; k++) {
            int n = i * 13 + k;
            deck[n] = {Suit(i), Wert(k)};
        }
    }

    return deck;
}

namespace {
// class State {
//  public:
//    State(int player_count, std::mt19937_64& rand); // TODO: player_count can decrease from phase to phase
//    ~State();
//
//    span<const Karte, 5> GetCommunityCards() const;
//
//    span<const Karte, 2> GetHoleCards(const int i) const;
//
//    void Shuffle();
//
//    Hand DetermineHand(const int i) const;
//
//    void Step(PolymorphicIterator<const Karte>& cards);
//
//    int GetPlayerCount() const;
//
//  private:
//    struct impl;
//    std::unique_ptr<impl> pimpl_;
//};

const std::array<Karte, 52> Deck{make_sorted_deck()};

const Karte leer{EmptyFarbe, EmptyWert};

template <typename ForwardIt, typename T>
bool contains(const ForwardIt first, const ForwardIt last, const T& value) {
    return std::find(first, last, value) != last;
}

template <typename ForwardIt, typename T>
void swap_to_position(const ForwardIt first, const ForwardIt last, const ForwardIt destination, const T& needle) {
    const auto source = std::find(first, last, needle);
    std::iter_swap(source, destination);
}

struct impl {
    impl(const int player_count, std::mt19937_64& rand) : player_count_{player_count}, rand_{rand} {}

    span<Karte, 5> GetCommunityCards() { return span<Karte, 5>(deck.begin() + 2, deck.begin() + 7); }

    span<Karte, 2> GetHoleCards(const int i) {
        return i == 0 ? span<Karte, 2>(deck.begin(), deck.begin() + 2)
                      : span<Karte, 2>(deck.begin() + 5 + 2 * i, deck.begin() + 7 + 2 * i);
    }

    span<const Karte, 5> GetCommunityCards() const { return span<const Karte, 5>(deck.begin() + 2, deck.begin() + 7); }

    span<const Karte, 2> GetHoleCards(const int i) const {
        if (i == 0) {
            return span<const Karte, 2>(deck.begin(), deck.begin() + 2);
        }
        return span<const Karte, 2>(deck.begin() + 5 + 2 * i, deck.begin() + 7 + 2 * i);
    }

    void Shuffle() {
        std::shuffle(first_unknown_card_, deck.end(), rand_);
        // assert(check());
    }

    Hand DetermineHand(const int pc) const {
        bool Karten[4][13] = {};

        for (const auto c : GetHoleCards(pc)) {
            Karten[c.col][c.val] = true;
        }

        for (const auto c : GetCommunityCards()) {
            Karten[c.col][c.val] = true;
        }

        return ::DetermineHand(Karten);
    }

    void SwapToPosition(Karte* destination, const Karte needle) {
        swap_to_position(deck.begin(), deck.end(), destination, needle);
    }

    void StartPreFlop(PolymorphicIterator<const Karte>& cards) {
        std::cerr << __PRETTY_FUNCTION__ << '\n';
        const auto cs = GetHoleCards(0).begin();
        SwapToPosition(cs, *cards);
        ++cards;
        SwapToPosition(cs + 1, *cards);
        ++cards;
        first_unknown_card_ += 2;
    }

    void StartFlop(PolymorphicIterator<const Karte>& cards) {
        std::cerr << __PRETTY_FUNCTION__ << '\n';
        const auto cs = GetCommunityCards().begin();
        SwapToPosition(cs, *cards);
        ++cards;
        SwapToPosition(cs + 1, *cards);
        ++cards;
        SwapToPosition(cs + 2, *cards);
        ++cards;
        first_unknown_card_ += 3;
    }

    void StartTurn(PolymorphicIterator<const Karte>& cards) {
        std::cerr << __PRETTY_FUNCTION__ << '\n';
        const auto cs = GetCommunityCards().begin();
        SwapToPosition(cs + 3, *cards);
        ++cards;
        ++first_unknown_card_;
    }

    void StartRiver(PolymorphicIterator<const Karte>& cards) {
        std::cerr << __PRETTY_FUNCTION__ << '\n';
        const auto cs = GetCommunityCards().begin();
        SwapToPosition(cs + 4, *cards);
        ++cards;
        ++first_unknown_card_;
    }

    void Step(PolymorphicIterator<const Karte>& cards) {
        const auto known_cards = first_unknown_card_ - deck.data();
        assert(known_cards < 7);
        switch (known_cards) {
        case 0:
            StartPreFlop(cards);
            break;
        case 2:
            StartFlop(cards);
            break;
        case 5:
            StartTurn(cards);
            break;
        case 6:
            StartRiver(cards);
            break;
        }
    }

    bool check() {
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 13; k++) {
                if (std::find(deck.begin(), deck.end(), Karte(Suit(i), Wert(k))) == deck.end()) {
                    // std::cout << Wert_Strings[k] << " of " << Farbe_Strings[i] << " nicht gefunden." << std::endl;
                    return false;
                }
            }
        }
        return true;
    }

    int GetPlayerCount() const { return player_count_; }

    int player_count_;
    std::array<Karte, 52> deck{Deck};
    Karte* first_unknown_card_{deck.data()};
    std::mt19937_64& rand_;
};

using State = impl;

// State::State(const int player_count, std::mt19937_64& rand) : pimpl_{std::make_unique<impl>(player_count, rand)} {}
// State::~State() = default;
//
// span<const Karte, 5> State::GetCommunityCards() const { return pimpl_->GetCommunityCards(); }
//
// span<const Karte, 2> State::GetHoleCards(const int i) const { return pimpl_->GetHoleCards(i); }
//
// void State::Shuffle() { return pimpl_->shuffle(); }
//
// Hand State::DetermineHand(const int i) const { return pimpl_->DetermineHand(i); }
//
// void State::Step(PolymorphicIterator<const Karte>& cards) { pimpl_->Step(cards); }
//
// int State::GetPlayerCount() const { return pimpl_->GetPlayerCount(); }

void log(const State& state,
         const span<const std::pair<int, Hand>, -1> best_hands,
         const std::ptrdiff_t count_of_winners,
         const double share,
         std::ostream& logger) {
    const auto& community_cards = state.GetCommunityCards();
    logger << "Community Cards: [" << community_cards[0] << " " << community_cards[1] << " " << community_cards[2]
           << " " << community_cards[3] << " " << community_cards[4] << "]" << std::endl;

    for (const auto& hand : best_hands) {
        const auto pc = hand.first;
        if (pc == 0) {
            logger << "You: [";
        } else {
            logger << "Player " << pc + 1 << ": [";
        }
        const auto& hole_cards = state.GetHoleCards(pc);
        logger << hole_cards[0] << " " << hole_cards[1] << "]: " << hand.second.r << std::endl;
    }

    logger << "Best hands:\n";
    for (const auto& hand : best_hands) {
        logger << hand.first << " with " << hand.second << '\n';
    }

    logger << "Player(s) ";
    for (auto i = 0; i < count_of_winners; ++i) {
        logger << best_hands[i].first << ' ';
    }
    logger << "won\n";

    logger << "Your share: " << share << '\n';
}

void DetermineBestHands(State& state, std::vector<std::pair<int, Hand>>& best_hands) {
    const auto pn = state.GetPlayerCount();
    best_hands.clear();
    std::transform(NumIterator<int>(0), NumIterator<int>(pn), std::back_inserter(best_hands), [&state](const int pc) {
        return std::make_pair(pc, state.DetermineHand(pc));
    });
}

void SortBestHands(std::vector<std::pair<int, Hand>>& best_hands) {
    std::sort(
        best_hands.begin(), best_hands.end(), [](const std::pair<int, Hand>& lhs, const std::pair<int, Hand>& rhs) {
            return compare(lhs.second, rhs.second) == -1; // lhs > rhs -> sorts descending
        });
}

std::ptrdiff_t ComputeCountOfWinners(std::vector<std::pair<int, Hand>>& best_hands) {
    return std::find_if(best_hands.begin() + 1,
                        best_hands.end(),
                        [&best0 = best_hands[0].second](const std::pair<int, Hand>& x) {
                            return compare(best0, x.second) != 0;
                        }) -
           best_hands.begin();
}

std::ptrdiff_t DetermineYourPlace(std::vector<std::pair<int, Hand>>& best_hands) {
    return std::find_if(
               best_hands.begin(), best_hands.end(), [](const std::pair<int, Hand>& hand) { return hand.first == 0; }) -
           best_hands.begin();
}

double ComputeYourShare(const std::ptrdiff_t count_of_winners, const std::ptrdiff_t your_place) {
    return your_place < count_of_winners ? 1.0 / count_of_winners : 0.0;
}

double evaluate(State& state, std::vector<std::pair<int, Hand>>& best_hands, std::ostream& logger) {
    DetermineBestHands(state, best_hands);
    SortBestHands(best_hands);
    const auto count_of_winners = ComputeCountOfWinners(best_hands);
    const auto your_place = DetermineYourPlace(best_hands);
    const double share = ComputeYourShare(count_of_winners, your_place);

    log(state, best_hands, count_of_winners, share, logger);

    return share;
}

double simulate(State& state, std::vector<std::pair<int, Hand>>& best_hands, std::ostream& logger) {
    state.Shuffle();
    return evaluate(state, best_hands, logger);
}

double simulate(State& state, std::vector<std::pair<int, Hand>>& best_hands, const int count, std::ostream& logger) {
    return std::accumulate(NumIterator<int>(0), NumIterator<int>(count), 0.0, [&](const double acc, const int) {
        return acc + simulate(state, best_hands, logger);
    });
}
} // namespace

void run(PolymorphicIterator<const Karte>& cards, PolymorphicIterator<const Karte>& stop, std::ostream& logger) {
    const int pn = 2;

    std::mt19937_64 foo;
    State state{pn, foo};

    std::vector<std::pair<int, Hand>> best_hands; // declared here to avoid reallocations
    best_hands.reserve(pn);

    int step = 0;
    do {
        state.Step(cards);
        const auto cov = simulate(state, best_hands, 10000, logger);
        logger << cov << " von 10000 Simulationen gewonnen (" << double(cov) / double(100) << "%)" << std::endl;

        // logger << "Pott: ";
        // double pott;
        // std::cin >> pott;

        // for (int betters = pn - 1; betters > 0; betters--) {
        //    logger << betters << " Mitbieter - Maximaler Einsatz";

        //    // Given an initial pott of 'pott', we estimate the value of the following two outcomes:
        //    // 1. bet x and win
        //    // 2. bet x and lose
        //    // If we bet x and #betters other players call, we may increase our stack by pott + betters * x with
        //    // probability cov / 10000. On the other hand we may lose x with probability 1 - cov / 10000.
        //    //

        //    if (10000.0 / cov < betters) {
        //        logger << " beliebig!" << std::endl;
        //    } else {
        //        logger << ": " << pott / (10000.0 / cov - betters) << std::endl;
        //    }
        //}
        ++step;
        std::cerr << "Done with step " << step << '\n';
    } while (cards != stop);
    // std::memset(mixed_Deck,52,sizeof(mixed_Deck));

    /*
    float Testwerte[52][52];
    std::memset(Testwerte, 0, sizeof(Testwerte));
    for(i=0;i<52;i++)
            Testwerte[i][mixed_Deck[i].col * 13 + mixed_Deck[i].val]++;

    // Bewertung der Stichprobe

    // logger << "Die Karten wurden 24346 Mal gemischt!" << std::endl;

    double lbound = double(1)/double(52)-double(3)*pow(double(1241646)/double(2704),double(0.5))/double(24346);
    double ubound = double(1)/double(52)+double(3)*pow(double(1241646)/double(2704),double(0.5))/double(24346);
    for(i=0;i<52;i++){
            for(k=0;k<52;k++){
                    if(Testwerte[i][k]/24346 < lbound || Testwerte[i][k]/24346 > ubound)
                            // logger << "Hochsignifikante Abweichung entdeckt: " << Testwerte[i][k] << std::endl;
            }
    } */

    /*

     // gemischtes Deck auf Vollständigkeit testen

    // logger << "gemischtes Deck wird auf Vollstaendigkeit getestet!" << std::endl;
    for(i=0;i<4;i++){
            for(int k=0;k<13;k++){
                    found = false;
                    l=0;
                    do {
                            if(mixed_Deck[l].val != k || mixed_Deck[l].col != i)
                                    found |= false;
                            else
                                    found = true;
                            l++;
                    } while(found != true && l < 53);
                    if(found != true && l == 53)
                            // logger << Wert_Strings[k] << " of " << Farbe_Strings[i] << " nicht gefunden." <<
    std::endl;
            }
    } */
}
