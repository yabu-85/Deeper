#include "NavigationAI.h"
#include "CreateStage.h"
#include "../GameManager/GameManager.h"
#include <queue>

namespace {
	int stageWidth = 0;
	int stageHeight = 0;

	const int DIRX[] = { 0, +1,  0, -1 };	//下、右、上、左
	const int DIRY[] = { -1,  0, +1,  0 };
	
	const int AROUND_DIRX[] = { -1,  0, 1,  1, 1, 0, -1, -1 };	//回りの配列時計回り
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
			//壁の場合は0にして次へ
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

	//スタート・ターゲットが範囲外・壁の場合
	if (!IsPositionOnMap(startX, startZ) || mapData_[startZ][startX] == M_WALL ||
		!IsPositionOnMap(targetX, targetZ) || mapData_[targetZ][targetX] == M_WALL)
	{
		std::vector<XMFLOAT3> none;
		return none;
	}

	if(size < 0.5f) return Navi(startX, startZ, targetX, targetZ);
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

	// 一番後列のデータはstartPosだから削除
	if (path.size() >= 2) {
		path.pop_back();

		//PathSmoothing(path);
		return path;
	}
	// パスが短すぎる場合は目標自体を返す
	else {
		path.clear();
		return path;
	}
}

std::vector<XMFLOAT3> NavigationAI::Navi(int sx, int sz, int tx, int tz)
{
	std::priority_queue<Node, std::vector<Node>, CompareNodes> open;
	open.push(Node(sx, sz, 0));

	std::vector<std::vector<bool>> closedList(stageWidth, std::vector<bool>(stageHeight, false));	//探索済みか
	std::vector<std::vector<int>> allCost(stageWidth, std::vector<int>(stageHeight, INT_MAX));		//ノードのコスト
	std::vector<std::vector<int>> value(stageWidth, std::vector<int>(stageHeight, 0));				//スタート地点からの最短距離
	std::vector<std::vector<int>> parentX(stageWidth, std::vector<int>(stageHeight, -1));			//そのノードの親ノードの座標X
	std::vector<std::vector<int>> parentZ(stageWidth, std::vector<int>(stageHeight, -1));			//そのノードの親ノードの座標Z

	while (!open.empty()) {
		//OpenListの優先度高いやつを取得
		Node current = open.top();
		open.pop();
		int x = current.x;
		int z = current.z;

		//閉じる
		closedList[x][z] = true;

		//回りのノード計算
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				int newX = x + i;
				int newZ = z + j;

				// 目標に到達したか確認して・Path返答
				if (x == tx && z == tz) {
					return CreatePath(x, z, parentX, parentZ);
				}

				// 隣接ノードが範囲内かつ通行可能か確認
				if (!IsPositionOnMap(newX, newZ) || closedList[newX][newZ] || mapData_[newZ][newX] == M_WALL) continue;

				//斜め移動の場合は条件をプラス
				if (abs(i) + abs(j) >= 2 && mapData_[z + j][x] == M_WALL || mapData_[z][x + i] == M_WALL) continue;

				//スコアがallCostより小さければpushする
				//斜め移動はプラス２
				int cellCost = value[x][z] + abs(i) + abs(j);
				int dxValue = tx - newX;
				int dzValue = tz - newZ;
				if (dxValue >= dzValue) cellCost += dxValue;
				else cellCost += dzValue;

				// 新しい経路が現在の最良経路より短いか確認
				if (cellCost < allCost[newX][newZ]) {
					// 隣接ノードの情報を更新
					int map = mapCost_[newX][newZ];
					value[newX][newZ] = value[x][z] + map;
					parentX[newX][newZ] = x;
					parentZ[newX][newZ] = z;
					allCost[newX][newZ] = cellCost;

					// 隣接ノードをopenListに追加
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

	std::vector<std::vector<bool>> closedList(stageWidth, std::vector<bool>(stageHeight, false));	//探索済みか
	std::vector<std::vector<int>> allCost(stageWidth, std::vector<int>(stageHeight, INT_MAX));		//ノードのコスト
	std::vector<std::vector<int>> value(stageWidth, std::vector<int>(stageHeight, 0));				//スタート地点からの最短距離
	std::vector<std::vector<int>> parentX(stageWidth, std::vector<int>(stageHeight, -1));			//そのノードの親ノードの座標X
	std::vector<std::vector<int>> parentZ(stageWidth, std::vector<int>(stageHeight, -1));			//そのノードの親ノードの座標Z

	while (!open.empty()) {
		//OpenListの優先度高いやつを取得
		Node current = open.top();
		open.pop();
		int x = current.x;
		int z = current.z;

		//閉じる
		closedList[x][z] = true;

		//回りのノード計算
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				int newX = x + i;
				int newZ = z + j;
				
				//目標に到達した場合
				if (newX == tx && newZ == tz) return CreatePath(x, z, parentX, parentZ);

				// 隣接ノードが範囲内かつ通行可能か確認
				if (!IsPositionOnMap(newX, newZ) || closedList[newX][newZ] ||
					mapData_[newZ][newX] == M_WALL || mapSize_[newX][newZ] < size) continue;

				//斜め移動の場合は条件をプラス
				if (abs(i) + abs(j) >= 2 && (mapData_[z + j][x] == M_WALL || mapData_[z][x + i] == M_WALL)) continue;

				//スコアがallCostより小さければpushする
				//斜め移動はプラス２
				int cellCost = value[x][z] + abs(i) + abs(j);
				int dxValue = tx - newX;
				int dzValue = tz - newZ;
				if (dxValue >= dzValue) cellCost += dxValue;
				else cellCost += dzValue;

				// 新しい経路が現在の最良経路より短いか確認
				if (cellCost < allCost[newX][newZ]) {
					// 隣接ノードの情報を更新
					value[newX][newZ] = value[x][z] + mapCost_[newX][newZ];
					parentX[newX][newZ] = x;
					parentZ[newX][newZ] = z;
					allCost[newX][newZ] = cellCost;

					// 隣接ノードをopenListに追加
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
	const float alpha = 0.2f;			//大きいほど、元のPathに似ているPathができる。　　　　 大きいほど処理が速い
	const float beta = 0.3f;			//大きいほど、隣接する点間での滑らかさが向上する。　   大きいほど処理が遅い
	const float tolerance = 0.5f;		//変化量がこの値以下の時平滑化を終了。　　　　　　　　 大きいほど処理が速い
	float change = tolerance;			//パスの位置の変化量

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