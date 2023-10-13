#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Triangle
{
	XMFLOAT3 position_[3];

public:
	void SetPosition(XMFLOAT3 *pos) { position_[0] = pos[0]; position_[1] = pos[1]; position_[2] = pos[2]; };

	void CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3);

	XMFLOAT3* GetPosition() { return position_; };

};

