#include <iostream>
#include <windows.h>
#include "classes/headers/MutableArraySequence.h"
#include "classes/headers/SegmentedDeque.h"
#include <tuple>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <vector>
#include <memory>
#include "classes/headers/UIDeque.h"

double Length(double g, double v, double alpha) {
    return v * v * sin(2.0 * alpha) / g;
}

std::tuple<double, double> solve_projectile(double g, double x, double v_start, double v_end, int count_step,
                                            double eps) {
    if (g < 0 || x < 0 || v_start < 0 || v_end < 0 || count_step < 0 || eps < 0) {
        throw std::invalid_argument("Invalid arguments");
    }
    double step = (v_end - v_start) / count_step;
    double v_current = 0;
    double alpha1 = 0, alpha2 = 0, alpha = 0;
    for (int i = 0; i < count_step; ++i) {
        v_current = step * i + v_start;
        alpha1 = 0;
        alpha2 = std::numbers::pi / 4;
        while (alpha2 - alpha1 > eps) {
            alpha = (alpha1 + alpha2) / 2;
            if ((Length(g, v_current, alpha) - x) * (Length(g, v_current, alpha2) - x) >= 0) {
                alpha2 = alpha;
            } else {
                alpha1 = alpha;
            }
        }
        if ((Length(g, v_current, (alpha2 + alpha1) / 2) - x) > 0) {
            return {v_current, (alpha2 + alpha1) / 2};
        }
    }
    return {0, 0};
}


int main() {
    system("chcp 65001");
    run();
    return 0;
}
