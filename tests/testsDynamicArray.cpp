#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include "../classes/headers/DynamicArray.h"

class DynamicArrayIntTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(DynamicArrayIntTest, ConstructorWithArray_ValidInput) {
    int source[] = {1, 2, 3, 4, 5};
    DynamicArray<int> arr(source, 5);

    EXPECT_EQ(arr.GetSize(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(arr.Get(i), source[i]);
    }
}

TEST_F(DynamicArrayIntTest, ConstructorWithArray_Nullptr_Throws) {
    EXPECT_THROW(
        DynamicArray<int> arr(nullptr, 5),
        std::invalid_argument
    );
}

TEST_F(DynamicArrayIntTest, ConstructorWithSize_DefaultInitialization) {
    DynamicArray<int> arr(10);

    EXPECT_EQ(arr.GetSize(), 10);
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(arr.Get(i), int{});
    }
}

TEST_F(DynamicArrayIntTest, ConstructorWithZeroSize) {
    DynamicArray<int> arr(0);
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST_F(DynamicArrayIntTest, CopyConstructor_DeepCopy) {
    int source[] = {10, 20, 30};
    DynamicArray<int> original(source, 3);
    DynamicArray<int> copy(original);

    EXPECT_EQ(copy.GetSize(), original.GetSize());
    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(copy.Get(i), original.Get(i));
    }

    copy.Set(0, 999);
    EXPECT_EQ(original.Get(0), 10);
    EXPECT_EQ(copy.Get(0), 999);
}

TEST_F(DynamicArrayIntTest, AssignmentOperator_NormalCase) {
    int source1[] = {1, 2, 3};
    int source2[] = {4, 5, 6, 7};

    DynamicArray<int> arr1(source1, 3);
    DynamicArray<int> arr2(source2, 4);

    arr1 = arr2;

    EXPECT_EQ(arr1.GetSize(), 4);
    for (size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(arr1.Get(i), source2[i]);
    }
    EXPECT_EQ(arr2.GetSize(), 4);
}

TEST_F(DynamicArrayIntTest, AssignmentOperator_SelfAssignment) {
    int source[] = {100, 200, 300};
    DynamicArray<int> arr(source, 3);

    arr = arr;

    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), 100);
    EXPECT_EQ(arr.Get(1), 200);
    EXPECT_EQ(arr.Get(2), 300);
}

TEST_F(DynamicArrayIntTest, Get_ValidIndex) {
    int source[] = {5, 10, 15};
    DynamicArray<int> arr(source, 3);

    EXPECT_EQ(arr.Get(0), 5);
    EXPECT_EQ(arr.Get(1), 10);
    EXPECT_EQ(arr.Get(2), 15);
}

TEST_F(DynamicArrayIntTest, Get_IndexOutOfRange_Lower) {
    DynamicArray<int> arr(5);
    EXPECT_THROW(arr.Get(5), std::out_of_range);
}

TEST_F(DynamicArrayIntTest, Get_IndexOutOfRange_Upper) {
    DynamicArray<int> arr(3);
    EXPECT_THROW(arr.Get(100), std::out_of_range);
}

TEST_F(DynamicArrayIntTest, GetSize_ConstCorrectness) {
    const DynamicArray<int> arr(7);
    EXPECT_EQ(arr.GetSize(), 7);
}

TEST_F(DynamicArrayIntTest, Set_ValidIndex) {
    DynamicArray<int> arr(3);

    arr.Set(0, 42);
    arr.Set(1, 84);
    arr.Set(2, 126);

    EXPECT_EQ(arr.Get(0), 42);
    EXPECT_EQ(arr.Get(1), 84);
    EXPECT_EQ(arr.Get(2), 126);
}

TEST_F(DynamicArrayIntTest, Set_IndexOutOfRange) {
    DynamicArray<int> arr(2);

    EXPECT_THROW(arr.Set(2, 100), std::out_of_range);
    EXPECT_THROW(arr.Set(10, 200), std::out_of_range);

    EXPECT_EQ(arr.Get(0), int{});
    EXPECT_EQ(arr.Get(1), int{});
}

TEST_F(DynamicArrayIntTest, Resize_IncreaseSize) {
    int source[] = {1, 2, 3};
    DynamicArray<int> arr(source, 3);

    arr.Resize(5);

    EXPECT_EQ(arr.GetSize(), 5);
    EXPECT_EQ(arr.Get(0), 1);
    EXPECT_EQ(arr.Get(1), 2);
    EXPECT_EQ(arr.Get(2), 3);
    EXPECT_EQ(arr.Get(3), int{});
    EXPECT_EQ(arr.Get(4), int{});
}

TEST_F(DynamicArrayIntTest, Resize_DecreaseSize) {
    int source[] = {10, 20, 30, 40, 50};
    DynamicArray<int> arr(source, 5);

    arr.Resize(3);

    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
    EXPECT_EQ(arr.Get(2), 30);
}

TEST_F(DynamicArrayIntTest, Resize_ToZero) {
    int source[] = {1, 2, 3};
    DynamicArray<int> arr(source, 3);

    arr.Resize(0);

    EXPECT_EQ(arr.GetSize(), 0);
    EXPECT_THROW(arr.Get(0), std::out_of_range);
}

TEST_F(DynamicArrayIntTest, Resize_SameSize) {
    int source[] = {7, 8, 9};
    DynamicArray<int> arr(source, 3);

    arr.Resize(3);

    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), 7);
    EXPECT_EQ(arr.Get(1), 8);
    EXPECT_EQ(arr.Get(2), 9);
}

class DynamicArrayStringTest : public ::testing::Test {
};

TEST_F(DynamicArrayStringTest, ConstructorWithArray_String) {
    std::string source[] = {"hello", "world", "gtest"};
    DynamicArray<std::string> arr(source, 3);

    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), "hello");
    EXPECT_EQ(arr.Get(1), "world");
    EXPECT_EQ(arr.Get(2), "gtest");
}

TEST_F(DynamicArrayStringTest, CopyConstructor_String_DeepCopy) {
    std::string source[] = {"alpha", "beta"};
    DynamicArray<std::string> original(source, 2);
    DynamicArray<std::string> copy(original);

    copy.Set(0, "modified");

    EXPECT_EQ(original.Get(0), "alpha");
    EXPECT_EQ(copy.Get(0), "modified");
}

TEST_F(DynamicArrayStringTest, Resize_String_PreserveData) {
    std::string source[] = {"one", "two"};
    DynamicArray<std::string> arr(source, 2);

    arr.Resize(4);

    EXPECT_EQ(arr.Get(0), "one");
    EXPECT_EQ(arr.Get(1), "two");
    EXPECT_EQ(arr.Get(2), std::string{});
    EXPECT_EQ(arr.Get(3), std::string{});
}

struct TestStruct {
    int value;
    std::string name;

    TestStruct() : value(0), name("") {
    }

    TestStruct(int v, std::string n) : value(v), name(std::move(n)) {
    }

    bool operator==(const TestStruct &other) const {
        return value == other.value && name == other.name;
    }
};

class DynamicArrayStructTest : public ::testing::Test {
};

TEST_F(DynamicArrayStructTest, ConstructorAndAccess_CustomType) {
    TestStruct source[] = {
        TestStruct(1, "first"),
        TestStruct(2, "second")
    };

    DynamicArray<TestStruct> arr(source, 2);

    EXPECT_EQ(arr.GetSize(), 2);
    EXPECT_EQ(arr.Get(0).value, 1);
    EXPECT_EQ(arr.Get(0).name, "first");
    EXPECT_EQ(arr.Get(1).value, 2);
    EXPECT_EQ(arr.Get(1).name, "second");
}

TEST_F(DynamicArrayStructTest, Set_CustomType) {
    DynamicArray<TestStruct> arr(2);

    arr.Set(0, TestStruct(42, "answer"));
    arr.Set(1, TestStruct(99, "test"));

    EXPECT_EQ(arr.Get(0).value, 42);
    EXPECT_EQ(arr.Get(0).name, "answer");
    EXPECT_EQ(arr.Get(1).value, 99);
    EXPECT_EQ(arr.Get(1).name, "test");
}

TEST(DynamicArrayExceptionSafety, Constructor_NullptrWithZeroCount) {
    EXPECT_THROW(
        DynamicArray<int> arr(nullptr, 0),
        std::invalid_argument
    );
}

TEST(DynamicArrayExceptionSafety, MultipleResizes) {
    DynamicArray<int> arr(5);

    for (size_t i = 0; i < 5; ++i) {
        arr.Set(i, static_cast<int>(i * 10));
    }

    arr.Resize(10);
    arr.Resize(3);
    arr.Resize(7);

    EXPECT_EQ(arr.GetSize(), 7);
    EXPECT_EQ(arr.Get(0), 0);
    EXPECT_EQ(arr.Get(1), 10);
    EXPECT_EQ(arr.Get(2), 20);
}

TEST(DynamicArrayExceptionSafety, AssignmentAfterResize) {
    DynamicArray<int> arr1(3);
    DynamicArray<int> arr2(10);

    arr1.Set(0, 1);
    arr2.Set(9, 99);

    arr1 = arr2;

    EXPECT_EQ(arr1.GetSize(), 10);
    EXPECT_EQ(arr1.Get(9), 99);
    EXPECT_EQ(arr1.Get(0), int{});
}

class DynamicArrayBoundaryTest : public ::testing::TestWithParam<size_t> {
};

TEST_P(DynamicArrayBoundaryTest, GetSize_MatchesConstructor) {
    size_t expected_size = GetParam();
    DynamicArray<int> arr(expected_size);
    EXPECT_EQ(arr.GetSize(), expected_size);
}

INSTANTIATE_TEST_SUITE_P(
    SizeTests,
    DynamicArrayBoundaryTest,
    ::testing::Values(0, 1, 10, 100, 1000)
);
