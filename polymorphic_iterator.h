#pragma once

template <typename T>
struct PolymorphicIterator {
  public:
    T& operator*() { return peek(); }
    const T& operator*() const { return peek(); }
    PolymorphicIterator& operator++() {
        step();
        return *this;
    }
    bool operator==(const PolymorphicIterator& rhs) const { return equals(rhs); }
    bool operator!=(const PolymorphicIterator& rhs) const { return !equals(rhs); }

    virtual ~PolymorphicIterator() = default;

  private:
    virtual void step() = 0;
    virtual T& peek() = 0;
    virtual const T& peek() const = 0;
    virtual bool equals(const PolymorphicIterator& rhs) const = 0;
};

template <typename T>
class ContiguousIterator : public PolymorphicIterator<T> {
  public:
    ContiguousIterator(T* x) : x_{x} {}

  private:
    void step() override { ++x_; }
    T& peek() override { return *x_; }
    const T& peek() const override { return *x_; }
    bool equals(const PolymorphicIterator<T>& rhs) const override {
        const auto x = dynamic_cast<const ContiguousIterator*>(&rhs);
        if (x == nullptr) {
            return false;
        }
        return x->x_ == x_;
    }

  private:
    T* x_;
};
