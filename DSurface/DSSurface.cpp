#include "StdAfx.h"
#include <list>
#include <vector>
#include "DSExternals.h"
#include "DSSurface.h"
using namespace std;

CDSSurface::CDSSurface(IDirect3DDevice9 *pD3DDevice)
{

	m_pD3DDevice = pD3DDevice;
	m_pBackgroundSprite = NULL;
	m_pLine = NULL;
	m_pFont = NULL;
	/*for (int i=0;i<6;i++) {
		m_pBackgroundTexture[i] = NULL;
	}*/
	InitializeSprites();
	InitializeLines();
	InitializeFonts();
	ReinitializeTextures();
	bshowData=true;
	locked=false;
	D3DXMatrixIdentity(&identityMatrix);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
}

void CDSSurface::InitializeSprites()
{
	D3DXCreateSprite(m_pD3DDevice,&m_pBackgroundSprite);
}

void CDSSurface::InitializeLines()
{
	D3DXCreateLine(m_pD3DDevice,&m_pLine);
}

void CDSSurface::InitializeFonts()
{
	D3DXCreateFont( m_pD3DDevice, 18, 8, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_pFont );
}

void CDSSurface::InitializeTextures()
{
	DSSTextures empty;
	IDirect3DTexture9* myTexture; 
	for (int i=0; i<DSSurfaceList.size(); i++) {
		Textures.push_back(empty);
		for (int j=0; j<DSSurfaceList[i].DSTextureList.size(); j++) {
		D3DXCreateTextureFromResourceEx(m_pD3DDevice,
		DSSurfaceList[i].DSTextureList[j].Handle,
		DSSurfaceList[i].DSTextureList[j].Sprite,
		DSSurfaceList[i].DSTextureList[j].Width,
		DSSurfaceList[i].DSTextureList[j].Height,
		0,
		NULL,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_POINT,
		D3DX_FILTER_NONE,
		0xFF000000,
		NULL,
		NULL,
		&myTexture);
		Textures[i].Texture.push_back(myTexture);
		}
	}
	/*D3DXCreateTextureFromResourceEx(m_pD3DDevice,
		gl_hThisInstance,
		MAKEINTRESOURCE(101),
		8,
		8,
		0,
		NULL,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_POINT,
		D3DX_FILTER_NONE,
		0xFF000000,
		NULL,
		NULL,
		&m_pBackgroundTexture[0]);
	D3DXCreateTextureFromResourceEx(m_pD3DDevice,
		gl_hThisInstance,
		MAKEINTRESOURCE(105),
		16,
		16,
		0,
		NULL,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_POINT,
		D3DX_FILTER_NONE,
		0xFF000000,
		NULL,
		NULL,
		&m_pBackgroundTexture[1]);
	D3DXCreateTextureFromResourceEx(m_pD3DDevice,
		gl_hThisInstance,
		MAKEINTRESOURCE(105),
		16,
		16,
		0,
		NULL,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_POINT,
		D3DX_FILTER_NONE,
		0xFF000000,
		NULL,
		NULL,
		&m_pBackgroundTexture[2]);
	D3DXCreateTextureFromResourceEx(m_pD3DDevice,
		gl_hThisInstance,
		MAKEINTRESOURCE(104),
		32,
		32,
		0,
		NULL,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_POINT,
		D3DX_FILTER_NONE,
		0xFF000000,
		NULL,
		NULL,
		&m_pBackgroundTexture[3]);
	D3DXCreateTextureFromResourceEx(m_pD3DDevice,
		gl_hThisInstance,
		MAKEINTRESOURCE(103),
		160,
		32,
		0,
		NULL,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_POINT,
		0xFF000000,
		NULL,
		NULL,
		&m_pBackgroundTexture[4]);
	D3DXCreateTextureFromResourceEx(m_pD3DDevice,
		gl_hThisInstance,
		MAKEINTRESOURCE(102),
		256,
		256,
		0,
		NULL,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_TRIANGLE,
		D3DX_FILTER_TRIANGLE,
		0xFF000000,
		NULL,
		NULL,
		&m_pBackgroundTexture[5]);*/

	/*HGLOBAL hResourceLoaded;  // handle to loaded resource
    HRSRC   hRes;
	LPCVOID     lpResLock;
	hRes = FindResource(gl_hThisInstance, MAKEINTRESOURCE(IDR_TGA1), "TGA");
    hResourceLoaded = LoadResource(gl_hThisInstance, hRes);
    lpResLock = (LPCVOID) LockResource(hResourceLoaded);
	D3DXCreateTextureFromFileInMemory(m_pD3DDevice,lpResLock,SizeofResource(gl_hThisInstance, hRes),&m_pBackgroundTexture[5]);*/

		/*Textures[0].Type=0;
		Textures[0].Height=8;
		Textures[0].Width=8;
		Textures[0].Frames=1;
		Textures[0].Angle=0;
		Textures[1].Type=1;
		Textures[1].Height=16;
		Textures[1].Width=16;
		Textures[1].Frames=1;
		Textures[1].Angle=0;
		Textures[2].Type=2;
		Textures[2].Frames=1;
		Textures[2].Height=16;
		Textures[2].Width=16;
		Textures[2].Angle=0;
		Textures[3].Type=3;
		Textures[3].Frames=9;
		Textures[3].Height=32;
		Textures[3].Width=32;
		Textures[3].Angle=10;
		Textures[4].Type=4;
		Textures[4].Frames=5;
		Textures[4].Height=32;
		Textures[4].Width=32;
		Textures[4].Angle=0;
		Textures[5].Type=5;
		Textures[5].Frames=1;
		Textures[5].Height=256;
		Textures[5].Width=256;
		Textures[5].Angle=0;*/
		//Textures[i].Height=m_pBackgroundTexture[i]->Height;
		//Textures[i].Width=m_pBackgroundTexture[i]->Width;
	//}
}
void CDSSurface::ReinitializeTextures()
{
	Lock();
	//OutputDebugString("ReInitialize Textures\n");
	DSSTextures empty;
	IDirect3DTexture9* myTexture = NULL; 
	Textures.clear();
	//OutputDebugString("Textures Cleared\n");
	for (int i=0; i<DSSurfaceList.size(); i++) {
		//OutputDebugString("ADDING ARRAY\n");
		Textures.push_back(empty);
		//OutputDebugString("ARRAY ADDED\n");
		for (int j=0; j<DSSurfaceList[i].DSTextureList.size(); j++) {
			//OutputDebugString("NEW TEXTURE\n");
			if (D3DXCreateTextureFromResourceEx(m_pD3DDevice,
				DSSurfaceList[i].DSTextureList[j].Handle,
				DSSurfaceList[i].DSTextureList[j].Sprite,
				DSSurfaceList[i].DSTextureList[j].Width,
				DSSurfaceList[i].DSTextureList[j].Height,
				0,
				NULL,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				DSSurfaceList[i].DSTextureList[j].Filter,
				DSSurfaceList[i].DSTextureList[j].MipFilter,
				DSSurfaceList[i].DSTextureList[j].ColorKey,//0xFF000000,
				NULL,
				NULL,
				&myTexture)==D3D_OK) {
			} else {
				char temp[1000];
				sprintf(temp,"DS: Failed to create sprite %d on surface %d\n",j,i);
				OutputDebugString(temp);
			}
			Textures[i].Texture.push_back(myTexture);
		}
	}
	Unlock();
}

void CDSSurface::Lock() {
	locked = true;
}

void CDSSurface::Unlock() {
	locked = false;
}

CDSSurface::~CDSSurface()
{
	if (m_pFont) m_pFont->Release();
	if (m_pBackgroundSprite) m_pBackgroundSprite->Release();
	if (m_pLine) m_pLine->Release();
	/*for (int i=0;i<DSTextureList.size();i++) {
		if (DSTextureList[i].Texture) DSTextureList[i].Texture->Release();
		//m_pBackgroundTexture[i] = NULL;
	}*/
	/*for (int i=0; i<Textures.size(); i++) {
		for (int j=0; j<Textures[i].Texture.size(); j++) {
			//Textures[i].Texture[j]->Release;
			Textures[i].Texture[j]=NULL;
		}
	}*/
	Textures.clear();
	m_pBackgroundSprite = NULL;
	m_pLine = NULL;
	m_pFont = NULL;
}

void CDSSurface::Render()
{
	if (!locked) {
		int j=0;
		DWORD state1,state2,state3;
		m_pD3DDevice->GetRenderState(D3DRS_ALPHABLENDENABLE,&state1);
		m_pD3DDevice->GetRenderState(D3DRS_SRCBLEND,&state2);
		m_pD3DDevice->GetRenderState(D3DRS_DESTBLEND,&state3);

		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		//m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);

		m_pD3DDevice->BeginScene();

		for (int i=0;i<DSSurfaceList.size();i++){
			if (!DSSurfaceList[i].free){
				D3DXMATRIX mMyMatrix;

				D3DXMatrixRotationZ( &DSSurfaceList[i].mRotation, D3DXToRadian(DSSurfaceList[i].myAngle) );

				mMyMatrix = DSSurfaceList[i].mRotation*DSSurfaceList[i].mScale*DSSurfaceList[i].mTranslation;

				//m_pLine->SetWidth(DSSurfaceList[i].myScale);
				m_pLine->Begin();
				for (j=0;j<DSSurfaceList[i].myLineList.index;j++){
					m_pD3DDevice->SetTransform( D3DTS_WORLD, &mMyMatrix );
					m_pLine->Draw(DSSurfaceList[i].myLineList.line[j].myVerts, 2, DSSurfaceList[i].myLineList.line[j].myColor );
				}
				m_pLine->End();

				D3DXMATRIX mTemp,mTempTranslation,mTempTranslationBack,mTempRotation,mTempCorrection,mTempScale;

				m_pBackgroundSprite->Begin(D3DXSPRITE_ALPHABLEND);
				//m_pBackgroundSprite->SetTransform(&mMyMatrix);
				for (j=0;j<DSSurfaceList[i].myVectorList.index;j++){
					//OutputDebugString("Drawing vector\n");
					RECT tempRect;
					int tempFrame;
					int mFrame = GetFrameTime(DSSurfaceList[i].DSTextureList[DSSurfaceList[i].myVectorList.vector[j].myType].Frames,50);
					D3DXMatrixRotationZ( &mTempRotation, D3DXToRadian(DSSurfaceList[i].myVectorList.vector[j].myAngle+mFrame*DSSurfaceList[i].DSTextureList[DSSurfaceList[i].myVectorList.vector[j].myType].Angle));
					D3DXMatrixScaling(&mTempScale,DSSurfaceList[i].myVectorList.vector[j].myScale,DSSurfaceList[i].myVectorList.vector[j].myScale,0);
					D3DXMatrixTranslation(&mTempTranslation,-DSSurfaceList[i].myVectorList.vector[j].myVector.x,-DSSurfaceList[i].myVectorList.vector[j].myVector.y,0);
					D3DXMatrixTranslation(&mTempTranslationBack,DSSurfaceList[i].myVectorList.vector[j].myVector.x,DSSurfaceList[i].myVectorList.vector[j].myVector.y,0);				
					D3DXMatrixTranslation(&mTempCorrection,DSSurfaceList[i].myVectorList.vector[j].xCorr,DSSurfaceList[i].myVectorList.vector[j].yCorr,0);
					//mTemp=mTempCorrection*mTempTranslation*mTempRotation*mTempTranslationBack*mMyMatrix;
					//D3DXMatrixTranslation(&mTempCorrection,DSSurfaceList[i].myVectorList.vector[j].xCorr*DSSurfaceList[i].myVectorList.vector[j].myScale,DSSurfaceList[i].myVectorList.vector[j].yCorr*DSSurfaceList[i].myVectorList.vector[j].myScale,0);
					mTemp=mTempCorrection*mTempTranslation*mTempRotation*mTempScale*mTempTranslationBack*DSSurfaceList[i].mRotation*DSSurfaceList[i].mScale*DSSurfaceList[i].mTranslation;
					m_pBackgroundSprite->SetTransform(&mTemp);
					tempRect.top=0;
					tempRect.bottom=DSSurfaceList[i].DSTextureList[DSSurfaceList[i].myVectorList.vector[j].myType].Height;
					if (mFrame>(DSSurfaceList[i].DSTextureList[DSSurfaceList[i].myVectorList.vector[j].myType].Frames-1) || DSSurfaceList[i].DSTextureList[DSSurfaceList[i].myVectorList.vector[j].myType].Angle>0) tempFrame=0;
					else tempFrame=mFrame;
					tempRect.left=DSSurfaceList[i].DSTextureList[DSSurfaceList[i].myVectorList.vector[j].myType].FWidth*tempFrame;
					tempRect.right=DSSurfaceList[i].DSTextureList[DSSurfaceList[i].myVectorList.vector[j].myType].FWidth*tempFrame+DSSurfaceList[i].DSTextureList[DSSurfaceList[i].myVectorList.vector[j].myType].FWidth;
					/*char temp[1000];
					sprintf(temp,"\nTextures[i].Texture.size()=%d>DSSurfaceList[i].myVectorList.vector[j].myType=%d\n",Textures[i].Texture.size(),DSSurfaceList[i].myVectorList.vector[j].myType);
					OutputDebugString(temp);*/
					if (Textures.size()>i) {
						if (Textures[i].Texture.size()>DSSurfaceList[i].myVectorList.vector[j].myType && Textures[i].Texture[DSSurfaceList[i].myVectorList.vector[j].myType]!=NULL) {
							m_pBackgroundSprite->Draw(Textures[i].Texture[DSSurfaceList[i].myVectorList.vector[j].myType],&tempRect,&DSSurfaceList[i].myCenter,&DSSurfaceList[i].myVectorList.vector[j].myVector,DSSurfaceList[i].myVectorList.vector[j].myColor);
						}
					}
				}
				m_pBackgroundSprite->End();

				for (j=0;j<DSSurfaceList[i].myTextList.index;j++){
					m_pFont->DrawText(NULL, DSSurfaceList[i].myTextList.text[j].text, -1, &DSSurfaceList[i].myTextList.text[j].rct, 0, DSSurfaceList[i].myTextList.text[j].myColor );
				}

#ifdef _DEBUG

				if (bshowData){
					char tempString[64];
					myDataTextList.clear();
					tempText.rct.top=0;
					tempText.rct.left=0;
					tempText.rct.right=0+300;
					tempText.rct.bottom=0+20;
					sprintf_s(tempString,"L%d T%d V%d",DSSurfaceList[0].myLineList.index,DSSurfaceList[0].myTextList.index,DSSurfaceList[0].myVectorList.index);
					strcpy_s(tempText.text,tempString);
					myDataTextList.push_back(tempText);
					m_pD3DDevice->SetTransform( D3DTS_WORLD, &identityMatrix );
					for (list<DSD3DText>::iterator it = myDataTextList.begin();it != myDataTextList.end(); it++){
						tempText=*it;
						m_pFont->DrawText(NULL, tempText.text, -1, &tempText.rct, 0, 0xf0ffffff );
					}
				}
#endif
				DSSurfaceList[i].updated=false;
			}
		}

		m_pD3DDevice->EndScene();
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,state1);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,state2);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,state3);
	}
}

void CDSSurface::OnLostDevice()
{
	m_pBackgroundSprite->OnLostDevice();
	m_pFont->OnLostDevice();
	m_pLine->OnLostDevice();
}

void CDSSurface::OnResetDevice()
{
	m_pBackgroundSprite->OnResetDevice();
	m_pFont->OnResetDevice();
	m_pLine->OnResetDevice();
}

