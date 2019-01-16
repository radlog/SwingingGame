// EXCLUDED FROM BUILD, PLEASE DO NOT MARK THIS


//#include "pch.h"
//#include "ParticleFactory.h"
//
//
//ParticleFactory::ParticleFactory() : Model(CB_STATE_TIME_SCALED)
//{
//	shader_file_ = "particle_shader.hlsl";
//	
//}
//
//
//ParticleFactory::~ParticleFactory()
//{
//}
//
//int ParticleFactory::create_particle()
//{
//	
//	HRESULT hr = S_OK;
//	XMFLOAT3 vertices[6] =
//	{
//		XMFLOAT3(-1.0f,-1.0f,0.0f),
//		XMFLOAT3(1.0f,1.0f,0.0f),
//		XMFLOAT3(-1.0f,1.0f,0.0f),
//		XMFLOAT3(-1.0f,-1.0f,0.0f),
//		XMFLOAT3(1.0f,-1.0f,0.0f),
//		XMFLOAT3(1.0f,1.0f,0.0f),
//
//	};
//	numverts_ = 6;
//
//	D3D11_RASTERIZER_DESC rasterizer_desc;
//	ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));
//	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
//	rasterizer_desc.CullMode = D3D11_CULL_NONE;//sets no culling
//	hr = device_->CreateRasterizerState(&rasterizer_desc, &rasterizer_solid_);
//	rasterizer_desc.CullMode = D3D11_CULL_BACK;//sets backface culling
//	hr = device_->CreateRasterizerState(&rasterizer_desc, &rasterizer_particle_);
//	//create the vert buffer
//	D3D11_BUFFER_DESC bufferDesc;
//	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
//	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;//both cpu and gpu
//	bufferDesc.ByteWidth = sizeof(XMFLOAT3) * 6/*VERTCOUNT*/;
//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//edit on the cpu
//
//	hr = device_->CreateBuffer(&bufferDesc, NULL, &vertex_buffer_);
//
//	if (FAILED(hr)) return 0;
//
//	//copy verts to buffer
//	D3D11_MAPPED_SUBRESOURCE ms;
//
//	//mapping = locking the buffer which allows writing
//	immediate_context_->Map(vertex_buffer_, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
//	memcpy(ms.pData, vertices, sizeof(vertices));//copy data
//	//unlock the buffer
//	immediate_context_->Unmap(vertex_buffer_, NULL);
//
//	create_particle_buffer();
//	compile_shaders();
//	set_default_input_layout();
//
//	return 0;
//
//}
//
//void ParticleFactory::draw(const XMMATRIX* view_projection, const D3D_PRIMITIVE_TOPOLOGY mode)
//{
//	Particle test;
//	test.color = XMFLOAT4(1.0f, 0.0f, 0.3f, 1.0f);
//	test.gravity = 1;
//	test.position = XMFLOAT3(0.0f, 3.0f, 14);
//	test.velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
//
//	draw_one(&test, view_projection,mode);
//}
//
//void ParticleFactory::draw_one(Particle* one, const XMMATRIX* view_projection, const D3D_PRIMITIVE_TOPOLOGY mode)
//{
//	UINT stride = sizeof(XMFLOAT3);
//	auto world_position = XMMatrixScalingFromVector(XMVectorSet(0.3f, 0.3f, 0.3f, 0)) *
//		XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(0, XMConvertToRadians(180), 0)) *
//		XMMatrixTranslationFromVector(XMVectorSet(one->position.x, one->position.y, one->position.z, 0));
//	update_particle_buffer(world_position * *view_projection,one->color);
//
//	D3D11_INPUT_ELEMENT_DESC iedesc[] =
//	{
//		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
//		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
//		{"COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0}
//	};
//	set_input_layout(iedesc, ARRAYSIZE(iedesc));
//
//	immediate_context_->IASetInputLayout(input_layout_);
//	immediate_context_->UpdateSubresource(constant_buffer_, 0, nullptr, get_constant_buffer_state(), 0, 0);
//
//	// set the constant buffer(s)
//	immediate_context_->VSSetConstantBuffers(0, 1, &constant_buffer_);
//
//	// set the shader objects for the pixel and vertex shader as actives
//	immediate_context_->VSSetShader(v_shader_, nullptr, 0);
//	immediate_context_->PSSetShader(p_shader_, nullptr, 0);
//
//	// set a sampler and a texture as the standard way to draw
//	immediate_context_->PSSetSamplers(0, 1, &sampler0_);
//	immediate_context_->PSSetShaderResources(0, 1, &texture_);
//
//	// set topology mode defined in the parameter of the method
//	immediate_context_->IASetPrimitiveTopology(mode);
//
//	UINT offset = 0;
//	// set the vertex buffer to draw all the vertices with no offset
//	immediate_context_->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
//
//	immediate_context_->Draw(numverts_, 0);
//	
//
//}
//
//HRESULT ParticleFactory::create_particle_buffer()
//{
//	cb_ = &cb_particle_;
//	create_constant_buffer(sizeof(cb_particle_));
//
//	return S_OK;
//}
//
//void ParticleFactory::update_particle_buffer(const XMMATRIX world_view_projection, XMFLOAT4 color)
//{
//	cb_particle_.world_view_projection = world_view_projection;
//	cb_particle_.color = color;
//
//	cb_ = &cb_particle_;
//}
//
//
//
