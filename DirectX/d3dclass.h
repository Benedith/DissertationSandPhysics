#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>

using namespace DirectX;

class D3DClass
{
public:
	D3DClass() = default;
	~D3DClass() = default;

	bool initialize(int width, int height, bool vsync_enabled, HWND h_window, bool full_screen, float screen_depth, float screen_near);
	void shutdown();

	void beginScene(float red, float green, float blue, float alpha);
	void endScene();

	ID3D11Device* getRawDevice();
	ID3D11DeviceContext* getRawDeviceContext();

	void getProjectionMatrix(XMMATRIX& projection_matrix);
	void getWorldMatrix(XMMATRIX& world_matrix);
	void getOrthoMatrix(XMMATRIX& ortho_matrix);

	void getVideoCardInfo(char* card_name, int& memory);


private:
	bool m_vsync_enabled = false;
	int m_video_card_memory = -1;
	char m_video_card_description[128];
	
	std::unique_ptr<IDXGISwapChain> m_swap_chain;
	std::unique_ptr<ID3D11Device> m_device;
	std::unique_ptr<ID3D11DeviceContext> m_device_context;
	std::unique_ptr<ID3D11RenderTargetView> m_render_target_view;
	std::unique_ptr<ID3D11Texture2D> m_depth_stencil_buffer;
	std::unique_ptr<ID3D11DepthStencilState> m_depth_stencil_state;
	std::unique_ptr<ID3D11DepthStencilView> m_depth_stencil_view;
	std::unique_ptr<ID3D11RasterizerState> m_rasterizer_state;

	XMMATRIX m_projection_matrix;
	XMMATRIX m_world_matrix;
	XMMATRIX m_ortho_matrix;
};