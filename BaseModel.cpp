#include "BaseModel.h"


// CONSTRUCTOR methods

// loads the main constructor of the Model class in addition with an .obj file with the path defined by char * filename
BaseModel::BaseModel(const LPCSTR filename, const CB_STATE state) : BaseModel(state)
{
	load_obj_model(filename);
}

// main constructor of the Model class to initialize a state and pass the device and context instances
BaseModel::BaseModel(const CB_STATE state) : obj_file_model_(nullptr)
{
	dx_handle_ = D3Dfw::get_instance();

	this->device_ = dx_handle_->get_device();
	this->immediate_context_ = dx_handle_->get_immediate_context();

	if (device_ == nullptr || immediate_context_ == nullptr ) return;

	this->state_ = state;
	switch (state)
	{
	case CB_STATE_FULL: create_constant_buffer_full(); break;
	case CB_STATE_LIGHTED: create_constant_buffer_lighted(); break;
	case CB_STATE_TIME_SCALED: create_constant_buffer_time_scaled(); break;
	default: create_constant_buffer_simple(); break;
	}

	compile_shaders();
	set_default_input_layout();
	create_default_sampler_for_texture();
	load_texture();

}

// default desctructor
BaseModel::~BaseModel()
{
}

// load an .obj file with char * filename defining the path of it
HRESULT BaseModel::load_obj_model(const LPCSTR filename)
{
	obj_file_model_ = new ObjFileModel(filename, device_, immediate_context_);
	objfile_ = filename;
	if (obj_file_model_->filename == "FILE NOT LOADED") return S_FALSE;

	numverts_ = obj_file_model_->numverts;
	vertices_ = obj_file_model_->vertices;
	vert_size_ = sizeof(obj_file_model_->vertices[0]);
	indices_ = obj_file_model_->indices;
	num_indices_ = numverts_;

	update_default_vertex_buffer(vertices_, vert_size_  * numverts_);
	create_index_buffer();
	calculate_origin();

	return 0;
}

// loads primitive topology shapes using only vertices, the number of them and their single bytesize
HRESULT BaseModel::load_geo_model(void* vertices, const UINT numverts, const UINT single_vertex_bytesize)
{
	vert_size_ = single_vertex_bytesize;
	this->vertices_ = vertices;
	this->numverts_ = numverts;

	origin_ = XMVectorZero();

	const auto hr = update_default_vertex_buffer(vertices, vert_size_ * numverts);


	return hr;
}

// loads primitive topology shapes using indices
HRESULT BaseModel::load_geo_model(void* vertices, const UINT num_verts, const UINT single_vertex_bytesize, unsigned int *indices, const UINT num_indices)
{
	auto hr = load_geo_model(vertices, num_verts, single_vertex_bytesize);
	this->indices_ = indices;
	this->num_indices_ = num_indices;
	hr = create_index_buffer();
	//ZeroMemory(&num_indices_, sizeof(num_indices_));

	return hr;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// DRAW

void BaseModel::draw(const XMMATRIX view_projection, const bool use_simple_cb, const D3D11_PRIMITIVE_TOPOLOGY mode)
{	
	immediate_context_->IASetInputLayout(input_layout_);
	// use the simple constant buffer, if no other was specifically defined
	if (use_simple_cb) {
		cb_simple_.world_view_projection = view_projection;
		immediate_context_->UpdateSubresource(constant_buffer_, 0, nullptr, &cb_simple_, 0, 0);
	}
	else
	{
		cb_time_scaled_lighted_.world_view_projection = view_projection;
		immediate_context_->UpdateSubresource(constant_buffer_, 0, nullptr, get_constant_buffer_state(), 0, 0);
	}

	// set the constant buffer(s)
	immediate_context_->VSSetConstantBuffers(0, 1, &constant_buffer_);

	// set the shader objects for the pixel and vertex shader as actives
	immediate_context_->VSSetShader(v_shader_, nullptr, 0);
	immediate_context_->PSSetShader(p_shader_, nullptr, 0);

	// set a sampler and a texture as the standard way to draw
	immediate_context_->PSSetSamplers(0, 1, &sampler0_);
	immediate_context_->PSSetShaderResources(0, 1, &texture_);

	// set topology mode defined in the parameter of the method
	immediate_context_->IASetPrimitiveTopology(mode);

	UINT stride = vert_size_;
	UINT offset = 0;
	// set the vertex buffer to draw all the vertices with no offset
	immediate_context_->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);

	// draw either with or without indices depending on whether an objfilemodel or a primitive topology was used
	if (num_indices_)
	{
		immediate_context_->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
		immediate_context_->DrawIndexed(num_indices_, 0, 0);
	}
	else
	{
		immediate_context_->Draw(numverts_, 0);
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////



// INITIALIZATION methods

// compile shaders from file -> TODO:: change the way this works, because it may be not a good one for every situation
HRESULT BaseModel::compile_shaders()
{
	HRESULT hr = S_OK;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// load and compile pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *error;

	//D3DReadFileToBlob(shader_file_, &ps_);
	//D3DX11PreprocessShaderFromFile(shader_file_.c_str(),nullptr,nullptr,)
	hr = D3DX11CompileFromFile(shader_file_.c_str(), nullptr, nullptr, "VShader", "vs_4_0", 0, 0, nullptr, &vs_, &error, nullptr);
	hr = D3DX11CompileFromFile(shader_file_.c_str(), nullptr, nullptr, "PShader", "ps_4_0", 0, 0, nullptr, &ps_, &error, nullptr);

	// create shader objects
	hr = device_->CreateVertexShader(vs_->GetBufferPointer(), vs_->GetBufferSize(), nullptr, &v_shader_);
	hr = device_->CreatePixelShader(ps_->GetBufferPointer(), ps_->GetBufferSize(), nullptr, &p_shader_);



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return hr;
}

HRESULT BaseModel::set_default_input_layout()
{
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	return set_input_layout(iedesc, ARRAYSIZE(iedesc));
}

HRESULT BaseModel::set_input_layout(D3D11_INPUT_ELEMENT_DESC iedesc[], const int size)
{
	const HRESULT hr = device_->CreateInputLayout(iedesc, size, vs_->GetBufferPointer(), vs_->GetBufferSize(), &input_layout_);
	immediate_context_->IASetInputLayout(input_layout_);
	return hr;
}

void BaseModel::set_shader_file(LPCSTR shader_file)
{
	this->shader_file_ = shader_file;
	compile_shaders();
	set_default_input_layout();
}

HRESULT BaseModel::create_default_sampler_for_texture()
{
	D3D11_SAMPLER_DESC  sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	const HRESULT hr = device_->CreateSamplerState(&sampler_desc, &sampler0_);

	return hr;
}

HRESULT BaseModel::load_texture(LPCSTR filename)
{	
	return D3DX11CreateShaderResourceViewFromFile(device_, filename, nullptr, nullptr, &texture_, nullptr);
}

HRESULT BaseModel::load_normal(LPCSTR filename)
{
	return D3DX11CreateShaderResourceViewFromFile(device_, filename, nullptr, nullptr, &normal_, nullptr);
}

void BaseModel::update_constant_buffer_full(const XMMATRIX world_view_projection, const XMMATRIX view_projection, const XMVECTOR directional_light_vector,
                                        const XMVECTOR directional_light_color, const XMVECTOR ambient_light_color, const XMVECTOR rgb_amount, const float gameTime)
{
	cb_full_.world_view_projection = world_view_projection;
	cb_full_.view_projection = view_projection;
	cb_full_.directional_light_vector = directional_light_vector;
	cb_full_.directional_light_colour = directional_light_color;
	cb_full_.ambient_light_colour = ambient_light_color;
	cb_full_.rgb_amount = rgb_amount;
	cb_full_.game_time = gameTime;

	cb_ = &cb_full_;
	//enable_glowing = true;
	//if (enable_glowing)
	//{
	//	static bool glow;
	//	delta *= 10;
	//	if (constantBuffer.RedAmount <= 0.01f || constantBuffer.GreenAmount <= 0.01f || constantBuffer.BlueAmount <= 0.01f) glow = true;
	//	else if (constantBuffer.RedAmount >= 0.99f || constantBuffer.GreenAmount >= 0.99f || constantBuffer.BlueAmount >= 0.99f) glow = false;

	//	if (glow)
	//	{
	//		constantBuffer.RedAmount += 0.1f * delta;
	//		constantBuffer.GreenAmount += 0.1f * delta;
	//		constantBuffer.BlueAmount += 0.1f * delta;
	//	}
	//	else
	//	{
	//		constantBuffer.RedAmount -= 0.1f * delta;
	//		constantBuffer.GreenAmount -= 0.1f * delta;
	//		constantBuffer.BlueAmount -= 0.1f * delta;
	//	}
	//}
	//else
	//{
	//	constantBuffer.RedAmount = 1.0f;
	//	constantBuffer.GreenAmount = 1.0f;
	//	constantBuffer.BlueAmount = 1.0f;
	//}

	//camera->look_at(XMVectorSet(0.0, 0.0, -4.0, 0.0));
	//const auto cube_rotation = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(timer->totalTime() * 4, timer->totalTime() * 2, timer->totalTime() * 3));
	//constantBuffer.WorldViewProjection = XMMatrixIdentity() * view_projection;

	//XMMATRIX transpose;
	//transpose = XMMatrixTranspose(constantBuffer.WorldViewProjection);

	//constantBuffer.directional_light_colour = directional_light_colour;
	//constantBuffer.ambient_light_colour = ambient_light_colour;
	//constantBuffer.directional_light_vector = XMVector3Normalize(XMVector3Transform(directional_light_shines_from, transpose));

}

void BaseModel::update_constant_buffer_time_scaled(const XMMATRIX world_view_projection, const XMMATRIX view_projection, const XMVECTOR directional_light_vector,
                                               const XMVECTOR directional_light_color, const XMVECTOR ambient_light_color, const float game_time)
{
	cb_time_scaled_lighted_.world_view_projection = world_view_projection;
	cb_time_scaled_lighted_.view_projection = view_projection;
	cb_time_scaled_lighted_.directional_light_vector = directional_light_vector;
	cb_time_scaled_lighted_.directional_light_colour = directional_light_color;
	cb_time_scaled_lighted_.ambient_light_colour = ambient_light_color;
	cb_time_scaled_lighted_.game_time = game_time;

	cb_ = &cb_time_scaled_lighted_;
}

void BaseModel::update_constant_buffer_lighted(const XMMATRIX world_view_projection, const XMMATRIX view_projection,  const XMVECTOR directional_light_vector,
                                           const XMVECTOR directional_light_color, const XMVECTOR ambient_light_color)
{
	cb_lighted_.world_view_projection = world_view_projection;
	cb_lighted_.view_projection = view_projection;
	cb_lighted_.directional_light_vector = directional_light_vector;
	cb_lighted_.directional_light_colour = directional_light_color;
	cb_lighted_.ambient_light_colour = ambient_light_color;
}

//SphereCollider Model::get_collision_sphere() const
//{
//	return sphere_collider_;
//}

void BaseModel::update() const
{
	
}

void BaseModel::calculate_origin()
{

	if (obj_file_model_ == nullptr) return;
	vector<XMVECTOR> positions = obj_file_model_->get_vertex_positions();
	min_outer_vector_ = positions[0];
	max_outer_vector_ = positions[1];

	float distance = 0;


	for (size_t i = 0; i < positions.size(); i++)
	{
		const auto a = positions[i];
		for (auto j = i; j < positions.size(); j++)
		{
			const auto b = positions[j];
			if (dist(a,b) > distance)
			{
				min_outer_vector_ = b;
				max_outer_vector_ = a;
				distance = dist(a,b);
			}
		}
	}

	origin_ = (max_outer_vector_ + min_outer_vector_ ) / 2;	
}

//void Model::initialize_sphere_collider()
//{
//	sphere_collider_.local_position = origin_;
//	sphere_collider_.collision_radius = sqrt(pow(min_outer_vector_.x - max_outer_vector_.x, 2) + pow(min_outer_vector_.y - max_outer_vector_.y, 2) + pow(min_outer_vector_.z - max_outer_vector_.z, 2));
//	sphere_collider_.collision_radius = 1;
//}

HRESULT BaseModel::create_constant_buffer_full()
{
	cb_ = &cb_full_;
	return create_constant_buffer(sizeof(cb_full_));
}

HRESULT BaseModel::create_constant_buffer_time_scaled()
{
	cb_ = &cb_time_scaled_lighted_;
	return create_constant_buffer(sizeof(cb_time_scaled_lighted_));
}

HRESULT BaseModel::create_constant_buffer_lighted()
{
	cb_ = &cb_lighted_;
	return create_constant_buffer(sizeof(cb_lighted_));
}

HRESULT BaseModel::create_constant_buffer_simple()
{
	cb_ = &cb_simple_;
	return create_constant_buffer(sizeof(cb_simple_));
}

HRESULT BaseModel::create_constant_buffer(UINT byte_width)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_DEFAULT; // can use UpdateSubresrource() to update
	desc.ByteWidth = byte_width;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	return device_->CreateBuffer(&desc, nullptr, &constant_buffer_);
}

HRESULT BaseModel::update_default_vertex_buffer(void *vertices, const UINT byte_width)
{
	// Set up and create vertex buffer
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(buffer_desc));
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.ByteWidth = byte_width;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	const HRESULT hr = device_->CreateBuffer(&buffer_desc, nullptr, &vertex_buffer_);

	if (FAILED(hr)) return hr;


	D3D11_MAPPED_SUBRESOURCE ms;
	immediate_context_->Map(vertex_buffer_, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, byte_width);
	immediate_context_->Unmap(vertex_buffer_, NULL);

	return hr;
}

HRESULT BaseModel::create_index_buffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = num_indices_ * sizeof(unsigned int);
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices_;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;	

	return device_->CreateBuffer(&desc, &indexData, &index_buffer_);
}

void* BaseModel::get_constant_buffer_state()
{
	switch (state_)
	{
		case CB_STATE_SIMPLE: return &cb_simple_;
		case CB_STATE_FULL: return &cb_full_; 
		case CB_STATE_LIGHTED: return &cb_lighted_;
		case CB_STATE_TIME_SCALED: return &cb_time_scaled_lighted_;
		default: return &cb_simple_;
	}
}

void BaseModel::cleanup() const
{
	if (this == nullptr)return;
	if (device_) device_->Release();
	if (immediate_context_) immediate_context_->Release();
	if (v_shader_) v_shader_->Release();
	if (p_shader_)p_shader_->Release();
	if (vs_) vs_->Release();
	if (ps_) ps_->Release();
	if (input_layout_)input_layout_->Release();
	if (constant_buffer_)constant_buffer_->Release();
	if (sampler0_)sampler0_->Release();
	if (texture_)texture_->Release();
}

