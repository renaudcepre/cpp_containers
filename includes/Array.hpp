
#pragma once

#include <cstddef> // for size_t type
#include <stdexcept>
#include "ReverseIterator.h"

namespace rc {
    template<typename T, size_t SIZE>
    class array {
    public:
        using value_type = T;
        using difference_type = ptrdiff_t;
        using iterator = value_type *;
        using const_iterator = const value_type *;
        using reverse_iterator = ReverseIterator<iterator>;
        using const_reverse_iterator = ReverseIterator<const_iterator>;

    public:
        T _data[SIZE]; //data is public for allowing aggregate initialization.
    public:

        // return the number of elements.
        [[nodiscard]]
        constexpr size_t size() const;

        // access specified element
        T &operator[](difference_type i);

        const T &operator[](difference_type i) const;

        // fill the container with specified value
        void fill(T const &value);

        // access specified element with bounds checking
        T &at(difference_type i);

        const T &at(difference_type i) const;

        // access the first and last elements
        T &front();

        const T &front() const;

        T &back();

        const T &back() const;

        // direct access to the underlying array
        T *data();

        const T *data() const;

    public:
        // Iterators

        iterator begin() { return iterator(_data); }

        const_iterator begin() const { return const_iterator(_data); }

        const_iterator cbegin() const { return const_iterator(_data); }


        iterator end() { return iterator(_data + SIZE); }

        const_iterator end() const { return const_iterator(_data + SIZE); }

        const_iterator cend() const { return const_iterator(_data + SIZE); }


        reverse_iterator rbegin() { return reverse_iterator(end()); }

        const_reverse_iterator rbegin() const { return const_reverse_iterator(cend()); }

        const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }


        reverse_iterator rend() { return reverse_iterator(begin()); }

        const_reverse_iterator rend() const { return const_reverse_iterator(cbegin()); }

        const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }
    };


    template<typename T, size_t SIZE>
    [[maybe_unused]]
    void array<T, SIZE>::fill(const T &value) {
        for (size_t i = 0; i < SIZE; ++i)
            _data[i] = value;
    }

    template<typename T, size_t SIZE>
    [[maybe_unused]]
    const T &array<T, SIZE>::at(difference_type i) const {
        if (i >= SIZE)
            throw std::out_of_range("index out of bounds");
        return _data[i];
    }

    template<typename T, size_t SIZE>
    [[maybe_unused]]
    T &array<T, SIZE>::at(difference_type i) {
        if (i >= SIZE)
            throw std::out_of_range("index out of bounds");
        return _data[i];
    }

    template<typename T, size_t SIZE>
    T &array<T, SIZE>::front() {
        return _data[0];
    }

    template<typename T, size_t SIZE>
    T &array<T, SIZE>::operator[](difference_type i) {
        return _data[i];
    }

    template<typename T, size_t SIZE>
    constexpr size_t array<T, SIZE>::size() const {
        return SIZE;
    }

    template<typename T, size_t SIZE>
    const T &array<T, SIZE>::operator[](difference_type i) const {
        return _data[i];
    }

    template<typename T, size_t SIZE>
    const T &array<T, SIZE>::front() const {
        return _data[0];
    }

    template<typename T, size_t SIZE>
    const T *array<T, SIZE>::data() const {
        return _data;
    }

    template<typename T, size_t SIZE>
    const T &array<T, SIZE>::back() const {
        return _data[SIZE - 1];
    }

    template<typename T, size_t SIZE>
    T *array<T, SIZE>::data() {
        return _data;
    }

    template<typename T, size_t SIZE>
    T &array<T, SIZE>::back() {
        return _data[SIZE - 1];
    }
}