#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class Stage;

class NavigationAI
{
	Stage* pStage_;
	std::vector<std::vector<int>> mapData_;

	struct Node {
		int x, z;
		int cost;
	};

    // openList����R�X�g���ŏ��̃m�[�h���擾
    int GetMinCostNodeIndex(std::vector<Node>& openList);

public:
	NavigationAI(Stage* s);
	XMFLOAT3 Navi(XMFLOAT3 target, XMFLOAT3 pos);
	void Navi(XMFLOAT3& target);

};

