#include <gtest/gtest.h>
#include "../../classes/secondlab/headers/LinkedList.h"
#include "../../classes/secondlab/headers/IEnumerator.h"

TEST(LinkedListTest, GetFirst_ReturnsHeadElement) {
    LinkedList<int> list;
    list.Append(5);
    list.Append(10);
    EXPECT_EQ(list.GetFirst(), 5);
}

TEST(LinkedListTest, GetLast_ReturnsTailElement) {
    LinkedList<int> list;
    list.Append(5);
    list.Append(10);
    EXPECT_EQ(list.GetLast(), 10);
}

TEST(LinkedListTest, Get_ReturnsElementByIndex) {
    int source[] = {100, 200, 300};
    LinkedList<int> list(source, 3);
    EXPECT_EQ(list.Get(0), 100);
    EXPECT_EQ(list.Get(1), 200);
    EXPECT_EQ(list.Get(2), 300);
}

TEST(LinkedListTest, GetSize_ReturnsCurrentElementCount) {
    LinkedList<int> list;
    EXPECT_EQ(list.GetSize(), 0);
    list.Append(1);
    list.Append(2);
    EXPECT_EQ(list.GetSize(), 2);
}

TEST(LinkedListTest, Append_AddsElementToEnd) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);
    EXPECT_EQ(list.GetSize(), 3);
    EXPECT_EQ(list.GetLast(), 3);
    EXPECT_EQ(list.Get(1), 2);
}

TEST(LinkedListTest, Prepend_AddsElementToBeginning) {
    LinkedList<int> list;
    list.Prepend(3);
    list.Prepend(2);
    list.Prepend(1);
    EXPECT_EQ(list.GetSize(), 3);
    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.Get(1), 2);
}

TEST(LinkedListTest, InsertAt_AddsElementAtSpecifiedPosition) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(3);
    list.InsertAt(1, 2);
    EXPECT_EQ(list.GetSize(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListTest, Del_RemovesElementAtIndex) {
    int source[] = {10, 20, 30, 40};
    LinkedList<int> list(source, 4);
    list.Del(1);
    EXPECT_EQ(list.GetSize(), 3);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 30);
    EXPECT_EQ(list.Get(2), 40);
}

TEST(LinkedListTest, GetEnumerator_ReturnsValidEnumerator) {
    int source[] = {7, 8, 9};
    LinkedList<int> list(source, 3);
    IEnumerator<int> *enumerator = list.GetEnumerator();
    ASSERT_NE(enumerator, nullptr);
    EXPECT_TRUE(enumerator->MoveNext());
    EXPECT_EQ(enumerator->Current(), 7);
    EXPECT_TRUE(enumerator->MoveNext());
    EXPECT_EQ(enumerator->Current(), 8);
    EXPECT_TRUE(enumerator->MoveNext());
    EXPECT_EQ(enumerator->Current(), 9);
    EXPECT_FALSE(enumerator->MoveNext());
    delete enumerator;
}

TEST(LinkedListTest, GetSubList_ReturnsNewListWithRange) {
    int source[] = {0, 1, 2, 3, 4};
    LinkedList<int> list(source, 5);
    LinkedList<int> *sublist = list.GetSubList(1, 3);
    ASSERT_NE(sublist, nullptr);
    EXPECT_EQ(sublist->GetSize(), 3);
    EXPECT_EQ(sublist->Get(0), 1);
    EXPECT_EQ(sublist->Get(1), 2);
    EXPECT_EQ(sublist->Get(2), 3);
    delete sublist;
}

TEST(LinkedListTest, Concat_ReturnsNewListWithCombinedElements) {
    int src1[] = {1, 2};
    int src2[] = {3, 4, 5};
    LinkedList<int> list1(src1, 2);
    LinkedList<int> list2(src2, 3);
    LinkedList<int> *result = list1.Concat(list2);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->GetSize(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(4), 5);
    EXPECT_EQ(list1.GetSize(), 2);
    EXPECT_EQ(list2.GetSize(), 3);
    delete result;
}

TEST(LinkedListExceptionTest, GetFirst_EmptyList_Throws) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetFirst(), std::out_of_range);
}

TEST(LinkedListExceptionTest, GetLast_EmptyList_Throws) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetLast(), std::out_of_range);
}

TEST(LinkedListExceptionTest, Get_IndexOutOfRange_Throws) {
    LinkedList<int> list;
    list.Append(1);
    EXPECT_THROW(list.Get(1), std::out_of_range);
}

TEST(LinkedListExceptionTest, Del_IndexOutOfRange_Throws) {
    LinkedList<int> list;
    list.Append(1);
    EXPECT_THROW(list.Del(1), std::out_of_range);
}

TEST(LinkedListExceptionTest, InsertAt_IndexOutOfRange_Throws) {
    LinkedList<int> list;
    list.Append(1);
    EXPECT_THROW(list.InsertAt(2, 999), std::out_of_range);
}

TEST(LinkedListExceptionTest, Enumerator_Current_BeforeMoveNext_Throws) {
    LinkedList<int> list;
    list.Append(1);
    IEnumerator<int> *e = list.GetEnumerator();
    EXPECT_THROW(e->Current(), std::out_of_range);
    delete e;
}

TEST(LinkedListExceptionTest, GetSubList_InvalidRange_Throws) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    EXPECT_THROW(list.GetSubList(2, 2), std::out_of_range);
}
