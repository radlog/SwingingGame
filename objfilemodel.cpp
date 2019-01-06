// turn off fopen warnings
#define _CRT_SECURE_NO_WARNINGS

#include "ObjFileModel.h"



// draw object
void ObjFileModel::draw() const
{
	UINT stride = sizeof(MODEL_POS_TEX_NORM_VERTEX);
	UINT offset = 0;
	immediate_context_->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
	immediate_context_->Draw(numverts, 0);
}

vector<XMVECTOR>  ObjFileModel::get_vertex_positions() const
{		
	vector<XMVECTOR> positions(numverts);
	for(size_t i = 0; i < numverts; i++)
	{
		positions[i] = XMLoadFloat3(&vertices[i].pos);
	}
	return positions;
}

// load object from obj file in constructor
ObjFileModel::ObjFileModel(LPCSTR filename, ID3D11Device* device, ID3D11DeviceContext* context)
{
	device_ = device;
	immediate_context_ = context;

	if(load_file(filename)==0)
	{
		// file not loaded, check debug output;
		this->filename="FILE NOT LOADED";
		return;
	}

	this->filename = filename;

	parse_file();

	create_vb();

	delete[] fbuffer_; // delete file buffer created in loadfile()
}


// load wavefront object file. adds terminating \n so last line of file can be correctly parsed as a 'line' later
// basic loader - only deals with vertices v, texcoords vt, normals vn 
//              - only copes with triangular meshes (no quads)
//              - doesn't deal with textures or materials
int ObjFileModel::load_file(LPCSTR fname)
{
	FILE* pFile = fopen(fname, "r"); // if changed to bin format will read carriage return \r (0d) as well as \n (0a) into fbuffer, may need to add \r checks(but seemed to work with basic test)
	if (pFile==NULL) { DXTRACE_MSG("Failed to open model file");DXTRACE_MSG(fname); return 0 ;}

	// get file size
	fseek(pFile, 0, SEEK_END);
	fbuffersize_ = ftell(pFile);
	rewind(pFile);

	// allocate memory for entire file size
	fbuffer_  = new char[fbuffersize_+1]; // 1 added to cope with adding a \n later in case file doesn't end with \n 
	if (fbuffer_ == NULL) {fclose(pFile); DXTRACE_MSG("Failed allocate memory for model file");DXTRACE_MSG(fname); return 0 ;}

	// copy file into memory
	actualsize_ = fread(fbuffer_,1,fbuffersize_,pFile); // actualsize may be less than fbuffersize in text mode as \r are stripped
	if (actualsize_ == 0) {fclose(pFile); DXTRACE_MSG("Failed to read model file");DXTRACE_MSG(fname); return 0 ;}

	// add a newline at end in case file does not, so can deal with whole buffer as a set of lines of text
	fbuffer_[actualsize_] = '\n'; fclose(pFile);

	return 1;
}
 

// uses concept of getting parsable tokens seperated by whitespace and '/'
// one line of file is parsed at a time, lines seperated by '\n'
void ObjFileModel::parse_file()
{
	tokenptr_=0; // token pointer points to first element of buffer

	int tokenstart, tokenlength;

	xyz temp_xyz;
	xy temp_xy;

	bool success;
	int line = 0;

	do
	{	
		line++; // keep track of current line number for error reporting

		if(!get_next_token(tokenstart, tokenlength)) continue; // get first token on line, go to next line if first token is \n

		// ADD FURTHER KEYWORDS HERE TO EXTEND CAPABILITIES
		if(strncmp(&fbuffer_[tokenstart], "v ", 2)==0) // VERTEX POSITION - note the space in the string is needed (see vt, etc)
		{
			success=true; // used to see if correct number of tokens left on line for this type of attribute
			success = success && get_next_token(tokenstart, tokenlength);
			temp_xyz.x = float(atof(&fbuffer_[tokenstart]));
			success = success && get_next_token(tokenstart, tokenlength);
			temp_xyz.y = float(atof(&fbuffer_[tokenstart]));
			success = success && get_next_token(tokenstart, tokenlength);
			temp_xyz.z = float(atof(&fbuffer_[tokenstart]));

			// if not correct number of tokens, display error in debug output
			if(!success) {char s[100] = "ERROR: Badly formatted vertex, line : "; _itoa(line, &s[strlen(s)], 10); strcat(s, " : "); strcat(s, filename.c_str());  DXTRACE_MSG(s); }

			position_list.push_back(temp_xyz); // add a new element to the list

		}
		else if(strncmp(&fbuffer_[tokenstart], "vt", 2)==0) // TEXTURE COORDINATES
		{
			success=true;
			success = success && get_next_token(tokenstart, tokenlength);
			temp_xy.x = float(atof(&fbuffer_[tokenstart]));
			success = success && get_next_token(tokenstart, tokenlength);
			temp_xy.y = float(atof(&fbuffer_[tokenstart]));

			if(!success) {char s[100] = "ERROR: Badly formatted texture coordinate, line : "; _itoa(line, &s[strlen(s)], 10); strcat(s, " : "); strcat(s, filename.c_str());  DXTRACE_MSG(s); }

			texcoord_list.push_back(temp_xy);
		}
		else if(strncmp(&fbuffer_[tokenstart], "vn", 2)==0)  // NORMALS
		{
			success=true;
			success = success && get_next_token(tokenstart, tokenlength);
			temp_xyz.x = float(atof(&fbuffer_[tokenstart]));
			success = success && get_next_token(tokenstart, tokenlength);
			temp_xyz.y = float(atof(&fbuffer_[tokenstart]));
			success = success && get_next_token(tokenstart, tokenlength);
			temp_xyz.z = float(atof(&fbuffer_[tokenstart]));

			if(!success) {char s[100] = "ERROR: Badly formatted normal, line : "; _itoa(line, &s[strlen(s)], 10); strcat(s, " : "); strcat(s, filename.c_str());  DXTRACE_MSG(s); }

			normal_list.push_back(temp_xyz);
		}
		else if(strncmp(&fbuffer_[tokenstart], "f ", 2)==0)  // FACE - only deals with triangles so far
		{
			int temp_ptr = tokenstart + 2; // skip "f "
			int slashes=0;
			bool adjacent_slash = false;

			// this works out how many elements are specified for a face, e.g.
			// f 1 2 3				-> 0 forward slashes = just position
			// f 1/1 2/2 3/3		-> 3 slashes = position and texcoords
			// f 1/1/1 2/2/2 3/3/3	-> 6 slashes = position, texcoords, normals
			// f 1//1 2//2 3//3		-> 6 slashes with adjacent = position, normals
			while(fbuffer_[temp_ptr] != '\n')
			{
				if(fbuffer_[temp_ptr] == '/')
				{
					slashes++;
					if(fbuffer_[temp_ptr-1] == '/') adjacent_slash=true;
				}
				temp_ptr++;
			}

			success=true;

			// Get 3 sets of indices per face
			for(int i=0; i<3; i++)
			{
				// get vertex index
				success = success && get_next_token(tokenstart, tokenlength);
				pindices.push_back(atoi(&fbuffer_[tokenstart]));

				if(slashes>=3&& adjacent_slash==false) // get texcoord index if required 
				{
					success = success && get_next_token(tokenstart, tokenlength);
					tindices.push_back(atoi(&fbuffer_[tokenstart]));
				}

				if(slashes==6 || adjacent_slash==true) // get normal index if required 
				{
					success = success && get_next_token(tokenstart, tokenlength);
					nindices.push_back(atoi(&fbuffer_[tokenstart]));
				}
			}

			if(!success) {char s[100] = "ERROR: Badly formatted face, line : "; _itoa(line, &s[strlen(s)], 10); strcat(s, " : "); strcat(s, filename.c_str());  DXTRACE_MSG(s); }
		}
	} while(get_next_line() == true);
}


// get next token. if \n is next token do not proceed, use get_next_line() to resume
bool ObjFileModel::get_next_token(int& tokenstart, int& tokenlength)
{
	tokenstart = tokenptr_; 
	tokenlength=1;

	while(fbuffer_[tokenptr_] == ' ' || fbuffer_[tokenptr_] == '\t' || fbuffer_[tokenptr_] == '/') tokenptr_++; //skip whitespace and '/'

	if(fbuffer_[tokenptr_] == '\n') { return false; } // keeps tokenptr pointing to \n as a token to indicate end of line
													// doesn't point to next token, dealt with in get_next_line()
	int token_end = tokenptr_ + 1;

	while(fbuffer_[token_end] != ' ' && fbuffer_[token_end] != '\t' && fbuffer_[token_end] != '\n' && fbuffer_[token_end] != '/') token_end++; // find length of token by finding next whitespace or '\n' or '/'

	tokenlength = token_end - tokenptr_;
	tokenstart = tokenptr_;
	tokenptr_+=tokenlength; //ready for next token

	return true;
}


// gets next line of buffer by skipping to next element after end of current line, returns false when end of buffer exceeded
bool ObjFileModel::get_next_line()
{
	// relies on getnexttoken()leaving tokenptr pointing to \n if encountered

	while(fbuffer_[tokenptr_] != '\n' && tokenptr_ < actualsize_) tokenptr_++; // skip to end of line

	tokenptr_++; // point to start of next line

	return tokenptr_ >= actualsize_ ? false : true;
}


// create Vertex buffer from parsed file data
bool ObjFileModel::create_vb()
{
	// create vertex array to pass to vertex buffer from parsed data
	numverts = pindices.size();

	vertices = new MODEL_POS_TEX_NORM_VERTEX[numverts]; // create big enough vertex array

	for(unsigned int i = 0; i< numverts; i++)
	{
		const int vindex = pindices[i]-1; // use -1 for indices as .obj files indices begin at 1

		// set position data
		vertices[i].pos.x = position_list[vindex].x;
		vertices[i].pos.y = position_list[vindex].y;
		vertices[i].pos.z = position_list[vindex].z;

		if(tindices.size() > 0)
		{ 
			// if there are any, set texture coord data
			const int tindex = tindices[i]-1;
			vertices[i].texcoord.x = texcoord_list[tindex].x;
			vertices[i].texcoord.y = texcoord_list[tindex].y;
		}

		if(nindices.size() > 0)
		{
			// if there are any, set normal data
			const int nindex = nindices[i]-1;
			vertices[i].normal.x = normal_list[nindex].x;
			vertices[i].normal.y = normal_list[nindex].y;
			vertices[i].normal.z = normal_list[nindex].z;
		}
	}

	// Set up and create vertex buffer
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(buffer_desc));
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;										// Used by CPU and GPU
	buffer_desc.ByteWidth = sizeof(vertices[0])*numverts;						// Total size of buffer
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// Use as a vertex buffer
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							// Allow CPU access
	const HRESULT hr = device_->CreateBuffer(&buffer_desc, nullptr, &vertex_buffer_);	// Create the buffer

	if(FAILED(hr))
    {
        return false;
    }

	indices = new unsigned int[numverts];
	for (size_t i = 0; i < numverts; i++)
	{
		indices[i] = i;
	}

	// Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	immediate_context_->Map(vertex_buffer_, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// Lock the buffer to allow writing
	memcpy(ms.pData, vertices, sizeof(vertices[0])*numverts);							// Copy the data
	immediate_context_->Unmap(vertex_buffer_, NULL);										// Unlock the buffer

	return true;
}


ObjFileModel::~ObjFileModel()
{
	// clean up memory used by object
	if(vertex_buffer_) vertex_buffer_->Release();

	delete [] vertices;

	position_list.clear();
	normal_list.clear();
	texcoord_list.clear();
}
