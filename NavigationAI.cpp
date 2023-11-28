#include "NavigationAI.h"
#include "Stage.h"

namespace {
	const int stageWidth = 25;
	const int stageHeight = 25;
	const int floarSize = 5;	//一マスのサイズ

}

NavigationAI::NavigationAI(Stage* s)
{
	pStage_ = s;
	mapData_ = pStage_->GetMapData();
	pStage_ = s;

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

std::vector<XMFLOAT3> NavigationAI::Navi(XMFLOAT3 target, XMFLOAT3 pos)
{
	int startX = static_cast<int>(pos.x / floarSize);
	int startZ = static_cast<int>(pos.z / floarSize);
	int targetX = static_cast<int>(target.x / floarSize);
	int targetZ = static_cast<int>(target.z / floarSize);

	std::vector<std::vector<bool>> closedList(stageWidth, std::vector<bool>(stageHeight, false));	//探索済みか
	std::vector<std::vector<int>> mapCost(stageHeight, std::vector<int>(stageWidth, 1));			//マップのコストすべて１
	
	std::vector<std::vector<int>> allCost(stageHeight, std::vector<int>(stageWidth, 2147483647));	//ノードのコスト
	std::vector<std::vector<int>> value(stageHeight, std::vector<int>(stageWidth, 0));				//スタート地点からの最短距離
	
	std::vector<std::vector<int>> parentX(stageWidth, std::vector<int>(stageHeight, -1));			//そのノードの親ノードの座標X
	std::vector<std::vector<int>> parentZ(stageWidth, std::vector<int>(stageHeight, -1));			//そのノードの親ノードの座標Z

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
				path.pop_back();
				return path;
			}
			else {
				// パスが短すぎる場合は目標自体を返す
				path.clear();
				return path;
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
					if (!closedList[newX][newZ] && mapData_[newZ][newX] == Stage::MAP::FLOAR) {
						int tentativeA = value[x][z];
						int tentativeB = mapCost[newX][newZ];
						int tentativeG = tentativeA + tentativeB;
						int currentCost = value[newX][newZ];
						bool flag = tentativeG < currentCost;

						//ここ変える
						//計算結果がallCostより小さければpushBackする
						//計算方法が、親ノードのスタート地点からの距離＋推定コスト(ゴール - 座標 : x,zで高い値)
						int cellCost = value[x][z] + abs(i) + abs(j);
						int dxValue = targetX - newX;
						int dzValue = targetZ - newZ;
						int dValue = cellCost;
						if (dxValue >= dzValue) dValue += dxValue;
						else dValue += dzValue;
						
						flag = dValue < allCost[newX][newZ];

						// 新しい経路が現在の最良経路より短いか確認
						if (flag) {
							// 隣接ノードの情報を更新
							value[newX][newZ] = tentativeG;
							parentX[newX][newZ] = x;
							parentZ[newX][newZ] = z;
							allCost[newX][newZ] = dValue;

							// 隣接ノードをopenListに追加
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