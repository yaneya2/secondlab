#ifndef SECONDLAB_UI_H
#define SECONDLAB_UI_H

#ifndef UI_H
#define UI_H

#include "Sequence.h"
#include "ArraySequence.h"
#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "ListSequence.h"
#include "MutableListSequence.h"
#include "ImmutableListSequence.h"

// Тип данных, с которым работает UI (можно изменить при необходимости)
using DataType = int;

// Функции для отображения меню и управления
void runUI();

// Вспомогательные функции для работы с последовательностями
void printSequence(Sequence<DataType>* seq);
void processCurrentSequence(Sequence<DataType>*& seq);
Sequence<DataType>* createSequence();

#endif // UI_H
#endif //SECONDLAB_UI_H