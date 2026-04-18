#include "../classes/headers/MutableArraySequence.h"
#include "../classes/headers/ImmutableArraySequence.h"
#include "../classes/headers/MutableListSequence.h"
#include "../classes/headers/ImmutableListSequence.h"
#include <functional>
#include <gtest/gtest.h>

int square(int x) { return x * x; }
bool isEven(int x) { return x % 2 == 0; }
int sumAcc(int acc, int x) { return acc + x; }
bool isGreaterThan10(int x) { return x > 10; }

int GetAt(Sequence<int> *seq, size_t index) {
    auto en = seq->GetEnumerator();
    for (size_t i = 0; i <= index; ++i) {
        if (!en->MoveNext()) {
            delete en;
            throw std::out_of_range("Index out of range");
        }
    }
    int val = en->Current();
    delete en;
    return val;
}

TEST(MutableArraySequence, GetLength) {
    MutableArraySequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableArraySequence, GetFirst) {
    MutableArraySequence<int> seq;
    seq.Append(10)->Append(20);
    EXPECT_EQ(seq.GetFirst(), 10);
    EXPECT_THROW(MutableArraySequence<int>().GetFirst(), std::out_of_range);
}

TEST(MutableArraySequence, GetLast) {
    MutableArraySequence<int> seq;
    seq.Append(10)->Append(20);
    EXPECT_EQ(seq.GetLast(), 20);
    EXPECT_THROW(MutableArraySequence<int>().GetLast(), std::out_of_range);
}

TEST(MutableArraySequence, GetEnumerator) {
    MutableArraySequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    auto en = seq.GetEnumerator();
    EXPECT_TRUE(en->MoveNext());
    EXPECT_EQ(en->Current(), 1);
    EXPECT_TRUE(en->MoveNext());
    EXPECT_EQ(en->Current(), 2);
    EXPECT_TRUE(en->MoveNext());
    EXPECT_EQ(en->Current(), 3);
    EXPECT_FALSE(en->MoveNext());
    delete en;
}

TEST(MutableArraySequence, GetSubsequence) {
    MutableArraySequence<int> seq;
    seq.Append(1)->Append(2)->Append(3)->Append(4);
    auto sub = seq.GetSubsequence(1, 2);
    EXPECT_EQ(sub->GetLength(), 2);
    EXPECT_EQ(GetAt(sub, 0), 2);
    EXPECT_EQ(GetAt(sub, 1), 3);
    delete sub;
}

TEST(MutableArraySequence, Append) {
    MutableArraySequence<int> seq;
    seq.Append(10)->Append(20);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(GetAt(&seq, 0), 10);
    EXPECT_EQ(GetAt(&seq, 1), 20);
}

TEST(MutableArraySequence, Prepend) {
    MutableArraySequence<int> seq;
    seq.Append(20)->Append(30);
    seq.Prepend(10);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(GetAt(&seq, 0), 10);
    EXPECT_EQ(GetAt(&seq, 1), 20);
}

TEST(MutableArraySequence, InsertAt) {
    MutableArraySequence<int> seq;
    seq.Append(10)->Append(30);
    seq.InsertAt(20, 1);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(GetAt(&seq, 1), 20);
    EXPECT_THROW(seq.InsertAt(99, 5), std::out_of_range);
}

TEST(MutableArraySequence, Concat) {
    MutableArraySequence<int> seq1, seq2;
    seq1.Append(1)->Append(2);
    seq2.Append(3)->Append(4);
    seq1.Concat(seq2);
    EXPECT_EQ(seq1.GetLength(), 4);
    EXPECT_EQ(GetAt(&seq1, 2), 3);
}

TEST(MutableArraySequence, Del) {
    MutableArraySequence<int> seq;
    seq.Append(10)->Append(20)->Append(30);
    seq.Del(1);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(GetAt(&seq, 0), 10);
    EXPECT_EQ(GetAt(&seq, 1), 30);
    EXPECT_THROW(seq.Del(5), std::out_of_range);
}

TEST(MutableArraySequence, Map) {
    MutableArraySequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    auto res = seq.Map(square);
    EXPECT_EQ(GetAt(res, 0), 1);
    EXPECT_EQ(GetAt(res, 1), 4);
    EXPECT_EQ(GetAt(res, 2), 9);
    delete res;
}

TEST(MutableArraySequence, Where) {
    MutableArraySequence<int> seq;
    seq.Append(1)->Append(2)->Append(3)->Append(4);
    auto res = seq.Where(isEven);
    EXPECT_EQ(res->GetLength(), 2);
    EXPECT_EQ(GetAt(res, 0), 2);
    EXPECT_EQ(GetAt(res, 1), 4);
    delete res;
}

TEST(MutableArraySequence, Reduce) {
    MutableArraySequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    EXPECT_EQ(seq.Reduce(sumAcc), 6);
    EXPECT_THROW(MutableArraySequence<int>().Reduce(sumAcc), std::out_of_range);
}

TEST(MutableArraySequence, GetFirstPredicate) {
    MutableArraySequence<int> seq;
    seq.Append(1)->Append(3)->Append(4)->Append(5);
    EXPECT_TRUE(seq.GetFirstP(isEven).HasValue());
    EXPECT_EQ(seq.GetFirstP(isEven).GetValue(), 4);
    EXPECT_FALSE(seq.GetFirstP(isGreaterThan10).HasValue());
}

TEST(MutableArraySequence, GetLastPredicate) {
    MutableArraySequence<int> seq;
    seq.Append(1)->Append(4)->Append(6)->Append(3);
    EXPECT_EQ(seq.GetLastP(isEven).GetValue(), 6);
    EXPECT_FALSE(seq.GetLastP(isGreaterThan10).HasValue());
}

TEST(ImmutableArraySequence, GetLength) {
    int arr[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(arr, 3);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(ImmutableArraySequence, GetFirst) {
    int arr[] = {10, 20};
    ImmutableArraySequence<int> seq(arr, 2);
    EXPECT_EQ(seq.GetFirst(), 10);
}

TEST(ImmutableArraySequence, GetLast) {
    int arr[] = {10, 20};
    ImmutableArraySequence<int> seq(arr, 2);
    EXPECT_EQ(seq.GetLast(), 20);
}

TEST(ImmutableArraySequence, GetEnumerator) {
    int arr[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(arr, 3);
    auto en = seq.GetEnumerator();
    EXPECT_TRUE(en->MoveNext());
    EXPECT_EQ(en->Current(), 1);
    EXPECT_TRUE(en->MoveNext());
    EXPECT_EQ(en->Current(), 2);
    delete en;
}

TEST(ImmutableArraySequence, GetSubsequence) {
    int arr[] = {1, 2, 3, 4};
    ImmutableArraySequence<int> seq(arr, 4);
    auto sub = seq.GetSubsequence(1, 2);
    EXPECT_EQ(sub->GetLength(), 2);
    EXPECT_EQ(GetAt(sub, 0), 2);
    delete sub;
}

TEST(ImmutableArraySequence, Append) {
    ImmutableArraySequence<int> seq;
    auto res = seq.Append(1)->Append(2);
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_EQ(res->GetLength(), 2);
    EXPECT_EQ(GetAt(res, 0), 1);
    EXPECT_EQ(GetAt(res, 1), 2);
    delete res;
}

TEST(ImmutableArraySequence, Prepend) {
    int arr[] = {2, 3};
    auto seq = new ImmutableArraySequence<int>(arr, 2);
    auto res = seq->Prepend(1);
    EXPECT_EQ(seq->GetLength(), 2);
    EXPECT_EQ(res->GetLength(), 3);
    EXPECT_EQ(GetAt(res, 0), 1);
    delete seq;
    delete res;
}

TEST(ImmutableArraySequence, InsertAt) {
    int arr[] = {1, 3};
    auto seq = new ImmutableArraySequence<int>(arr, 2);
    auto res = seq->InsertAt(2, 1);
    EXPECT_EQ(seq->GetLength(), 2);
    EXPECT_EQ(res->GetLength(), 3);
    EXPECT_EQ(GetAt(res, 1), 2);
    delete seq;
    delete res;
}

TEST(ImmutableArraySequence, Concat) {
    int arr1[] = {1, 2};
    auto seq1 = new ImmutableArraySequence<int>(arr1, 2);
    int arr2[] = {3, 4};
    ImmutableArraySequence<int> seq2(arr2, 2);
    auto res = seq1->Concat(seq2);
    EXPECT_EQ(seq1->GetLength(), 2);
    EXPECT_EQ(res->GetLength(), 4);
    EXPECT_EQ(GetAt(res, 2), 3);
    delete seq1;
    delete res;
}

TEST(ImmutableArraySequence, Del) {
    int arr[] = {10, 20, 30};
    auto seq = new ImmutableArraySequence<int>(arr, 3);
    auto res = seq->Del(1);
    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(res->GetLength(), 2);
    EXPECT_EQ(GetAt(res, 0), 10);
    EXPECT_EQ(GetAt(res, 1), 30);
    delete seq;
    delete res;
}

TEST(ImmutableArraySequence, Map) {
    int arr[] = {1, 2, 3};
    auto seq = new ImmutableArraySequence<int>(arr, 3);
    auto res = seq->Map(square);
    EXPECT_EQ(GetAt(res, 0), 1);
    EXPECT_EQ(GetAt(res, 1), 4);
    delete seq;
    delete res;
}

TEST(ImmutableArraySequence, Where) {
    int arr[] = {1, 2, 3, 4};
    auto seq = new ImmutableArraySequence<int>(arr, 4);
    auto res = seq->Where(isEven);
    EXPECT_EQ(res->GetLength(), 2);
    EXPECT_EQ(GetAt(res, 0), 2);
    delete seq;
    delete res;
}

TEST(ImmutableArraySequence, Reduce) {
    int arr[] = {1, 2, 3};
    auto seq = new ImmutableArraySequence<int>(arr, 3);
    EXPECT_EQ(seq->Reduce(sumAcc), 6);
    delete seq;
}

TEST(ImmutableArraySequence, GetFirstPredicate) {
    int arr[] = {1, 3, 4};
    auto seq = new ImmutableArraySequence<int>(arr, 3);
    EXPECT_EQ(seq->GetFirstP(isEven).GetValue(), 4);
    delete seq;
}

TEST(ImmutableArraySequence, GetLastPredicate) {
    int arr[] = {1, 4, 6, 3};
    auto seq = new ImmutableArraySequence<int>(arr, 4);
    EXPECT_EQ(seq->GetLastP(isEven).GetValue(), 6);
    delete seq;
}

TEST(MutableListSequence, GetLength) {
    MutableListSequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableListSequence, GetFirst) {
    MutableListSequence<int> seq;
    seq.Append(10)->Append(20);
    EXPECT_EQ(seq.GetFirst(), 10);
}

TEST(MutableListSequence, GetLast) {
    MutableListSequence<int> seq;
    seq.Append(10)->Append(20);
    EXPECT_EQ(seq.GetLast(), 20);
}

TEST(MutableListSequence, GetEnumerator) {
    MutableListSequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    auto en = seq.GetEnumerator();
    EXPECT_TRUE(en->MoveNext());
    EXPECT_EQ(en->Current(), 1);
    EXPECT_TRUE(en->MoveNext());
    EXPECT_EQ(en->Current(), 2);
    delete en;
}

TEST(MutableListSequence, GetSubsequence) {
    MutableListSequence<int> seq;
    seq.Append(1)->Append(2)->Append(3)->Append(4);
    auto sub = seq.GetSubsequence(1, 2);
    EXPECT_EQ(sub->GetLength(), 2);
    EXPECT_EQ(GetAt(sub, 0), 2);
    delete sub;
}

TEST(MutableListSequence, Append) {
    MutableListSequence<int> seq;
    seq.Append(10)->Append(20);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(GetAt(&seq, 0), 10);
}

TEST(MutableListSequence, Prepend) {
    MutableListSequence<int> seq;
    seq.Append(20)->Append(30);
    seq.Prepend(10);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(GetAt(&seq, 0), 10);
}

TEST(MutableListSequence, InsertAt) {
    MutableListSequence<int> seq;
    seq.Append(10)->Append(30);
    seq.InsertAt(20, 1);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableListSequence, Concat) {
    MutableListSequence<int> seq1, seq2;
    seq1.Append(1)->Append(2);
    seq2.Append(3)->Append(4);
    seq1.Concat(seq2);
    EXPECT_EQ(seq1.GetLength(), 4);
    EXPECT_EQ(GetAt(&seq1, 2), 3);
}

TEST(MutableListSequence, Del) {
    MutableListSequence<int> seq;
    seq.Append(10)->Append(20)->Append(30);
    seq.Del(1);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(GetAt(&seq, 0), 10);
    EXPECT_EQ(GetAt(&seq, 1), 30);
}

TEST(MutableListSequence, Map) {
    MutableListSequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    auto res = seq.Map(square);
    EXPECT_EQ(GetAt(res, 0), 1);
    EXPECT_EQ(GetAt(res, 1), 4);
    delete res;
}

TEST(MutableListSequence, Where) {
    MutableListSequence<int> seq;
    seq.Append(1)->Append(2)->Append(3)->Append(4);
    auto res = seq.Where(isEven);
    EXPECT_EQ(res->GetLength(), 2);
    EXPECT_EQ(GetAt(res, 0), 2);
    delete res;
}

TEST(MutableListSequence, Reduce) {
    MutableListSequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    EXPECT_EQ(seq.Reduce(sumAcc), 6);
}

TEST(MutableListSequence, GetFirstPredicate) {
    MutableListSequence<int> seq;
    seq.Append(1)->Append(3)->Append(4)->Append(5);
    EXPECT_EQ(seq.GetFirstP(isEven).GetValue(), 4);
}

TEST(MutableListSequence, GetLastPredicate) {
    MutableListSequence<int> seq;
    seq.Append(1)->Append(4)->Append(6)->Append(3);
    EXPECT_EQ(seq.GetLastP(isEven).GetValue(), 6);
}

TEST(ImmutableListSequence, GetLength) {
    int arr[] = {1, 2, 3};
    ImmutableListSequence<int> seq(arr, 3);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(ImmutableListSequence, GetFirst) {
    int arr[] = {10, 20};
    ImmutableListSequence<int> seq(arr, 2);
    EXPECT_EQ(seq.GetFirst(), 10);
}

TEST(ImmutableListSequence, GetLast) {
    int arr[] = {10, 20};
    ImmutableListSequence<int> seq(arr, 2);
    EXPECT_EQ(seq.GetLast(), 20);
}

TEST(ImmutableListSequence, GetEnumerator) {
    int arr[] = {1, 2, 3};
    ImmutableListSequence<int> seq(arr, 3);
    auto en = seq.GetEnumerator();
    EXPECT_TRUE(en->MoveNext());
    EXPECT_EQ(en->Current(), 1);
    EXPECT_TRUE(en->MoveNext());
    EXPECT_EQ(en->Current(), 2);
    delete en;
}

TEST(ImmutableListSequence, GetSubsequence) {
    int arr[] = {1, 2, 3, 4};
    ImmutableListSequence<int> seq(arr, 4);
    auto sub = seq.GetSubsequence(1, 2);
    EXPECT_EQ(sub->GetLength(), 2);
    EXPECT_EQ(GetAt(sub, 0), 2);
    delete sub;
}

TEST(ImmutableListSequence, Append) {
    ImmutableListSequence<int> seq;
    auto res = seq.Append(1)->Append(2);
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_EQ(res->GetLength(), 2);
    EXPECT_EQ(GetAt(res, 0), 1);
    delete res;
}

TEST(ImmutableListSequence, Prepend) {
    int arr[] = {2, 3};
    auto seq = new ImmutableListSequence<int>(arr, 2);
    auto res = seq->Prepend(1);
    EXPECT_EQ(seq->GetLength(), 2);
    EXPECT_EQ(res->GetLength(), 3);
    EXPECT_EQ(GetAt(res, 0), 1);
    delete seq;
    delete res;
}

TEST(ImmutableListSequence, InsertAt) {
    int arr[] = {1, 3};
    auto seq = new ImmutableListSequence<int>(arr, 2);
    auto res = seq->InsertAt(2, 1);
    EXPECT_EQ(seq->GetLength(), 2);
    EXPECT_EQ(res->GetLength(), 3);
    EXPECT_EQ(GetAt(res, 1), 2);
    delete seq;
    delete res;
}

TEST(ImmutableListSequence, Concat) {
    int arr1[] = {1, 2};
    auto seq1 = new ImmutableListSequence<int>(arr1, 2);
    int arr2[] = {3, 4};
    ImmutableListSequence<int> seq2(arr2, 2);
    auto res = seq1->Concat(seq2);
    EXPECT_EQ(seq1->GetLength(), 2);
    EXPECT_EQ(res->GetLength(), 4);
    EXPECT_EQ(GetAt(res, 2), 3);
    delete seq1;
    delete res;
}

TEST(ImmutableListSequence, Del) {
    int arr[] = {10, 20, 30};
    auto seq = new ImmutableListSequence<int>(arr, 3);
    auto res = seq->Del(1);
    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(res->GetLength(), 2);
    EXPECT_EQ(GetAt(res, 0), 10);
    EXPECT_EQ(GetAt(res, 1), 30);
    delete seq;
    delete res;
}

TEST(ImmutableListSequence, Map) {
    int arr[] = {1, 2, 3};
    auto seq = new ImmutableListSequence<int>(arr, 3);
    auto res = seq->Map(square);
    EXPECT_EQ(GetAt(res, 0), 1);
    EXPECT_EQ(GetAt(res, 1), 4);
    delete seq;
    delete res;
}

TEST(ImmutableListSequence, Where) {
    int arr[] = {1, 2, 3, 4};
    auto seq = new ImmutableListSequence<int>(arr, 4);
    auto res = seq->Where(isEven);
    EXPECT_EQ(res->GetLength(), 2);
    EXPECT_EQ(GetAt(res, 0), 2);
    delete seq;
    delete res;
}

TEST(ImmutableListSequence, Reduce) {
    int arr[] = {1, 2, 3};
    auto seq = new ImmutableListSequence<int>(arr, 3);
    EXPECT_EQ(seq->Reduce(sumAcc), 6);
    delete seq;
}

TEST(ImmutableListSequence, GetFirstPredicate) {
    int arr[] = {1, 3, 4};
    auto seq = new ImmutableListSequence<int>(arr, 3);
    EXPECT_EQ(seq->GetFirstP(isEven).GetValue(), 4);
    delete seq;
}

TEST(ImmutableListSequence, GetLastPredicate) {
    int arr[] = {1, 4, 6, 3};
    auto seq = new ImmutableListSequence<int>(arr, 4);
    EXPECT_EQ(seq->GetLastP(isEven).GetValue(), 6);
    delete seq;
}