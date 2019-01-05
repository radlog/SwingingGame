#pragma once
#include "InputHandler.h"

class D3Dfw
{
public:
	HINSTANCE h_inst;
	HWND h_wnd;

	static D3Dfw* get_instance();
	HRESULT initialise_window(HINSTANCE instance, int n_cmd_show, WNDPROC wnd_proc);
	HRESULT initialise_dx();
	HRESULT initialise_input();
	void clear_rtv() const;
	Input *input;

	ID3D11Device *device;
	ID3D11DeviceContext *immediate_context;
	IDXGISwapChain *swap_chain = nullptr;
private:
	D3Dfw();
	~D3Dfw();
	static D3Dfw *instance_;
	void cleanup() const;
	char g_title_[100] = "Swing to Win(g)";


	ID3D11Buffer *vertex_buffer_;
	ID3D11VertexShader *v_shader_;
	ID3D11PixelShader *p_shader_;
	ID3DBlob *vs_, *ps_;
	ID3D11InputLayout *input_layout_;
	ID3D11Buffer *constant_buffer_;


	D3D_DRIVER_TYPE driver_type_ = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL feature_level_ = D3D_FEATURE_LEVEL_11_0;


	ID3D11RenderTargetView *render_target_view_ = nullptr;
	ID3D11Buffer *constant_buffer0_;

	ID3D11DepthStencilView *z_buffer_;


};
