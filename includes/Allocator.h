//
// Created by rcepre on 05/10/23.
//

#pragma once

#include <cstddef>

namespace rc {
    template<typename T>
    class allocator {
    public:
        using value_type = T;

        allocator() = default;

        T *allocate(std::size_t n) {
            // Use ::operator new to allocate raw memory for 'n' elements of type 'T'.
            // Note that this raw memory allocation does NOT call the constructor of 'T'.
            return static_cast<T *>(::operator new(n * sizeof(T)));
        }

        void deallocate(T *p, std::size_t n) {
            ::operator delete(p, n * sizeof(T));
        }
    };
}
