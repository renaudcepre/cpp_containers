//
// Created by rcepre on 3/8/21.
//

#pragma once
namespace rc {

// Pair

    template<typename T1, typename T2>
    struct Pair {
        T1 first;
        T2 second;
    };

// Iterator tags ()

    struct input_iterator_tag {
    };
    struct output_iterator_tag {
    };
    struct forward_iterator_tag : public input_iterator_tag {
    };
    struct bidirectional_iterator_tag : public forward_iterator_tag {
    };
    struct random_access_iterator_tag : public bidirectional_iterator_tag {
    };

// Iterator traits

    template<typename IT>
    struct iterator_traits {
        using difference_type = typename IT::difference_type;
        using value_type = typename IT::value_type;
        using pointer = typename IT::pointer;
        using reference = typename IT::reference;
        using iterator_category = typename IT::iterator_category;
    };

// Need to specialize iterator_traits for array, since array iterator is just a pointer.
    template<typename T>
    struct iterator_traits<T *> {
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator_category = random_access_iterator_tag;
    };

    /**
     * @return the difference between two iterators as a `difference_type` type.
     */
    template<typename IT>
    typename iterator_traits<IT>::difference_type distance(IT begin, IT end) {
        return _distance(begin, end, typename iterator_traits<IT>::iterator_category());
    }

    template<typename IT>
    typename iterator_traits<IT>::difference_type _distance(IT begin, IT end, random_access_iterator_tag) {
        return end - begin;
    }

    template<typename IT>
    typename iterator_traits<IT>::difference_type _distance(IT begin, IT end, input_iterator_tag) {
        typename iterator_traits<IT>::difference_type distance = 0;
        while (begin != end)
            distance++;
        return distance;
    }

}