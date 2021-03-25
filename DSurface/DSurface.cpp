#define _WIN32_WINNT		0x0400
#define WIN32
#define NT

#include <stdio.h>
#include <windows.h>
#include <ddraw.h>
#include <d3d9.h>
//#include <D3dx9core.h>
#include <wingdi.h>
#include "detours.h"
#include "DSSurface.h"

#ifdef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif

//#pragma warning(disable:4100)   // Trampolines don't use formal parameters.
#pragma data_seg("SHARED")
IDirect3DDevice9* pDevice = NULL;
HINSTANCE gl_hThisInstance;
CDSSurface* m_pDSSurface;
#pragma data_seg()
#pragma comment(linker, "/section:SHARED,RWS")

//IDirect3DDevice9* pDevice;
int fCount;
long tick;
char frate[16];

LPD3DXFONT pFont;
HDC hDC;
HFONT hFont;

//////////////////////////////////////////////////////////////// 
// Detour Class.
//

class CDetour // add ": public CTarget" to enable access member variables... 
{
  public:
    HRESULT WINAPI CreateDevice(UINT Adapter,
						 D3DDEVTYPE DeviceType,
						 HWND  hFocusWindow,
						 DWORD BehaviorFlags,
						 D3DPRESENT_PARAMETERS* pPresentationParameters,
						 IDirect3DDevice9** ppReturnedDeviceInterface
						 )
	{
		//MessageBox(NULL,"D3D9 CreateDvice","Test",MB_OK);
		//Sleep(1000);
		HRESULT hr=CreateDevice_Tramp(Adapter,DeviceType,hFocusWindow,BehaviorFlags,pPresentationParameters,ppReturnedDeviceInterface);
		m_pDSSurface = NULL;
		pDevice=*ppReturnedDeviceInterface;

		void * pvtbl;
		memcpy(&pvtbl,*ppReturnedDeviceInterface,sizeof(pvtbl));
		PBYTE tmp = (PBYTE)pvtbl+0x44;
	
    	//char str[256];
    	//sprintf(str,"Direct3DDevice9 Address:%09x\nadd:%09x\tmethod:%09x",**ppReturnedDeviceInterface,tmp,*tmp);
    	//MessageBox(NULL,str,"Test",MB_OK);

		//Create D3DXFONT for displaying frame count
		hDC = CreateCompatibleDC( NULL );
		INT nHeight = -MulDiv( 12,(INT)GetDeviceCaps(hDC, LOGPIXELSY), 72 );
   		hFont = CreateFont(nHeight, 0, 0, 0, FW_BOLD, FALSE,
           	               FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
               	           CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                   	       VARIABLE_PITCH, "Arial" );

	    HRESULT (WINAPI CDetour::* pfDetour)(CONST RECT* pSourceRect,
						   CONST RECT* pDestRect,
					       HWND hDestWindowOverride,
					       CONST RGNDATA* pDirtyRegion
					      ) = &CDetour::Present;
	    HRESULT (WINAPI CDetour::* pfTrampoline)(CONST RECT* pSourceRect,
						   CONST RECT* pDestRect,
					       HWND hDestWindowOverride,
					       CONST RGNDATA* pDirtyRegion
					      ) = &CDetour::Present_Tramp;
	    
		/*DetourFunctionWithEmptyTrampoline(*(PBYTE*)&pfTrampoline,
	                                      *(PBYTE*)tmp,
	                                      *(PBYTE*)&pfDetour);*/
		//OutputDebugString(str);
		DetourFunctionWithEmptyTrampoline(*(PBYTE*)&pfTrampoline,
	                                      *(PBYTE*)tmp,
	                                      *(PBYTE*)&pfDetour);
		/*sprintf(str,"\nTrampoline %d (%X,%X,%X)\n",DetourFunctionWithEmptyTrampoline(*(PBYTE*)&pfTrampoline,
	                                      *(PBYTE*)tmp,
	                                      *(PBYTE*)&pfDetour),
										  *(PBYTE*)&pfTrampoline,
	                                      *(PBYTE*)tmp,
	                                      *(PBYTE*)&pfDetour);*/
		tmp = (PBYTE)pvtbl+0x40;
	    HRESULT (WINAPI CDetour::* rfDetour)(D3DPRESENT_PARAMETERS* pPresentationParameters
			) = &CDetour::Reset;
	    HRESULT (WINAPI CDetour::* rfTrampoline)(D3DPRESENT_PARAMETERS* pPresentationParameters
			) = &CDetour::Reset_Tramp;

		DetourFunctionWithEmptyTrampoline(*(PBYTE*)&rfTrampoline,
	                                      *(PBYTE*)tmp,
	                                      *(PBYTE*)&rfDetour);

		/*tmp = (PBYTE)pvtbl+0x08;
	    ULONG (WINAPI CDetour::* relfDetour)(void
			) = &CDetour::Release;
	    ULONG (WINAPI CDetour::* relfTrampoline)(void
			) = &CDetour::Release_Tramp;

		DetourFunctionWithEmptyTrampoline(*(PBYTE*)&relfTrampoline,
	                                      *(PBYTE*)tmp,
	                                      *(PBYTE*)&relfDetour);*/

		//OutputDebugString(str);
		return hr;
	}


ULONG WINAPI Release()
{

	//extern DSIDirect3DDevice9* pDevice;
	//OutputDebugString("RELEASE\n");
	if (m_pDSSurface) {
		//OutputDebugString("DESTROYING SURFACE CONTROLLER\n");
		delete(m_pDSSurface);
		m_pDSSurface = NULL;
	}

	ULONG count = pDevice->Release();

    if (count==0)
	{
		//OutputDebugString("DESTROYING EVERYTHING\n");
		pDevice = NULL;
		delete(this);  // destructor will be called automatically
	}
	return (count);
}

 HRESULT WINAPI Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (m_pDSSurface) m_pDSSurface->OnLostDevice();
	if (m_pDSSurface) m_pDSSurface->OnResetDevice();
    return Reset_Tramp(pPresentationParameters);
}

    HRESULT WINAPI Present(CONST RECT* pSourceRect,
				    CONST RECT* pDestRect,
				    HWND hDestWindowOverride,
				    CONST RGNDATA* pDirtyRegion
				   )
	{
	
		// Obtain adapter number and device window

		/*D3DDEVICE_CREATION_PARAMETERS dcp;
		pDevice->GetCreationParameters(&dcp);
		RECT rc;
		GetClientRect(dcp.hFocusWindow, &rc);
		D3DXCreateFont( pDevice, 18, 8, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &pFont );
		fCount+=1;
		if ((GetTickCount()-tick)>1000) {
			sprintf(frate,"FPS:%4.2f\0",(1000.0*fCount)/(GetTickCount()-tick));
			tick = GetTickCount();
			fCount=0;
		}
		pFont->DrawTextA(NULL,frate,strlen(frate),&rc,DT_BOTTOM,D3DCOLOR_ARGB(255,255,255,0)); 
		pFont->Release();*/
		if (!m_pDSSurface) {
			//OutputDebugString("CREATING\n");
			m_pDSSurface = new CDSSurface(pDevice);
		}
		if (m_pDSSurface) m_pDSSurface->Render();
		return Present_Tramp(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
	}

	ULONG WINAPI Release_Tramp(void);

	HRESULT WINAPI Reset_Tramp(D3DPRESENT_PARAMETERS* pPresentationParameters);

    HRESULT WINAPI Present_Tramp(CONST RECT* pSourceRect,
					      CONST RECT* pDestRect,
				          HWND hDestWindowOverride,
				          CONST RGNDATA* pDirtyRegion
				         );

	HRESULT WINAPI CreateDevice_Tramp(UINT Adapter,
						 	   D3DDEVTYPE DeviceType,
						 	   HWND  hFocusWindow,
						 	   DWORD BehaviorFlags,
						 	   D3DPRESENT_PARAMETERS* pPresentationParameters,
						 	   IDirect3DDevice9** ppReturnedDeviceInterface
						 	  );
    // Class shouldn't have any member variables or virtual functions.
};

DETOUR_TRAMPOLINE_EMPTY(ULONG WINAPI CDetour::Release_Tramp(void));

DETOUR_TRAMPOLINE_EMPTY(HRESULT WINAPI CDetour::Reset_Tramp(D3DPRESENT_PARAMETERS* pPresentationParameters));

DETOUR_TRAMPOLINE_EMPTY(HRESULT WINAPI CDetour::Present_Tramp(CONST RECT* pSourceRect,
												       CONST RECT* pDestRect,
												       HWND hDestWindowOverride,
												       CONST RGNDATA* pDirtyRegion
												      ));

DETOUR_TRAMPOLINE_EMPTY(HRESULT WINAPI CDetour::CreateDevice_Tramp(UINT Adapter,
													  	    D3DDEVTYPE DeviceType,
													  	    HWND  hFocusWindow,
													  	    DWORD BehaviorFlags,
													  	    D3DPRESENT_PARAMETERS* pPresentationParameters,
													  	    IDirect3DDevice9** ppReturnedDeviceInterface
													  	   ));
// End Class
///////////////////////////////////////////////////////////////////

DETOUR_TRAMPOLINE(UINT WINAPI Real_SetTimer(HWND hWnd,              // handle of window for timer messages
										    UINT nIDEvent,          // timer identifier
											UINT uElapse,           // time-out value
										    TIMERPROC lpTimerFunc), // address of timer procedure
				  SetTimer);

DETOUR_TRAMPOLINE(IDirect3D9* WINAPI Real_Direct3DCreate9(UINT SDKVersion ),
				  Direct3DCreate9);

/////////////////////////////////////////////////////////////
// Detours
//
UINT WINAPI Mine_SetTimer(
	HWND hWnd,              // handle of window for timer messages
	UINT nIDEvent,          // timer identifier
	UINT uElapse,           // time-out value
	TIMERPROC lpTimerFunc   // address of timer procedure
	)
{
	uElapse=10*uElapse;
	return Real_SetTimer(hWnd,nIDEvent,uElapse,lpTimerFunc);
}

IDirect3D9* WINAPI Mine_Direct3DCreate9(
	UINT SDKVersion
	)
{
	//MessageBox(NULL,"Direct3DCreate9","Test",MB_OK);
    IDirect3D9* pd3d;
    pd3d=Real_Direct3DCreate9(SDKVersion);

	void * pvtbl;
	memcpy(&pvtbl,pd3d,sizeof(pvtbl));
	PBYTE tmp = (PBYTE)pvtbl+0x40;
	
    /*char str[256];
	sprintf(str,"Direct3D9 Address:%09x\nCreateDevice Virtual Address:%09x\nadd:%09x\tmethod:%09x",*pd3d,&IDirect3D9::CreateDevice,tmp,*tmp);
    MessageBox(NULL,str,"Test",MB_OK);*/

    HRESULT (WINAPI CDetour::* pf_d_CreateDevice)(UINT Adapter,
									 	   D3DDEVTYPE DeviceType,
									 	   HWND  hFocusWindow,
									 	   DWORD BehaviorFlags,
									 	   D3DPRESENT_PARAMETERS* pPresentationParameters,
									 	   IDirect3DDevice9** ppReturnedDeviceInterface
									 	  ) = &CDetour::CreateDevice;
									 	  
    HRESULT (WINAPI CDetour::* pf_t_CreateDevice)(UINT Adapter,
									 	   D3DDEVTYPE DeviceType,
									 	   HWND  hFocusWindow,
									 	   DWORD BehaviorFlags,
									 	   D3DPRESENT_PARAMETERS* pPresentationParameters,
									 	   IDirect3DDevice9** ppReturnedDeviceInterface
									 	  ) = &CDetour::CreateDevice_Tramp;
    
	DetourFunctionWithEmptyTrampoline(*(PBYTE*)&pf_t_CreateDevice,
                                      *(PBYTE*)tmp,
                                      *(PBYTE*)&pf_d_CreateDevice);

	/*sprintf(str,"\nTrampoline Create %d (%X,%X,%X)\n",DetourFunctionWithEmptyTrampoline(*(PBYTE*)&pf_t_CreateDevice,
                                      *(PBYTE*)tmp,
                                      *(PBYTE*)&pf_d_CreateDevice),
									  *(PBYTE*)&pf_t_CreateDevice,
                                      *(PBYTE*)tmp,
                                      *(PBYTE*)&pf_d_CreateDevice);*/
	//OutputDebugString(str);
	return pd3d;
}
	
/////////////////////////////////////////////////////////////
// TrampolineWith
//
void TrampolineWith(VOID)
{
    DetourFunctionWithTrampoline((PBYTE)Real_SetTimer,
                                 (PBYTE)Mine_SetTimer);
    DetourFunctionWithTrampoline((PBYTE)Real_Direct3DCreate9,
                                 (PBYTE)Mine_Direct3DCreate9);
}

VOID TrampolineRemove(VOID)
{
	DeleteDC( hDC );
	DeleteObject( hFont );

    DetourRemove((PBYTE)Real_SetTimer,
      		     (PBYTE)Mine_SetTimer);
    DetourRemove((PBYTE)Real_Direct3DCreate9,
          	     (PBYTE)Mine_Direct3DCreate9);
}

VOID NullExport()
{
}

//////////////////////////////////////////////////////////////////////////////
//
// DLL module information
//
BOOL ThreadAttach(HMODULE hDll)
{
	//Do nothing
	return TRUE;
}

BOOL ThreadDetach(HMODULE hDll)
{
	//Do nothing
	return TRUE;
}

BOOL ProcessAttach(HMODULE hDll)
{
	gl_hThisInstance = (HINSTANCE)  hDll;
	TrampolineWith();
	ThreadAttach(hDll);
	return TRUE;
}

BOOL ProcessDetach(HMODULE hDll)
{
	TrampolineRemove();
	ThreadDetach(hDll);
	return TRUE;
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, PVOID lpReserved)
{
	switch (dwReason) {
	  case DLL_PROCESS_ATTACH:
		return ProcessAttach(hModule);
	  case DLL_PROCESS_DETACH:
		return ProcessDetach(hModule);
	  case DLL_THREAD_ATTACH:
		return ThreadAttach(hModule);
	  case DLL_THREAD_DETACH:
		return ThreadDetach(hModule);
	}
	return TRUE;
}
//
///////////////////////////////////////////////////////////////// End of File.
