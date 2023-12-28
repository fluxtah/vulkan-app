package com.fluxtah.application.api

import com.fluxtah.application.MyApplication
import com.fluxtah.application.api.interop.*
import com.fluxtah.application.api.interop.model.Viewport
import kotlinx.cinterop.ExperimentalForeignApi
import kotlinx.cinterop.cValue
import kotlinx.cinterop.memScoped
import kotlinx.cinterop.pointed

interface Application {
    fun initialize()
    fun draw()
}

private lateinit var applicationInstance: Application

@OptIn(kotlin.experimental.ExperimentalNativeApi::class)
@CName("ktCreateApplication")
fun ktCreateApplication() {
    applicationInstance = MyApplication()
}

@OptIn(kotlin.experimental.ExperimentalNativeApi::class)
@CName("ktInitApplication")
fun ktInitApplication() {
    applicationInstance.initialize()
}

@OptIn(kotlin.experimental.ExperimentalNativeApi::class)
@CName("ktDrawApplication")
fun ktDrawApplication() {
    applicationInstance.draw()
}

enum class ShaderType {
    Vertex,
    Fragment
}

@OptIn(ExperimentalForeignApi::class)
fun Application.createGraphicsPipeline(name: String, block: GraphicsPipelineConfigurationScope.() -> Unit) : PipelineResource {
    val configurator = GraphicsPipelineConfigurationScope()
    block(configurator)
    println("============> HELLO KT")
    val layout = createPipelineLayoutFuncCallback!!.invoke(ktGetVkDevice(), ktGetVkDescriptorSetLayout())
    val renderPass = ktGetVkRenderPass(configurator.renderPassName!!)
    val vertexShaderName = configurator.shaders.firstOrNull { it.type == ShaderType.Vertex }
    val fragmentShaderName = configurator.shaders.firstOrNull { it.type == ShaderType.Fragment }
    val vertexShader = ktGetVkShaderModule(vertexShaderName!!.name)
    val fragmentShader = ktGetVkShaderModule(fragmentShaderName!!.name)
    val viewport = cValue<Viewport> {
        x = 0f
        y = 0f
        width = ktGetSwapChainExtent()!!.pointed.width.toFloat()
        height = ktGetSwapChainExtent()!!.pointed.height.toFloat()
        minDepth = 0f
        maxDepth = 1f
    }
    val pipeline = memScoped {
        createPipelineFuncCallback!!.invoke(ktGetVkDevice(), layout, renderPass!!, viewport.ptr, vertexShader!!, fragmentShader!!)
    }
    registerPipeline(name, pipeline, layout)
    return PipelineResource(name, pipeline)
}

@OptIn(ExperimentalForeignApi::class)
data class PipelineResource(val name: String, val pipeline: VkPipeline)

class GraphicsPipelineConfigurationScope {
    internal var renderPassName: String? = null
    internal val shaders = mutableListOf<ShaderConfig>()
    data class ShaderConfig(val name: String, val type: ShaderType)
}

fun GraphicsPipelineConfigurationScope.renderPass(name: String) {
    renderPassName = name
}
fun GraphicsPipelineConfigurationScope.shader(name: String, type: ShaderType) {
    shaders.add(GraphicsPipelineConfigurationScope.ShaderConfig(name, type))
}

@OptIn(ExperimentalForeignApi::class)
fun Application.createShader(name: String) : ShaderResource {
    val module = createShaderModuleFuncCallback!!.invoke(ktGetVkDevice(), name)
    registerShaderModule(name, module)
    return ShaderResource(name, module)
}

@OptIn(ExperimentalForeignApi::class)
data class ShaderResource(val name: String, val shaderModule: VkShaderModule)

@OptIn(ExperimentalForeignApi::class)
fun Application.createRenderPass(name: String) : RenderPassResource {
    val renderPass = createRenderPassFuncCallback!!.invoke(ktGetVulkanContext())
    registerRenderPass(name, renderPass)
    return RenderPassResource(name, renderPass)
}

@OptIn(ExperimentalForeignApi::class)
data class RenderPassResource(val name: String, val renderPass: VkRenderPass)
