#include "NavigationAI.h"
#include "Stage.h"

namespace {
	const int stageWidth = 25;
	const int stageHeight = 25;
	const int floarSize = 1;	//一マスのサイズ

}

void PathSmoothing(std::vector<XMFLOAT3>& path);

NavigationAI::NavigationAI(Stage* s)
{
	pStage_ = s;
	mapData_ = pStage_->GetMapData();

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
	target.x += (float)floarSize / 2.0f;
	target.z += (float)floarSize / 2.0f;
	pos.x += (float)floarSize / 2.0f;
	pos.z += (float)floarSize / 2.0f;

	int startX = static_cast<int>(pos.x / floarSize);
	int startZ = static_cast<int>(pos.z / floarSize);
	int targetX = static_cast<int>(target.x / floarSize);
	int targetZ = static_cast<int>(target.z / floarSize);

	//targetが壁の場合
	if (mapData_[targetZ][targetX] == Stage::MAP::WALL) {
		std::vector<XMFLOAT3> none;
		return none;
	}

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

			// 一番後列のデータはstartPosだから削除、マスのサイズを合わせる
			if (path.size() >= 2) {
				path.pop_back();
				for (auto p : path) {
					p.x -= (floarSize) / 2.0f;
					p.z -= (floarSize) / 2.0f;
				}

				PathSmoothing(path);
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
						int nodeCost = value[x][z] + mapCost[newX][newZ];

						//計算結果がallCostより小さければpushBackする
						//計算方法：親ノードのスタート地点からの距離＋推定コスト(ゴール - 座標 : x,zで高い値)
						int cellCost = value[x][z] + abs(i) + abs(j);	//斜め移動はプラス２
						int dxValue = targetX - newX;
						int dzValue = targetZ - newZ;
						if (dxValue >= dzValue) cellCost += dxValue;
						else cellCost += dzValue;
						
						// 新しい経路が現在の最良経路より短いか確認
						if (cellCost < allCost[newX][newZ]) {
							// 隣接ノードの情報を更新
							value[newX][newZ] = nodeCost;
							parentX[newX][newZ] = x;
							parentZ[newX][newZ] = z;
							allCost[newX][newZ] = cellCost;

							// 隣接ノードをopenListに追加
							openList.push_back({ newX, newZ, nodeCost });
						}
					}
				}
			}
		}
	}

	std::vector<XMFLOAT3> none;
	return none;
}

void PathSmoothing(std::vector<XMFLOAT3>& path) {
	const float alpha = 0.2f;			// 大きいほど、現在のパスが元のパスに迅速に近づきます。 大きいほど処理が速い
	const float beta = 0.1f;			// 大きいほど、隣接する点間での滑らかさが向上します。   大きいほど処理が遅い
	const float tolerance = 0.5;		// 変化量がこの値以下の時平滑化を終了。　　　　　　　　 大きいほど処理が速い
	float change = tolerance;			// パスの位置の変化量

	while (change >= tolerance) {
		change = 0.0f;

		for (int i = 1; i < path.size() - 1; ++i) {
			XMFLOAT3 prePath = path[i];

			XMVECTOR prePathVector = XMLoadFloat3(&prePath);
			XMVECTOR currentPathVector = XMLoadFloat3(&path[i]);
			XMVECTOR previousPathVector = XMLoadFloat3(&path[i - 1]);
			XMVECTOR nextPathVector = XMLoadFloat3(&path[i + 1]);

			XMVECTOR smoothedPathVector = currentPathVector - alpha * (currentPathVector - prePathVector);
			smoothedPathVector = smoothedPathVector - beta * (2.0f * smoothedPathVector - previousPathVector - nextPathVector);

			XMFLOAT3 smoothedPath;
			XMStoreFloat3(&smoothedPath, smoothedPathVector);

			path[i] = smoothedPath;

			change += XMVectorGetX(XMVector3LengthEst(smoothedPathVector - prePathVector));
		}
	}
}