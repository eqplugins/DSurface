#pragma once
#include <d3dx9core.h>
#include <vector>
using namespace std;
struct DSSTextures {
	vector<IDirect3DTexture9*> Texture;
};

class CDSSurface
{
public:
	CDSSurface(IDirect3DDevice9 *pD3DDevice);
	~CDSSurface();
	void Render();
	void OnLostDevice();
	void OnResetDevice();
	void ReinitializeTextures();
	void Lock();
	void Unlock();
private:
	void InitializeSprites();
	void InitializeLines();
	void InitializeFonts();
	void InitializeTextures();
	IDirect3DDevice9*  m_pD3DDevice;
	ID3DXSprite*	   m_pBackgroundSprite;
	ID3DXFont*		   m_pFont;
	ID3DXLine*		   m_pLine;
	vector<DSSTextures> Textures;
	bool locked;
	//IDirect3DTexture9* m_pBackgroundTexture;
	//IDirect3DTexture9* m_pTexture;
};

