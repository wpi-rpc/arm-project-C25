#ifndef HANDLER_H
#define HANDLER_H

/**
 * @brief Approximates a specified number to non-zero when within range of zero. 
 * @param x (double) : The specified number to approximate
 * @param tolerance (double) : The number zero is mapped to when approximating
 * @return The approximate non-zero value. 
 */
double nonZero(double x, double tolerance);

#endif