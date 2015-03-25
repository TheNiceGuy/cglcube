#include "draw.h"

void draw_2Dgrid(int count, float distance) {
    int counter;

    glPushMatrix();
    for(counter = 0; counter < count+1; counter++) {
        glBegin(GL_LINES);
            glColor3ub(0, 0, 255);
            glVertex3f( counter*distance, count*distance, 0);
            glVertex3f( counter*distance,-count*distance, 0);
            glVertex3f(-counter*distance, count*distance, 0);
            glVertex3f(-counter*distance,-count*distance, 0);
            glColor3ub(255, 0, 0);
            glVertex3f( count*distance, counter*distance, 0);
            glVertex3f(-count*distance, counter*distance, 0);
            glVertex3f( count*distance,-counter*distance, 0);
            glVertex3f(-count*distance,-counter*distance, 0);
        glEnd();
    }
    glPopMatrix();
}
