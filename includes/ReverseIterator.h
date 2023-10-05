//
// Created by rcepre on 3/9/21.
//

#pragma once

#include "Utility.h"

/**
 *
 * @tparam IT represente l'iterateur a renverser.
 */

template<typename IT>
class ReverseIterator {
private:
    IT _source;

public:
    using difference_type = typename rc::iterator_traits<IT>::difference_type;
    using value_type = typename rc::iterator_traits<IT>::value_type;
    using pointer = typename rc::iterator_traits<IT>::pointer;
    using const_pointer = typename rc::iterator_traits<IT>::pointer;
    using reference = typename rc::iterator_traits<IT>::reference;
    using const_reference = typename rc::iterator_traits<IT>::reference;
    using iterator_category = typename rc::iterator_traits<IT>::iterator_category;

public:
    ReverseIterator(IT source) : _source(source) {}

    ReverseIterator(ReverseIterator const &o) = default;

    virtual ~ReverseIterator() = default;

    ReverseIterator() = delete;

    ReverseIterator<IT> &operator=(const ReverseIterator<IT> &o) {
        _source = o._source;
        return *this;
    }

public:
    // POINTER
    reference operator*() const {
        // Since we instanciate this iterator with the end of the source of another,
        // we need to decrement the pointer for find the latest element of the iterated container.
        IT it(_source - 1);
        return *it;
    }

    pointer operator->() const {
        // Since we instanciate this iterator with the end of the source of another,
        // we need to decrement the pointer for find the latest element of the iterated container.
        IT it(_source - 1);
        return it.operator->();
    }

    // INCREMENT / DECREMENT
    ReverseIterator &operator++() {
        --_source;
        return *this;
    }

    ReverseIterator &operator++(int) { return ReverseIterator<IT>(_source--); }

    ReverseIterator &operator--() {
        ++_source;
        return *this;
    }

    ReverseIterator &operator--(int) {
        return ReverseIterator<IT>(_source++);
    }


    ReverseIterator &operator+=(const difference_type i) {
        _source -= i;
        return *this;
    }

    ReverseIterator &operator-=(const difference_type i) {
        _source += i;
        return *this;
    }

    // ARITHMETIC

    // it = it + scalar
    ReverseIterator operator+(const difference_type i) { return ReverseIterator<IT>(_source - i); }

    // it = scalar + it;
    template<typename U>
    friend ReverseIterator<U>
    operator+(typename ReverseIterator<U>::difference_type i, const ReverseIterator<U> &rhs);

    // it = it - scalar;
    ReverseIterator operator-(const difference_type i) { return ReverseIterator<IT>(_source + i); }

    // diff = it - it;
    template<typename T>
    friend difference_type operator-(const ReverseIterator<T> &lhs, const ReverseIterator<T> &rhs);

    // ACCESS
    reference operator[](difference_type i) { return (*(_source - i - 1)); }

    const_reference operator[](difference_type i) const { return (*(_source - i - 1)); }

    // COMPARE
    bool operator==(const ReverseIterator &rhs) const { return this->_source == rhs._source; }

    bool operator!=(const ReverseIterator &rhs) const { return this->_source != rhs._source; }

    bool operator<(const ReverseIterator &rhs) const { return this->_source > rhs._source; }

    bool operator<=(const ReverseIterator &rhs) const { return this->_source >= rhs._source; }

    bool operator>(const ReverseIterator &rhs) const { return this->_source < rhs._source; }

    bool operator>=(const ReverseIterator &rhs) const { return this->_source <= rhs._source; }

};

template<typename T>
ptrdiff_t operator-(const ReverseIterator<T> &lhs, const ReverseIterator<T> &rhs) {
    return rhs._source - lhs._source;
}

template<typename U>
ReverseIterator<U>
operator+(typename ReverseIterator<U>::difference_type i, const ReverseIterator<U> &rhs) {
    return rhs._source - i;
}