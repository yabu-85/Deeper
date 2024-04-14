#include "Cell.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"

namespace {
	bool boxDraw = false;
}

Cell::Cell()
	:position_(0,-99999,0), length_(0)
{
	for (int i = 0; i < 8; i++) verPos_[i] = XMFLOAT3(0, 0, 0);
}

void Cell::SetPosLeng(XMFLOAT3 pos, float leng)
{
	position_ = pos;
	length_ = leng;
	
	//下時計回り左上スタート（上から視点）
	verPos_[0] = XMFLOAT3(pos.x, pos.y, pos.z + leng);
	verPos_[1] = XMFLOAT3(pos.x + leng, pos.y, pos.z + leng);
	verPos_[2] = XMFLOAT3(pos.x + leng, pos.y, pos.z);
	verPos_[3] = XMFLOAT3(pos.x, pos.y, pos.z);

	//上時計回り
	verPos_[4] = XMFLOAT3(pos.x, pos.y + leng, pos.z + leng);
	verPos_[5] = XMFLOAT3(pos.x + leng, pos.y + leng, pos.z + leng);
	verPos_[6] = XMFLOAT3(pos.x + leng, pos.y + leng, pos.z);
	verPos_[7] = XMFLOAT3(pos.x, pos.y + leng, pos.z);

}

bool Cell::SetTriangle(Triangle& t)
{
	Polygons tp = t.GetAllPosition();

	//全頂点が Cellの頂点位置より外側にある場合は、関数を終わる
	if (tp.pos1.x < verPos_[7].x && tp.pos2.x < verPos_[7].x && tp.pos3.x < verPos_[7].x || //全頂点が右
		tp.pos1.x > verPos_[6].x && tp.pos2.x > verPos_[6].x && tp.pos3.x > verPos_[6].x || //全頂点が左
		
		tp.pos1.y < verPos_[3].y && tp.pos2.y < verPos_[3].y && tp.pos3.y < verPos_[3].y || //全頂点が下
		tp.pos1.y > verPos_[7].y && tp.pos2.y > verPos_[7].y && tp.pos3.y > verPos_[7].y || //全頂点が上

		tp.pos1.z < verPos_[7].z && tp.pos2.z < verPos_[7].z && tp.pos3.z < verPos_[7].z || //全頂点が奥
		tp.pos1.z > verPos_[4].z && tp.pos2.z > verPos_[4].z && tp.pos3.z > verPos_[4].z )  //全頂点が前
	{
		return false;
	}

	triangles_.push_back(&t);
	return true;
}

void Cell::ResetTriangles()
{
	triangles_.clear();
}

bool Cell::SegmentVsTriangle(RayCastData* _data, float& minDist)
{
	bool hit = false;
	minDist = FBXSDK_FLOAT_MAX;

	for (int i = 0; i < (int)triangles_.size(); i++) {
		triangles_.at(i)->RayCast(_data);

		//レイ当たった・最小距離だったら上書き
		if (_data->hit && minDist > _data->dist) {
			minDist = _data->dist;
			hit = true;
		}
	}

	return hit;
}