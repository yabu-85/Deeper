#include "Cell.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

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
	
	//�����v��荶��X�^�[�g�i�ォ�王�_�j
	verPos_[0] = XMFLOAT3(pos.x, pos.y, pos.z + leng);
	verPos_[1] = XMFLOAT3(pos.x + leng, pos.y, pos.z + leng);
	verPos_[2] = XMFLOAT3(pos.x + leng, pos.y, pos.z);
	verPos_[3] = XMFLOAT3(pos.x, pos.y, pos.z);

	//�㎞�v���
	verPos_[4] = XMFLOAT3(pos.x, pos.y + leng, pos.z + leng);
	verPos_[5] = XMFLOAT3(pos.x + leng, pos.y + leng, pos.z + leng);
	verPos_[6] = XMFLOAT3(pos.x + leng, pos.y + leng, pos.z);
	verPos_[7] = XMFLOAT3(pos.x, pos.y + leng, pos.z);

}

bool Cell::SetTriangle(Triangle& t)
{
	XMFLOAT3* tp = t.GetPosition();

	//�S���_�� Cell�̒��_�ʒu���O���ɂ���ꍇ�́A�֐����I���
	if (tp[0].x < verPos_[7].x && tp[1].x < verPos_[7].x && tp[2].x < verPos_[7].x || //�S���_���E
		tp[0].x > verPos_[6].x && tp[1].x > verPos_[6].x && tp[2].x > verPos_[6].x || //�S���_����
		
		tp[0].y < verPos_[3].y && tp[1].y < verPos_[3].y && tp[2].y < verPos_[3].y || //�S���_����
		tp[0].y > verPos_[7].y && tp[1].y > verPos_[7].y && tp[2].y > verPos_[7].y || //�S���_����

		tp[0].z < verPos_[7].z && tp[1].z < verPos_[7].z && tp[2].z < verPos_[7].z || //�S���_����
		tp[0].z > verPos_[4].z && tp[1].z > verPos_[4].z && tp[2].z > verPos_[4].z )  //�S���_���O
	{
		return false;
	}

	Triangles.push_back(&t);
	return true;
}

void Cell::ResetTriangles()
{
	for (Triangle* t : Triangles) {
		delete t;
	}

	Triangles.clear();
}

bool Cell::SegmentVsTriangle(RayCastData* _data, float& minDist)
{
	bool hit = false;
	minDist = FBXSDK_FLOAT_MAX;

	for (int i = 0; i < (int)Triangles.size(); i++) {
		Triangles.at(i)->RayCast(_data);

		//���C���������E�ŏ�������������㏑��
		if (_data->hit && minDist > _data->dist) {
			minDist = _data->dist;
			hit = true;
		}
	}

	return hit;
}

void Cell::MapDataVsBox(BoxCollider* collider)
{
}

#include "Player.h"
//�f�o�b�O�p�H�e�X�g�p�H�󒆂ɂ��鏈���ƁE�ǂ̏���
void Cell::MapDataVsSphere(SphereCollider* collider, XMFLOAT3 prePos)
{
	XMFLOAT3 pos = collider->GetGameObject()->GetPosition();
	float height = 0.75f;
	pos.y += height;

	Player* pPlayer = static_cast<Player*>(collider->GetGameObject());

	RayCastData data;
	data.start = pos;
	data.dir = XMFLOAT3(0.0f, -1.0f, 0.0f);
	float dist = 0.0f;
	bool hit = SegmentVsTriangle(&data, dist);
	if (hit && dist <= height) {
		XMFLOAT3 newPos = XMFLOAT3(pos.x, pos.y - height + (height - dist), pos.z);
		collider->GetGameObject()->SetPosition(newPos);
	}
	else {
		XMFLOAT3 newPos = pPlayer->GetPosition();
		pPlayer->SetPosition(XMFLOAT3(newPos.x, newPos.y - 0.1f, newPos.z));

		pos = collider->GetGameObject()->GetPosition();
		pos.y += height;
		data.start = pos;
		hit = SegmentVsTriangle(&data, dist);
		if (hit && dist <= height) {
			XMFLOAT3 newPos = XMFLOAT3(pos.x, pos.y - height + (height - dist), pos.z);
			collider->GetGameObject()->SetPosition(newPos);
		}
	}

	RayCastData data2;
	data2.start = pos;
	data2.dir = XMFLOAT3(0.0f, 1.0f, 0.0f);
	hit = SegmentVsTriangle(&data2, dist);
	if (hit) {
		collider->GetGameObject()->SetPosition(prePos);
	}
	
	for (int i = 0; i < (int)Triangles.size(); i++) {
	//	Triangles.at(i)->TestSphereTriangle(collider);
	}

}


//----------------------------------------------------------------------


void CellBox::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Model/BoxCollider.fbx");
	assert(hModel_ >= 0);

}

void CellBox::Update()
{
	//�f�o�b�O�p
	if (Input::IsKeyDown(DIK_R)) boxDraw = !boxDraw;

}

void CellBox::Draw()
{
	if (boxDraw) {
		Transform t = transform_;
		t.position_ = {
			transform_.position_.x + (transform_.scale_.x / 2),
			transform_.position_.y + (transform_.scale_.y / 2),
			transform_.position_.z + (transform_.scale_.z / 2) };

		Model::SetTransform(hModel_, t);
		Model::Draw(hModel_, 2);
	}

}