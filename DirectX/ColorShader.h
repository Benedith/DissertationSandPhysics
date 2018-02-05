#pragma once

//System includes
#include <d3d11.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>

#include <fstream>
#include <d3dx11async.h>
//Custom Includes



class ColorShader
{
public:
	ColorShader() = default;
	~ColorShader() = default;

	bool initialize(ID3D11Device* device, HWND h_window);
	void shutdown();
	bool render(ID3D11DeviceContext* device_context, int vertex_count, int instance_count,
				DirectX::XMMATRIX world_matrix, DirectX::XMMATRIX view_matrix,
				DirectX::XMMATRIX projection_matrix);

private:

	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	bool initializeShader(ID3D11Device*, HWND, CHAR*, CHAR*);
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob* error_message, HWND h_window, CHAR* shader_file_name);

	bool setshaderParameters(ID3D11DeviceContext* device_context, DirectX::XMMATRIX world_matrix, DirectX::XMMATRIX view_matrix, DirectX::XMMATRIX projection_matrix);
	void renderShader(ID3D11DeviceContext* device_context, int vertex_count, int instance_count);

	ID3D11VertexShader* m_vertex_shader = nullptr;
	ID3D11PixelShader* m_pixel_shader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11Buffer* m_matrix_buffer = nullptr;
};