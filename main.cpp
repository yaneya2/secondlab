#include <iostream>
#include <ostream>

#include "classes/headers/UI.h"
#include <windows.h>

#include "classes/headers/BItSequence.h"

int main() {
    system("chcp 65001");
    //runUI();
    BitSequence bits("1101011");
    std::cout << bits.toDecimalString() << std::endl;
    return 0;
}
