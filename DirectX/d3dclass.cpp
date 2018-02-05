#include "d3dclass.h"
#include "Constants.h"

bool D3DClass::initialize(int width, int height, bool vsync_enabled, HWND h_window, bool full_screen, float screen_depth, float screen_near)
{
	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter = nullptr;
	IDXGIOutput* adapter_output = nullptr;
	unsigned int num_modes = 0;
	unsigned int numerator = 0;
	unsigned int denominator = 0;
	unsigned long long string_length = 0;
	DXGI_MODE_DESC* display_mode_list = nullptr;
	DXGI_ADAPTER_DESC adapter_description;
	DXGI_SWAP_CHAIN_DESC swap_chain_description;
	D3D_FEATURE_LEVEL feature_level;
	ID3D11Texture2D* back_buffer_ptr = nullptr;
	D3D11_TEXTURE2D_DESC depth_buffer_description;
	D3D11_DEPTH_STENCIL_DESC depth_stencil_description;
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_description;
	D3D11_RASTERIZER_DESC raster_description;
	D3D11_VIEWPORT viewport;
	float field_of_view = 0.0f;
	float screen_aspect = 0.0f;

	m_vsync_enabled = vsync_enabled;

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
		return false;

	if (FAILED(factory->EnumAdapters(0, &adapter)))
		return false;

	if (FAILED(adapter->EnumOutputs(0, &adapter_output)))
		return false;

	if (FAILED(adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL)))
		return false;

	if(!(display_mode_list = new DXGI_MODE_DESC[num_modes]))
		return false;

	if (FAILED(adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list)))
		return false;

	for (int i = 0; i < num_modes; i++)
	{
		if (display_mode_list[i].Width == (unsigned int)width &&
			display_mode_list[i].Height == (unsigned int)height)
		{
			numerator = display_mode_list[i].RefreshRate.Numerator;
			denominator = display_mode_list[i].RefreshRate.Denominator;
		}
	}

	if (FAILED(adapter->GetDesc(&adapter_description)))
		return false;

	m_video_card_memory = (int)(adapter_description.DedicatedVideoMemory / 1024 / 1024);
	if (wcstombs_s((size_t*)string_length, m_video_card_description, 128, adapter_description.Description, 128 != 0))
		return false;

	
	
	if (display_mode_list)
		delete[] display_mode_list;

	display_mode_list = nullptr;

	if (adapter_output)
		adapter_output->Release();

	adapter_output = nullptr;

	if (adapter)
		adapter->Release();

	adapter = nullptr;

	if (factory)
		factory->Release();

	factory = nullptr;



	ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));

	swap_chain_description.BufferCount = 1;
	
	swap_chain_description.BufferDesc.Width = width;
	swap_chain_description.BufferDesc.Height = height;

	swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;



	if (m_vsync_enabled)
	{
		swap_chain_description.BufferDesc.RefreshRate.Numerator = numerator;
		swap_chain_description.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swap_chain_description.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
	}

	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swap_chain_description.OutputWindow = h_window;

	swap_chain_description.SampleDesc.Count = 1;
	swap_chain_description.SampleDesc.Quality = 0;

	if (full_screen)
		swap_chain_description.Windowed = false;
	else
		swap_chain_description.Windowed = true;

	swap_chain_description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swap_chain_description.Flags = 0;


	feature_level = D3D_FEATURE_LEVEL_11_0;

	IDXGISwapChain* swap_chain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* device_context = nullptr;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &feature_level, 1,
												D3D11_SDK_VERSION, &swap_chain_description, &swap_chain,
												&device, NULL, &device_context)))
		return false;


	m_swap_chain.reset(swap_chain);
	swap_chain = nullptr;

	m_device.reset(device);
	device = nullptr;

	m_device_context.reset(device_context);
	device_context = nullptr;

	if (FAILED(m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_ptr)))
		return false;

	ID3D11RenderTargetView* render_target_view = nullptr;
	if (FAILED(m_device->CreateRenderTargetView(back_buffer_ptr, nullptr, &render_target_view)))
		return false;
	


	if (back_buffer_ptr)
		back_buffer_ptr->Release();

	back_buffer_ptr = nullptr;



	ZeroMemory(&depth_buffer_description, sizeof(depth_buffer_description));

	depth_buffer_description.Width = width;
	depth_buffer_description.Height = height;
	depth_buffer_description.MipLevels = 1;
	depth_buffer_description.ArraySize = 1;
	depth_buffer_description.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_buffer_description.SampleDesc.Count = 1;
	depth_buffer_description.SampleDesc.Quality = 0;
	depth_buffer_description.Usage = D3D11_USAGE_DEFAULT;
	depth_buffer_description.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_buffer_description.CPUAccessFlags = 0;
	depth_buffer_description.MiscFlags = 0;


	ID3D11Texture2D* depth_stencil_buffer = nullptr;
	if (FAILED(m_device->CreateTexture2D(&depth_buffer_description, nullptr, &depth_stencil_buffer)))
		return false;

	m_depth_stencil_buffer.reset(depth_stencil_buffer);
	depth_stencil_buffer = nullptr;



	ZeroMemory(&depth_stencil_description, sizeof(depth_stencil_description));

	depth_stencil_description.DepthEnable = true;
	depth_stencil_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_description.DepthFunc = D3D11_COMPARISON_LESS;
	
	depth_stencil_description.StencilEnable = true;
	depth_stencil_description.StencilReadMask = 0xFF;
	depth_stencil_description.StencilWriteMask = 0xFF;

	depth_stencil_description.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depth_stencil_description.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_description.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	ID3D11DepthStencilState* depth_stencil_state = nullptr;
	if (FAILED(m_device->CreateDepthStencilState(&depth_stencil_description, &depth_stencil_state)))
		return false;

	m_depth_stencil_state.reset(depth_stencil_state);
	depth_stencil_state = nullptr;

	m_device_context->OMSetDepthStencilState(m_depth_stencil_state.get(), 1);



	ZeroMemory(&depth_stencil_view_description, sizeof(depth_stencil_view_description));

	depth_stencil_view_description.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_description.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_description.Texture2D.MipSlice = 0;

	ID3D11DepthStencilView* depth_stencil_view = nullptr;
	if (FAILED(m_device->CreateDepthStencilView(m_depth_stencil_buffer.get(), &depth_stencil_view_description, &depth_stencil_view)))
		return false;
	
	m_depth_stencil_view.reset(depth_stencil_view);
	depth_stencil_view = nullptr;



	m_device_context->OMSetRenderTargets(1, &render_target_view, m_depth_stencil_view.get());
	m_render_target_view.reset(render_target_view);



	raster_description.AntialiasedLineEnable = false;
	raster_description.CullMode = D3D11_CULL_BACK;
	raster_description.DepthBias = 0;
	raster_description.DepthBiasClamp = 0.0f;
	raster_description.DepthClipEnable = true;
	raster_description.FillMode = D3D11_FILL_SOLID;
	raster_description.FrontCounterClockwise = false;
	raster_description.ScissorEnable = false;
	raster_description.SlopeScaledDepthBias = 0.0f;

	ID3D11RasterizerState* rasterize_state = nullptr;
	if (FAILED(m_device->CreateRasterizerState(&raster_description, &rasterize_state)))
		return false;
	m_rasterizer_state.reset(rasterize_state);
	rasterize_state = nullptr;

	m_device_context->RSSetState(m_rasterizer_state.get());



	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_device_context->RSSetViewports(1, &viewport);



	field_of_view = Constant::PI / 4.0f;
	screen_aspect = (float)width / (float)height;

	m_projection_matrix = XMMatrixPerspectiveFovLH(field_of_view, screen_aspect, screen_near, screen_depth);
	m_world_matrix = XMMatrixIdentity();
	m_ortho_matrix = XMMatrixOrthographicLH((float)width, (float)height, screen_near, screen_depth);

	return true;
}



void D3DClass::shutdown()
{
	if (m_swap_chain)
		m_swap_chain->SetFullscreenState(false, nullptr);

	if (m_rasterizer_state)
		m_rasterizer_state->Release();

	if (m_depth_stencil_view)
		m_depth_stencil_view->Release();

	if (m_depth_stencil_state)
		m_depth_stencil_state->Release();

	if (m_depth_stencil_buffer)
		m_depth_stencil_buffer->Release();

	if (m_render_target_view)
		m_render_target_view->Release();

	if (m_device_context)
		m_device_context->Release();

	if (m_device)
		m_device->Release();

	if (m_swap_chain)
		m_swap_chain->Release();
}



void D3DClass::beginScene(float red, float green, float blue, float alpha)
{
	float colour[4];

	colour[0] = red;
	colour[1] = green;
	colour[2] = blue;
	colour[3] = alpha;

	m_device_context->ClearRenderTargetView(m_render_target_view.get(), colour);
	m_device_context->ClearDepthStencilView(m_depth_stencil_view.get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void D3DClass::endScene()
{
	if (m_vsync_enabled)
		m_swap_chain->Present(1, 0);
	else
		m_swap_chain->Present(0, 0);
}



ID3D11Device* D3DClass::getRawDevice()
{
	return m_device.get();
}



ID3D11DeviceContext* D3DClass::getRawDeviceContext()
{
	return m_device_context.get();
}



void D3DClass::getProjectionMatrix(XMMATRIX& projection_matrix)
{
	projection_matrix = m_projection_matrix;
}



void D3DClass::getWorldMatrix(XMMATRIX& world_matrix)
{
	world_matrix = m_world_matrix;
}



void D3DClass::getOrthoMatrix(XMMATRIX& ortho_matrix)
{
	ortho_matrix = m_ortho_matrix;
}



void D3DClass::getVideoCardInfo(char* card_name, int& memory)
{
	strcpy_s(card_name, 128, m_video_card_description);
	memory = m_video_card_memory;
}
