#include "include/pipelines/debug/debug_pipeline.h"

VkPipeline createDebugPipeline(
        VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, Viewport viewport,
        VkShaderModule vertShaderModule, VkShaderModule fragShaderModule) {

    VkPipelineShaderStageCreateInfo *shaderStages = createBasicShaderStages(vertShaderModule, fragShaderModule);

    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0; // Binding index
    bindingDescription.stride = sizeof(DebugVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // Move to the next data entry after each vertex

    VkVertexInputAttributeDescription attributeDescriptions[1];

    // Position attribute
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0; // location = 0 in the shader
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Pointer to binding descriptions
    vertexInputInfo.vertexAttributeDescriptionCount = 1;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions; // Pointer to attribute descriptions

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport vkViewport = viewportToVkViewport(&viewport);

    VkRect2D scissor = viewportToScissor(&viewport);

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &vkViewport; // This can be dynamic
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor; // This can also be dynamic

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;         // If set to VK_TRUE, fragments beyond near and far planes are clamped
    rasterizer.rasterizerDiscardEnable = VK_FALSE;  // If set to VK_TRUE, geometry never passes through the rasterizer stage
    rasterizer.polygonMode = VK_POLYGON_MODE_LINE;  // Determines how fragments are generated. For filling the area of the polygon with fragments, use VK_POLYGON_MODE_FILL.
    rasterizer.lineWidth = 1.0f;                    // Line width (greater than 1.0f requires enabling wideLines GPU feature)
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;    // Type of face culling to use
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // Specifies the vertex order for faces to be considered front-facing
    rasterizer.depthBiasEnable = VK_FALSE;          // Set to VK_TRUE if you want to enable depth bias (good for shadow mapping)
    // rasterizer.depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor are ignored if depthBiasEnable is VK_FALSE

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;               // Enable for better anti-aliasing
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // Number of samples to use per fragment
    // multisampling.minSampleShading, pSampleMask, alphaToCoverageEnable, and alphaToOneEnable are ignored if sampleShadingEnable is VK_FALSE

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE; // Set to VK_TRUE if you need blending
    // Other properties like srcColorBlendFactor, dstColorBlendFactor, colorBlendOp, srcAlphaBlendFactor, dstAlphaBlendFactor, alphaBlendOp are ignored if blendEnable is VK_FALSE

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;   // Set to VK_TRUE to enable logical operation
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Logical operation to use (ignored if logicOpEnable is VK_FALSE)
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;                       // Vertex and Fragment shaders
    pipelineInfo.pStages = shaderStages;               // Shader stages set up previously
    pipelineInfo.pVertexInputState = &vertexInputInfo; // Vertex input state
    pipelineInfo.pInputAssemblyState = &inputAssembly; // Input assembly state
    pipelineInfo.pViewportState = &viewportState;      // Viewport and scissor state
    pipelineInfo.pRasterizationState = &rasterizer;    // Rasterization state
    pipelineInfo.pMultisampleState = &multisampling;   // Multisampling state
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending; // Color blending state
    // Dynamic state if you're using dynamic states like viewport, scissor (optional)
    pipelineInfo.layout = pipelineLayout; // PipelineConfig layout
    pipelineInfo.renderPass = renderPass; // Render pass
    pipelineInfo.subpass = 0;             // Index of the subpass where this pipeline will be used

    VkPipeline graphicsPipeline;
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &graphicsPipeline) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create DEBUG graphics pipeline\n");
        return NULL;
    }

    free(shaderStages);

    return graphicsPipeline;
}
