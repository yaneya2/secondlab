#include <gtest/gtest.h>
#include "../classes/headers/SegmentedDeque.h"
#include <vector>
#include <sstream>
#include <stdexcept>


using TestDeque = SegmentedDeque<int>;

static void FillDeque(TestDeque &dq, const std::vector<int> &values, bool prepend = false) {
    for (int v: values) {
        if (prepend) dq.PrependImpl(v);
        else dq.AppendImpl(v);
    }
}

TEST(SegmentedDequeTest, ConstructorValidState) {
    TestDeque dq(4);
    EXPECT_TRUE(dq.IsEmpty());
    EXPECT_EQ(dq.GetLength(), 0);
}

TEST(SegmentedDequeTest, ConstructorThrowsOnInvalidSegmentLength) {
    EXPECT_THROW(TestDeque(0), std::invalid_argument);
    EXPECT_THROW(TestDeque(1), std::invalid_argument);
}

TEST(SegmentedDequeTest, AppendIncreasesSize) {
    TestDeque dq(4);
    dq.AppendImpl(10);
    EXPECT_EQ(dq.GetLength(), 1);
    EXPECT_FALSE(dq.IsEmpty());
}

TEST(SegmentedDequeTest, PrependIncreasesSize) {
    TestDeque dq(4);
    dq.PrependImpl(5);
    EXPECT_EQ(dq.GetLength(), 1);
    EXPECT_FALSE(dq.IsEmpty());
}

TEST(SegmentedDequeTest, MixedAppendPrependOrder) {
    TestDeque dq(4);
    dq.AppendImpl(10);
    dq.PrependImpl(5);
    dq.AppendImpl(15);
    EXPECT_EQ(dq.GetLength(), 3);
    EXPECT_EQ(dq.GetFirst(), 5);
    EXPECT_EQ(dq.GetLast(), 15);
}


TEST(SegmentedDequeTest, GetFirstLastOnEmptyThrows) {
    TestDeque dq(4);
    EXPECT_THROW(dq.GetFirst(), std::out_of_range);
    EXPECT_THROW(dq.GetLast(), std::out_of_range);
}

TEST(SegmentedDequeTest, PopEmptyThrows) {
    TestDeque dq(4);
    EXPECT_THROW(dq.PopFirst(), std::out_of_range);
    EXPECT_THROW(dq.PopLast(), std::out_of_range);
}

TEST(SegmentedDequeTest, PopFirstCorrect) {
    TestDeque dq(4);
    FillDeque(dq, {1, 2, 3});
    EXPECT_EQ(dq.PopFirst(), 1);
    EXPECT_EQ(dq.GetLength(), 2);
    EXPECT_EQ(dq.GetFirst(), 2);
}

TEST(SegmentedDequeTest, PopLastCorrect) {
    TestDeque dq(4);
    FillDeque(dq, {1, 2, 3});
    EXPECT_EQ(dq.PopLast(), 3);
    EXPECT_EQ(dq.GetLength(), 2);
    EXPECT_EQ(dq.GetLast(), 2);
}

TEST(SegmentedDequeTest, PopUntilEmptyReinitializes) {
    TestDeque dq(4);
    dq.AppendImpl(42);
    dq.PopFirst();
    EXPECT_TRUE(dq.IsEmpty());
    // Внутренний пустой сегмент должен сохраняться
    EXPECT_NO_THROW(dq.AppendImpl(99));
    EXPECT_EQ(dq.GetLength(), 1);
}

TEST(SegmentedDequeTest, IteratorEmptyDeque) {
    TestDeque dq(4);
    auto *it = dq.GetEnumerator();
    EXPECT_FALSE(it->MoveNext());
    EXPECT_THROW(it->Current(), std::out_of_range);
    delete it;
}

TEST(SegmentedDequeTest, IteratorTraversesCorrectly) {
    TestDeque dq(4);
    FillDeque(dq, {10, 20, 30});
    auto *it = dq.GetEnumerator();
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 10);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 20);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 30);
    EXPECT_FALSE(it->MoveNext());
    delete it;
}

TEST(SegmentedDequeTest, IteratorResetWorks) {
    TestDeque dq(4);
    FillDeque(dq, {1, 2, 3});
    auto *it = dq.GetEnumerator();
    it->MoveNext();
    it->MoveNext();
    it->Reset();
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 1);
    delete it;
}

TEST(SegmentedDequeTest, FindSubsequenceExactMatch) {
    TestDeque main(4);
    FillDeque(main, {1, 2, 3, 4, 5});
    TestDeque sub(4);
    FillDeque(sub, {3, 4});
    EXPECT_EQ(main.FindSubsequence(sub), 2);
}

TEST(SegmentedDequeTest, FindSubsequenceNotFound) {
    TestDeque main(4);
    FillDeque(main, {1, 2, 3});
    TestDeque sub(4);
    FillDeque(sub, {2, 4});
    EXPECT_EQ(main.FindSubsequence(sub), -1);
}

TEST(SegmentedDequeTest, FindSubsequenceEmptySubsequence) {
    TestDeque main(4);
    FillDeque(main, {1, 2});
    TestDeque sub(4); // Пустая
    EXPECT_EQ(main.FindSubsequence(sub), 0);
}

TEST(SegmentedDequeTest, FindSubsequenceOverlapping) {
    TestDeque main(4);
    FillDeque(main, {1, 2, 1, 2, 1});
    TestDeque sub(4);
    FillDeque(sub, {1, 2});
    EXPECT_EQ(main.FindSubsequence(sub), 0);
}

TEST(SegmentedDequeTest, GetSubsequenceValidRange) {
    TestDeque dq(4);
    FillDeque(dq, {0, 1, 2, 3, 4});
    Sequence<int> *subSeq = dq.GetSubsequence(1, 3);
    ASSERT_NE(subSeq, nullptr);
    EXPECT_EQ(subSeq->GetLength(), 3);

    auto *it = subSeq->GetEnumerator();
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 1);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 2);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 3);
    delete it;
    delete subSeq;
}

TEST(SegmentedDequeTest, GetSubsequenceInvalidRangesThrow) {
    TestDeque dq(4);
    FillDeque(dq, {1, 2, 3});
    EXPECT_THROW(dq.GetSubsequence(2, 1), std::out_of_range);
    EXPECT_THROW(dq.GetSubsequence(0, 5), std::out_of_range);
}

TEST(SegmentedDequeTest, InsertAtImplCreatesNewDeque) {
    TestDeque dq(4);
    FillDeque(dq, {10, 30});
    Sequence<int> *resSeq = dq.InsertAtImpl(20, 1);
    TestDeque *res = dynamic_cast<TestDeque *>(resSeq);
    ASSERT_NE(res, nullptr);

    EXPECT_EQ(res->GetLength(), 3);
    auto *it = res->GetEnumerator();
    it->MoveNext();
    EXPECT_EQ(it->Current(), 10);
    it->MoveNext();
    EXPECT_EQ(it->Current(), 20);
    it->MoveNext();
    EXPECT_EQ(it->Current(), 30);
    delete it;
    delete res;
    // Оригинальный дек не изменён
    EXPECT_EQ(dq.GetLength(), 2);
}

TEST(SegmentedDequeTest, DelImplCreatesNewDeque) {
    TestDeque dq(4);
    FillDeque(dq, {10, 20, 30});
    Sequence<int> *resSeq = dq.DelImpl(1);
    TestDeque *res = dynamic_cast<TestDeque *>(resSeq);
    ASSERT_NE(res, nullptr);

    EXPECT_EQ(res->GetLength(), 2);
    auto *it = res->GetEnumerator();
    it->MoveNext();
    EXPECT_EQ(it->Current(), 10);
    it->MoveNext();
    EXPECT_EQ(it->Current(), 30);
    delete it;
    delete res;
}

TEST(SegmentedDequeTest, ConcatImplCreatesNewDeque) {
    TestDeque dq1(4), dq2(4);
    FillDeque(dq1, {1, 2});
    FillDeque(dq2, {3, 4});
    Sequence<int> *resSeq = dq1.ConcatImpl(dq2);
    TestDeque *res = dynamic_cast<TestDeque *>(resSeq);
    ASSERT_NE(res, nullptr);

    EXPECT_EQ(res->GetLength(), 4);
    auto *it = res->GetEnumerator();
    for (int v: {1, 2, 3, 4}) {
        it->MoveNext();
        EXPECT_EQ(it->Current(), v);
    }
    delete it;
    delete res;
}

TEST(SegmentedDequeTest, CreateEmptyReturnsFreshInstance) {
    TestDeque dq(5);
    FillDeque(dq, {1, 2, 3});
    Sequence<int> *emptySeq = dq.CreateEmpty();
    TestDeque *emptyDq = dynamic_cast<TestDeque *>(emptySeq);
    ASSERT_NE(emptyDq, nullptr);
    EXPECT_TRUE(emptyDq->IsEmpty());
    EXPECT_EQ(emptyDq->GetLength(), 0);
    delete emptyDq;
}

TEST(SegmentedDequeTest, OperationsAcrossMultipleSegments) {
    TestDeque dq(3);
    for (int i = 1; i <= 10; ++i) {
        dq.AppendImpl(i);
    }
    EXPECT_EQ(dq.GetLength(), 10);

    auto *it = dq.GetEnumerator();
    for (int i = 1; i <= 10; ++i) {
        EXPECT_TRUE(it->MoveNext());
        EXPECT_EQ(it->Current(), i);
    }
    delete it;

    EXPECT_EQ(dq.PopFirst(), 1);
    EXPECT_EQ(dq.PopLast(), 10);
    EXPECT_EQ(dq.GetLength(), 8);
}

TEST(SegmentedDequeTest, AppendWhenTailIsFull_CreatesNewSegment) {
    TestDeque dq(3);
    dq.AppendImpl(10);
    dq.AppendImpl(20);
    dq.AppendImpl(30);
    EXPECT_EQ(dq.GetLength(), 3);
    auto *it = dq.GetEnumerator();
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 10);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 20);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 30);
    EXPECT_FALSE(it->MoveNext());
    delete it;
}

TEST(SegmentedDequeTest, PrependWhenHeadIsZero_CreatesNewSegment) {
    TestDeque dq(3);
    dq.PrependImpl(20);
    dq.PrependImpl(10);
    dq.PrependImpl(5);
    EXPECT_EQ(dq.GetLength(), 3);
    auto *it = dq.GetEnumerator();
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 5);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 10);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 20);
    EXPECT_FALSE(it->MoveNext());
    delete it;
}

TEST(SegmentedDequeTest, AppendPrepend_MultipleSegmentTransitions) {
    TestDeque dq(2);
    dq.AppendImpl(1);
    dq.PrependImpl(0);
    dq.AppendImpl(2);
    dq.PrependImpl(-1);
    EXPECT_EQ(dq.GetLength(), 4);
    auto *it = dq.GetEnumerator();
    std::vector<int> result;
    while (it->MoveNext()) result.push_back(it->Current());
    delete it;
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], -1);
    EXPECT_EQ(result[1], 0);
    EXPECT_EQ(result[2], 1);
    EXPECT_EQ(result[3], 2);
}

TEST(SegmentedDequeTest, FindSubsequence_EmptyMain_NonEmptySub) {
    TestDeque main(4);
    TestDeque sub(4);
    FillDeque(sub, {1, 2});
    EXPECT_EQ(main.FindSubsequence(sub), -1);
}

TEST(SegmentedDequeTest, FindSubsequence_SubLongerThanMain) {
    TestDeque main(4);
    FillDeque(main, {1, 2});
    TestDeque sub(4);
    FillDeque(sub, {1, 2, 3, 4});
    EXPECT_EQ(main.FindSubsequence(sub), -1);
}

TEST(SegmentedDequeTest, FindSubsequence_OverlappingPattern) {
    TestDeque main(4);
    FillDeque(main, {1, 2, 1, 2, 1});
    TestDeque sub(4);
    FillDeque(sub, {1, 2, 1});
    EXPECT_EQ(main.FindSubsequence(sub), 0);
}

TEST(SegmentedDequeTest, FindSubsequence_OverlapWithPartialMatch) {
    TestDeque main(4);
    FillDeque(main, {1, 2, 1, 2, 3});
    TestDeque sub(4);
    FillDeque(sub, {1, 2, 3});
    EXPECT_EQ(main.FindSubsequence(sub), 2);
}

TEST(SegmentedDequeTest, FindSubsequence_SingleElementPatterns) {
    TestDeque main(4);
    FillDeque(main, {5, 5, 5, 5});
    TestDeque sub(4);
    FillDeque(sub, {5});
    EXPECT_EQ(main.FindSubsequence(sub), 0);
}

TEST(SegmentedDequeTest, GetSubsequence_StartEqualsEnd_SingleElement) {
    TestDeque dq(4);
    FillDeque(dq, {10, 20, 30, 40});
    Sequence<int> *sub = dq.GetSubsequence(2, 2);
    ASSERT_NE(sub, nullptr);
    EXPECT_EQ(sub->GetLength(), 1);
    auto *it = sub->GetEnumerator();
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 30);
    EXPECT_FALSE(it->MoveNext());
    delete it;
    delete sub;
}

TEST(SegmentedDequeTest, GetSubsequence_FullRange) {
    TestDeque dq(4);
    FillDeque(dq, {1, 2, 3});
    Sequence<int> *sub = dq.GetSubsequence(0, 2);
    ASSERT_NE(sub, nullptr);
    EXPECT_EQ(sub->GetLength(), 3);
    auto *it = sub->GetEnumerator();
    for (int expected: {1, 2, 3}) {
        EXPECT_TRUE(it->MoveNext());
        EXPECT_EQ(it->Current(), expected);
    }
    EXPECT_FALSE(it->MoveNext());
    delete it;
    delete sub;
}

TEST(SegmentedDequeTest, GetSubsequence_EmptyResult_InvalidRange) {
    TestDeque dq(4);
    FillDeque(dq, {1, 2, 3});
    EXPECT_THROW(dq.GetSubsequence(2, 1), std::out_of_range);
    EXPECT_THROW(dq.GetSubsequence(0, 3), std::out_of_range);
}

TEST(SegmentedDequeTest, GetSubsequence_AcrossSegmentBoundaries) {
    TestDeque dq(3);
    for (int i = 1; i <= 7; ++i) dq.AppendImpl(i);
    Sequence<int> *sub = dq.GetSubsequence(2, 4);
    EXPECT_EQ(sub->GetLength(), 3);
    auto *it = sub->GetEnumerator();
    for (int expected: {3, 4, 5}) {
        EXPECT_TRUE(it->MoveNext());
        EXPECT_EQ(it->Current(), expected);
    }
    delete it;
    delete sub;
}

TEST(SegmentedDequeTest, InsertAtImpl_IndexZero_EqualsPrepend) {
    TestDeque dq(4);
    FillDeque(dq, {20, 30, 40});
    Sequence<int> *res = dq.InsertAtImpl(10, 0);
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->GetLength(), 4);
    auto *it = res->GetEnumerator();
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 10);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 20);
    delete it;
    delete res;
    EXPECT_EQ(dq.GetLength(), 3);
    EXPECT_EQ(dq.GetFirst(), 20);
}

TEST(SegmentedDequeTest, InsertAtImpl_IndexEqualsLength_EqualsAppend) {
    TestDeque dq(4);
    FillDeque(dq, {10, 20, 30});
    Sequence<int> *res = dq.InsertAtImpl(40, 3);
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->GetLength(), 4);
    auto *it = res->GetEnumerator();
    for (int i = 0; i < 3; ++i) it->MoveNext();
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 40);
    EXPECT_FALSE(it->MoveNext());
    delete it;
    delete res;
}

TEST(SegmentedDequeTest, InsertAtImpl_IndexGreaterThanLength_Throws) {
    TestDeque dq(4);
    FillDeque(dq, {1, 2, 3});
    EXPECT_THROW(dq.InsertAtImpl(99, 4), std::out_of_range);
    EXPECT_THROW(dq.InsertAtImpl(99, 100), std::out_of_range);
    EXPECT_EQ(dq.GetLength(), 3);
}

TEST(SegmentedDequeTest, InsertAtImpl_EmptyDeque_InsertAtZero) {
    TestDeque dq(4);
    Sequence<int> *res = dq.InsertAtImpl(42, 0);
    EXPECT_EQ(res->GetLength(), 1);
    auto *it = res->GetEnumerator();
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 42);
    EXPECT_FALSE(it->MoveNext());
    delete it;
    delete res;
}

TEST(SegmentedDequeTest, InsertAtImpl_PreservesOriginalUnmodified) {
    TestDeque dq(4);
    FillDeque(dq, {1, 2, 3});
    Sequence<int> *res = dq.InsertAtImpl(99, 1);
    EXPECT_EQ(dq.GetLength(), 3);
    auto *itOrig = dq.GetEnumerator();
    for (int expected: {1, 2, 3}) {
        EXPECT_TRUE(itOrig->MoveNext());
        EXPECT_EQ(itOrig->Current(), expected);
    }
    delete itOrig;
    auto *itRes = res->GetEnumerator();
    for (int expected: {1, 99, 2, 3}) {
        EXPECT_TRUE(itRes->MoveNext());
        EXPECT_EQ(itRes->Current(), expected);
    }
    delete itRes;
    delete res;
}
