#include "../headers/UI.h"
#include "../headers/IEnumerator.h"
#include "../headers/Option.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

#include "../headers/ImmutableArraySequence.h"
#include "../headers/ImmutableListSequence.h"
#include "../headers/MutableArraySequence.h"
#include "../headers/MutableListSequence.h"

using namespace std;

// ----------------------------------------------------------------------
// Вспомогательные функции
// ----------------------------------------------------------------------
void printSequence(Sequence<DataType> *seq) {
    if (!seq) {
        cout << "Нет последовательности!" << endl;
        return;
    }
    cout << "Последовательность (длина " << seq->GetLength() << "): ";
    for (size_t i = 0; i < seq->GetLength(); ++i) {
        cout << seq->Get(i) << " ";
    }
    cout << endl;
}

void printMenu() {
    cout << "\n=== Меню операций ===" << endl;
    cout << "1. Вывести последовательность" << endl;
    cout << "2. Append (добавить в конец)" << endl;
    cout << "3. Prepend (добавить в начало)" << endl;
    cout << "4. InsertAt (вставить по индексу)" << endl;
    cout << "5. Get (получить элемент по индексу)" << endl;
    cout << "6. GetFirst / GetLast" << endl;
    cout << "7. GetSubsequence (получить подпоследовательность)" << endl;
    cout << "8. Concat (сцепить с другой последовательностью)" << endl;
    cout << "9. Map (применить функцию)" << endl;
    cout << "10. Where (фильтрация)" << endl;
    cout << "11. Reduce (свёртка)" << endl;
    cout << "12. GetFirst (с предикатом) / GetLast (с предикатом)" << endl;
    cout << "13. Итератор (обход с помощью IEnumerator)" << endl;
    cout << "14. Создать новую последовательность (текущая будет заменена)" << endl;
    cout << "0. Выйти из программы" << endl;
    cout << "Ваш выбор: ";
}

// Функция для ввода целого числа с проверкой
int inputInt(const string &prompt) {
    int val;
    cout << prompt;
    cin >> val;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Некорректный ввод. Повторите: ";
        cin >> val;
    }
    return val;
}

size_t inputSize(const string &prompt) {
    int val = inputInt(prompt);
    return static_cast<size_t>(val);
}

// Ввод массива элементов
vector<DataType> inputArray() {
    size_t n = inputSize("Введите количество элементов: ");
    vector<DataType> arr;
    cout << "Введите " << n << " элементов: ";
    for (size_t i = 0; i < n; ++i) {
        DataType x;
        cin >> x;
        arr.push_back(x);
    }
    return arr;
}

// Создание последовательности на основе выбора пользователя
Sequence<DataType> *createSequence() {
    cout << "\nВыберите тип последовательности:" << endl;
    cout << "1. MutableArraySequence" << endl;
    cout << "2. ImmutableArraySequence" << endl;
    cout << "3. MutableListSequence" << endl;
    cout << "4. ImmutableListSequence" << endl;
    cout << "5. Создать пустую последовательность (без ввода)" << endl;
    int choice = inputInt("Ваш выбор: ");

    if (choice == 5) {
        switch (inputInt("Выберите тип (1-4): ")) {
            case 1: return new MutableArraySequence<DataType>();
            case 2: return new ImmutableArraySequence<DataType>();
            case 3: return new MutableListSequence<DataType>();
            case 4: return new ImmutableListSequence<DataType>();
            default: return nullptr;
        }
    }

    vector<DataType> items = inputArray();
    const DataType *arr = items.data();
    size_t cnt = items.size();

    switch (choice) {
        case 1: return new MutableArraySequence<DataType>(arr, cnt);
        case 2: return new ImmutableArraySequence<DataType>(arr, cnt);
        case 3: return new MutableListSequence<DataType>(arr, cnt);
        case 4: return new ImmutableListSequence<DataType>(arr, cnt);
        default:
            cout << "Неверный выбор!" << endl;
            return nullptr;
    }
}

// Функция для демонстрации итератора
void testIterator(Sequence<DataType> *seq) {
    if (!seq) {
        cout << "Нет последовательности!" << endl;
        return;
    }
    IEnumerator<DataType> *it = seq->GetEnumerator();
    cout << "Итерация по последовательности: ";
    while (it->MoveNext()) {
        cout << it->Current() << " ";
    }
    cout << endl;
    delete it;
}

// Примеры функций для Map и Where
DataType square(DataType x) { return x * x; }
DataType addOne(DataType x) { return x + 1; }
bool isEven(DataType x) { return x % 2 == 0; }
bool isPositive(DataType x) { return x > 0; }
DataType sum(DataType a, DataType b) { return a + b; }
DataType product(DataType a, DataType b) { return a * b; }

void processCurrentSequence(Sequence<DataType> *&seq) {
    // system("chcp 65001");
    if (!seq) {
        cout << "Сначала создайте последовательность (пункт 14)." << endl;
        return;
    }

    int choice;
    do {
        printMenu();
        choice = inputInt("");
        switch (choice) {
            case 1: // Вывести
                printSequence(seq);
                break;
            case 2: // Append
            {
                DataType val;
                cout << "Введите значение: ";
                cin >> val;
                Sequence<DataType> *newSeq = seq->Append(val);
                if (newSeq != seq) {
                    // Для immutable: нужно заменить текущий указатель
                    delete seq;
                    seq = newSeq;
                    cout << "Создана новая последовательность (immutable)." << endl;
                } else {
                    cout << "Элемент добавлен (mutable)." << endl;
                }
                printSequence(seq);
                break;
            }
            case 3: // Prepend
            {
                DataType val;
                cout << "Введите значение: ";
                cin >> val;
                Sequence<DataType> *newSeq = seq->Prepend(val);
                if (newSeq != seq) {
                    delete seq;
                    seq = newSeq;
                    cout << "Создана новая последовательность (immutable)." << endl;
                } else {
                    cout << "Элемент добавлен в начало (mutable)." << endl;
                }
                printSequence(seq);
                break;
            }
            case 4: // InsertAt
            {
                DataType val;
                size_t idx;
                cout << "Введите значение: ";
                cin >> val;
                idx = inputSize("Введите индекс: ");
                try {
                    Sequence<DataType> *newSeq = seq->InsertAt(val, idx);
                    if (newSeq != seq) {
                        delete seq;
                        seq = newSeq;
                        cout << "Создана новая последовательность (immutable)." << endl;
                    } else {
                        cout << "Элемент вставлен (mutable)." << endl;
                    }
                    printSequence(seq);
                } catch (const exception &e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
                break;
            }
            case 5: // Get
            {
                size_t idx = inputSize("Введите индекс: ");
                try {
                    DataType val = seq->Get(idx);
                    cout << "Элемент [" << idx << "] = " << val << endl;
                } catch (const exception &e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
                break;
            }
            case 6: // GetFirst / GetLast
            {
                try {
                    cout << "GetFirst = " << seq->GetFirst().GetValueOrDefault() << endl;
                    cout << "GetLast  = " << seq->GetLast().GetValueOrDefault() << endl;
                } catch (const exception &e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
                break;
            }
            case 7: // GetSubsequence
            {
                size_t start = inputSize("Введите начальный индекс: ");
                size_t end = inputSize("Введите конечный индекс: ");
                try {
                    Sequence<DataType> *sub = seq->GetSubsequence(start, end);
                    cout << "Подпоследовательность: ";
                    printSequence(sub);
                    delete sub;
                } catch (const exception &e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
                break;
            }
            case 8: // Concat
            {
                cout << "Создайте последовательность для сцепления:" << endl;
                Sequence<DataType> *other = createSequence();
                if (!other) break;
                Sequence<DataType> *newSeq = seq->Concat(other);
                if (newSeq != seq) {
                    delete seq;
                    seq = newSeq;
                    cout << "Создана новая последовательность (immutable)." << endl;
                } else {
                    cout << "Сцепление выполнено (mutable)." << endl;
                }
                printSequence(seq);
                delete other;
                break;
            }
            case 9: // Map
            {
                cout << "Выберите функцию:" << endl;
                cout << "1. square(x) = x*x" << endl;
                cout << "2. addOne(x) = x+1" << endl;
                int funcChoice = inputInt("Ваш выбор: ");
                Sequence<DataType> *mapped = nullptr;
                if (funcChoice == 1)
                    mapped = seq->Map(square);
                else
                    mapped = seq->Map(addOne);
                cout << "Результат Map: ";
                printSequence(mapped);
                delete mapped;
                break;
            }
            case 10: // Where
            {
                cout << "Выберите предикат:" << endl;
                cout << "1. isEven (чётное)" << endl;
                cout << "2. isPositive (положительное)" << endl;
                int predChoice = inputInt("Ваш выбор: ");
                Sequence<DataType> *filtered = nullptr;
                if (predChoice == 1)
                    filtered = seq->Where(isEven);
                else
                    filtered = seq->Where(isPositive);
                cout << "Результат Where: ";
                printSequence(filtered);
                delete filtered;
                break;
            }
            case 11: // Reduce
            {
                cout << "Выберите функцию свёртки:" << endl;
                cout << "1. sum (сумма)" << endl;
                cout << "2. product (произведение)" << endl;
                int redChoice = inputInt("Ваш выбор: ");
                try {
                    DataType result;
                    if (redChoice == 1)
                        result = seq->Reduce(sum);
                    else
                        result = seq->Reduce(product);
                    cout << "Результат Reduce: " << result << endl;
                } catch (const exception &e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
                break;
            }
            case 12: // GetFirst / GetLast с предикатом
            {
                cout << "Выберите предикат:" << endl;
                cout << "1. isEven" << endl;
                cout << "2. isPositive" << endl;
                int predChoice = inputInt("Ваш выбор: ");
                bool (*pred)(DataType) = (predChoice == 1) ? isEven : isPositive;
                Option<DataType> first = seq->GetFirst(pred);
                Option<DataType> last = seq->GetLast(pred);
                cout << "Первый элемент, удовлетворяющий условию: ";
                if (first.HasValue()) cout << first.GetValue() << endl;
                else cout << "не найден" << endl;
                cout << "Последний элемент, удовлетворяющий условию: ";
                if (last.HasValue()) cout << last.GetValue() << endl;
                else cout << "не найден" << endl;
                break;
            }
            case 13: // Итератор
                testIterator(seq);
                break;
            case 14: // Создать новую
            {
                Sequence<DataType> *newSeq = createSequence();
                if (newSeq) {
                    delete seq;
                    seq = newSeq;
                    cout << "Создана новая последовательность." << endl;
                    printSequence(seq);
                }
                break;
            }
            case 0:
                cout << "Выход из подменю." << endl;
                break;
            default:
                cout << "Неверный пункт!" << endl;
        }
    } while (choice != 0);
}

void runUI() {
    cout << "=== Демонстрация работы Sequence (ArraySequence / ListSequence) ===\n";
    Sequence<DataType> *current = nullptr;
    int mainChoice;
    do {
        cout << "\nГлавное меню:" << endl;
        cout << "1. Создать последовательность" << endl;
        cout << "2. Работать с текущей последовательностью" << endl;
        cout << "0. Выйти" << endl;
        mainChoice = inputInt("Ваш выбор: ");
        switch (mainChoice) {
            case 1:
                if (current) delete current;
                current = createSequence();
                if (current) {
                    cout << "Последовательность создана." << endl;
                    printSequence(current);
                }
                break;
            case 2:
                if (!current) {
                    cout << "Сначала создайте последовательность (пункт 1)." << endl;
                } else {
                    processCurrentSequence(current);
                }
                break;
            case 0:
                cout << "До свидания!" << endl;
                break;
            default:
                cout << "Неверный выбор!" << endl;
        }
    } while (mainChoice != 0);
    delete current;
}
