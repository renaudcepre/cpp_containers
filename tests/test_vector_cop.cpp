#include <gtest/gtest.h>
#include "TestEntity.h"
#include "../includes/Vector.h"


class VectorCopTest : public ::testing::Test {
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
    std::vector<TestEntityCall> reference_calls;
    std::vector<TestEntityCall> calls;
    TestEntity entity;

    const int first_elem = 0;
    const int last_elem = 11;
};

TEST_F(VectorCopTest, copy) {
    std::vector ref_cpy(reference);
    reference_calls = TestEntity::getCallHistoryAndClean();

    rc::vector cpy(vector);
    calls = TestEntity::getCallHistoryAndClean();

    ASSERT_EQ(reference_calls, calls);

    displayCalls(reference_calls);
}

TEST_F(VectorCopTest, assign) {
    std::vector<TestEntity> ref_cpy;

    ref_cpy = reference;
    reference_calls = TestEntity::getCallHistoryAndClean();

    rc::vector<TestEntity> cpy;
    cpy = vector;
    calls = TestEntity::getCallHistoryAndClean();

    ASSERT_EQ(reference_calls, calls);
    ASSERT_EQ(ref_cpy.size(), cpy.size());

    displayCalls(reference_calls);

    ref_cpy = std::vector<TestEntity>({42, 1});
    reference_calls = TestEntity::getCallHistoryAndClean();

    cpy = rc::vector<TestEntity>({42, 1});
    calls = TestEntity::getCallHistoryAndClean();

    ASSERT_EQ(reference_calls, calls);
    ASSERT_EQ(ref_cpy.size(), cpy.size());

    displayCalls(reference_calls);
}


TEST_F(VectorCopTest, mv_assign) {
    std::vector<TestEntity> ref_cpy{42, 42, 42};
    rc::vector<TestEntity> cpy{42, 42, 42};
    TestEntity::clearCallHistory();

    ref_cpy = std::move(reference);
    reference_calls = TestEntity::getCallHistoryAndClean();

    cpy = std::move(vector);
    calls = TestEntity::getCallHistoryAndClean();

    ASSERT_EQ(reference_calls, calls);
    ASSERT_EQ(ref_cpy.size(), cpy.size());
    ASSERT_EQ(cpy.size(), last_elem + 1);
    ASSERT_EQ(vector.size(), 0);
    ASSERT_EQ(vector.capacity(), 0);
    ASSERT_EQ(vector.data(), nullptr);

    displayCalls(reference_calls);
}


