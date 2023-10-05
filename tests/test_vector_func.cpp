#include <gtest/gtest.h>
#include "TestEntity.h"
#include "../includes/Vector.h"


class VectorFuncTest : public ::testing::Test {
protected:
    void SetUp() override {
        reference.reserve(100);
        vector.reserve(100);
        for (int i = first_elem; i < last_elem + 1; i++) {
            reference.emplace_back(i);
            vector.emplace_back(i);
        }
        TestEntity::clearCallHistory();
        ASSERT_EQ(reference.size(), vector.size());
    }

    std::vector<TestEntity> reference;
    rc::vector<TestEntity> vector;
    const rc::vector<TestEntity> &const_vector = vector;
    std::vector<TestEntityCall> reference_calls;
    std::vector<TestEntityCall> calls;
    TestEntity entity;

    const int first_elem = 0;
    const int last_elem = 11;
};

TEST_F(VectorFuncTest, push_back) {
    reference.push_back(42);  // NOLINT(modernize-use-emplace)
    reference.push_back(entity);
    reference_calls = TestEntity::getCallHistoryAndClean();

    vector.push_back(42);
    vector.push_back(entity);
    calls = TestEntity::getCallHistoryAndClean();

    ASSERT_EQ(reference_calls, calls);
    ASSERT_EQ(vector.size(), reference.size());

    displayCalls(reference_calls);
}

TEST_F(VectorFuncTest, push_back_many) {
    size_t previous_capacity = 0;
    for (int i = 0; i < 1024; ++i) {
        vector.push_back(1);

        if (vector.size() == vector.capacity()) {
            ASSERT_GE(vector.capacity(), previous_capacity) << "Capacity should have increased";
        }
        previous_capacity = vector.capacity();
    }
}


TEST_F(VectorFuncTest, emplace_back) {

    reference.emplace_back(42);
    reference.emplace_back(entity);
    reference_calls = TestEntity::getCallHistoryAndClean();

    vector.emplace_back(42);
    vector.emplace_back(entity);
    calls = TestEntity::getCallHistoryAndClean();

    ASSERT_EQ(reference_calls, calls);
    ASSERT_EQ(vector.size(), reference.size());

    displayCalls(reference_calls);
}

TEST_F(VectorFuncTest, emplace_back_many) {
    size_t previous_capacity = 0;
    for (int i = 0; i < 1024; ++i) {
        vector.emplace_back(1);

        if (vector.size() == vector.capacity()) {
            ASSERT_GE(vector.capacity(), previous_capacity) << "Capacity should have increased";
        }
        previous_capacity = vector.capacity();
    }
}

TEST_F(VectorFuncTest, clear) {

    reference.clear();
    reference_calls = TestEntity::getCallHistoryAndClean();

    vector.clear();
    calls = TestEntity::getCallHistoryAndClean();

    ASSERT_EQ(reference_calls, calls);
    ASSERT_EQ(vector.size(), 0);
    ASSERT_GT(vector.capacity(), 0) << "Clear should clear content without changing the capacity";

    displayCalls(reference_calls);
}


TEST_F(VectorFuncTest, empty) {
    ASSERT_FALSE(vector.empty());
    ASSERT_NE(vector.size(), 0);
    vector.clear();
    ASSERT_TRUE(vector.empty());
    ASSERT_EQ(vector.size(), 0);
}

TEST_F(VectorFuncTest, op_brackets) {
    for (int i = first_elem; i < last_elem + 1; i++) {
        EXPECT_EQ(vector[i], i);
    }
}

TEST_F(VectorFuncTest, at) {
    for (int i = first_elem; i < last_elem + 1; i++) {
        EXPECT_EQ(vector.at(i), i); // See SetUp()
    }
    ASSERT_THROW(vector.at(last_elem + 1), std::out_of_range);

}


TEST_F(VectorFuncTest, const_at) {
    for (int i = first_elem; i < last_elem + 1; i++) {
        EXPECT_EQ(const_vector.at(i), i); // See SetUp()
    }
    ASSERT_THROW(vector.at(last_elem + 1), std::out_of_range);

}

TEST_F(VectorFuncTest, front_and_back) {
    ASSERT_EQ(vector.front(), first_elem);
    ASSERT_EQ(vector.back(), last_elem);
}

TEST_F(VectorFuncTest, const_front_and_back) {
    ASSERT_EQ(const_vector.front(), first_elem);
    ASSERT_EQ(const_vector.back(), last_elem);
}

TEST_F(VectorFuncTest, reserve) {
    size_t prev_size = vector.size();
    vector.reserve(1024);
    ASSERT_EQ(vector.size(), prev_size) << "reserve() shouln't change size";
    ASSERT_EQ(vector.capacity(), 1024) << "reserve() should set capacity";
    vector.reserve(8);
    ASSERT_EQ(vector.capacity(), 1024) << "reserve() shouldn't decrease capacity";
}


TEST_F(VectorFuncTest, pop_back) {
    reference.pop_back();
    reference_calls = TestEntity::getCallHistoryAndClean();

    vector.pop_back();
    calls = TestEntity::getCallHistoryAndClean();

    ASSERT_EQ(reference_calls, calls);
    ASSERT_EQ(vector.size(), reference.size());

    displayCalls(reference_calls);
}

TEST_F(VectorFuncTest, data) {
    for (int i = first_elem; i < last_elem + 1; i++) {
        ASSERT_EQ(vector.data()[i], i);
    }
    ASSERT_EQ(&vector.front(), vector.data());

    vector.clear();
    ASSERT_EQ(vector.data(), nullptr);
}

TEST_F(VectorFuncTest, const_data) {
    for (int i = first_elem; i < last_elem + 1; i++) {
        ASSERT_EQ(const_vector.data()[i], i);
    }
    ASSERT_EQ(&const_vector.front(), const_vector.data());

    const rc::vector<TestEntity> empty_const_vector;
    ASSERT_EQ(empty_const_vector.data(), nullptr);
}