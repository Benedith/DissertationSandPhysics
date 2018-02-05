#include "ColorShader.h"

bool ColorShader::initialize(ID3D11Device* device, HWND h_window)
{
	if (!initializeShader(device, h_window, "../DirectX/Color.vs", "../DirectX/Color.ps"))
		return false;

	return true;
}



void ColorShader::shutdown()
{
	shutdownShader();
}



bool ColorShader::render(ID3D11DeviceContext* device_context, int vertex_count, int instance_count,
							DirectX::XMMATRIX world_matrix, DirectX::XMMATRIX view_matrix,
							DirectX::XMMATRIX projection_matrix)
{
	if (!setshaderParameters(device_context, world_matrix, view_matrix, projection_matrix))
		return false;

	renderShader(device_context, vertex_count, instance_count);

	return true;
}



bool ColorShader::initializeShader(ID3D11Device* device, HWND h_window, CHAR* vsfilename, CHAR* psfilename)
{
	HRESULT result;
	ID3D10Blob* error_message;
	ID3D10Blob* vertex_shader_buffer;
	ID3D10Blob* pixel_shader_buffer;
	D3D11_INPUT_ELEMENT_DESC polygon_layout[6];
	unsigned int num_elements = 0;
	D3D11_BUFFER_DESC matrix_buffer_description;

	result = D3DX11CompileFromFile(vsfilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertex_shader_buffer, &error_message, NULL);

	if (FAILED(result))
	{
		if (error_message)
			outputShaderErrorMessage(error_message, h_window, vsfilename);
		else
			MessageBox(h_window, (LPSTR)vsfilename, "Missing Shader File", MB_OK);

		return false;
	}

	result = D3DX11CompileFromFile(psfilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixel_shader_buffer, &error_message, NULL);

	if (FAILED(result))
	{
		if (error_message)
			outputShaderErrorMessage(error_message, h_window, psfilename);
		else
			MessageBox(h_window, (LPSTR)psfilename, "Missing Shader File", MB_OK);

		return false;
	}

	result = device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(),
										nullptr, &m_vertex_shader);
	if (FAILED(result))
		return false;

	result = device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(), pixel_shader_buffer->GetBufferSize(),
										nullptr, &m_pixel_shader);
	if (FAILED(result))
		return false;

	polygon_layout[0].SemanticName = "POSITION";
	polygon_layout[0].SemanticIndex = 0;
	polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygon_layout[0].InputSlot = 0;
	polygon_layout[0].AlignedByteOffset = 0;
	polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[0].InstanceDataStepRate = 0;

	polygon_layout[1].SemanticName = "TEXCOORD";
	polygon_layout[1].SemanticIndex = 0;
	polygon_layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygon_layout[1].InputSlot = 0;
	polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[1].InstanceDataStepRate = 0;


	// Instance world mat
	polygon_layout[2].SemanticName = "INSTANCE";
	polygon_layout[2].SemanticIndex = 0;
	polygon_layout[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygon_layout[2].InputSlot = 1;
	polygon_layout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygon_layout[2].InstanceDataStepRate = 1;

	polygon_layout[3].SemanticName = "INSTANCE";
	polygon_layout[3].SemanticIndex = 1;
	polygon_layout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygon_layout[3].InputSlot = 1;
	polygon_layout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygon_layout[3].InstanceDataStepRate = 1;

	polygon_layout[4].SemanticName = "INSTANCE";
	polygon_layout[4].SemanticIndex = 2;
	polygon_layout[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygon_layout[4].InputSlot = 1;
	polygon_layout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[4].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygon_layout[4].InstanceDataStepRate = 1;

	polygon_layout[5].SemanticName = "INSTANCE";
	polygon_layout[5].SemanticIndex = 3;
	polygon_layout[5].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygon_layout[5].InputSlot = 1;
	polygon_layout[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[5].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygon_layout[5].InstanceDataStepRate = 1;


	num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

	result = device->CreateInputLayout(polygon_layout, num_elements, vertex_shader_buffer->GetBufferPointer(),
										vertex_shader_buffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
		return false;

	if (vertex_shader_buffer)
		vertex_shader_buffer->Release();

	vertex_shader_buffer = nullptr;

	if (pixel_shader_buffer)
		pixel_shader_buffer->Release();

	pixel_shader_buffer = nullptr;
	
	matrix_buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	matrix_buffer_description.ByteWidth = sizeof(MatrixBufferType);
	matrix_buffer_description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrix_buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrix_buffer_description.MiscFlags = 0;
	matrix_buffer_description.StructureByteStride = 0;

	result = device->CreateBuffer(&matrix_buffer_description, NULL, &m_matrix_buffer);

	if (FAILED(result))
		return false;

	return true;
}



void ColorShader::shutdownShader()
{
	if (m_matrix_buffer)
		m_matrix_buffer->Release();

	m_matrix_buffer = nullptr;

	if (m_layout)
		m_layout->Release();

	m_layout = nullptr;

	if (m_pixel_shader)
		m_pixel_shader->Release();

	m_pixel_shader = nullptr;

	if (m_vertex_shader)
		m_vertex_shader->Release();

	m_vertex_shader = nullptr;
}



void ColorShader::outputShaderErrorMessage(ID3D10Blob* error_message, HWND h_window, CHAR* shader_file_name)
{
	char* compile_errors = nullptr;
	unsigned long long buffer_size = 0;
	std::ofstream file_out;

	compile_errors = (char*)(error_message->GetBufferPointer());

	buffer_size = error_message->GetBufferSize();

	file_out.open("shader-error.txt");

	for (int i = 0; i < buffer_size; i++)
	{
		file_out << compile_errors[i];
	}

	file_out.close();

	if (error_message)
		error_message->Release();

	error_message = nullptr;

	MessageBox(h_window, "Error compiling shader. Check shader-error.txt for info.", (LPCSTR)shader_file_name, MB_OK);
}



bool ColorShader::setshaderParameters(ID3D11DeviceContext* device_context, DirectX::XMMATRIX world_matrix, DirectX::XMMATRIX view_matrix, DirectX::XMMATRIX projection_matrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	MatrixBufferType* data = nullptr;
	unsigned int buffer_number = 0;

	world_matrix = DirectX::XMMatrixTranspose(world_matrix);
	view_matrix = DirectX::XMMatrixTranspose(view_matrix);
	projection_matrix = DirectX::XMMatrixTranspose(projection_matrix);

	result = device_context->Map(m_matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return false;

	data = (MatrixBufferType*)mapped_resource.pData;

	data->world = world_matrix;
	data->view = view_matrix;
	data->projection = projection_matrix;

	device_context->Unmap(m_matrix_buffer, 0);

	buffer_number = 0;

	device_context->VSSetConstantBuffers(buffer_number, 1, &m_matrix_buffer);

	return true;
}



void ColorShader::renderShader(ID3D11DeviceContext* device_context, int vertex_count, int instance_count)
{
	device_context->IASetInputLayout(m_layout);
	device_context->VSSetShader(m_vertex_shader, nullptr, 0);
	device_context->PSSetShader(m_pixel_shader, nullptr, 0);

	device_context->DrawInstanced(vertex_count, instance_count, 0, 0);
}
