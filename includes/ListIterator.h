//
// Created by rcepre on 3/16/21.
//
#pragma once

#include "Utility.h"

namespace rc {
    template<typename T>
    class list;

    template<typename T>
    class list_iterator {
        // list<> must have access to the _node private member.
        template<typename _T>
        friend
        class list;

    public:
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using const_pointer = value_type const *;
        using reference = value_type &;
        using const_reference = value_type const &;
        using iterator_category = bidirectional_iterator_tag;

    private:
        using Node = typename list<T>::Node;
        Node *_node;

    private:

    public:
        list_iterator(Node *node) : _node(node) {}

        list_iterator() : _node(nullptr) {}

        list_iterator(list_iterator const &other) = default;

        list_iterator &operator=(list_iterator const &other) = default;

        virtual ~list_iterator() = default;

    public:
        reference operator*() { return _node->data; }

        const_reference operator*() const { return _node->data; }

        pointer operator->() { return &_node->data; }

        const_pointer operator->() const { return &_node->data; }

        list_iterator &operator++() {
            _node = _node->next;
            return *this;
        }

        list_iterator operator++(int) {
            list_iterator cpy(*this);
            _node = _node->next;
            return cpy;
        }

        list_iterator &operator--() {
            _node = _node->prev;
            return *this;
        }

        list_iterator operator--(int) {
            list_iterator cpy(*this);
            _node = _node->prev;
            return cpy;
        }

        // COMPARE
        bool operator==(const list_iterator &rhs) const { return this->_node == rhs._node; }

        bool operator!=(const list_iterator &rhs) const { return this->_node != rhs._node; }

        bool operator<(const list_iterator &rhs) const { return this->_node < rhs._node; }

        bool operator<=(const list_iterator &rhs) const { return this->_node <= rhs._node; }

        bool operator>(const list_iterator &rhs) const { return this->_node > rhs._node; }

        bool operator>=(const list_iterator &rhs) const { return this->_node >= rhs._node; }
    };

    template<typename T>
    class list_const_iterator {
        // list<> must have access to the _node private member.
        template<typename _T>
        friend
        class list;

    public:
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using const_pointer = value_type const *;
        using reference = value_type &;
        using const_reference = value_type const &;
        using iterator_category = bidirectional_iterator_tag;

    private:
        using Node = typename list<T>::Node;
        Node *_node;

    private:

    public:
        list_const_iterator(Node *node) : _node(node) {}

        list_const_iterator() : _node(nullptr) {}

        list_const_iterator(list_const_iterator const &other) = default;

        list_const_iterator &operator=(list_const_iterator const &other) = default;

        virtual ~list_const_iterator() = default;

    public:
        reference operator*() { return _node->data; }

        const_reference operator*() const { return _node->data; }

        pointer operator->() { return &_node->data; }

        const_pointer operator->() const { return &_node->data; }

        list_const_iterator &operator++() {
            _node = _node->next;
            return *this;
        }

        list_const_iterator operator++(int) {
            list_const_iterator cpy(*this);
            _node = _node->next;
            return cpy;
        }

        list_const_iterator &operator--() {
            _node = _node->prev;
            return *this;
        }

        list_const_iterator operator--(int) {
            list_const_iterator cpy(*this);
            _node = _node->prev;
            return cpy;
        }

        // COMPARE
        bool operator==(const list_const_iterator &rhs) const { return this->_node == rhs._node; }

        bool operator!=(const list_const_iterator &rhs) const { return this->_node != rhs._node; }

        bool operator<(const list_const_iterator &rhs) const { return this->_node < rhs._node; }

        bool operator<=(const list_const_iterator &rhs) const { return this->_node <= rhs._node; }

        bool operator>(const list_const_iterator &rhs) const { return this->_node > rhs._node; }

        bool operator>=(const list_const_iterator &rhs) const { return this->_node >= rhs._node; }
    };
}
