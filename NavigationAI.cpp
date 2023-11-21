#include "NavigationAI.h"
#include "Stage.h"

namespace {
	const int stageWidth = 25;
	const int stageHeight = 25;
	const int floarSize = 1;	//一マスのサイズ

}

int NavigationAI::GetMinCostNodeIndex(std::vector<Node>& openList)
{
	// 最小のノードを探す
	int minIndex = 0;
	for (int i = 1; i < openList.size(); ++i) {
		if (openList[i].cost < openList[minIndex].cost) {
			minIndex = i;
		}
	}
	return minIndex;
}

NavigationAI::NavigationAI(Stage* s)
{
	pStage_ = s;
	mapData_ = pStage_->GetMapData();
	pStage_ = s;

}

XMFLOAT3 NavigationAI::Navi(XMFLOAT3 target, XMFLOAT3 pos)
{
	int startX = static_cast<int>(pos.x / floarSize);
	int startZ = static_cast<int>(pos.z / floarSize);
	int targetX = static_cast<int>(target.x / floarSize);
	int targetZ = static_cast<int>(target.z / floarSize);

	std::vector<std::vector<bool>> closedList(stageWidth, std::vector<bool>(stageHeight, false));	//探索済みか
	std::vector<std::vector<int>> mapCost(stageHeight, std::vector<int>(stageWidth, 1));			//マップのコストすべて１
	std::vector<std::vector<int>> value(stageHeight, std::vector<int>(stageWidth, 2147483647));		//スタート地点からの最短距離
	std::vector<std::vector<int>> parentX(stageWidth, std::vector<int>(stageHeight, -1));			//そのノードの親ノードの座標
	std::vector<std::vector<int>> parentZ(stageWidth, std::vector<int>(stageHeight, -1));			//親の座標

	std::vector<Node> openList;
	openList.push_back({ startX, startZ, 0 });

	while (!openList.empty()) {
		// openListからコストが最小のノードを取り出す
		int minIndex = GetMinCostNodeIndex(openList);
		Node current = openList[minIndex];
		openList.erase(openList.begin() + minIndex);

		int x = current.x;
		int z = current.z;

		// 目標に到達したか確認
		if (x == targetX && z == targetZ) {
			// パスを再構築
			std::vector<XMFLOAT3> path;
			while (x != -1 && z != -1) {
				path.push_back(XMFLOAT3(static_cast<float>(x), 0.0f, static_cast<float>(z)));
				int tempX = parentX[x][z];
				int tempY = parentZ[x][z];
				x = tempX;
				z = tempY;
			}

			// 次に進む座標を返す
			if (path.size() >= 2) {
				XMFLOAT3 nextPos = path[path.size() - 2];
				nextPos = XMFLOAT3(nextPos.x * floarSize, 0.0f, nextPos.z * floarSize);

				std::string strNumber = std::to_string(nextPos.x);
				OutputDebugStringA(strNumber.c_str());
				OutputDebugString(" , ");

				strNumber = std::to_string(nextPos.z);
				OutputDebugStringA(strNumber.c_str());
				OutputDebugString("\n");

				return nextPos;
			}
			else {
				// パスが短すぎる場合は目標自体を返す
				return target;
			}
		}

		closedList[x][z] = true;

		// 隣接するノードを生成
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				int newX = x + i;
				int newZ = z + j;

				// 隣接ノードが範囲内かつ通行可能か確認
				if (newX >= 0 && newX < stageWidth && newZ >= 0 && newZ < stageHeight) {
					if (!closedList[newX][newZ] && mapData_[newX][newZ] == Stage::MAP::FLOAR) {
						int tentativeG = value[x][z] + mapCost[newX][newZ];
						int currentCost = value[newX][newZ];

						// 新しい経路が現在の最良経路より短いか確認
						if (tentativeG < currentCost) {
							// 隣接ノードの情報を更新
							value[newX][newZ] = tentativeG;
							parentX[newX][newZ] = x;
							parentZ[newX][newZ] = z;

							// 隣接ノードをopenListに追加
							openList.push_back({ newX, newZ, tentativeG });
						}
					}
				}
			}
		}
	}

	return pos;
}

void NavigationAI::Navi(XMFLOAT3& target)
{
	target = XMFLOAT3(target.x + (float)(rand() % 20 -10 ), 0.0f, target.z + (float)(rand() % 20 - 10));

}

