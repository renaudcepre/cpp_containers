//
// Created by Renaud Cepre on 2/27/21.
//

#pragma once

#include <cstddef> // for size_t type
#include <stdexcept>
#include <new>
#include "VectorIterator.h"
#include "ReverseIterator.h"
#include "Utility.h"
#include "Allocator.h"

namespace rc {
    template<typename T, typename Alloc = rc::allocator<T>>
    class vector {
    public:
        using difference_type = ptrdiff_t;

        using iterator = vector_iterator<T>;
        using reverse_iterator = ReverseIterator<iterator>;

        using const_iterator = vector_iterator<const T>;
        using const_reverse_iterator = ReverseIterator<const_iterator>;

    private:
        size_t _capacity = 0;
        size_t _size = 0;
        T *_data = nullptr;
    public:
        vector() = default;

        vector(vector const &other);

        vector(vector &&other) noexcept;

        vector &operator=(vector const &other);

        vector &operator=(vector &&other) noexcept;

        vector(std::initializer_list<T> list);

        ~vector();

    public:

        //      CAPACITY

        //  Returns the number of elements
        [[nodiscard]] size_t size() const noexcept;

        // Returns the number of elements that can be held in currently allocated storage
        [[nodiscard]] size_t capacity() const noexcept;

        // increase the capacity of the vector to a value that's greater or equal to new_cap.
        void reserve(size_t new_cap);

        [[nodiscard]] bool empty() const;

        //      ELEMENT ACCESS

        // Access the last and first element
        T &back();

        const T &back() const;

        T &front();

        const T &front() const;

        // access specified element with bounds checking
        T &at(difference_type i);

        const T &at(difference_type i) const;

        // Access specified element
        const T &operator[](difference_type pos) const;

        T &operator[](difference_type pos);

        // Direct access to the underlying array
        T *data() noexcept;

        const T *data() const noexcept;

        //      MODIFIERS

        // Adds an element to the end
        void push_back(const T &value);

        void push_back(T &&value);

        // Removes the last element
        void pop_back();

        // Constructs an element in-place at the end
        template<typename... Args>
        T &emplace_back(Args &&... args);

        // Inserts a new element into the container directly before pos.
        template<typename... Args>
        iterator emplace(iterator pos, Args &&... args);

        // Inserts elements at the specified pos in the container.
        template<typename IT>
        iterator insert(iterator pos, IT first, IT last);

        iterator insert(iterator pos, size_t count, const T &value);

        iterator insert(iterator pos, const T &value);

        // Changes the number of elements stored
        void resize(size_t count, T value = T());

        // Clears the contents
        void clear() noexcept;

    private:
        void _grow();

        void _realloc(size_t new_capacity);

        void _move(size_t end_dist, size_t begin_dist, size_t count) {
            for (size_t i = end_dist; i; --i) {
                size_t idx = begin_dist + i - 1;
                new(static_cast<void *>(_data + idx + count ))T(std::move(_data[idx]));
            }
        }

    public:
        // BEGIN
        iterator begin() noexcept { return iterator(_data); }

        const_iterator begin() const noexcept { return const_iterator(_data); }

        const_iterator cbegin() const noexcept { return const_iterator(_data); }

        // END
        iterator end() noexcept { return iterator(_data + _size); }

        const_iterator end() const noexcept { return const_iterator(_data + _size); }

        const_iterator cend() const noexcept { return const_iterator(_data + _size); }

        // REVERSE BEGIN
        reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }

        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

        // REVERSE END
        reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }

        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }
    };

    //              IMPLEMENTATIONS

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(std::initializer_list<T> list) {
        reserve(list.size());
        for (auto &el: list)
            push_back(std::move(el));
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(const vector<T, Alloc> &other) {
        reserve(other.size());
        for (size_t i = 0; i < other.size(); ++i)
            push_back(other[i]);
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(vector<T, Alloc> &&other) noexcept {
        _capacity = other._capacity;
        _size = other._size;
        _data = other._data;
        other._data = nullptr;
        other._capacity = 0;
        other._size = 0;
    }

    template<typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(const vector<T, Alloc> &other) {
        if (this != &other) {
            clear();
            ::operator delete(_data, sizeof(T) * _capacity);
            _size = 0;
            _capacity = 0;

            reserve(other.size());
            for (size_t i = 0; i < other.size(); ++i)
                push_back(other[i]);
        }
        return *this;
    }

    template<typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(vector &&other) noexcept {
        if (this != &other) {
            clear();
            ::operator delete(_data, sizeof(T) * _capacity);

            _capacity = other._capacity;
            _size = other._size;
            _data = other._data;
            other._data = nullptr;
            other._capacity = 0;
            other._size = 0;
        }
        return *this;
    }

    //      CAPACITY

    template<typename T, typename Alloc>
    size_t vector<T, Alloc>::size() const noexcept {
        return _size;
    }

    template<typename T, typename Alloc>
    size_t vector<T, Alloc>::capacity() const noexcept {
        return _capacity;
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::reserve(size_t new_cap) {
        if (new_cap > _capacity)
            _realloc(new_cap);
    }

    template<typename T, typename Alloc>
    bool vector<T, Alloc>::empty() const {
        return (_size == 0);
    }

    //      ELEMENT ACCESS

    template<typename T, typename Alloc>
    T &vector<T, Alloc>::operator[](difference_type pos) {
        return _data[pos];
    }

    template<typename T, typename Alloc>
    const T &vector<T, Alloc>::operator[](difference_type pos) const {
        return _data[pos];
    }

    template<typename T, typename Alloc>
    T &vector<T, Alloc>::at(difference_type i) {
        if (i >= _size)
            throw std::out_of_range("index out of bounds");

        return _data[i];
    }

    template<typename T, typename Alloc>
    const T &vector<T, Alloc>::at(difference_type i) const {
        if (i >= _size)
            throw std::out_of_range("index out of bounds");

        return _data[i];
    }

    template<typename T, typename Alloc>
    T &vector<T, Alloc>::back() {
        return _data[_size - 1];
    }

    template<typename T, typename Alloc>
    const T &vector<T, Alloc>::back() const {
        return _data[_size - 1];
    }

    template<typename T, typename Alloc>
    T &vector<T, Alloc>::front() {
        return _data[0];
    }

    template<typename T, typename Alloc>
    const T &vector<T, Alloc>::front() const {
        return _data[0];
    }

    template<typename T, typename Alloc>
    const T *vector<T, Alloc>::data() const noexcept {
        if (_size == 0)
            return nullptr;
        return _data;
    }

    template<typename T, typename Alloc>
    T *vector<T, Alloc>::data() noexcept {
        if (_size == 0)
            return nullptr;
        return _data;
    }


    //      MODIFIERS

    template<typename T, typename Alloc>
    vector<T, Alloc>::~vector() {
        clear();
        ::operator delete(_data, sizeof(T) * _capacity);
    }

    template<typename T, typename Alloc>
    template<typename... Args>
    T &vector<T, Alloc>::emplace_back(Args &&... args) {
        if (_size >= _capacity)
            _grow();

        // This version call the operator=(&&), wich is not relevant:
        // _first[_size] = T(std::forward<Args>(args)...);

        //This version construct the object directly in _first, like std::vector does :
        new(static_cast<void *>(_data + _size))T(std::forward<Args>(args)...);
        return _data[_size++];
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::push_back(T &&value) {
        if (_size >= _capacity)
            _grow();

        new(static_cast<void *>(_data + _size++))T(std::move(value));
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::push_back(const T &value) {
        if (_size >= _capacity)
            _grow();

        new(static_cast<void *>(_data + _size++))T(value);
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::pop_back() {
        _data[--_size].~T();
    }


    template<typename T, typename Alloc>
    void vector<T, Alloc>::clear() noexcept {
        for (size_t i = 0; i < _size; ++i)
            _data[i].~T();
        _size = 0;
    }

    //      PRIVATE
    template<typename T, typename Alloc>
    void vector<T, Alloc>::_realloc(size_t new_capacity) {
        Alloc alloc;

        if (_size == 0) {
            _data = alloc.allocate(new_capacity);
            _capacity = new_capacity;
            return;
        }

        T* tmp = alloc.allocate(new_capacity);

        if (new_capacity < _size) {
            _size = new_capacity;
        }

        for (size_t i = 0; i < _size; ++i) {
            std::allocator_traits<Alloc>::construct(alloc, tmp + i, std::move(_data[i]));
            std::allocator_traits<Alloc>::destroy(alloc, _data + i);
        }

        alloc.deallocate(_data, _capacity);

        _data = tmp;
        _capacity = new_capacity;
    }


    template<typename T, typename Alloc>
    void vector<T, Alloc>::_grow() {
        if (_size == 0)
            _realloc(2);
        else
            _realloc(_capacity + _capacity / 2);
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::resize(size_t count, T value) {
        if (_size > count) {
            _size = count;
        } else if (_size < count) {
            _realloc(count);
            for (size_t i = _size; i < count; ++i)
                new(static_cast<void *>(_data + i))T(value);
            _size = count;
        }
    }

    template<typename T, typename Alloc>
    template<typename... Args>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::emplace(const vector::iterator pos, Args &&... args) {
        size_t end_dist = distance(pos, end());
        size_t begin_dist = distance(begin(), pos);

        _realloc(_size + 1);
        _size += 1;

        _move(end_dist, begin_dist, 1);

        new(static_cast<void *>(_data + begin_dist))T(std::forward<Args>(args)...);

        return begin() + begin_dist;
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const vector::iterator pos, const T &value) {
        return insert(pos, 1, value);
    }

    template<typename T, typename Alloc>
    template<typename IT>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const vector::iterator pos, IT first, IT last) {
        size_t count = rc::distance(first, last);
        if (count == 0) // avoids unnecessary calls to distance() ...
            return pos;

        size_t end_dist = distance(pos, end());
        size_t begin_dist = distance(begin(), pos);

        _realloc(_size + count);
        _size += count;

        // moves all elements after `pos` count times.
        _move(end_dist, begin_dist, count);

        for (size_t i = 0; i < count; ++i)
            new(static_cast<void *>(_data + begin_dist + i))T(*first++);

        return begin() + begin_dist;
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const vector::iterator pos, size_t count, const T &value) {
        if (count == 0) // avoids unnecessary calls to distance() ...
            return pos;

        size_t end_dist = distance(pos, end());
        size_t begin_dist = distance(begin(), pos);

        _realloc(_size + count);
        _size += count;

        // moves all elements after `pos` count times.
        _move(end_dist, begin_dist, count);

        for (size_t i = 0; i < count; ++i)
            new(static_cast<void *>(_data + begin_dist + i))T(value);

        return begin() + begin_dist;
    }
}
