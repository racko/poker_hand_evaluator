#pragma once

#include <array>
#include <iterator>

template <typename T, std::ptrdiff_t Extent = -1>
class span {
  public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using index_type = std::ptrdiff_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // TODO: This overload only participates in overload resolution if extent == 0 || extent == std::dynamic_extent
    constexpr span() noexcept = default;

    constexpr span(pointer ptr, index_type count) : data_{ptr}, stop_{ptr + count} {}

    constexpr span(pointer first, pointer last) : data_{first}, stop_{last} {}

    // TODO: These overloads only participate in overload resolution if extent == std::dynamic_extent || N == extent is
    // true and std::remove_pointer_t<decltype(std::data(arr))>(*)[] is convertible to element_type (*)[]
    template <std::size_t N>
    constexpr span(element_type (&arr)[N]) noexcept : data_{arr}, stop_{arr + N} {}
    template <std::size_t N>
    constexpr span(std::array<value_type, N>& arr) noexcept : data_{arr}, stop_{arr + N} {}
    template <std::size_t N>
    constexpr span(const std::array<value_type, N>& arr) noexcept : data_{arr}, stop_{arr + N} {}

    // TODO: These overloads only participate in overload resolution if
    //       - Container is not a specialization of std::span, a specialization of std::array, or an array type;
    //       - std::data(cont) and std::size(cont) are both well-formed; and
    //       - std::remove_pointer_t<decltype(std::data(cont))>(*)[] is convertible to element_type (*)[]
    template <class Container>
    constexpr span(Container& cont) : data_{cont.data()}, stop_{cont.data() + cont.size()} {}
    template <class Container>
    constexpr span(const Container& cont) : data_{cont.data()}, stop_{cont.data() + cont.size()} {}

    // TODO: This overload only participates in overload resolution if extent == std::dynamic_extent || N == extent is true and U (*)[] is convertible to element_type (*)[]
    template <class U, std::size_t N>
    constexpr span(const span<U, N>& s) noexcept : data_{s.begin()}, stop_{s.end()} {}

    constexpr iterator begin() const { return data_; }
    constexpr iterator end() const { return stop_; }
    constexpr const_iterator cbegin() const { return data_; }
    constexpr const_iterator cend() const { return stop_; }

    constexpr index_type size() const { return stop_ - data_; }
    constexpr bool empty() const { return data_ == stop_; }

    constexpr reference front() const { assert(!empty()); return data_[0]; }
    constexpr reference back() const { assert(!empty()); return *(stop_ - 1); }
    constexpr reference operator[](index_type idx) const { return data_[idx]; }
    constexpr pointer data() const { return data_; }

  private:
    pointer data_ = nullptr;
    pointer stop_ = nullptr;
};
