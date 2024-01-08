#include "include/modeldata.h"

void printData(cgltf_data *data, const char *filename) {
    printf("Printing glTF Model Data for %s\n", filename);

    // Print Mesh Information
    printf("Meshes Count: %lu\n", data->meshes_count);
    for (cgltf_size i = 0; i < data->meshes_count; i++) {
        cgltf_mesh mesh = data->meshes[i];
        printf("  Mesh %lu: Primitives Count: %lu\n", i, mesh.primitives_count);

        for (cgltf_size j = 0; j < mesh.primitives_count; j++) {
            cgltf_primitive primitive = mesh.primitives[j];
            printf("    Primitive %lu: Attributes Count: %lu\n", j, primitive.attributes_count);

            for (cgltf_size k = 0; k < primitive.attributes_count; k++) {
                cgltf_attribute attribute = primitive.attributes[k];
                printf("      Attribute %lu: Type: %d\n", k, attribute.type);
            }

            printf("    Material Properties:\n");
            if (primitive.material != NULL) {
                cgltf_texture *baseColorTexture = primitive.material->pbr_metallic_roughness.base_color_texture.texture;
                if (baseColorTexture != NULL) {
                    printf("        Base Color Texture: %s\n", baseColorTexture->image->name);
                }
                cgltf_texture *normalTexture = primitive.material->normal_texture.texture;
                if (normalTexture != NULL) {
                    printf("        Normal RenderDataMap: %s\n", normalTexture->image->name);
                }
                cgltf_texture *metallicRoughnessTexture = primitive.material->pbr_metallic_roughness.metallic_roughness_texture.texture;
                if (metallicRoughnessTexture != NULL) {
                    printf("        Metallic/Roughness RenderDataMap: %s\n", metallicRoughnessTexture->image->name);
                }
            } else {
                printf("        NOT SET\n");
            }
        }
    }

    // Print Image Information
    printf("Images Count: %lu\n", data->images_count);
    for (cgltf_size i = 0; i < data->images_count; i++) {
        cgltf_image image = data->images[i];
        printf("  Image %lu: Name: %s, URI: %s\n", i, image.name, image.uri);
    }

    // Additional data (like materials, nodes, etc.) can be printed similarly
    // exit(1);
}

cgltf_accessor *
getAttributeAccessorByType(cgltf_data *data, int meshIndex, int primitiveIndex, cgltf_attribute_type type) {
    cgltf_mesh *mesh = &data->meshes[meshIndex];
    cgltf_primitive *primitive = &mesh->primitives[primitiveIndex];
    cgltf_accessor *accessor = NULL;

    for (cgltf_size i = 0; i < primitive->attributes_count; i++) {
        if (primitive->attributes[i].type == type) {
            accessor = primitive->attributes[i].data;
        }
    }

    return accessor;
}

void readModelData(cgltf_data *data, ModelData *modelData) {
    // Meshes (lets just get the first mesh)
    cgltf_mesh *mesh = &data->meshes[0];
    cgltf_primitive *primitive = &mesh->primitives[0];
    cgltf_accessor *positionAccessor = getAttributeAccessorByType(data, 0, 0, cgltf_attribute_type_position);
    cgltf_accessor *normalAccessor = getAttributeAccessorByType(data, 0, 0, cgltf_attribute_type_normal);
    cgltf_accessor *uvAccessor = getAttributeAccessorByType(data, 0, 0, cgltf_attribute_type_texcoord);
    cgltf_accessor *tangentAccessor = getAttributeAccessorByType(data, 0, 0, cgltf_attribute_type_tangent);
    cgltf_accessor *indexAccessor = primitive->indices;

    readModelAccessorData(modelData, positionAccessor, normalAccessor, uvAccessor, indexAccessor, tangentAccessor);

    cgltf_texture *baseColorTexture = primitive->material->pbr_metallic_roughness.base_color_texture.texture;
    cgltf_texture *normalTexture = primitive->material->normal_texture.texture;
    cgltf_texture *metallicRoughnessTexture = primitive->material->pbr_metallic_roughness.metallic_roughness_texture.texture;

    //
    // Images (textures)
    //
    modelData->diffuseImageData = malloc(sizeof(ModelImageData));

    if (baseColorTexture != NULL && baseColorTexture->image->buffer_view->size > 0) {
        readImageData(baseColorTexture->image, modelData->diffuseImageData);
    } else {
        createDefaultImageData(modelData->diffuseImageData, onePixelDiffuseImage);
    }

    modelData->normalMapImageData = malloc(sizeof(ModelImageData));
    if (normalTexture != NULL && normalTexture->image->buffer_view->size > 0) {
        readImageData(normalTexture->image, modelData->normalMapImageData);
    } else {
        createDefaultImageData(modelData->normalMapImageData, onePixelNormalMap);
    }

    modelData->metallicRoughnessMapImageData = malloc(sizeof(ModelImageData));
    if (metallicRoughnessTexture != NULL && metallicRoughnessTexture->image->buffer_view->size > 0) {
        readImageData(metallicRoughnessTexture->image, modelData->metallicRoughnessMapImageData);
    } else {
        createDefaultImageData(modelData->metallicRoughnessMapImageData, onePixelMetallicRoughnessMap);
    }
}

void readImageData(const cgltf_image *image, ModelImageData *imageData) {
    if (image->buffer_view != NULL) {
        const uint8_t *bufferData = (uint8_t *) image->buffer_view->buffer->data + image->buffer_view->offset;
        size_t bufferSize = image->buffer_view->size;

        int width, height, channels;
        // Load image data with stbi_load_from_memory
        unsigned char *loadedImage = stbi_load_from_memory(bufferData, (int) bufferSize, &width, &height, &channels,
                                                           STBI_rgb_alpha);
        if (loadedImage) {
            // Assign width, height, and size to your struct
            imageData->image_width = width;
            imageData->image_height = height;
            imageData->image_size = width * height * 4; // 4 bytes per pixel (RGBA)

            // Allocate memory for and copy imageData
            imageData->image_data = (uint8_t *) malloc(imageData->image_size);
            if (imageData->image_data != NULL) {
                memcpy(imageData->image_data, loadedImage, imageData->image_size);
            } else {
                printf("Failed to allocate image data memory for %s", image->name);
                stbi_image_free(loadedImage);
                exit(1);
            }

            // Free the loaded image data
            stbi_image_free(loadedImage);
        } else {
            printf("Failed to load image data for %s", image->name);
            exit(1);
        }
    }
}

void readModelAccessorData(ModelData *modelData, const cgltf_accessor *positionAccessor,
                           const cgltf_accessor *normalAccessor, const cgltf_accessor *uvAccessor,
                           const cgltf_accessor *indexAccessor,
                           cgltf_accessor *tangentAccessor) {// Allocate and populate the vertex array
    if (positionAccessor && normalAccessor) {
        modelData->num_vertices = positionAccessor->count;
        modelData->vertices = malloc(sizeof(Vertex) * modelData->num_vertices);

        for (cgltf_size i = 0; i < modelData->num_vertices; i++) {
            float *pos = (float *) ((char *) positionAccessor->buffer_view->buffer->data +
                                    positionAccessor->buffer_view->offset +
                                    positionAccessor->offset +
                                    i * positionAccessor->stride);
            float *norm = (float *) ((char *) normalAccessor->buffer_view->buffer->data +
                                     normalAccessor->buffer_view->offset +
                                     normalAccessor->offset +
                                     i * normalAccessor->stride);

            float *tangent, *uv;

            static float defaultUV[2] = {0.0f, 0.0f};
            static float defaultTangent[4] = {1.0f, 0.0f, 0.0f, 1.0f};

            if (uvAccessor) {
                uv = (float *) ((char *) uvAccessor->buffer_view->buffer->data +
                                uvAccessor->buffer_view->offset +
                                uvAccessor->offset +
                                i * uvAccessor->stride);
            } else {
                uv = defaultUV;
            }

            if (tangentAccessor) {
                tangent = (float *) ((char *) tangentAccessor->buffer_view->buffer->data +
                                     tangentAccessor->buffer_view->offset +
                                     tangentAccessor->offset +
                                     i * tangentAccessor->stride);
            } else {
                tangent = defaultTangent;
            }

            modelData->vertices[i] = (Vertex) {
                    {pos[0],     pos[1],     pos[2]},
                    {norm[0],    norm[1],    norm[2]},
                    {uv[0],      uv[1]},
                    {tangent[0], tangent[1], tangent[2], tangent[3]}
            };

        }
    }

    // Allocate and populate the index array
    if (indexAccessor) {
        modelData->num_indices = indexAccessor->count;
        modelData->indices = malloc(sizeof(uint16_t) * modelData->num_indices);

        for (cgltf_size i = 0; i < modelData->num_indices; i++) {
            modelData->indices[i] = (uint16_t) cgltf_accessor_read_index(indexAccessor, i);
        }
    }
}

void createDefaultImageData(ModelImageData *modelData, const uint8_t *imageData) {
    int width = 1, height = 1, channels = 4; // Assuming RGBA format
    modelData->image_width = width;
    modelData->image_height = height;
    modelData->image_size = width * height * channels;

    // Allocate memory for the 1x1 image
    modelData->image_data = (uint8_t *) malloc(modelData->image_size);
    if (modelData->image_data != NULL) {
        memcpy(modelData->image_data, imageData, modelData->image_size);
    } else {
        printf("Failed to allocate image data memory for 1x1 image");
        exit(1);
    }
}

void printModelData(const ModelData *modelData) {
    if (!modelData) {
        printf("ModelData is NULL\n");
        return;
    }

    printf("Vertices:\n");
    for (size_t i = 0; i < modelData->num_vertices; i++) {
        printf("Vertex %zu: Position: (%f, %f, %f), Normal: (%f, %f, %f)\n",
               i,
               modelData->vertices[i].position[0], modelData->vertices[i].position[1],
               modelData->vertices[i].position[2],
               modelData->vertices[i].normal[0], modelData->vertices[i].normal[1], modelData->vertices[i].normal[2]);
    }

    printf("\nIndices:\n");
    for (size_t i = 0; i < modelData->num_indices; i++) {
        printf("Index %zu: %u\n", i, modelData->indices[i]);
    }
}

ModelData *loadModelData(const char *filename) {
    cgltf_options options = {0};
    cgltf_data *data = NULL;
    cgltf_result result = cgltf_parse_file(&options, filename, &data);

    if (result != cgltf_result_success) {
        fprintf(stderr, "Failed to parse GLTF file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    result = cgltf_load_buffers(&options, data, filename);

    if (result != cgltf_result_success) {
        cgltf_free(data);
        fprintf(stderr, "Failed to load GLTF buffers for file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    ModelData *modelData = malloc(sizeof(ModelData));
    printData(data, filename);
    readModelData(data, modelData);

    cgltf_free(data);

    return modelData;
}

void destroyModelData(ModelData *modelData) {
    if (modelData) {
        if (modelData->vertices) {
            free(modelData->vertices);
            modelData->vertices = NULL;
        }

        if (modelData->indices) {
            free(modelData->indices);
            modelData->indices = NULL;
        }

        modelData->num_vertices = 0;
        modelData->num_indices = 0;

        free(modelData->diffuseImageData->image_data);
        free(modelData->diffuseImageData);
        free(modelData->normalMapImageData->image_data);
        free(modelData->normalMapImageData);
        free(modelData->metallicRoughnessMapImageData->image_data);
        free(modelData->metallicRoughnessMapImageData);
        free(modelData);
    }
}