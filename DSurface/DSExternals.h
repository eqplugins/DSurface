#pragma once
#define EXTERNAL extern "C" __declspec(dllexport)
#define MAXVECTORBUFFER 10000
#define MAXLINEBUFFER 10000
#define MAXTEXTBUFFER 500
#define countof( array ) ( sizeof( array )/sizeof( array[0] ) )
#include <list>
#include <vector>
using namespace std;

struct point {
	float x;
	float y;
	float z;
};

struct line {
	float x1;
	float y1;
	float z1;
	float x2;
	float y2;
	float z2;
};

struct intersectCoord {
	float x1;
	float y1;
	float z1;
	float x2;
	float y2;
	float z2;
	unsigned short intersections;
};

struct DSD3DSprite{
	//int myID;
	unsigned short myType;
	float myAngle;
	float myScale;
	int xCorr;
	int yCorr;
	D3DXVECTOR3 myVector;
	D3DCOLOR myColor;
};

struct DSD3DLine{
	//int myID;
	//unsigned short myType;
	D3DXVECTOR2 myVerts[2];
	D3DCOLOR myColor;
};

struct DSD3DText{
	RECT rct;
	D3DCOLOR myColor;
	char text[64];
};

struct DSLines{
	int index;
	DSD3DLine line[MAXLINEBUFFER];
};

struct DSVectors{
	int index;
	DSD3DSprite vector[MAXVECTORBUFFER];
};

struct DSText{
	int index;
	DSD3DText text[MAXTEXTBUFFER];
};

struct DSTexture{
	//IDirect3DTexture9* Texture;
	float Angle;
	//int Type;
	int Frames;
	int Height;
	int Width;
	int FHeight;
	int FWidth;
	DWORD Filter;
	DWORD MipFilter;
	DWORD ColorKey;
	HMODULE Handle;
	LPCTSTR Sprite;
};

struct DSSurface{
	bool free;
	bool updated;
	D3DXMATRIX mTranslation;
	D3DXMATRIX mRotation;
	D3DXMATRIX mCorrection;
	D3DXMATRIX mScale;
	D3DXVECTOR3 myCenter;
	float myAngle;
	float myScale;
	DSVectors myVectorList;
	DSText myTextList;
	DSLines myLineList;
	vector<DSTexture> DSTextureList;
};

list<DSD3DText> myDataTextList;
DSSurface tempSurface;
DSD3DText tempText;
vector<DSSurface> DSSurfaceList;
D3DXMATRIX identityMatrix;
bool bshowData;

extern HINSTANCE gl_hThisInstance;
extern IDirect3DDevice9* pDevice;
extern CDSSurface* m_pDSSurface;

/*int GetFrameTime(int count, int accuracy);
EXTERNAL bool DSAddSprite(int surface, float x, float y, int color, int type, float angle);
EXTERNAL bool DSClearPoints(int surface);
EXTERNAL bool DSChangeAngle(int surface, float angle);
EXTERNAL bool DSChangeCenter(int surface, float x, float y);
EXTERNAL bool DSAddSprite(HMODULE handle, LPCTSTR sprite, int size, int x, int y);
EXTERNAL bool DSChangeScale(int surface, float scale);
EXTERNAL bool DSClearText(int surface);
EXTERNAL bool DSAddText(int surface, float x1, float y1, float x2, float y2, char* text);
EXTERNAL float DSGetVersion();
EXTERNAL bool DSAddLine(int surface, float x1, float y1, float x2, float y2, int color, int type);
EXTERNAL bool DSClearLines(int surface);
EXTERNAL bool ShowData();
EXTERNAL int DSCreateSurface();
EXTERNAL bool DSDestroySurface(int surface);
EXTERNAL intersectCoord DSCLIntersection (float cx, float cy, float radius, float x1, float y1, float x2, float y2);*/

int GetFrameTime(int count, int accuracy) {
	return ((GetTickCount() % (accuracy*10))/(10.0f/(float)count*accuracy));
}

EXTERNAL bool DSAddSprite(int surface, float x, float y, int color, int type, float angle, float scale){
	if (surface>=0 && surface<DSSurfaceList.size() && DSSurfaceList[surface].myVectorList.index<MAXVECTORBUFFER) {
		DSD3DSprite myVector;
		myVector.myVector = D3DXVECTOR3(x,y,0);
		myVector.myColor = color;
		myVector.myType = type;
		myVector.myAngle = angle;
		myVector.myScale = scale;
		myVector.xCorr=-DSSurfaceList[surface].DSTextureList[type].FWidth/2;
		myVector.yCorr=-DSSurfaceList[surface].DSTextureList[type].FHeight/2;
		DSSurfaceList[surface].myVectorList.vector[DSSurfaceList[surface].myVectorList.index++]=myVector;
		DSSurfaceList[surface].updated=true;
		return true;
	}
	else
		return false;
}

EXTERNAL bool DSClearSprites(int surface)
{
	if (surface>=0 && surface<DSSurfaceList.size()) {
		DSSurfaceList[surface].myVectorList.index=0;
		DSSurfaceList[surface].updated=true;
		return true;
	} else
		return false;
}

EXTERNAL bool DSAddLine(int surface, float x1, float y1, float x2, float y2, int color){
	if (surface>=0 && surface<DSSurfaceList.size() && DSSurfaceList[surface].myLineList.index<MAXLINEBUFFER) {
		DSD3DLine myLine;
		myLine.myVerts[0]=D3DXVECTOR2(x1,y1);
		myLine.myVerts[1]=D3DXVECTOR2(x2,y2);
		myLine.myColor=color;
		//myLine.myType=type;
		DSSurfaceList[surface].myLineList.line[DSSurfaceList[surface].myLineList.index++]=myLine;
		DSSurfaceList[surface].updated=true;
		return true;
	}
	else
		return false;
}

EXTERNAL bool DSClearLines(int surface)
{
	if (surface>=0 && surface<DSSurfaceList.size()) {
		DSSurfaceList[surface].myLineList.index=0;
		DSSurfaceList[surface].updated=true;
		return true;
	}
	else
		return false;

}

EXTERNAL bool DSChangeCenter(int surface, float x, float y)
{
	if (surface>=0 && surface<DSSurfaceList.size()) {
		D3DXMatrixTranslation( &DSSurfaceList[surface].mTranslation, x, y, 0.0f );
		//D3DXMatrixTranslation( &DSSurfaceList[surface].mCorrection, -8.0f, -8.0f, 0.0f );
		DSSurfaceList[surface].updated=true;
		return true;
	}
	else
		return false;
}

EXTERNAL bool DSChangeAngle(int surface, float angle)
{
	if (surface>=0 && surface<DSSurfaceList.size()) {
		DSSurfaceList[surface].myAngle=angle;
		DSSurfaceList[surface].updated=true;
		return true;
	}
	else
		return false;
}

EXTERNAL bool DSChangeScale(int surface, float scale)
{
	if (surface>=0 && surface<DSSurfaceList.size()) {
		DSSurfaceList[surface].myScale=scale;
		D3DXMatrixScaling(&DSSurfaceList[surface].mScale, scale,scale,0);
		//D3DXMatrixTranslation( &DSSurfaceList[surface].mCorrection, -8*scale, -8*scale, 0.0f );
		DSSurfaceList[surface].updated=true;
		return true;
	}
	else
		return false;
}

EXTERNAL bool DSAddText(int surface, float x1, float y1, float x2, float y2, char* text, int color)
{
	if (surface>=0 && surface<DSSurfaceList.size() && DSSurfaceList[surface].myTextList.index<MAXTEXTBUFFER) {
		DSD3DText tempText;
		tempText.rct.top=y1;
		tempText.rct.left=x1;
		tempText.rct.right=x2;
		tempText.rct.bottom=y2;
		strcpy_s(tempText.text,text);
		tempText.myColor=color;
		DSSurfaceList[surface].myTextList.text[DSSurfaceList[surface].myTextList.index++]=tempText;
		DSSurfaceList[surface].updated=true;
		return true;
	}
	else
		return false;
}

EXTERNAL bool DSClearText(int surface)
{
	if (surface>=0 && surface<DSSurfaceList.size()) {
		DSSurfaceList[surface].myTextList.index=0;
		DSSurfaceList[surface].updated=true;
		return true;
	} else
		return false;
}

EXTERNAL char* DSGetVersion()
{
	#define DSVERSION "0.127\0"
	return DSVERSION;
}

EXTERNAL bool ShowData()
{
	return true;
}
EXTERNAL int DSCreateSurface()
{
	unsigned int i;
	tempSurface.free=false;
	tempSurface.updated=false;
	tempSurface.myAngle=0.0f;
	tempSurface.myScale=1.0f;
	tempSurface.myCenter = D3DXVECTOR3(0,0,0);

	D3DXMatrixTranslation( &tempSurface.mTranslation, 0.0f, 0.0f, 0.0f );
	D3DXMatrixScaling(&tempSurface.mScale, 1,1,0);
	tempSurface.myLineList.index=0;
	tempSurface.myTextList.index=0;
	tempSurface.myVectorList.index=0;
	for (i=0;i<DSSurfaceList.size();i++)
	{
		if (DSSurfaceList[i].free) break;
	}
	if (i<DSSurfaceList.size()){
		DSSurfaceList[i]=tempSurface;
		return i;
	}else{
		DSSurfaceList.push_back(tempSurface);
		return DSSurfaceList.size()-1;
	}
}

EXTERNAL bool DSDestroySurface(int surface)
{
	if (surface>=0 && surface<DSSurfaceList.size()){
		DSSurfaceList[surface].free=true;
		DSSurfaceList[surface].myLineList.index=0;
		DSSurfaceList[surface].myTextList.index=0;
		DSSurfaceList[surface].myVectorList.index=0;
		DSSurfaceList[surface].DSTextureList.clear();
		return true;
	}else
		return false;

}
EXTERNAL intersectCoord DSCLIntersection (float cx, float cy, float radius, float x1, float y1, float z1, float x2, float y2, float z2)
{
	intersectCoord tempCoord={0,0,0,0,0,0,0};
	float dx=x2-x1;
	float dy=y2-y1;
	float dr=(float)sqrt((float)(dx*dx+dy*dy));
	float D=x1*y2-x2*y1;
	float discriminant=radius*radius*dr*dr-D*D;
	if (discriminant<0) {
		tempCoord.intersections=0;
		//memcpy(buffer,&tempCoord,sizeof(intersectCoord));
		return tempCoord;
		//return &tempCoord;  //No intersection;
	}
	else if (discriminant==0)
	{
		tempCoord.intersections=1;
		//memcpy(buffer,&tempCoord,sizeof(intersectCoord));
		return tempCoord;
		//return &tempCoord;//Tangant line there is only ONE intersection
	}
	else if (discriminant>0) {
		//The line intersects at TWO points
		//Compute intersection for ONE point (to compute intersection at OTHER point change + to a -)
		tempCoord.intersections=2;
		if (dy<0) tempCoord.x1=(D*dy-1*dx*sqrt(discriminant))/(dr*dr);
		if (dy>=0) tempCoord.x1=(D*dy+1*dx*sqrt(discriminant))/(dr*dr);
		tempCoord.y1=(-D*dx+abs(dy)*sqrt(discriminant))/(dr*dr);

		if (dy<0) tempCoord.x2=(D*dy+1*dx*sqrt(discriminant))/(dr*dr);
		if (dy>=0) tempCoord.x2=(D*dy-1*dx*sqrt(discriminant))/(dr*dr);
		tempCoord.y2=(-D*dx-abs(dy)*sqrt(discriminant))/(dr*dr);
	}
		//memcpy(buffer,&tempCoord,sizeof(intersectCoord));
		float t1,t2;
		if( abs(tempCoord.x2-tempCoord.x1) > abs(tempCoord.y2-tempCoord.y1) ) {
						t1 = (tempCoord.x1 - x1) / (x2-x1);
						t2 = (tempCoord.x2 - x2) / (x2-x1);
				} else {
						t1 = (tempCoord.y1 - y1) / (y2-y1);
						t2 = (tempCoord.y2 - y2) / (y2-y1);
				}

		tempCoord.z1=z1+t1*(z2-z1);
		tempCoord.z2=z2+t2*(z2-z1);
		return tempCoord;
	//return &tempCoord;
}

void circlePoints(int surface, float cx, float cy, float x, float y, int type, int color)
{

	if (x == 0) {
		DSAddSprite(surface,cx, cy + y, color, type,0,1);
		DSAddSprite(surface,cx, cy - y, color, type,0,1);
		DSAddSprite(surface,cx + y, cy, color, type,0,1);
		DSAddSprite(surface,cx - y, cy, color, type,0,1);
	} else 
		if (x == y) {
			DSAddSprite(surface,cx + x, cy + y, color, type,0,1);
			DSAddSprite(surface,cx - x, cy + y, color, type,0,1);
			DSAddSprite(surface,cx + x, cy - y, color, type,0,1);
			DSAddSprite(surface,cx - x, cy - y, color, type,0,1);
		} else 
			if (x < y) {
				DSAddSprite(surface,cx + x, cy + y, color, type,0,1);
				DSAddSprite(surface,cx - x, cy + y, color, type,0,1);
				DSAddSprite(surface,cx + x, cy - y, color, type,0,1);
				DSAddSprite(surface,cx - x, cy - y, color, type,0,1);
				DSAddSprite(surface,cx + y, cy + x, color, type,0,1);
				DSAddSprite(surface,cx - y, cy + x, color, type,0,1);
				DSAddSprite(surface,cx + y, cy - x, color, type,0,1);
				DSAddSprite(surface,cx - y, cy - x, color, type,0,1);
			}
}

EXTERNAL bool DSDrawCircle(int surface, float xCoord, float yCoord, float radius, int type, int color)
{
	float x = 0;
	float y = radius;
	float p = (5 - radius*4)/4;

	circlePoints(surface, xCoord, yCoord, x, y, type, color);
	while (x < y) {
		x++;
		if (p < 0) {
			p += 2*x+1;
		} else {
			y--;
			p += 2*(x-y)+1;
		}
		circlePoints(surface, xCoord, yCoord, x, y, type, color);
	}
	return true;
}

EXTERNAL intersectCoord DSPLIntersection(float x1,float y1,float z1,float x2,float y2,float z2, float zlower, float zupper)
{
	intersectCoord tempCoord;
	D3DXPLANE plane;

	if (z1<z2) {
		tempCoord.x1=x1;
		tempCoord.y1=y1;
		tempCoord.z1=z1;
		tempCoord.x2=x2;
		tempCoord.y2=y2;
		tempCoord.z2=z2;
	} else {
		tempCoord.x1=x2;
		tempCoord.y1=y2;
		tempCoord.z1=z2;
		tempCoord.x2=x1;
		tempCoord.y2=y1;
		tempCoord.z2=z1;
	}

	D3DXVECTOR3 point1=D3DXVECTOR3(tempCoord.x1,tempCoord.y1,tempCoord.z1);
	D3DXVECTOR3 point2=D3DXVECTOR3(tempCoord.x2,tempCoord.y2,tempCoord.z2);
	D3DXVECTOR3 intersectionPoint;
	
	if ((z1<zlower && z2<zlower) || (z1>zupper && z2>zupper)) {
		tempCoord.intersections=0;
	} else
	{
		if (z1<zlower) {
			plane=D3DXPLANE(0.0f, 0.0f, 1.0f, -zlower);
			if (D3DXPlaneIntersectLine(&intersectionPoint,&plane,&point1,&point2)) {
				tempCoord.x1=intersectionPoint.x;
				tempCoord.y1=intersectionPoint.y;
				tempCoord.z1=intersectionPoint.z;
			}
		}
		if (z2>zupper) {
			plane=D3DXPLANE(0.0f, 0.0f, 1.0f, -zupper);
			if (D3DXPlaneIntersectLine(&intersectionPoint,&plane,&point1,&point2)) {
				tempCoord.x2=intersectionPoint.x;
				tempCoord.y2=intersectionPoint.y;
				tempCoord.z2=intersectionPoint.z;
			}
		}
		tempCoord.intersections=2;
	}
	return tempCoord;
}

EXTERNAL bool DSPointInCircle (float cx, float cy, float radius, float x, float y) {
	float xd = x-cx;
	float yd = y-cy;
	float distance = sqrt(xd*xd + yd*yd);
	if (distance<=radius) return true;
	else return false;

}

EXTERNAL bool DSPointInCircleBorder (float cx, float cy, float radius, float x, float y, float bordersize) {
	float xd = x-cx;
	float yd = y-cy;
	float distance = sqrt(xd*xd + yd*yd);
	if (distance>=radius && distance<=(radius+bordersize)) return true;
	else return false;

}

EXTERNAL bool DSPointDegInCircle (float cx, float cy, float radius, float x, float y, float degangle) {

	float newX,newY,angle;
	angle = degangle*0.01745329f;
	newX = x*cos(angle) - y*sin(angle);
	newY = x*sin(angle) + y*cos(angle);
	float xd = newX-cx;
	float yd = newY-cy;
	float distance = sqrt(xd*xd + yd*yd);
	if (distance<=radius) return true;
	else return false;

}

EXTERNAL point DSPointDeg(float x,float y,float degangle)
{
	float angle;
	point newCoord;
	angle=degangle*0.01745329f;
	newCoord.x = x*cos(angle) - y*sin(angle);
	newCoord.y = x*sin(angle) + y*cos(angle);
	return newCoord;
}

EXTERNAL line DSLineDeg(float x1,float y1, float x2,float y2,float degangle)
{
	float angle;
	line newCoord;
	angle=degangle*0.01745329f;
	newCoord.x1 = x1*cos(angle) - y1*sin(angle);
	newCoord.y1 = x1*sin(angle) + y1*cos(angle);
	newCoord.x2 = x2*cos(angle) - y2*sin(angle);
	newCoord.y2 = x2*sin(angle) + y2*cos(angle);
	return newCoord;
}

EXTERNAL int DSCreateSprite(int surface, HMODULE handle, LPCTSTR sprite, float width, float height, float fwidth, float fheight, int frames, float angle, DWORD filter, DWORD mipfilter, DWORD colorkey)
{	
		//OutputDebugString("Adding Texture\n");
		DSTexture myTexture;
		myTexture.Angle=angle;
		myTexture.Height=height;
		myTexture.Width=width;
		myTexture.FHeight=fheight;
		myTexture.FWidth=fwidth;
		myTexture.Frames=frames;
		myTexture.Handle=handle;
		myTexture.Sprite=sprite;
		myTexture.Filter=filter;
		myTexture.MipFilter=mipfilter;
		myTexture.ColorKey=colorkey;
		/*D3DXCreateTextureFromResourceEx(pDevice,
		handle,
		sprite,
		width,
		height,
		0,
		NULL,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_POINT,
		D3DX_FILTER_NONE,
		0xFF000000,
		NULL,
		NULL,
		&myTexture.Texture);*/

		/*D3DXCreateTextureFromFileInMemoryEx(pDevice,
		sprite,
		size,
		x,
		y,
		0,
		NULL,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		0xFFFF00FF,
		NULL,
		NULL,
		&myTexture.Texture);*/
		//OutputDebugString("Pushing Texture\n");
		//char temp[1000];
		//sprintf(temp,"size=%d, %d\n",DSSurfaceList.size(),sizeof(myTexture));
		//OutputDebugString(temp);
		DSSurfaceList[surface].DSTextureList.push_back(myTexture);
		//OutputDebugString("Added Texture\n");
		if (m_pDSSurface) {
			m_pDSSurface->ReinitializeTextures();
			//OutputDebugString("Reinitialized Textures\n");
		}
		return DSSurfaceList[surface].DSTextureList.size()-1;

	return true;
}

EXTERNAL bool DSWireFrame (bool wireframe) 
{
	if (wireframe) {
	   //OutputDebugString("Wireframe ON\n");
       pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	}
	else {
	   //OutputDebugString("Wireframe OFF\n");
       pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	}
	return true;
}