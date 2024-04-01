#include "NavigationAI.h"
#include "CreateStage.h"
#include "../GameManager/GameManager.h"
#include <queue>

namespace {
	int stageWidth = 0;
	int stageHeight = 0;

	const int DIRX[] = { 0, +1,  0, -1 };	//���A�E�A��A��
	const int DIRY[] = { -1,  0, +1,  0 };
	
	const int AROUND_DIRX[] = { -1,  0, 1,  1, 1, 0, -1, -1 };	//���̔z�񎞌v���
	const int AROUND_DIRY[] = { 1,  1, 1,  0, -1, -1, -1, 0 };	//

}

NavigationAI::NavigationAI()
{
}

void NavigationAI::SetMapData()
{
	mapData_ = GameManager::GetCreateStage()->GetMapData();
	stageWidth = (int)mapData_[0].size();
	stageHeight = (int)mapData_.size();

	mapCost_.resize(stageHeight);
	for (int i = 0; i < stageHeight; i++) mapCost_[i].resize(stageWidth, 1);

	mapSize_.resize(stageHeight);
	for (int i = 0; i < stageHeight; i++) mapSize_[i].resize(stageWidth, 1);
	for (int x = 0; x < stageWidth; x++) {
		for (int z = 0; z < stageHeight; z++) {
			//�ǂ̏ꍇ��0�ɂ��Ď���
			if (mapData_[z][x] == M_WALL) {
				mapSize_[x][z] = 0.0f;
				continue;
			}
			else {
				mapSize_[x][z] = 0.5f;
				bool flag = true;
				for (int i = 0; i < 8; i++) {
					int posX = x + AROUND_DIRX[i];
					int posZ = z + AROUND_DIRY[i];
					if (mapData_[posZ][posX] == M_WALL) {
						flag = false;
						break;
					}
				}
				if (flag) mapSize_[x][z] = 1.0f;
			}
		}
	}
}

std::vector<XMFLOAT3> NavigationAI::Navi(XMFLOAT3 target, XMFLOAT3 pos, float size)
{
	int startX = static_cast<int>(pos.x);
	int startZ = static_cast<int>(pos.z);
	int targetX = static_cast<int>(target.x);
	int targetZ = static_cast<int>(target.z);

	//�X�^�[�g�E�^�[�Q�b�g���͈͊O�E�ǂ̏ꍇ
	if (!IsPositionOnMap(startX, startZ) || mapData_[startZ][startX] == M_WALL ||
		!IsPositionOnMap(targetX, targetZ) || mapData_[targetZ][targetX] == M_WALL)
	{
		std::vector<XMFLOAT3> none;
		return none;
	}

	if(size <= 0.5f) return Navi(startX, startZ, targetX, targetZ);
	else return Navi(startX, startZ, targetX, targetZ, size);
}

//--------------------------------------------------------------------------------------------

bool NavigationAI::IsPositionOnMap(int x, int z)
{
	if (x < 0 || x >= stageWidth || z < 0 || z >= stageHeight) return false;
	return true;
}

std::vector<XMFLOAT3> NavigationAI::CreatePath(int x, int z, std::vector<std::vector<int>> px, std::vector<std::vector<int>> pz)
{
	std::vector<XMFLOAT3> path;
	while (x != -1 && z != -1) {
		path.push_back(XMFLOAT3(static_cast<float>(x), 0.0f, static_cast<float>(z)));
		int tempX = px[x][z];
		int tempY = pz[x][z];
		x = tempX;
		z = tempY;
	}

	// ��Ԍ��̃f�[�^��startPos������폜
	if (path.size() >= 2) {
		path.pop_back();

		//PathSmoothing(path);
		return path;
	}
	// �p�X���Z������ꍇ�͖ڕW���̂�Ԃ�
	else {
		path.clear();
		return path;
	}
}

std::vector<XMFLOAT3> NavigationAI::Navi(int sx, int sz, int tx, int tz)
{
	std::priority_queue<Node, std::vector<Node>, CompareNodes> open;
	open.push(Node(sx, sz, 0));

	std::vector<std::vector<bool>> closedList(stageWidth, std::vector<bool>(stageHeight, false));	//�T���ς݂�
	std::vector<std::vector<int>> allCost(stageWidth, std::vector<int>(stageHeight, INT_MAX));		//�m�[�h�̃R�X�g
	std::vector<std::vector<int>> value(stageWidth, std::vector<int>(stageHeight, 0));				//�X�^�[�g�n�_����̍ŒZ����
	std::vector<std::vector<int>> parentX(stageWidth, std::vector<int>(stageHeight, -1));			//���̃m�[�h�̐e�m�[�h�̍��WX
	std::vector<std::vector<int>> parentZ(stageWidth, std::vector<int>(stageHeight, -1));			//���̃m�[�h�̐e�m�[�h�̍��WZ

	while (!open.empty()) {
		Node current = open.top();
		open.pop();

		int x = current.x;
		int z = current.z;

		// �ڕW�ɓ��B�������m�F���āEPath�ԓ�
		if (x == tx && z == tz) {
			return CreatePath(x, z, parentX, parentZ);
		}

		closedList[x][z] = true;

		// �אڂ���m�[�h�𐶐�
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				int newX = x + i;
				int newZ = z + j;

				// �אڃm�[�h���͈͓����ʍs�\���m�F
				if (!IsPositionOnMap(newX, newZ) || closedList[newX][newZ] || mapData_[newZ][newX] == M_WALL) continue;

				//�΂߈ړ��̏ꍇ�͏������v���X
				if (abs(i) + abs(j) >= 2 && mapData_[z + j][x] == M_WALL || mapData_[z][x + i] == M_WALL) continue;

				//�X�R�A��allCost��菬�������push����
				//�΂߈ړ��̓v���X�Q
				int cellCost = value[x][z] + abs(i) + abs(j);
				int dxValue = tx - newX;
				int dzValue = tz - newZ;
				if (dxValue >= dzValue) cellCost += dxValue;
				else cellCost += dzValue;

				// �V�����o�H�����݂̍ŗǌo�H���Z�����m�F
				if (cellCost < allCost[newX][newZ]) {
					// �אڃm�[�h�̏����X�V
					int map = mapCost_[newX][newZ];
					value[newX][newZ] = value[x][z] + map;
					parentX[newX][newZ] = x;
					parentZ[newX][newZ] = z;
					allCost[newX][newZ] = cellCost;

					// �אڃm�[�h��openList�ɒǉ�
					open.push(Node(newX, newZ, cellCost));
				}
			}
		}
	}

	std::vector<XMFLOAT3> none;
	return none;
}

std::vector<XMFLOAT3> NavigationAI::Navi(int sx, int sz, int tx, int tz, float size)
{
	std::priority_queue<Node, std::vector<Node>, CompareNodes> open;
	open.push(Node(sx, sz, 0));

	std::vector<std::vector<bool>> closedList(stageWidth, std::vector<bool>(stageHeight, false));	//�T���ς݂�
	std::vector<std::vector<int>> allCost(stageWidth, std::vector<int>(stageHeight, INT_MAX));		//�m�[�h�̃R�X�g
	std::vector<std::vector<int>> value(stageWidth, std::vector<int>(stageHeight, 0));				//�X�^�[�g�n�_����̍ŒZ����
	std::vector<std::vector<int>> parentX(stageWidth, std::vector<int>(stageHeight, -1));			//���̃m�[�h�̐e�m�[�h�̍��WX
	std::vector<std::vector<int>> parentZ(stageWidth, std::vector<int>(stageHeight, -1));			//���̃m�[�h�̐e�m�[�h�̍��WZ

	while (!open.empty()) {
		Node current = open.top();
		open.pop();

		int x = current.x;
		int z = current.z;

		// �ڕW�ɓ��B�������m�F
		if (x == tx && z == tz) {
			return CreatePath(x, z, parentX, parentZ);
		}

		closedList[x][z] = true;

		// �אڂ���m�[�h�𐶐�
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				int newX = x + i;
				int newZ = z + j;

				// �אڃm�[�h���͈͓����ʍs�\���m�F
				if (!IsPositionOnMap(newX, newZ) || closedList[newX][newZ] ||
					mapData_[newZ][newX] == M_WALL || mapSize_[newX][newZ] < size) continue;

				//�΂߈ړ��̏ꍇ�͏������v���X
				if (abs(i) + abs(j) >= 2 && (mapData_[z + j][x] == M_WALL || mapData_[z][x + i] == M_WALL)) continue;

				//�X�R�A��allCost��菬�������push����
				//�΂߈ړ��̓v���X�Q
				int cellCost = value[x][z] + abs(i) + abs(j);
				int dxValue = tx - newX;
				int dzValue = tz - newZ;
				if (dxValue >= dzValue) cellCost += dxValue;
				else cellCost += dzValue;

				// �V�����o�H�����݂̍ŗǌo�H���Z�����m�F
				if (cellCost < allCost[newX][newZ]) {
					// �אڃm�[�h�̏����X�V
					value[newX][newZ] = value[x][z] + mapCost_[newX][newZ];
					parentX[newX][newZ] = x;
					parentZ[newX][newZ] = z;
					allCost[newX][newZ] = cellCost;

					// �אڃm�[�h��openList�ɒǉ�
					open.push(Node(newX, newZ, cellCost));
				}
			}
		}
	}

	std::vector<XMFLOAT3> none;
	return none;
}

void NavigationAI::PathSmoothing(std::vector<XMFLOAT3>& path) {
	const std::vector<XMFLOAT3> prePath = path;
	const float alpha = 0.2f;			//�傫���قǁA����Path�Ɏ��Ă���Path���ł���B�@�@�@�@ �傫���قǏ���������
	const float beta = 0.3f;			//�傫���قǁA�אڂ���_�Ԃł̊��炩�������シ��B�@   �傫���قǏ������x��
	const float tolerance = 0.5f;		//�ω��ʂ����̒l�ȉ��̎����������I���B�@�@�@�@�@�@�@�@ �傫���قǏ���������
	float change = tolerance;			//�p�X�̈ʒu�̕ω���

	while (change >= tolerance) {
		change = 0.0f;

		for (int i = 1; i < path.size() - 1; ++i) {
			XMVECTOR currentPathVector = XMLoadFloat3(&path[i]);
			XMVECTOR previousPathVector = XMLoadFloat3(&path[i - 1]);
			XMVECTOR nextPathVector = XMLoadFloat3(&path[i + 1]);
			
			XMVECTOR smoothedPathVector = currentPathVector - alpha * (currentPathVector - XMLoadFloat3(&prePath[i]));
			smoothedPathVector = smoothedPathVector - beta * (2.0f * currentPathVector - previousPathVector - nextPathVector);
			XMStoreFloat3(&path[i], smoothedPathVector);

			change += XMVectorGetX(XMVector3Length(smoothedPathVector - currentPathVector));
		}
	}
}