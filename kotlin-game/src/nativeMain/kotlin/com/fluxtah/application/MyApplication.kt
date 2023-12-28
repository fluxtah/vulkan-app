package com.fluxtah.application

import com.fluxtah.application.api.*

class MyApplication : Application {
    override fun initialize() {
        createRenderPass("default")
        createShader("shaders/basic.vert.spv")
        createShader("shaders/basic.frag.spv")
        createGraphicsPipeline("default") {
            renderPass("default")
            shader("shaders/basic.vert.spv", ShaderType.Vertex)
            shader("shaders/basic.frag.spv", ShaderType.Fragment)
        }
    }

    override fun draw() {
        println("Draw game...")
    }
}
