#ifndef _D3DVERTEX_H_
#define _D3DVERTEX_H_

#include "d3dUtility.h"

//
// Classes and Structures
//

struct VERTEX
{
	VERTEX(){}
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Normal;
	float m_u;
	float m_v;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
};

struct HaloVertex
{
	HaloVertex(){}
	HaloVertex(
		float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		_x = x;  _y = y;  _z = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u = u;  _v = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v; // texture coordinates

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
};

struct SkyBoxVertex
{
	SkyBoxVertex(){}
	SkyBoxVertex(
		float x, float y, float z,
		float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		_u  = u;  _v  = v;
	}
    float _x, _y, _z;
    float _u, _v; // texture coordinates

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
};
#endif //_D3DVERTEX_H_
