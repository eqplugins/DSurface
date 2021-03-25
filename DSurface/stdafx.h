// stdafx.h 
#pragma once

#define WIN32_LEAN_AND_MEAN
#ifdef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif
#include <stdio.h>
#include "d3d9.h"
#include "DSSurface.h"
#include "DSExternals.h"
