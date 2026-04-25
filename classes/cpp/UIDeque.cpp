#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "../headers/SegmentedDeque.h"
#include "../headers/Sequence.h"
#include "../headers/UIDeque.h"

using T = int;
using DequeType = SegmentedDeque<T>;
using SeqType = Sequence<T>;

void safeReplace(DequeType *&current, SeqType *result) {
    if (result && result != current) {
        delete current;
        current = static_cast<DequeType *>(result);
        std::cout << "[Memory] Old instance deleted, replaced with new one.\n";
    }
}

SeqType *createTempSequence() {
    std::cout << "Введите количество элементов: ";
    size_t n = 0;
    std::cin >> n;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return nullptr;
    }
    if (n == 0) return nullptr;

    auto *temp = new DequeType(4);
    std::cout << "Введите элементы: ";
    for (size_t i = 0; i < n; ++i) {
        T val;
        std::cin >> val;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
        temp->AppendImpl(val);
    }
    return temp;
}

static void printMenu() {
    std::cout << "\n========== SegmentedDeque UI ==========\n"
            << "0.  Выход из программы\n"
            << "1.  Создать новый Deque (запрос длины сегмента)\n"
            << "2.  Отобразить текущий Deque\n"
            << "3.  Показать информацию (длина, пусто ли)\n"
            << "4.  Append (добавить в конец)\n"
            << "5.  Prepend (добавить в начало)\n"
            << "6.  InsertAt (вставить по индексу)\n"
            << "7.  DeleteAt (удалить по индексу)\n"
            << "8.  GetSubsequence (получить подпоследовательность)\n"
            << "9.  Concat (объединить с другой последовательностью)\n"
            << "10. FindSubsequence (поиск подпоследовательности)\n"
            << "11. PopFirst (извлечь первый)\n"
            << "12. PopLast (извлечь последний)\n"
            << "13. PeekFirst (посмотреть первый)\n"
            << "14. PeekLast (посмотреть последний)\n"
            << "15. Replace Empty (заменить на пустой экземпляр)\n"
            << "=========================================\n"
            << "Выбор: ";
}

int run() {
    DequeType *current = nullptr;
    int choice = -1;

    while (choice != 0) {
        printMenu();
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Некорректный ввод. Попробуйте снова.\n";
            continue;
        }

        try {
            switch (choice) {
                case 0: break;

                case 1: {
                    if (current) {
                        delete current;
                        std::cout << "[Memory] Previous instance deleted.\n";
                    }
                    size_t segLen = 0;
                    std::cout << "Введите длину сегмента (>=2): ";
                    std::cin >> segLen;
                    if (segLen < 2) throw std::invalid_argument("Длина сегмента должна быть >= 2");
                    current = new DequeType(segLen);
                    std::cout << "Новый Deque создан.\n";
                    break;
                }

                case 2:
                    if (!current) {
                        std::cout << "Сначала создайте Deque (пункт 1).\n";
                        break;
                    }
                    std::cout << *current << "\n";
                    break;

                case 3:
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    std::cout << "Длина: " << current->GetLength() << "\n";
                    std::cout << "Пусто: " << (current->IsEmpty() ? "Да" : "Нет") << "\n";
                    break;

                case 4: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    T val;
                    std::cout << "Значение для добавления в конец: ";
                    std::cin >> val;
                    safeReplace(current, current->AppendImpl(val));
                    std::cout << "Элемент добавлен.\n";
                    break;
                }

                case 5: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    T val;
                    std::cout << "Значение для добавления в начало: ";
                    std::cin >> val;
                    safeReplace(current, current->PrependImpl(val));
                    std::cout << "Элемент добавлен.\n";
                    break;
                }

                case 6: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    T val;
                    size_t idx;
                    std::cout << "Значение: ";
                    std::cin >> val;
                    std::cout << "Индекс вставки: ";
                    std::cin >> idx;
                    safeReplace(current, current->InsertAtImpl(val, idx));
                    std::cout << "Элемент вставлен.\n";
                    break;
                }

                case 7: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    size_t idx;
                    std::cout << "Индекс для удаления: ";
                    std::cin >> idx;
                    safeReplace(current, current->DelImpl(idx));
                    std::cout << "Элемент удален.\n";
                    break;
                }

                case 8: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    size_t start, end;
                    std::cout << "Начальный индекс: ";
                    std::cin >> start;
                    std::cout << "Конечный индекс: ";
                    std::cin >> end;
                    safeReplace(current, current->GetSubsequence(start, end));
                    std::cout << "Подпоследовательность получена.\n";
                    break;
                }

                case 9: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    auto *temp = createTempSequence();
                    if (!temp) {
                        std::cout << "Пустая последовательность, объединение отменено.\n";
                        break;
                    }
                    auto *res = current->ConcatImpl(*temp);
                    delete temp; // Временный объект удаляется сразу после использования
                    safeReplace(current, res);
                    std::cout << "Объединение выполнено.\n";
                    break;
                }

                case 10: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    auto *sub = createTempSequence();
                    if (!sub) {
                        std::cout << "Пустая подпоследовательность.\n";
                        break;
                    }
                    int pos = current->FindSubsequence(*sub);
                    delete sub; // Удаляем временный объект
                    if (pos != -1) std::cout << "Найдено, начало по индексу: " << pos << "\n";
                    else std::cout << "Подпоследовательность не найдена.\n";
                    break;
                }

                case 11: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    T val = current->PopFirst();
                    std::cout << "Извлечен первый элемент: " << val << "\n";
                    break;
                }

                case 12: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    T val = current->PopLast();
                    std::cout << "Извлечен последний элемент: " << val << "\n";
                    break;
                }

                case 13: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    T val = current->GetFirst();
                    std::cout << "Первый элемент: " << val << "\n";
                    break;
                }

                case 14: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    T val = current->GetLast();
                    std::cout << "Последний элемент: " << val << "\n";
                    break;
                }

                case 15: {
                    if (!current) {
                        std::cout << "Сначала создайте Deque.\n";
                        break;
                    }
                    auto *res = current->CreateEmpty();
                    safeReplace(current, res);
                    std::cout << "Заменено на пустой экземпляр.\n";
                    break;
                }

                default:
                    std::cout << "Неверный выбор. Попробуйте снова.\n";
                    break;
            }
        } catch (const std::exception &e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }
    delete current;
    std::cout << "Завершение работы. Память освобождена.\n";
    return 0;
}
