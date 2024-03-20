#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Cursor
{
	int hPict_;
	XMFLOAT3 scale_;

public:
	Cursor();
	~Cursor();
	void Draw();

};

