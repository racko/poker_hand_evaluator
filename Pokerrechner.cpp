#include "card.h"
#include "deck.h"
#include "polymorphic_iterator.h"
#include <iostream>

class CardReader : public PolymorphicIterator<const Karte> {
  public:
    CardReader(std::istream& in, const int cards_read = 7) : in_{in}, cards_read_{cards_read} { read(); }

  private:
    void read() {
        if (cards_read_ == 7) {
            return;
        }
        std::cerr << "Please enter card " << cards_read_ << ": ";
        in_ >> card_;
    }

    void step() override {
        ++cards_read_;
        read();
    }
    const Karte& peek() override { return card_; }
    const Karte& peek() const override { return card_; }
    bool equals(const PolymorphicIterator<const Karte>& rhs) const override {
        const auto x = dynamic_cast<const CardReader*>(&rhs);
        if (x == nullptr) {
            return false;
        }
        return &x->in_ == &in_ && x->cards_read_ == cards_read_;
    }

  private:
    std::istream& in_;
    int cards_read_;
    Karte card_;
};

int main() {
    CardReader cards{std::cin, 0};
    CardReader stop{std::cin, 7};

    std::cerr << "run\n";
    run(cards, stop, std::cerr);

    return 0;
}
