#include "GraphicsClass.h"
#include "Constants.h"
#include "Model.h"

//Statics
D3DClass* GraphicsClass::m_direct_3d = new D3DClass();

bool GraphicsClass::initialize(int width, int height, HWND h_window)
{
	if(!m_direct_3d)
		return false;

	if (!m_direct_3d->initialize(width, height, Constant::VSYNC_ENABLED, h_window, Constant::FULL_SCREEN, Constant::SCREEN_DEPTH, Constant::SCREEN_NEAR))
	{
		MessageBox(h_window, " Could not initialize Direct3D ", "Error: ", MB_OK);
		return false;
	}

	if (!m_camera)
		return false;
	/*m_camera->setRotation(180.f, 10.f, 10.f);*/
	m_camera->setPosition(0.0f, -35.0f, -40.0f);

	if (!m_color_shader)
		return false;

	if (!m_color_shader->initialize(m_direct_3d->getRawDevice(), h_window))
	{
		MessageBox(h_window, "Could not initialize color shader object.", "ERROR", MB_OK);
		return false;
	}

	if (!m_swarm)
		return false;

	if (!m_swarm->Initialize("../DirectX/cube.txt", m_swarm->GetInstanceCount()))
	{
		MessageBox(h_window, "Could not initialize model object.", "ERROR", MB_OK);
		return false;
	}
	

	return true;
}



bool GraphicsClass::frame()
{
	if(!render())
		return false;

	return true;
}



void GraphicsClass::shutdown()
{
	if (m_camera)
	{
		delete[] m_camera;
		m_camera = nullptr;
	}

	/*if (m_color_shader)
	{
		m_color_shader->shutdown();
		delete[] m_color_shader;
	}
*/
	if (m_swarm)
	{
		m_swarm->Shutdown();
		delete[] m_swarm;
	}

	if (m_direct_3d)
	{
		m_direct_3d->shutdown();
		delete[] m_direct_3d;
	}
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
}



Camera* GraphicsClass::getCamera()
{
	return m_camera;
}



bool GraphicsClass::render()
{
	DirectX::XMMATRIX world_mat;
	DirectX::XMMATRIX view_mat;
	DirectX::XMMATRIX projection_mat;
	bool result = false;



	m_direct_3d->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->render();

	m_direct_3d->getWorldMatrix(world_mat);
	m_camera->getViewMatrix(view_mat);
	m_direct_3d->getProjectionMatrix(projection_mat);

	m_swarm->Render();

	result = m_color_shader->render(m_direct_3d->getRawDeviceContext(), m_swarm->GetVertexCount(), m_swarm->GetInstanceCount(),
		world_mat, view_mat, projection_mat);
	if (!result)
		return false;

	m_direct_3d->endScene();

	return true;
}
