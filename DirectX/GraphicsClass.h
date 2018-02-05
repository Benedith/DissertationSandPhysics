#pragma once

//System includes
#include <Windows.h>
#include <memory>

//Custom includes
#include "d3dclass.h"
#include "Camera.h"
#include "ColorShader.h"
#include "SandManager.h"
#include "TextureShaderClass.h"

class Model;

class GraphicsClass
{
public:
	GraphicsClass() = default;
	~GraphicsClass() = default;

	bool initialize(int width, int height, HWND h_window);
	bool frame();
	void shutdown();
	

	Camera* getCamera();
	static D3DClass* m_direct_3d;

private:
	bool render();

	Camera* m_camera = new Camera();
	ColorShader* m_color_shader = new ColorShader();
	SandManager* m_swarm = new SandManager();
	TextureShaderClass* m_TextureShader;
	Model*m_Model;
};
