//
// Created by rcepre on 3/16/21.
//

#pragma once

#include "ListIterator.h"
#include "ReverseIterator.h"
#include <cassert>

namespace rc {
    template<typename T>
    class list {
    public:
        struct Node {
            T data;
            Node *next;
            Node *prev;

            Node() : prev(nullptr), next(nullptr) {};

            Node(Node const &other) = default;

            Node(T value) : data(value), prev(nullptr), next(nullptr) {}

            Node(T value, Node *prev, Node *next) : data(value), prev(prev), next(next) {}

            Node(Node *prev, Node *next) : prev(prev), next(next) {}

            ~Node() = default;
        };

    public:
        using iterator = list_iterator<T>;
        using reverse_iterator = ReverseIterator<iterator>;
        using const_iterator = list_const_iterator<const T>;
        using const_reverse_iterator = ReverseIterator<const_iterator>;

    private:
        Node *_first = nullptr;
        Node *_last = nullptr;
        size_t _size = 0;

    public:
        list();

        list(list const &other);

        list(list &&other);

        list &operator=(list const &other);

        list &operator=(list &&other);

        list(std::initializer_list<T> init);

        ~list();

    public:

        void clear();

        void push_back(const T &value);

        void push_back(T &&value);

        void push_front(const T &value);

        void push_front(T &&value);

        void pop_front();

        void pop_back();

        void resize(size_t count, T value = T());

        iterator erase(iterator first, iterator last);

        template<typename IT>
        iterator insert(const iterator pos, IT first, IT last);

        iterator insert(const iterator pos, size_t count, const T &value);

        iterator insert(const iterator pos, std::initializer_list<T> ilist);

        iterator insert(const iterator pos, const T &value);

        iterator insert(const iterator position, T &&value);

        size_t size() const;

        bool empty() const;

        T &front();

        T const &front() const;

        T &back();

        T const &back() const;

    public:
        // BEGIN
        iterator begin() noexcept { return iterator(_first); }

        const_iterator begin() const noexcept {
            return const_iterator(_first);
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(_first);
        }

        // END
        iterator end() noexcept { return iterator(_last); }

        const_iterator end() const noexcept {
//            typedef typename list<const T>::Node const_node;
//            return const_iterator(reinterpret_cast<const_node *>(_last));
            return const_iterator(_last);
        }

        const_iterator cend() const noexcept {
//            typedef typename list<const T>::Node const_node;
//            return const_iterator(reinterpret_cast<const_node *>(_last));
            return const_iterator(_last);
        }

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

    template<typename T>
    list<T>::list() : _size(0) {
        // This node represent the end of the list.
        _first = _last = new Node();
    }

    template<typename T>
    list<T>::list(const list &other) {
        _first = _last = new Node();
        insert(begin(), other.begin(), other.end());
    }

    template<typename T>
    list<T>::~list() {
        clear();
        delete _last;
    }

    // CAPACITY

    template<typename T>
    size_t list<T>::size() const { return _size; }

    template<typename T>
    bool list<T>::empty() const { return _size == 0; }

    template<typename T>
    T &list<T>::front() { return _first->data; }

    template<typename T>
    T &list<T>::back() { return _last->prev->data; }

    template<typename T>
    T const &list<T>::front() const { return _first->data; }

    template<typename T>
    T const &list<T>::back() const { return _last->prev->data; }

    // MODIFIERS
    template<typename T>
    typename list<T>::iterator list<T>::insert(const list::iterator position, T &&value) {

        // Creates a new node that takes its place between the element preceding the one pointed to by the "pos"
        //      iterator and the one pointed to by the iterator.
        Node *tmp = new Node(std::move(value), position._node->prev, position._node);

        if (tmp->prev)
            tmp->prev->next = tmp;
        else
            _first = tmp;

        tmp->next->prev = tmp;
        ++_size;
        return iterator(tmp);
    }

    template<typename T>
    typename list<T>::iterator list<T>::insert(const list::iterator pos, const T &value) {

        // Creates a new node that takes its place between the element preceding the one pointed to by the "pos"
        //      iterator and the one pointed to by the iterator.
        Node *tmp = new Node(value, pos._node->prev, pos._node);

        if (tmp->prev)
            tmp->prev->next = tmp;
        else
            _first = tmp;

        tmp->next->prev = tmp;
        ++_size;
        return iterator(tmp);
    }

    template<typename T>
    template<typename IT>
    typename list<T>::iterator list<T>::insert(const list::iterator pos, IT first, IT last) {
        if (first == last)
            return pos;

        Node *prev = pos._node->prev;
        Node *first_inserted = prev;

        // each elements to insert before pos:
        for (IT it = first; it != last; ++it) {
            ++_size;
            // creating a new node linked with the previous one
            Node *tmp = new Node(*it, prev, nullptr);

            // them link the previous to the new one
            if (prev)
                prev->next = tmp;
            else
                _first = tmp;
            prev = tmp;
        }

        // link the last inserted element to pos
        pos._node->prev = prev;
        if (prev)
            prev->next = pos._node;

        return first_inserted ? first_inserted : begin();
    }

    template<typename T>
    typename list<T>::iterator list<T>::insert(const list::iterator pos, std::initializer_list<T> ilist) {
        return insert(pos, ilist.begin(), ilist.end());
    }

    template<typename T>
    typename list<T>::iterator list<T>::insert(const list::iterator pos, size_t count, const T &value) {
        if (count == 0)
            return pos;

        Node *prev = pos._node->prev;
        Node *first_inserted = prev;

        // Pour chauque element a inserer AVANT pos:
        for (int i = 0; i < count; ++i) {
            // on cree un nouveau Node, avec le contenu de l'it, et en prev l'element avant pos.
            Node *tmp = new Node(value, prev, nullptr);

            // Si on a un element avant, on l'accroche, sinon, celui ci est le premier.
            if (prev)
                prev->next = tmp;
            else
                _first = tmp;
            prev = tmp;
        }

        // On attache le dernier ajoute avant pos
        pos._node->prev = prev;
        // Et inversement
        if (prev)
            prev->next = pos._node;
        _size += count;

        return first_inserted;
    }

    template<typename T>
    typename list<T>::iterator list<T>::erase(list::iterator first, list::iterator last) {
        iterator runner = first;
        Node *first_prev_node = first._node->prev; //save the ptr, cause it will be deleted ...

        while (runner != last) {
            iterator tmp(runner);
            --_size;
            ++runner;
            delete tmp._node; // ... here, then accessed ...
        }
        if (first_prev_node != nullptr) // ...here .
            first_prev_node->next = last._node;
        else
            _first = last._node;

        last._node->prev = first_prev_node;

        return iterator(last._node);
    }

    template<typename T>
    void list<T>::pop_back() {
        --_size;
        Node *tmp = _last->prev;
        _last->prev = _last->prev->prev;
        _last->prev->next = _last;
        delete tmp;
    }

    template<typename T>
    void list<T>::pop_front() {
        --_size;
        _first = _first->next;
        delete _first->prev;
        _first->prev = nullptr;
    }

    template<typename T>
    void list<T>::push_front(T &&value) {
        insert(begin(), std::move(value));
    }

    template<typename T>
    void list<T>::push_front(const T &value) {
        insert(begin(), value);
    }

    template<typename T>
    void list<T>::push_back(T &&value) {
        // insert() insert element BEFORE the iterator passed as parameter.
        insert(end(), std::move(value));
    }

    template<typename T>
    void list<T>::push_back(const T &value) {
        // insert() insert element BEFORE the iterator passed as parameter.
        insert(end(), value);
    }

    template<typename T>
    void list<T>::clear() {
        erase(begin(), end());
    }


}