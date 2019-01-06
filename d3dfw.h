#pragma once
#include "InputHandler.h"

// directX framework class to initialise directX and create an instance to be used as handle
class D3Dfw
{
public:
	HINSTANCE h_inst; // the instance of the window
	HWND h_wnd; // window handle

	static D3Dfw* get_instance(); // returns the unique instance of the directX handler class
	/**
	 * \brief initialises window with programs instance, whether it is visible and the corresponding process handling events
	 * \param instance program instance
	 * \param n_cmd_show show the window or not
	 * \param wnd_proc window process handler
	 * \return hardware result
	 */
	HRESULT initialise_window(HINSTANCE instance, int n_cmd_show, WNDPROC wnd_proc);
	/**
	 * \brief initialises graphics with directX
	 * \return hardware result
	 */
	HRESULT initialise_dx();
	/**
	 * \brief initialises the input
	 * \return hardware result
	 */
	HRESULT initialise_input(); 
	void clear_rtv() const; //  clears render target view to default color
	Input *input; // pointer to input handler class

	ID3D11Device *get_device() const; // returns the instance's device
	ID3D11DeviceContext *get_immediate_context() const; // returns the instance's context
	IDXGISwapChain *get_swap_chain() const; // returns the instance's swap chain 
	
private:
	D3Dfw();
	~D3Dfw();
	static D3Dfw *instance_; // singleton instance object
	void cleanup() const; // cleanup pointers to prevent memory leaks
	char g_title_[100] = "Swing to Win(g)"; // the window's title

	ID3D11Device *device_; // pointer hardware device
	ID3D11DeviceContext *immediate_context_; // pointer to context
	IDXGISwapChain *swap_chain_ = nullptr; // pointer to swap chain

	D3D_DRIVER_TYPE driver_type_ = D3D_DRIVER_TYPE_NULL; // define driver type
	D3D_FEATURE_LEVEL feature_level_ = D3D_FEATURE_LEVEL_11_0; // define feature level directX 11

	ID3D11RenderTargetView *render_target_view_ = nullptr; // pointer to render target view
	ID3D11DepthStencilView *z_buffer_; // pointer to depth stencil buffer


};
