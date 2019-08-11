#include "hand.h"

#include <ostream>

namespace {
const char* Rank_Strings[] = {"HighCard", "OnePair",   "TwoPair",     "ThreeOfAKind", "Straight",
                              "Flush",    "FullHouse", "FourOfAKind", "StraightFlush"};
}

bool operator==(const Hand& lhs, const Hand& rhs) {
    return lhs.r == rhs.r && lhs.cards == rhs.cards;
}

std::ostream& operator<<(std::ostream& s, const Rank rank) {
    return s << Rank_Strings[rank];
}

std::ostream& operator<<(std::ostream& s, const Hand hand) {
    s << hand.r << " [";
    for (const Karte card : hand.cards) {
        s << card << ',';
    }
    return s << ']';
}

int compare(const Hand& lhs, const Hand& rhs) {
    if (rhs.r < lhs.r) {
        return -1;
    }

    if (rhs.r > lhs.r) {
        return 1;
    }

    int i;
    switch (rhs.r) {
    case HighCard: // High Card
        i = 0;
        do {
            if (rhs.cards[i].val > lhs.cards[i].val) {
                return 1;
            } else if (rhs.cards[i].val < lhs.cards[i].val) {
                return -1;
            }
            i++;
        } while (i < 5);
        break;
    case OnePair: // One Pair
        if (rhs.cards[0].val > lhs.cards[0].val) {
            return 1;
        } else if (rhs.cards[0].val < lhs.cards[0].val) {
            return -1;
        } else {
            i = 2;
            do {
                if (rhs.cards[i].val > lhs.cards[i].val) {
                    return 1;
                } else if (rhs.cards[i].val < lhs.cards[i].val) {
                    return -1;
                }
                i++;
            } while (i < 5);
        }
        break;
    case TwoPair: // Two Pair
        if (rhs.cards[0].val > lhs.cards[0].val) {
            return 1;
        } else if (rhs.cards[0].val < lhs.cards[0].val) {
            return -1;
        } else {
            if (rhs.cards[2].val > lhs.cards[2].val) {
                return 1;
            } else if (rhs.cards[2].val < lhs.cards[2].val) {
                return -1;
            } else {
                if (rhs.cards[4].val > lhs.cards[4].val) {
                    return 1;
                } else if (rhs.cards[4].val < lhs.cards[4].val) {
                    return -1;
                }
            }
        }
        break;
    case ThreeOfAKind: // Three of a Kind
        if (rhs.cards[0].val > lhs.cards[0].val) {
            return 1;
        } else if (rhs.cards[0].val < lhs.cards[0].val) {
            return -1;
        } else {
            i = 3;
            do {
                if (rhs.cards[i].val > lhs.cards[i].val) {
                    return 1;
                } else if (rhs.cards[i].val < lhs.cards[i].val) {
                    return -1;
                }
                i++;
            } while (i < 5);
        }
        break;
    case Straight: // Straight
        if (rhs.cards[0].val > lhs.cards[0].val) {
            return 1;
        } else if (rhs.cards[0].val < lhs.cards[0].val) {
            return -1;
        }
        break;
    case Flush: // Flush
        i = 0;
        do {
            if (rhs.cards[i].val > lhs.cards[i].val) {
                return 1;
            } else if (rhs.cards[i].val < lhs.cards[i].val) {
                return -1;
            }
            i++;
        } while (i < 5);
        break;
    case FullHouse: // Full House
        if (rhs.cards[0].val > lhs.cards[0].val) {
            return 1;
        } else if (rhs.cards[0].val < lhs.cards[0].val) {
            return -1;
        } else {
            if (rhs.cards[3].val > lhs.cards[3].val) {
                return 1;
            } else if (rhs.cards[3].val < lhs.cards[3].val) {
                return -1;
            }
        }
        break;
    case FourOfAKind: // Four of a Kind
        if (rhs.cards[0].val > lhs.cards[0].val) {
            return 1;
        } else if (rhs.cards[0].val < lhs.cards[0].val) {
            return -1;
        } else {
            if (rhs.cards[4].val > lhs.cards[4].val) {
                return 1;
            } else if (rhs.cards[4].val < lhs.cards[4].val) {
                return -1;
            }
        }
        break;
    case StraightFlush: // Straight Flush
        if (rhs.cards[0].val > lhs.cards[0].val) {
            return 1;
        } else if (rhs.cards[0].val < lhs.cards[0].val) {
            return -1;
        }
        break;
    }
    return 0;
}

