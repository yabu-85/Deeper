#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//PauseとかTitle画面で使うカーソル
class Cursor
{
	int hPict_;
	XMFLOAT3 scale_;

public:
	Cursor();
	~Cursor();
	void Draw();

};

