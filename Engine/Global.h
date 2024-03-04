#pragma once
#include "Direct3D.h"

//ΐSΙπJϊ·ι½ίΜ}N
#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if ((p)!=nullptr) { delete[] (p); (p)=nullptr;}}
#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

//XMFLOAT3―mΜ«΅Z
static XMFLOAT3 Float3Add(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}

//XMFLOAT3―mΜψ«Z
static XMFLOAT3 Float3Sub(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
}

//XMFLOAT3ΖfloatΜ|―Z
static XMFLOAT3 Float3Multiply(XMFLOAT3 a, float scalar)
{
	return XMFLOAT3(a.x * scalar, a.y * scalar, a.z * scalar);
}

//XMFLOAT3π³K»
static XMFLOAT3 Float3Normalize(XMFLOAT3 a)
{
	XMVECTOR v = XMVector3Normalize(XMLoadFloat3(&a));
	XMStoreFloat3(&a, v);
	return a;
}

//£πvZ
static float DistanceCalculation(XMFLOAT3 a, XMFLOAT3 b)
{
	XMFLOAT3 c = Float3Sub(a, b);
	return sqrtf((c.x * c.x) + (c.y * c.y) + (c.z * c.z));
}