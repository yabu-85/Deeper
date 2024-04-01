#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class NavigationAI
{
	std::vector<std::vector<int>> mapData_;
	std::vector<std::vector<int>> mapCost_;
	std::vector<std::vector<float>> mapSize_;

	struct Node {
		int x, z;
		int cost;
		Node(int x, int z, int cost) : x(x), z(z), cost(cost) {}
	};
	//‘å‚«‚¢‡‚Éƒ\[ƒg‚·‚é‚½‚ß‚Ì”äŠrŠÖ”
	struct CompareNodes {
		bool operator()(const Node& a, const Node& b) {
			return a.cost > b.cost;
		}
	};

	bool IsPositionOnMap(int x, int z);
	std::vector<XMFLOAT3> CreatePath(int x, int z, std::vector<std::vector<int>> px, std::vector<std::vector<int>> pz);

	std::vector<XMFLOAT3> Navi(int sx, int sz, int tx, int tz);
	std::vector<XMFLOAT3> Navi(int sx, int sz, int tx, int tz, float size);

	void PathSmoothing(std::vector<XMFLOAT3>& path);

public:
	NavigationAI();

	void SetMapData();

	//AStar
	std::vector<XMFLOAT3> Navi(XMFLOAT3 target, XMFLOAT3 pos, float size = 0.5f);
	
	std::vector<std::vector<float>> GetMapSize() { return mapSize_; }

};

