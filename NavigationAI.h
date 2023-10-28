#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class Stage;

class NavigationAI
{
	Stage* pStage_;
	std::vector<std::vector<int>> mapData_;

public:
	NavigationAI(Stage* s);
	void Navi(XMFLOAT3 target, XMFLOAT3 pos);
	void Navi(XMFLOAT3& target);

};

