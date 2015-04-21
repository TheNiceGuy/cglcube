#include <stdlib.h>
#include <GL/glew.h>
#include "config.h"
#include "draw.h"
#include "cubies.h"

void cubies_init(struct cubies* st_cubies) {
    st_cubies->st_pos.x = 0;
    st_cubies->st_pos.y = 0;
    st_cubies->st_pos.z = 0;
}

void cubies_set_mesh(struct cubies* st_cubies, struct mesh* st_mesh) {
    st_cubies->st_mesh = st_mesh;
}

void cubies_free(struct cubies* st_cubies) {

}

void cubies_set_pos(struct cubies* st_cubies, int x, int y, int z) {
    st_cubies->st_pos.x = x;
    st_cubies->st_pos.y = y;
    st_cubies->st_pos.z = z;
}

void cubies_draw(struct cubies* st_cubies) {
    glPushMatrix();
    glTranslatef(st_cubies->st_pos.x,
                 st_cubies->st_pos.y,
                 st_cubies->st_pos.z);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, st_cubies->st_mesh->vboID);

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, 0);
    glNormalPointer(GL_FLOAT, 0, (void*)(st_cubies->st_mesh->elementsize));
    glColorPointer(3, GL_FLOAT, 0, (void*)(2*st_cubies->st_mesh->elementsize));

    glDrawArrays(GL_TRIANGLES, 0, 3*st_cubies->st_mesh->nface);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glPopMatrix();
}
