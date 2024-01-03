#ifndef APP_MODELDATA_H
#define APP_MODELDATA_H
#include "libs/cgltf.h"
#include "libs/stb_image.h"

#include "include/vulkan/pipeline.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    uint8_t* image_data;
    size_t image_size;
    int image_width;
    int image_height;
} ModelImageData;

typedef struct {
    Vertex* vertices;
    size_t num_vertices;
    uint16_t* indices;
    size_t num_indices;
    ModelImageData *diffuseImageData;
    ModelImageData *normalMapImageData;
    ModelImageData *metallicRoughnessMapImageData;
} ModelData;

void readModelData(cgltf_data *data, ModelData *modelData);
void printModelData(const ModelData *modelData);
ModelData *loadModelData(const char *filename);
void destroyModelData(ModelData *modelData);

// Define a small buffer for a 1x1 white pixel in RGBA format
static const uint8_t onePixelDiffuseImage[4] = {50, 56, 123, 255 }; // white pixel
static const uint8_t onePixelNormalMap[4] = {128, 128, 255, 255}; // Neutral blue pixel
static const uint8_t onePixelMetallicRoughnessMap[4] = {0, 80, 0, 0}; // Half rough, zero metal

void printData(cgltf_data *data, const char *filename);

void createDefaultImageData(ModelImageData *modelData, const uint8_t *imageData);

void readModelAccessorData(ModelData *modelData, const cgltf_accessor *positionAccessor,
                           const cgltf_accessor *normalAccessor, const cgltf_accessor *uvAccessor,
                           const cgltf_accessor *indexAccessor, cgltf_accessor *tangentAccessor);

void readImageData(const cgltf_image *image, ModelImageData *imageData);

#endif //APP_MODELDATA_H
