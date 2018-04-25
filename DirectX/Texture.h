#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
//#include <d3dx11tex.h>
#include<DirectXMath.h>

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();
	bool Initialize(ID3D11Device*, CHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif