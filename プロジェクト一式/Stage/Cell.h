#pragma once
#include "../Engine/Camera.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Transform.h"
#include <vector>
#include "Triangle.h"

class BoxCollider;
class SphereCollider;

/// <summary>
/// ƒRƒŠƒWƒ‡ƒ“ƒ}ƒbƒv‚É“o˜^‚³‚ê‚éƒ[ƒ‹ƒh‚ğƒ}ƒX‚Å‹æØ‚Á‚Ä“o˜^
/// Cell‚Ì’†‚ÉTriangle‚ª“o˜^‚³‚êA“o˜^‚³‚ê‚½‚Ì‚Å“–‚½‚è”»’è‚Æ‚©‚â‚é
/// </summary>
class Cell
{
	//¶‰œ‰º‚ÌÀ•W
	XMFLOAT3 position_;

	//cube‚Ìˆê•Ó‚Ì’·‚³
	float length_ = 0;

	//’¸“_ˆÊ’u posision
	XMFLOAT3 verPos_[8];

	//‚±‚ÌCELL‚É‘®‚·‚éOŠpƒ|ƒŠƒSƒ“
	std::vector<Triangle*> triangles_;
	int NumTriangles = 0;

public:
	Cell();

	void SetPosLeng(XMFLOAT3 pos, float leng);

	//OŠpƒ|ƒŠƒSƒ“‚ªAAB‚ÉG‚ê‚Ä‚¢‚½‚ç‚±‚ÌCELL‚É“o˜^
	bool SetTriangle(Triangle& t);

	//Setter
	void ResetTriangles();

	//Getter
	XMFLOAT3 GetPosision() { return position_; };
	std::vector<Triangle*>& GetTriangles() { return triangles_; };

	//“–‚½‚Á‚½ê‡‚ÌÅ¬‹——£‚ğ•Ô‚·
	bool SegmentVsTriangle(RayCastData* _data, float& minDist);

};