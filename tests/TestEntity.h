//
// Created by Renaud Cepre on 2/28/21.
//

#pragma once

#include <vector>
#include <memory>

typedef enum {
    CTOR, CTORVAL, U_DTOR, DTOR, CPYCTOR, MOVCTOR, ASSIGN, MOVASSIGN
} TestEntityCall;

inline std::string callToStr(TestEntityCall call) {
    switch (call) {

        case CTOR:
            return "CTOR";
        case CTORVAL:
            return "CTORVAL";
        case U_DTOR:
            return "U_DTOR";
        case DTOR:
            return "DTOR";
        case CPYCTOR:
            return "CPYCTOR";
        case MOVCTOR:
            return "MOVCTOR";
        case ASSIGN:
            return "ASSIGN";
        case MOVASSIGN:
            return "MOVASSIGN";
    }
    return "";
}

inline void displayCalls(const std::vector<TestEntityCall> &calls) {
    for (const auto &call: calls) {
        std::cout << callToStr(call) << " - ";
    }
    std::cout << std::endl;
}


class TestEntity {
private:
    inline static std::vector<TestEntityCall> callHistory{};

public:
    std::unique_ptr<int> ptr;

public:
    TestEntity() : ptr(std::make_unique<int>(42)) {
        callHistory.push_back(CTOR);
    }

#pragma ide diagnostic ignored "google-explicit-constructor"

    TestEntity(int val) : ptr(std::make_unique<int>(val)) {
        callHistory.push_back(CTORVAL);
    }

    TestEntity(TestEntity const &other) : ptr(std::make_unique<int>(*other.ptr)) {
        callHistory.push_back(CPYCTOR);
    }

    TestEntity(TestEntity &&other) noexcept: ptr(std::move(other.ptr)) {
        callHistory.push_back(MOVCTOR);
    }

    TestEntity &operator=(TestEntity const &other) {
        // Do not use copy and swap idiom for keep callHistory clear.
        // (Avoid to add a CPYCTOR call to the history)
        callHistory.push_back(ASSIGN);

        ptr = std::make_unique<int>(*other.ptr);
        return *this;
    }

    TestEntity &operator=(TestEntity &&other) noexcept {
        callHistory.push_back(MOVASSIGN);
        ptr = std::move(other.ptr);
        return *this;
    }

    ~TestEntity() {
        if (ptr)
            callHistory.push_back(DTOR);
        else
            callHistory.push_back(U_DTOR);
    }

    static std::vector<TestEntityCall> getCallHistory() {
        return TestEntity::callHistory;
    }

    static std::vector<TestEntityCall> getCallHistoryAndClean() {
        auto tmp = TestEntity::callHistory;
        TestEntity::clearCallHistory();
        return tmp;
    }

    static void clearCallHistory() {
        callHistory.clear();
    }

    bool operator==(const TestEntity &rhs) const {
        return *ptr == *rhs.ptr;
    }

    bool operator!=(const TestEntity &rhs) const {
        return !(rhs == *this);
    }


    friend std::ostream &operator<<(std::ostream &o, TestEntity const &entity) {
        o << *entity.ptr;
        return o;
    }
};


