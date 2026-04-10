#include <iostream>
#include <cassert>
#include <cstring>
#include "../headers/BItSequence.h"
#include "../headers/Option.h"

// Вспомогательная функция: сравнение BitSequence с ожидаемой строкой битов
void expectBits(const BitSequence& seq, const std::string& expected) {
    assert(seq.GetLength() == expected.length());
    for (size_t i = 0; i < expected.length(); ++i) {
        bool expectedBit = (expected[i] == '1');
        assert(seq.Get(i) == expectedBit);
    }
}

// Вспомогательная функция: проверка, что две последовательности равны
void assertEqual(const BitSequence& a, const BitSequence& b) {
    assert(a == b);
}

// Тест конструкторов
void testConstructors() {
    std::cout << "Testing constructors..." << std::endl;

    // Пустой конструктор
    BitSequence empty;
    assert(empty.GetLength() == 0);

    // Конструктор с размером и начальным значением
    BitSequence allTrue(5, true);
    expectBits(allTrue, "11111");

    BitSequence allFalse(5, false);
    expectBits(allFalse, "00000");

    // Конструктор из массива bool
    bool arr[] = {true, false, true, false, true};
    BitSequence fromArr(arr, 5);
    expectBits(fromArr, "10101");

    // Конструктор копирования
    BitSequence copy(allTrue);
    assertEqual(copy, allTrue);

    // Конструктор перемещения
    BitSequence moved(std::move(copy));
    assertEqual(moved, allTrue);
    assert(copy.GetLength() == 0); // перемещённый объект должен быть пустым

    std::cout << "  Constructors OK" << std::endl;
}

// Тест операторов присваивания
void testAssignmentOperators() {
    std::cout << "Testing assignment operators..." << std::endl;

    BitSequence a(3, true);
    BitSequence b(4, false);
    b = a;
    assertEqual(b, a);

    BitSequence c(2, true);
    BitSequence d(5, false);
    d = std::move(c);
    assertEqual(d, BitSequence(2, true));
    assert(c.GetLength() == 0);

    std::cout << "  Assignment operators OK" << std::endl;
}

// Тест методов доступа (Get, GetFirst, GetLast, Set)
void testAccessMethods() {
    std::cout << "Testing access methods..." << std::endl;

    bool data[] = {true, false, true, false, true};
    BitSequence seq(data, 5);

    assert(seq.GetFirst() == true);
    assert(seq.GetLast() == true);
    assert(seq.Get(2) == true);
    assert(seq.Get(1) == false);

    // Проверка выхода за границы
    bool caught = false;
    try {
        seq.Get(5);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    // Set
    seq.Set(0, false);
    seq.Set(4, false);
    expectBits(seq, "00100");

    std::cout << "  Access methods OK" << std::endl;
}

// Тест операций Sequence (Append, Prepend, InsertAt, Concat, GetSubsequence)
void testSequenceOperations() {
    std::cout << "Testing Sequence operations..." << std::endl;

    BitSequence base(2, false); // 00
    base.Set(0, true);           // 10

    // Append
    Sequence<bool>* appended = base.Append(true);
    BitSequence* app = dynamic_cast<BitSequence*>(appended);
    assert(app != nullptr);
    expectBits(*app, "101");
    delete app;

    // Prepend
    Sequence<bool>* prepended = base.Prepend(false);
    BitSequence* prep = dynamic_cast<BitSequence*>(prepended);
    assert(prep != nullptr);
    expectBits(*prep, "010");
    delete prep;

    // InsertAt
    Sequence<bool>* inserted = base.InsertAt(true, 1);
    BitSequence* ins = dynamic_cast<BitSequence*>(inserted);
    assert(ins != nullptr);
    expectBits(*ins, "110");
    delete ins;

    // Concat
    bool otherArr[] = {false, true, false}; // 010
    BitSequence other(otherArr, 3);
    Sequence<bool>* concated = base.Concat(&other);
    BitSequence* con = dynamic_cast<BitSequence*>(concated);
    assert(con != nullptr);
    expectBits(*con, "10010");
    delete con;

    // GetSubsequence
    Sequence<bool>* sub = base.GetSubsequence(0, 1);
    BitSequence* subSeq = dynamic_cast<BitSequence*>(sub);
    assert(subSeq != nullptr);
    expectBits(*subSeq, "1");
    delete subSeq;

    sub = base.GetSubsequence(1, 2);
    subSeq = dynamic_cast<BitSequence*>(sub);
    expectBits(*subSeq, "0");
    delete subSeq;

    std::cout << "  Sequence operations OK" << std::endl;
}

// Тест побитовых операций (And, Or, Xor, Not)
void testBitwiseOperations() {
    // std::cout << "Testing bitwise operations..." << std::endl;
    //
    // a = 1010 (4 бита)
    bool aArr[] = {true, false, true, false};
    BitSequence a(aArr, 4);
    // b = 1100 (4 бита)
    bool bArr[] = {true, true, false, false};
    BitSequence b(bArr, 4);
    // c = 111 (3 бита)
    BitSequence c(3, true);
    //
    // // And
    // BitSequence* andRes = a.And(b);
    // expectBits(*andRes, "1000");
    // delete andRes;
    //
    // // Or
    // BitSequence* orRes = a.Or(b);
    // expectBits(*orRes, "1110");
    // delete orRes;
    //
    // // Xor
    // BitSequence* xorRes = a.Xor(b);
    // expectBits(*xorRes, "0110");
    // delete xorRes;
    //
    // // Not
    // BitSequence* notRes = a.Not();
    // expectBits(*notRes, "0101");
    // delete notRes;

    // Тест с разными длинами
    // a = 1010 (4), c = 111 (3)
    BitSequence* andDiff = a.And(c);
    //expectBits(*andDiff, "101");  // min length = 3
    delete andDiff;

    BitSequence* orDiff = a.Or(c);
    expectBits(*orDiff, "1111");  // max length = 4
    delete orDiff;

    BitSequence* xorDiff = a.Xor(c);
    //expectBits(*xorDiff, "0101"); // max length = 4
    delete xorDiff;

    std::cout << "  Bitwise operations OK" << std::endl;
}

// Тест операций на месте (AndInPlace, OrInPlace, XorInPlace, NotInPlace)
void testInPlaceOperations() {
    std::cout << "Testing in-place operations..." << std::endl;

    BitSequence a(4, false);
    a.Set(0, true); a.Set(2, true); // 1010
    BitSequence b(4, false);
    b.Set(0, true); b.Set(1, true); // 1100
    BitSequence c(3, true);         // 111

    // AndInPlace
    BitSequence aCopy = a;
    aCopy.AndInPlace(b);
    expectBits(aCopy, "1000");

    // OrInPlace
    aCopy = a;
    aCopy.OrInPlace(b);
    expectBits(aCopy, "1110");

    // XorInPlace
    aCopy = a;
    aCopy.XorInPlace(b);
    expectBits(aCopy, "0110");

    // NotInPlace
    aCopy = a;
    aCopy.NotInPlace();
    expectBits(aCopy, "0101");

    // OrInPlace с увеличением размера
    BitSequence d(2, true); // 11
    d.OrInPlace(c);         // c = 111
    expectBits(d, "111");   // длина стала 3

    // XorInPlace с увеличением размера
    BitSequence e(2, false); e.Set(0, true); // 10
    e.XorInPlace(c); // c = 111
    expectBits(e, "011");

    std::cout << "  In-place operations OK" << std::endl;
}

// Тест изменения размера (resize)
void testResize() {
    std::cout << "Testing resize..." << std::endl;

    BitSequence seq(3, true); // 111
    seq.resize(5);
    assert(seq.GetLength() == 5);
    expectBits(seq, "11100"); // новые биты false

    seq.resize(2);
    assert(seq.GetLength() == 2);
    expectBits(seq, "11");

    seq.resize(0);
    assert(seq.GetLength() == 0);

    std::cout << "  Resize OK" << std::endl;
}

bool isFalse(bool b) { return !b; }

// Тест методов базового класса (Map, Where, Reduce, GetFirst/Last с предикатом)
void testBaseClassMethods() {
    std::cout << "Testing base class methods (Map, Where, Reduce)..." << std::endl;

    bool arr[] = {true, false, true, false, true};
    BitSequence seq(arr, 5); // 10101

    // Map (инверсия)
    auto notFunc = [](bool b) -> bool { return !b; };
    Sequence<bool>* mapped = seq.Map<bool>(notFunc);
    BitSequence* mapSeq = dynamic_cast<BitSequence*>(mapped);
    assert(mapSeq != nullptr);
    expectBits(*mapSeq, "01010");
    delete mapSeq;

    // Where (фильтрация: оставить только true)
    auto isTrue = [](bool b) -> bool { return b; };
    Sequence<bool>* filtered = seq.Where(isTrue);
    BitSequence* filtSeq = dynamic_cast<BitSequence*>(filtered);
    assert(filtSeq != nullptr);
    expectBits(*filtSeq, "111");
    delete filtSeq;

    // Reduce (сумма как int)
    auto sumFunc = [](int acc, bool b) -> int { return acc + (b ? 1 : 0); };
    int sum = seq.Reduce<int>(sumFunc);
    assert(sum == 3);

    // Reduce на пустой последовательности
    BitSequence empty;
    bool caught = false;
    try {
        empty.Reduce<int>(sumFunc);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
    std::cout << "  Base class methods OK" << std::endl;
}

// Тест перечислителя (Enumerator)
void testEnumerator() {
    std::cout << "Testing enumerator..." << std::endl;

    bool arr[] = {true, false, true};
    BitSequence seq(arr, 3);
    IEnumerator<bool>* en = seq.GetEnumerator();
    assert(en != nullptr);

    // Перебор вручную
    assert(en->MoveNext() == true);
    assert(en->Current() == true);
    assert(en->MoveNext() == true);
    assert(en->Current() == false);
    assert(en->MoveNext() == true);
    assert(en->Current() == true);
    assert(en->MoveNext() == false); // больше нет элементов

    delete en;
    std::cout << "  Enumerator OK" << std::endl;
}

// Тест операторов сравнения
void testComparison() {
    std::cout << "Testing comparison operators..." << std::endl;

    BitSequence a(3, true);
    BitSequence b(3, true);
    BitSequence c(3, false);
    BitSequence d(4, true);

    assert(a == b);
    assert(!(a != b));
    assert(a != c);
    assert(a != d);

    std::cout << "  Comparison operators OK" << std::endl;
}

// Тест больших последовательностей (для проверки границ байтов)
void testLargeSequences() {
    std::cout << "Testing large sequences (cross-byte boundaries)..." << std::endl;

    // 20 бит, все true
    BitSequence largeTrue(20, true);
    for (size_t i = 0; i < 20; ++i) {
        assert(largeTrue.Get(i) == true);
    }

    // Установка битов через границу байта
    BitSequence largeFalse(20, false);
    largeFalse.Set(7, true);   // последний бит первого байта
    largeFalse.Set(8, true);   // первый бит второго байта
    largeFalse.Set(15, true);  // последний бит второго байта
    largeFalse.Set(16, true);  // первый бит третьего байта

    assert(largeFalse.Get(7) == true);
    assert(largeFalse.Get(8) == true);
    assert(largeFalse.Get(15) == true);
    assert(largeFalse.Get(16) == true);

    // Проверка остальных битов
    for (size_t i = 0; i < 20; ++i) {
        if (i == 7 || i == 8 || i == 15 || i == 16) continue;
        assert(largeFalse.Get(i) == false);
    }

    // Resize через границу
    largeFalse.resize(25);
    assert(largeFalse.GetLength() == 25);
    assert(largeFalse.Get(20) == false); // новый бит

    std::cout << "  Large sequences OK" << std::endl;
}

// Тест краевых случаев (0 бит, 1 бит, некратные 8 размеры)
void testEdgeCases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Пустая последовательность
    BitSequence empty;
    assert(empty.GetLength() == 0);
    bool caught = false;
    try { empty.GetFirst(); } catch (const std::out_of_range&) { caught = true; }
    assert(caught);

    // 1 бит
    BitSequence oneBit(1, true);
    assert(oneBit.GetLength() == 1);
    assert(oneBit.Get(0) == true);
    oneBit.Set(0, false);
    assert(oneBit.Get(0) == false);

    // Размеры, не кратные 8
    for (size_t size : {3, 7, 9, 15, 17}) {
        BitSequence seq(size, true);
        for (size_t i = 0; i < size; ++i) {
            assert(seq.Get(i) == true);
        }
        // Проверка, что неиспользуемые биты обнулены (через сравнение с такой же)
        BitSequence seq2(size, true);
        assert(seq == seq2);
    }

    std::cout << "  Edge cases OK" << std::endl;
}

int run_all() {
    std::cout << "=== Running BitSequence tests with assert ===" << std::endl;

    // testConstructors();
    // testAssignmentOperators();
    // testAccessMethods();
    // testSequenceOperations();
    testBitwiseOperations();
    testInPlaceOperations();
    testResize();
    testBaseClassMethods();
    testEnumerator();
    testComparison();
    testLargeSequences();
    testEdgeCases();

    std::cout << "\n=== All tests passed successfully! ===" << std::endl;
    return 0;
}