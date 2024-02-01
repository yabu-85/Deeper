#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class CreateStage;

class NavigationAI
{
	CreateStage* pCreateStage_;
	std::vector<std::vector<int>> mapData_;

	struct Node {
		int x, z;
		int cost;
		Node(int x, int z, int cost) : x(x), z(z), cost(cost) {}
	};

	void PathSmoothing(std::vector<XMFLOAT3>& path);

public:
	NavigationAI();

	void SetMapData();

	//AStar
	std::vector<XMFLOAT3> Navi(XMFLOAT3 target, XMFLOAT3 pos);
	
};

