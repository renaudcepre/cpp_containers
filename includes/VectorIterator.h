//
// Created by rcepre on 3/8/21.
//

#pragma once

#include "Utility.h"

/**
 *  Ce template prend u container en parametre. ce container doit avoir certains types declares, commme value_type.
 *
 * @tparam CONTAINER est le conteneur depuis lequel l'iterateur va deduire ses types, notamment value_type.
 */

namespace rc {
    template<typename T>
    class vector_iterator {
    public:
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using const_pointer = value_type const *;
        using reference = value_type &;
        using const_reference = value_type const &;
        using iterator_category = rc::random_access_iterator_tag;

    private:
        pointer _ptr;

    public:
        vector_iterator() : _ptr(nullptr) {}

        explicit vector_iterator(pointer ptr) : _ptr(ptr) {}

        vector_iterator(vector_iterator const &other) = default;

        vector_iterator &operator=(vector_iterator const &other) = default;

        ~vector_iterator() = default;

    public:
        // POINTER

        reference operator*() { return *_ptr; }

        const_reference operator*() const { return *_ptr; }

        pointer operator->() { return _ptr; }

        const_pointer operator->() const { return _ptr; }

        // INCREMENT / DECREMENT

        // +
        vector_iterator &operator++() {
            ++_ptr;
            return *this;
        }

        vector_iterator operator++(int) {
            vector_iterator cpy(*this);
            ++_ptr;
            return cpy;
        }

        vector_iterator &operator--() {
            --_ptr;
            return *this;
        }

        // -
        vector_iterator operator--(int) {
            vector_iterator cpy(*this);
            --_ptr;
            return cpy;
        }

        vector_iterator &operator-=(const difference_type i) {
            _ptr -= i;
            return *this;
        }

        vector_iterator &operator+=(const difference_type i) {
            _ptr += i;
            return *this;
        }

        // ARITHMETIC

        // it = begin() - scalar;
        vector_iterator operator-(const difference_type i) const {
            return vector_iterator(_ptr - i);
        }

        // diff = begin() - it;
        template<typename U>
        friend typename vector_iterator<U>::difference_type
        operator-(const vector_iterator<U> &lhs, const vector_iterator<U> &rhs);


        // it = begin() + scalar;
        vector_iterator operator+(const difference_type i) const {
            return vector_iterator(_ptr + i);
        }

        // it = scalar + it;
        template<typename U>
        friend vector_iterator<U>
        operator+(typename vector_iterator<U>::difference_type i, const vector_iterator<U> &rhs);


        // ACCESS ELEMENTS
        reference operator[](difference_type val) { return (*(_ptr + val)); }

        const_reference operator[](difference_type val) const { return (*(_ptr + val)); }

        // COMPARE
        bool operator==(const vector_iterator &rhs) const { return this->_ptr == rhs._ptr; }

        bool operator!=(const vector_iterator &rhs) const { return this->_ptr != rhs._ptr; }

        bool operator<(const vector_iterator &rhs) const { return this->_ptr < rhs._ptr; }

        bool operator<=(const vector_iterator &rhs) const { return this->_ptr <= rhs._ptr; }

        bool operator>(const vector_iterator &rhs) const { return this->_ptr > rhs._ptr; }

        bool operator>=(const vector_iterator &rhs) const { return this->_ptr >= rhs._ptr; }
    };

    template<typename U>
    typename vector_iterator<U>::difference_type
    operator-(const vector_iterator<U> &lhs, const vector_iterator<U> &rhs) {
        return lhs._ptr - rhs._ptr;
    }


    template<typename U>
    vector_iterator<U>
    operator+(typename vector_iterator<U>::difference_type i, const vector_iterator<U> &rhs) {
        return rhs._ptr + i;
    }
}