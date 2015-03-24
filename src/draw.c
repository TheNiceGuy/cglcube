#include "draw.h"

void draw_2Dgrid(int interval, float distance) {
    int counter;

    glPushMatrix();
    for(counter = 0; counter < interval+1; counter++) {
        glBegin(GL_LINES);
            glColor3ub(0, 0, 255);
            glVertex3f( counter*distance, interval*distance, 0);
            glVertex3f( counter*distance,-interval*distance, 0);
            glVertex3f(-counter*distance, interval*distance, 0);
            glVertex3f(-counter*distance,-interval*distance, 0);
            glColor3ub(255, 0, 0);
            glVertex3f( interval*distance, counter*distance, 0);
            glVertex3f(-interval*distance, counter*distance, 0);
            glVertex3f( interval*distance,-counter*distance, 0);
            glVertex3f(-interval*distance,-counter*distance, 0);
        glEnd();
    }
    glPopMatrix();
}
