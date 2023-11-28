#include "NavigationAI.h"
#include "Stage.h"

namespace {
	const int stageWidth = 25;
	const int stageHeight = 25;
	const int floarSize = 5;	//��}�X�̃T�C�Y

}

NavigationAI::NavigationAI(Stage* s)
{
	pStage_ = s;
	mapData_ = pStage_->GetMapData();

}

int NavigationAI::GetMinCostNodeIndex(std::vector<Node>& openList)
{
	// �ŏ��̃m�[�h��T��
	int minIndex = 0;
	for (int i = 1; i < openList.size(); ++i) {
		if (openList[i].cost < openList[minIndex].cost) {
			minIndex = i;
		}
	}
	return minIndex;
}

std::vector<XMFLOAT3> NavigationAI::Navi(XMFLOAT3 target, XMFLOAT3 pos)
{
	int startX = static_cast<int>(pos.x / floarSize);
	int startZ = static_cast<int>(pos.z / floarSize);
	int targetX = static_cast<int>(target.x / floarSize);
	int targetZ = static_cast<int>(target.z / floarSize);

	std::vector<std::vector<bool>> closedList(stageWidth, std::vector<bool>(stageHeight, false));	//�T���ς݂�
	std::vector<std::vector<int>> mapCost(stageHeight, std::vector<int>(stageWidth, 1));			//�}�b�v�̃R�X�g���ׂĂP
	
	std::vector<std::vector<int>> allCost(stageHeight, std::vector<int>(stageWidth, 2147483647));	//�m�[�h�̃R�X�g
	std::vector<std::vector<int>> value(stageHeight, std::vector<int>(stageWidth, 0));				//�X�^�[�g�n�_����̍ŒZ����
	
	std::vector<std::vector<int>> parentX(stageWidth, std::vector<int>(stageHeight, -1));			//���̃m�[�h�̐e�m�[�h�̍��WX
	std::vector<std::vector<int>> parentZ(stageWidth, std::vector<int>(stageHeight, -1));			//���̃m�[�h�̐e�m�[�h�̍��WZ

	std::vector<Node> openList;
	openList.push_back({ startX, startZ, 0 });

	while (!openList.empty()) {
		// openList����R�X�g���ŏ��̃m�[�h�����o��
		int minIndex = GetMinCostNodeIndex(openList);
		Node current = openList[minIndex];
		openList.erase(openList.begin() + minIndex);

		int x = current.x;
		int z = current.z;

		// �ڕW�ɓ��B�������m�F
		if (x == targetX && z == targetZ) {
			// �p�X���č\�z
			std::vector<XMFLOAT3> path;
			while (x != -1 && z != -1) {
				path.push_back(XMFLOAT3(static_cast<float>(x), 0.0f, static_cast<float>(z)));
				int tempX = parentX[x][z];
				int tempY = parentZ[x][z];
				x = tempX;
				z = tempY;
			}

			// ���ɐi�ލ��W��Ԃ�
			if (path.size() >= 2) {
				path.pop_back();
				return path;
			}
			else {
				// �p�X���Z������ꍇ�͖ڕW���̂�Ԃ�
				path.clear();
				return path;
			}
		}

		closedList[x][z] = true;

		// �אڂ���m�[�h�𐶐�
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				int newX = x + i;
				int newZ = z + j;

				// �אڃm�[�h���͈͓����ʍs�\���m�F
				if (newX >= 0 && newX < stageWidth && newZ >= 0 && newZ < stageHeight) {
					if (!closedList[newX][newZ] && mapData_[newZ][newX] == Stage::MAP::FLOAR) {
						int tentativeG = value[x][z] + mapCost[newX][newZ];

						//�v�Z���ʂ�allCost��菬�������pushBack����
						//�v�Z���@�F�e�m�[�h�̃X�^�[�g�n�_����̋����{����R�X�g(�S�[�� - ���W : x,z�ō����l)
						int cellCost = value[x][z] + abs(i) + abs(j);
						int dxValue = targetX - newX;
						int dzValue = targetZ - newZ;
						int dValue = cellCost;
						if (dxValue >= dzValue) dValue += dxValue;
						else dValue += dzValue;
						
						bool flag = dValue < allCost[newX][newZ];

						// �V�����o�H�����݂̍ŗǌo�H���Z�����m�F
						if (flag) {
							// �אڃm�[�h�̏����X�V
							value[newX][newZ] = tentativeG;
							parentX[newX][newZ] = x;
							parentZ[newX][newZ] = z;
							allCost[newX][newZ] = dValue;

							// �אڃm�[�h��openList�ɒǉ�
							openList.push_back({ newX, newZ, tentativeG });
						}
					}
				}
			}
		}
	}

	std::vector<XMFLOAT3> none;
	return none;
}