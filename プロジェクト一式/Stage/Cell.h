#pragma once
#include "../Engine/Camera.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Transform.h"
#include <vector>
#include "Triangle.h"

class BoxCollider;
class SphereCollider;

/// <summary>
/// コリジョンマップに登録されるワールドをマスで区切って登録
/// Cellの中にTriangleが登録され、登録されたので当たり判定とかやる
/// </summary>
class Cell
{
	//左奥下の座標
	XMFLOAT3 position_;

	//cubeの一辺の長さ
	float length_ = 0;

	//頂点位置 posision
	XMFLOAT3 verPos_[8];

	//このCELLに属する三角ポリゴン
	std::vector<Triangle*> triangles_;
	int NumTriangles = 0;

public:
	Cell();

	void SetPosLeng(XMFLOAT3 pos, float leng);

	//三角ポリゴンがAABに触れていたらこのCELLに登録
	bool SetTriangle(Triangle& t);

	//Setter
	void ResetTriangles();

	//Getter
	XMFLOAT3 GetPosision() { return position_; };
	std::vector<Triangle*>& GetTriangles() { return triangles_; };

	//当たった場合の最小距離を返す
	bool SegmentVsTriangle(RayCastData* _data, float& minDist);

};