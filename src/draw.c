#include <GL/gl.h>
#include "config.h"
#include "draw.h"

void draw_2Dgrid(int count, float distance) {
    int counter;

    glPushMatrix();
    for(counter = 0; counter < count+1; counter++) {
        glBegin(GL_LINES);
            glColor3ub(0, 0, 0);
            glVertex3f( counter*distance, 0, count*distance);
            glVertex3f( counter*distance, 0,-count*distance);
            glVertex3f(-counter*distance, 0, count*distance);
            glVertex3f(-counter*distance, 0,-count*distance);
            glVertex3f( count*distance, 0, counter*distance);
            glVertex3f(-count*distance, 0, counter*distance);
            glVertex3f( count*distance, 0,-counter*distance);
            glVertex3f(-count*distance, 0,-counter*distance);
        glEnd();
    }
    glPopMatrix();
}

void draw_guide() {
    glPushMatrix();
    glBegin(GL_LINES);
        glColor3ub(255, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glColor3ub(0, 255, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glColor3ub(0, 0, 255);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
    glEnd();
    glPopMatrix();
}
