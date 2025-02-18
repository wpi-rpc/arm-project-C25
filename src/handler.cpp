#include "handler.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <unistd.h>

double nonZero(double x, double tolerance) {
    double magnitude = std::max(std::abs(x), std::abs(tolerance));
    double sign = (x >= 0) ? 1.0 : -1.0;
    return magnitude * sign;
}