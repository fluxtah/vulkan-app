// Vertex __vertices[] = {
//     // Front face
//     {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}, // Bottom-left
//     {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},  // Bottom-right
//     {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},   // Top-right
//     {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},  // Top-left

//     // Back face
//     {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}}, // Bottom-left
//     {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},  // Bottom-right
//     {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},   // Top-right
//     {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},  // Top-left

//     // Left face
//     {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}}, // Bottom-left
//     {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},  // Top-left
//     {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},   // Top-right
//     {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},  // Bottom-right

//     // Right face
//     {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},  // Bottom-left
//     {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},   // Top-left
//     {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},  // Top-right
//     {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // Bottom-right

//     // Top face
//     {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},  // Bottom-left
//     {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},   // Bottom-right
//     {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},  // Top-right
//     {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // Top-left

//     // Bottom face
//     {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}}, // Bottom-left
//     {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},  // Bottom-right
//     {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},   // Top-right
//     {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},  // Top-left
// };

// uint16_t __indices[] = {
//     // Front face
//     0, 1, 2, 2, 3, 0,
//     // Back face
//     4, 7, 6, 6, 5, 4,

//     // Left face
//     8, 11, 10, 10, 9, 8,
//     // Right face
//     12, 15, 14, 14, 13, 12,

//     // Top face
//     16, 17, 18, 18, 19, 16,
//     // Bottom face
//     20, 21, 22, 22, 23, 20,
// };