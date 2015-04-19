#ifndef DRAW_H
#define DRAW_H

/**
 * This function draws a 2D grid on the X and Y axis.
 *
 * @param count The number of line to draw on each side of the origin.
 * @param distance The distance between each lines.
 */
void draw_2Dgrid(int count, float distance);

/**
 * This function draws a guide using the current matrix. It usefull when
 * analyzing how the matrix behaves. 
 *
 */
void draw_guide();

/** @file */
#endif
