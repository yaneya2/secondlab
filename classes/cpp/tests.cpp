#include "../headers/tests.h"
#include "../headers/MutableArraySequence.h"
#include <cassert>
#include <iostream>
#include <string>

// Helper to print a sequence (for debugging, not used in assertions)
template<typename T>
void print_sequence(Sequence<T>* seq, const std::string& name) {
    std::cout << name << ": [";
    for (size_t i = 0; i < seq->GetLength(); ++i) {
        std::cout << seq->Get(i);
        if (i != seq->GetLength() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

void test_append() {
    MutableArraySequence<int> base;
    Sequence<int>* s = &base;
    Sequence<int>* s2 = s->Append(10);
    assert(s2->GetLength() == 1);
    assert(s2->Get(0) == 10);

    Sequence<int>* s3 = s2->Append(20);
    assert(s3->GetLength() == 2);
    assert(s3->Get(0) == 10);
    assert(s3->Get(1) == 20);

    delete s2;
    delete s3;
    std::cout << "test_append passed" << std::endl;
}

void test_prepend() {
    MutableArraySequence<int> base;
    Sequence<int>* s = &base;
    Sequence<int>* s2 = s->Prepend(10);
    assert(s2->GetLength() == 1);
    assert(s2->Get(0) == 10);

    Sequence<int>* s3 = s2->Prepend(5);
    assert(s3->GetLength() == 2);
    assert(s3->Get(0) == 5);
    assert(s3->Get(1) == 10);

    delete s2;
    delete s3;
    std::cout << "test_prepend passed" << std::endl;
}

void test_insertAt() {
    MutableArraySequence<int> base;
    Sequence<int>* s = &base;
    Sequence<int>* s2 = s->Append(10)->Append(30);
    Sequence<int>* s3 = s2->InsertAt(20, 1);
    assert(s3->GetLength() == 3);
    assert(s3->Get(0) == 10);
    assert(s3->Get(1) == 20);
    assert(s3->Get(2) == 30);

    // Insert at beginning
    Sequence<int>* s4 = s3->InsertAt(5, 0);
    assert(s4->GetLength() == 4);
    assert(s4->Get(0) == 5);

    // Insert at end
    Sequence<int>* s5 = s4->InsertAt(40, 4);
    assert(s5->GetLength() == 5);
    assert(s5->Get(4) == 40);

    delete s2;
    delete s3;
    delete s4;
    delete s5;
    std::cout << "test_insertAt passed" << std::endl;
}

void test_concat() {
    MutableArraySequence<int> base1, base2;
    Sequence<int>* s1 = &base1;
    Sequence<int>* s2 = &base2;
    Sequence<int>* a = s1->Append(1)->Append(2);
    Sequence<int>* b = s2->Append(3)->Append(4);
    Sequence<int>* c = a->Concat(b);
    assert(c->GetLength() == 4);
    assert(c->Get(0) == 1);
    assert(c->Get(1) == 2);
    assert(c->Get(2) == 3);
    assert(c->Get(3) == 4);

    delete a;
    delete b;
    delete c;
    std::cout << "test_concat passed" << std::endl;
}

void test_getters() {
    MutableArraySequence<int> base;
    Sequence<int>* s = &base;
    Sequence<int>* s2 = s->Append(5)->Append(10)->Append(15);
    assert(s2->GetFirst().GetValue() == 5);
    assert(s2->GetLast().GetValue() == 15);
    assert(s2->Get(1) == 10);
    assert(s2->GetLength() == 3);

    delete s2;
    std::cout << "test_getters passed" << std::endl;
}

void test_subsequence() {
    MutableArraySequence<int> base;
    Sequence<int>* s = &base;
    Sequence<int>* s2 = s->Append(0)->Append(1)->Append(2)->Append(3)->Append(4);
    // Assuming GetSubsequence(startIndex, endIndex) returns [startIndex, endIndex) (half-open)
    Sequence<int>* sub = s2->GetSubsequence(1, 4);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 1);
    assert(sub->Get(1) == 2);
    assert(sub->Get(2) == 3);

    delete s2;
    delete sub;
    std::cout << "test_subsequence passed" << std::endl;
}

int square_(int x) { return x * x; }
bool isEven_(int x) { return x % 2 == 0; }
int sum_(int acc, int x) { return acc + x; }

void test_map() {
    MutableArraySequence<int> base;
    Sequence<int>* s = &base;
    Sequence<int>* s2 = s->Append(1)->Append(2)->Append(3);
    Sequence<int>* mapped = s2->Map<int>(square_);
    assert(mapped->GetLength() == 3);
    assert(mapped->Get(0) == 1);
    assert(mapped->Get(1) == 4);
    assert(mapped->Get(2) == 9);

    delete s2;
    delete mapped;
    std::cout << "test_map passed" << std::endl;
}

void test_where() {
    MutableArraySequence<int> base;
    Sequence<int>* s = &base;
    Sequence<int>* s2 = s->Append(1)->Append(2)->Append(3)->Append(4);
    Sequence<int>* filtered = s2->Where(isEven_);
    assert(filtered->GetLength() == 2);
    assert(filtered->Get(0) == 2);
    assert(filtered->Get(1) == 4);

    delete s2;
    delete filtered;
    std::cout << "test_where passed" << std::endl;
}

void test_reduce() {
    MutableArraySequence<int> base;
    Sequence<int>* s = &base;
    Sequence<int>* s2 = s->Append(1)->Append(2)->Append(3)->Append(4);
    int result = s2->Reduce<int>(sum_);
    assert(result == 10);

    // Single element
    Sequence<int>* s3 = s->Append(42);
    int result2 = s3->Reduce<int>(sum_);
    assert(result2 == 42);

    delete s2;
    delete s3;
    std::cout << "test_reduce passed" << std::endl;
}

void test_option_first_last() {
    MutableArraySequence<int> base;
    Sequence<int>* s = &base;
    Sequence<int>* s2 = s->Append(10)->Append(20)->Append(30);

    // Without predicate
    Option<int> optFirst = s2->GetFirst();
    assert(optFirst.HasValue());
    assert(optFirst.GetValue() == 10);
    Option<int> optLast = s2->GetLast();
    assert(optLast.HasValue());
    assert(optLast.GetValue() == 30);

    // With predicate
    auto greaterThan15 = [](int x) { return x > 15; };
    Option<int> optFirstPred = s2->GetFirst(greaterThan15);
    assert(optFirstPred.HasValue());
    assert(optFirstPred.GetValue() == 20);
    Option<int> optLastPred = s2->GetLast(greaterThan15);
    assert(optLastPred.HasValue());
    assert(optLastPred.GetValue() == 30);

    // Predicate matches nothing
    auto greaterThan100 = [](int x) { return x > 100; };
    Option<int> optNone = s2->GetFirst(greaterThan100);
    assert(!optNone.HasValue());

    delete s2;
    std::cout << "test_option_first_last passed" << std::endl;
}

void test_empty_sequence() {
    MutableArraySequence<int> base;
    Sequence<int>* s = &base;

    // GetFirst/Last return None
    Option<int> first = s->GetFirst();
    assert(!first.HasValue());
    Option<int> last = s->GetLast();
    assert(!last.HasValue());

    // Reduce on empty throws
    bool caught = false;
    try {
        s->Reduce<int>(sum_);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    // Append works on empty
    Sequence<int>* s2 = s->Append(5);
    assert(s2->GetLength() == 1);
    assert(s2->Get(0) == 5);

    delete s2;
    std::cout << "test_empty_sequence passed" << std::endl;
}

int run_all() {
    test_append();
    test_prepend();
    test_insertAt();
    test_concat();
    test_getters();
    test_subsequence();
    test_map();
    test_where();
    test_reduce();
    test_option_first_last();
    test_empty_sequence();
    std::cout << "\nAll tests passed successfully!" << std::endl;
    return 0;
}
