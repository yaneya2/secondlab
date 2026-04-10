#ifndef SECONDLAB_UI_H
#define SECONDLAB_UI_H

#ifndef UI_H
#define UI_H

#include "Sequence.h"

using DataType = int;

void runUI();

void printSequence(Sequence<DataType> *seq);

void processCurrentSequence(Sequence<DataType> *&seq);

Sequence<DataType> *createSequence();

#endif // UI_H
#endif //SECONDLAB_UI_H
