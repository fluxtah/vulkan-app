//
// Created by Ian Warwick on 12/01/2024.
//

#ifndef APP_DEBUG_CUBE_H
#define APP_DEBUG_CUBE_H

#include "include/vertex.h"

DebugVertex cubeVertices[] = {
        // Front face
        {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
        {-0.5f,  0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
        // Back face
        {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f},
        // Connectors
        {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f,  0.5f}
};

#endif //APP_DEBUG_CUBE_H
