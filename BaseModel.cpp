#include "BaseModel.h"
#include "MathHelper.h"

// CONSTRUCTOR methods

// loads the main constructor of the Model class in addition with an .obj file with the path defined by char * filename
BaseModel::BaseModel(const LPCSTR filename, const CB_STATE state) : BaseModel(state)
{
	load_obj_model(filename);
}

// main constructor of the Model class to initialize a state and pass the device and context instances
BaseModel::BaseModel(const CB_STATE state) : obj_file_model_(nullptr)
{
	dx_handle_ = D3Dfw::get_instance(); // get directX instance

	this->device_ = dx_handle_->get_device(); // get device
	this->immediate_context_ = dx_handle_->get_immediate_context(); // get device context

	if (device_ == nullptr || immediate_context_ == nullptr ) return; // do not create this object if there is no instance of directX

	this->state_ = state; // set constant buffer state
	// create constant buffer depending on set state
	switch (state)
	{
	case CB_STATE_FULL: create_constant_buffer_full(); break;
	case CB_STATE_LIGHTED: create_constant_buffer_lighted(); break;
	case CB_STATE_TIME_SCALED: create_constant_buffer_time_scaled(); break;
	default: create_constant_buffer_simple(); break;
	}

	compile_shaders(); // compile shaders
	set_default_input_layout(); // set input layout
	create_default_sampler_for_texture(); // create single texture sampler
	load_texture(); // load texture

}

// default desctructor
BaseModel::~BaseModel()
{
}

// load an .obj file with char * filename defining the path of it
HRESULT BaseModel::load_obj_model(const LPCSTR filename)
{
	obj_file_model_ = new ObjFileModel(filename, device_, immediate_context_); // load objfilemodel
	objfile_ = filename;
	if (obj_file_model_->filename == "FILE NOT LOADED") return S_FALSE; // return error when file not found

	numverts_ = obj_file_model_->numverts; // get and set number of vertices
	vertices_ = obj_file_model_->vertices; // get and set vertices
	vert_size_ = sizeof(obj_file_model_->vertices[0]); // calculate single vertex size
	indices_ = obj_file_model_->indices; // get and set indices
	num_indices_ = numverts_; // set number of indices

	update_default_vertex_buffer(vertices_, vert_size_  * numverts_); //update vertex buffer
	create_index_buffer(); // create index buffer

	return 0;
}

// loads primitive topology shapes using only vertices, the number of them and their single bytesize
HRESULT BaseModel::load_geo_model(void* vertices, const UINT numverts, const UINT single_vertex_bytesize)
{
	vert_size_ = single_vertex_bytesize; // set vertex size
	this->vertices_ = vertices; // set vertices
	this->numverts_ = numverts; // set number of vertices

	origin_ = XMVectorZero(); // set origin to zero

	const auto hr = update_default_vertex_buffer(vertices, vert_size_ * numverts); // update vertex buffer


	return hr;
}

// loads primitive topology shapes using indices
HRESULT BaseModel::load_geo_model(void* vertices, const UINT num_verts, const UINT single_vertex_bytesize, unsigned int *indices, const UINT num_indices)
{
	auto hr = load_geo_model(vertices, num_verts, single_vertex_bytesize); // overload method
	this->indices_ = indices; // set indices
	this->num_indices_ = num_indices; // set number of indices
	hr = create_index_buffer(); // create vertex buffer	

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
	// set input layout, since multiple shaders and constant buffers are used
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

// compile vertex and pixel shader
HRESULT BaseModel::compile_shaders()
{
	HRESULT hr = S_OK;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// load and compile pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *error;

	hr = D3DX11CompileFromFile(shader_file_.c_str(), nullptr, nullptr, "VShader", "vs_4_0", 0, 0, nullptr, &vs_, &error, nullptr); // compile vertex shader and save it to vs_ blob
	hr = D3DX11CompileFromFile(shader_file_.c_str(), nullptr, nullptr, "PShader", "ps_4_0", 0, 0, nullptr, &ps_, &error, nullptr); // compile pixel shader and save it to ps_ blob

	// create shader objects
	hr = device_->CreateVertexShader(vs_->GetBufferPointer(), vs_->GetBufferSize(), nullptr, &v_shader_); // create vertex shader and store it in v_shader_
	hr = device_->CreatePixelShader(ps_->GetBufferPointer(), ps_->GetBufferSize(), nullptr, &p_shader_); // create pixel shader and store it in p_shader_



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return hr;
}

// set default input layout
HRESULT BaseModel::set_default_input_layout()
{
	// layout description
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}, // vertex position
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0}, // vertex texture coordinate
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0}, // vertex normal
		{"COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0} // vertex color
	};
	return set_input_layout(iedesc, ARRAYSIZE(iedesc)); // use overload method
}

// set specific input layout
HRESULT BaseModel::set_input_layout(D3D11_INPUT_ELEMENT_DESC iedesc[], const int size)
{
	const auto hr = device_->CreateInputLayout(iedesc, size, vs_->GetBufferPointer(), vs_->GetBufferSize(), &input_layout_); // create input layout with device and store it in input_layout_
	immediate_context_->IASetInputLayout(input_layout_); // set layout to the device context
	return hr;
}

// set shader
void BaseModel::set_shader_file(LPCSTR shader_file)
{
	this->shader_file_ = shader_file; // set shader file name
	compile_shaders(); // compile new set shader
	set_default_input_layout(); // update input layout
}

// create a single sampler for the default texture
HRESULT BaseModel::create_default_sampler_for_texture()
{
	// sampler description
	D3D11_SAMPLER_DESC  sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc)); // free memory
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; 
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	const auto hr = device_->CreateSamplerState(&sampler_desc, &sampler0_); // create sampler with the device ant store it into sampler0_

	return hr;
}

// loads texture with given filename(path included)
HRESULT BaseModel::load_texture(const LPCSTR filename)
{	
	// load texture with path and name of filename and store it into texture_
	return D3DX11CreateShaderResourceViewFromFile(device_, filename, nullptr, nullptr, &texture_, nullptr); 
}

// loads normal texture with given filename(path included)
HRESULT BaseModel::load_normal(const LPCSTR filename)
{
	// load normal texture with path and name of filename and store it into normal_
	return D3DX11CreateShaderResourceViewFromFile(device_, filename, nullptr, nullptr, &normal_, nullptr);
}

// update constant buffer with all possible values
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
}

// update time scaled constant buffer 
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

// update lighted constant buffer
void BaseModel::update_constant_buffer_lighted(const XMMATRIX world_view_projection, const XMMATRIX view_projection,  const XMVECTOR directional_light_vector,
                                           const XMVECTOR directional_light_color, const XMVECTOR ambient_light_color)
{
	cb_lighted_.world_view_projection = world_view_projection;
	cb_lighted_.view_projection = view_projection;
	cb_lighted_.directional_light_vector = directional_light_vector;
	cb_lighted_.directional_light_colour = directional_light_color;
	cb_lighted_.ambient_light_colour = ambient_light_color;
}

// calculates the origin of a geometry with given vertices
void BaseModel::calculate_origin()
{
	if (obj_file_model_ == nullptr) return;
	auto positions = obj_file_model_->get_vertex_positions(); // get vertices of the model
	min_outer_vector_ = positions[0]; // set initial minimum vector
	max_outer_vector_ = positions[1]; // set initial maximum vector

	float distance = 0; // set initial distance

	// double loop through all vectors and measure distances
	for (size_t i = 0; i < positions.size(); i++)
	{
		const auto a = positions[i];
		for (auto j = i; j < positions.size(); j++)
		{
			const auto b = positions[j];
			// check if the distance of a and b i greater than the old compared vector distances
			if (dist(a,b) > distance)
			{
				min_outer_vector_ = b; // store new min vector
				max_outer_vector_ = a; // store new max vector
				distance = dist(a,b); // update new distance
			}
		}
	}

	origin_ = (max_outer_vector_ + min_outer_vector_ ) / 2;	// origin lies between the outer vectors
}

// create full constant buffer
HRESULT BaseModel::create_constant_buffer_full()
{
	cb_ = &cb_full_;
	return create_constant_buffer(sizeof(cb_full_));
}

// create time scaled constant buffer
HRESULT BaseModel::create_constant_buffer_time_scaled()
{
	cb_ = &cb_time_scaled_lighted_;
	return create_constant_buffer(sizeof(cb_time_scaled_lighted_));
}

// create lighted constant buffer
HRESULT BaseModel::create_constant_buffer_lighted()
{
	cb_ = &cb_lighted_;
	return create_constant_buffer(sizeof(cb_lighted_));
}

// create simple constant buffer
HRESULT BaseModel::create_constant_buffer_simple()
{
	cb_ = &cb_simple_;
	return create_constant_buffer(sizeof(cb_simple_));
}

// create constant buffer
HRESULT BaseModel::create_constant_buffer(UINT byte_width)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_DEFAULT; // can use UpdateSubresrource() to update
	desc.ByteWidth = byte_width; // set byte width
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // set flags to constant

	return device_->CreateBuffer(&desc, nullptr, &constant_buffer_); // create constant buffer and store it in constant_buffer_
}

// update vertex buffer with new vertices and their byte size
HRESULT BaseModel::update_default_vertex_buffer(void *vertices, const UINT byte_width)
{
	// Set up and create vertex buffer
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(buffer_desc)); // free memory
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC; // dynamic buffer use
	buffer_desc.ByteWidth = byte_width; // set byte width
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // say that this is a vertex buffer
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // can write

	const auto hr = device_->CreateBuffer(&buffer_desc, nullptr, &vertex_buffer_); // create vertex buffer and store it into vertex_buffer_

	if (FAILED(hr)) return hr;

	// map resource
	D3D11_MAPPED_SUBRESOURCE ms;
	immediate_context_->Map(vertex_buffer_, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // map the vertex buffer to the resource
	memcpy(ms.pData, vertices, byte_width); // copy vertex data to map
	immediate_context_->Unmap(vertex_buffer_, NULL); // unmap to allow new writes

	return hr;
}

// create index buffer
HRESULT BaseModel::create_index_buffer()
{
	// index buffer description
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); // free memory
	desc.Usage = D3D11_USAGE_DEFAULT; // default use
	desc.ByteWidth = num_indices_ * sizeof(unsigned int); // set byte width
	desc.CPUAccessFlags = 0; // set cpu flag 
	desc.MiscFlags = 0; 
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // set buffer type to index buffer

	// index data resource
	D3D11_SUBRESOURCE_DATA index_data;
	index_data.pSysMem = indices_; // copy index data
	index_data.SysMemPitch = 0; 
	index_data.SysMemSlicePitch = 0;	

	return device_->CreateBuffer(&desc, &index_data, &index_buffer_); // create index buffer and store it into index_buffer_;
}

// update constant buffer
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

// cleanup to prevent memory leak
void BaseModel::cleanup() const
{
	if (this == nullptr) return;
	if (v_shader_) v_shader_->Release();
	if (p_shader_)p_shader_->Release();
	if (vs_) vs_->Release();
	if (ps_) ps_->Release();
	if (input_layout_)input_layout_->Release();
	if (constant_buffer_)constant_buffer_->Release();
	if (sampler0_)sampler0_->Release();
	if (texture_)texture_->Release();
}

