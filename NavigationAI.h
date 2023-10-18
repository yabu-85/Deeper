#pragma once
#include <DirectXMath.h>
using namespace DirectX;

//目的地までまでの最適な経路を探索し、次に進む位置を示す

//ウォーフレームのマップ自動生成もメタAIらしいから参考にしたらええんやないか

class NavigationAI
{

public:
	void Navi(XMFLOAT3 target, XMFLOAT3 pos);
	void Navi(XMFLOAT3& target);

};

