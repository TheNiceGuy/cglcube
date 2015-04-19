#include <stdlib.h>
#include <GL/gl.h>
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

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(3, GL_FLOAT, 0, st_cubies->st_mesh->colorpointer);
    glVertexPointer(3, GL_FLOAT, 0, st_cubies->st_mesh->vertexpointer);
    glDrawElements(GL_TRIANGLES, 3*st_cubies->st_mesh->nface, GL_UNSIGNED_BYTE, st_cubies->st_mesh->indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glPopMatrix();
}
