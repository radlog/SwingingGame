#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "Geometry.h"
using namespace std;


class ObjFileModel
{
private:
	ID3D11Device*           device_;
	ID3D11DeviceContext*    immediate_context_;



//////////////////////////////////////////////////

	int load_file(LPCSTR filename);

	char* fbuffer_;
	long fbuffersize_; // filesize
	size_t actualsize_; // actual size of loaded data (can be less if loading as text files as ASCII CR (0d) are stripped out)

//////////////////////////////////////////////////

	void parse_file();
	bool get_next_line() ;
	bool get_next_token(int& tokenstart, int& tokenlength);

	unsigned int  tokenptr_;

//////////////////////////////////////////////////

	bool create_vb();

	ID3D11Buffer* vertex_buffer_; 

public:

	struct xyz { float x, y, z; };	//used for vertices and normals during file parse
	struct xy { float x, y; };		//used for texture coordinates during file parse




	string filename;

	ObjFileModel(LPCSTR filename, ID3D11Device* device, ID3D11DeviceContext* context);
	~ObjFileModel();

	void draw(void) const;

	vector <xyz> position_list;		// list of parsed positions
	vector <xyz> normal_list;		// list of parsed normals
	vector <xy> texcoord_list;		// list of parsed texture coordinates
	vector <int> pindices, tindices, nindices; // lists of indicies into above lists derived from faces

	POS_TEX_NORM_VERTEX* vertices;
	unsigned int *indices;
	unsigned int numverts;

	vector<XMVECTOR>  get_vertex_positions() const;
};


