#include <cstdint>
#include <implementation.hpp>
#include <vulkan/vulkan_core.h>
#include <except.hpp>
#include <debug.hpp>

///
void App::CreateGraphicsPipeline(Vulkan& vk)
{
  // --- Shaders ---
  auto vert_shader_code = Impl::ReadShaderCode("vert.spv");
  VkShaderModule vert_shader_module = Impl::CreateShaderModule(vk.device, vert_shader_code);
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created vertex shader module");
  vert_shader_code.clear();
  vert_shader_code.shrink_to_fit();
  
  auto frag_shader_code = Impl::ReadShaderCode("frag.spv");
  VkShaderModule frag_shader_module = Impl::CreateShaderModule(vk.device, frag_shader_code);
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created fragment shader module");
  frag_shader_code.clear();
  frag_shader_code.shrink_to_fit();
  
  VkPipelineShaderStageCreateInfo vert_shader_stage_info {};
  vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vert_shader_stage_info.module = vert_shader_module;
  vert_shader_stage_info.pName = "main";
  
  VkPipelineShaderStageCreateInfo frag_shader_stage_info {};
  frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_shader_stage_info.module = frag_shader_module;
  frag_shader_stage_info.pName = "main";

  VkPipelineShaderStageCreateInfo shader_stages[] = { 
    vert_shader_stage_info, frag_shader_stage_info
  };

  // --- Vertex Input ---
  VkPipelineVertexInputStateCreateInfo vertex_input_info {};
  vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  // bindings - is spacing between data and whether the data is per-vertex or per-instance
  vertex_input_info.vertexBindingDescriptionCount = 0;
  // attribute descriptions - is type of the attributes passed to the vertex shader, which binding to load them from and at which offset
  vertex_input_info.pVertexAttributeDescriptions = nullptr;

  // --- Input Assembly ---
  // Input assembly describes two things:
  // 1. what kind of gometry will be drawn from the vertices
  // 2. whether primtive restart should be enables
  VkPipelineInputAssemblyStateCreateInfo input_assembly {};
  input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly.primitiveRestartEnable = VK_FALSE;

  // --- Viewport and Scissor ---
  VkPipelineViewportStateCreateInfo viewport_state {};
  viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state.viewportCount = 1;
  viewport_state.scissorCount = 1;
  
  // --- Rasterizer ---
  VkPipelineRasterizationStateCreateInfo rasterizer {};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL; // fill polygons
  rasterizer.lineWidth = 1.0f; // should be extension enabled for value > 1.0f
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;

  // --- Multisampling ---
  VkPipelineMultisampleStateCreateInfo multisampling {};
  multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  // multisampling.minSampleShading = 1.0f;
  // multisampling.pSampleMask = nullptr;
  // multisampling.alphaToCoverageEnable = VK_FALSE;
  // multisampling.alphaToOneEnable = VK_FALSE;

  // --- Color Blending ---
  // Accomplish the following final color computation:
  // finalColor.rgb = newAlpha * newColor + (1 - newAlpha) * oldColor
  // finalColor.a = newAlpha.a;
  VkPipelineColorBlendAttachmentState color_blend_attachment {};
  color_blend_attachment.colorWriteMask = 
    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | 
    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment.blendEnable = VK_FALSE;
  // color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  // color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  // color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
  // color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  // color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  // color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo color_blending {};
  color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blending.logicOpEnable = VK_FALSE; // set to VK_TRUE to enable bitwise blending
  color_blending.logicOp = VK_LOGIC_OP_COPY;
  color_blending.attachmentCount = 1;
  color_blending.pAttachments = &color_blend_attachment;
  color_blending.blendConstants[0] = 0.0f;
  color_blending.blendConstants[1] = 0.0f;
  color_blending.blendConstants[2] = 0.0f;
  color_blending.blendConstants[3] = 0.0f;
  

  // --- Dynamic States ---
  // dynamic_states describes which parts of pipeline will be changes dynamically wothout it's recreation
  VkDynamicState dynamic_states[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };

  VkPipelineDynamicStateCreateInfo dynamic_state {};
  dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state.dynamicStateCount = 2;
  dynamic_state.pDynamicStates = dynamic_states;


  // --- Pipeline Layout ---
  VkPipelineLayoutCreateInfo pipeline_layout_info {};
  pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_info.setLayoutCount = 0;
  pipeline_layout_info.pSetLayouts = nullptr;
  pipeline_layout_info.pushConstantRangeCount = 0;
  pipeline_layout_info.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(vk.device, &pipeline_layout_info, nullptr, &vk.pipeline_layout) != VK_SUCCESS)
    throw Except::Pipeline_Layout_Creation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created pipeline layout");
  
  // --- Graphics Pipeline ---
  VkGraphicsPipelineCreateInfo pipeline_info {};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.stageCount = 2;
  pipeline_info.pStages = shader_stages;
  pipeline_info.pVertexInputState = &vertex_input_info;
  pipeline_info.pInputAssemblyState = &input_assembly;
  pipeline_info.pViewportState = &viewport_state;
  pipeline_info.pRasterizationState = &rasterizer;
  pipeline_info.pMultisampleState = &multisampling;
  // pipeline_info.pDepthStencilState = nullptr;
  pipeline_info.pColorBlendState = &color_blending;
  pipeline_info.pDynamicState = &dynamic_state;
  pipeline_info.layout = vk.pipeline_layout;
  pipeline_info.renderPass = vk.render_pass;
  pipeline_info.subpass = 0;
  pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

  if (vkCreateGraphicsPipelines(vk.device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &vk.graphics_pipeline) != VK_SUCCESS)
    throw Except::Graphics_Pipeline_Creation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created graphics pipeline");

  // --- Clearing Up ---
  vkDestroyShaderModule(vk.device, vert_shader_module, nullptr);
  vkDestroyShaderModule(vk.device, frag_shader_module, nullptr);
}


/// Creates shader module. Passed vector 'code' may be cleared after module creation.
VkShaderModule App::Impl::CreateShaderModule(VkDevice dev, const std::vector<char>& code)
{
  VkShaderModuleCreateInfo create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shader_module;
  if (vkCreateShaderModule(dev, &create_info, nullptr, &shader_module) != VK_SUCCESS)
    throw Except::Shader_Module_Creation_Failure{__FUNCTION__};
  return shader_module;
}


