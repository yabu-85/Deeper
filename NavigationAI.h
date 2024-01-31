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
	};

    // openList����R�X�g���ŏ��̃m�[�h���擾
    int GetMinCostNodeIndex(std::vector<Node>& openList);

	void PathSmoothing(std::vector<XMFLOAT3>& path);

public:
	NavigationAI();

	void SetMapData();

	//AStar�̏c���ړ��̂�
	std::vector<XMFLOAT3> Navi(XMFLOAT3 target, XMFLOAT3 pos);
	
	//AStar�̎΂߈ړ�����
	std::vector<XMFLOAT3> NaviDiagonal(XMFLOAT3 target, XMFLOAT3 pos);

};

