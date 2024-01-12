#ifndef GAME_VERTEX_H
#define GAME_VERTEX_H

typedef struct {
    float position[3];
    float normal[3];
    float uv[2];
    float tangent[4];
} Vertex;

typedef struct {
    float position[3];
} DebugVertex;


#endif //GAME_VERTEX_H
