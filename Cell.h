#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class Triangle;

class Cell
{
	//�������̍��W
	XMFLOAT3 position_;

	//�I�u�W�F�N�g��艺�ɂ���ł��߂����̍��W�𓾂�
	int GetNearestFloorY(
		const XMFLOAT3* pos,
		float offsetY, //�������x���������āA����˂�������ꍇ�̒����l 
		float ny,
		float* tanTheta,
		float* floorY
	)const { return 0; };

	//Axis Aligned Cube Box----------------------------------------------------
	//cube�̈�ӂ̒���
	float Len = 0;
	//�������̍��W
	float X = 0, Z = 0, Y = 0;
	//���_�ʒu posision
	XMFLOAT3 P[8];
	//�S�p�|���S���̖@��
	XMFLOAT3 N[6] = {
		XMFLOAT3(0, 0, 1),
		XMFLOAT3(0, 0,-1),
		XMFLOAT3(1, 0, 0),
		XMFLOAT3(-1, 0, 0),
		XMFLOAT3(0, 1, 0),
		XMFLOAT3(0,-1, 0),
	};
	//�S�p�|���S���̃C���f�b�N�X
	int Idx[6][4] = {
		0,1,2,3,
		4,5,6,7,
		1,5,3,7,
		0,4,2,6,
		4,5,0,1,
		6,7,2,3
	};
	//����CELL�ɑ�����O�p�|���S��---------------------------------------------------------
	std::vector<Triangle*>Triangles;
	int NumTriangles = 0;

public:
	//Axis Aligned Cube Box�̈�ӂ̒����ƒ��_���WP[8]�ݒ�
	void setLen_Pos(float len, float x, float y, float z) {};
	
	//�O�p�|���S����AAB�ɐG��Ă����炱��CELL�ɓo�^
	void regist(Triangle& t) {};
	
	//�J�v�Z���ƕǁA�V��Ƃ̓����蔻�菈��
	int capsule_triangles(XMFLOAT3* pos, float* dirY, int* jumpFlag, float radius, float height, float degree, int* step) const { return 0; };
	
	//�J�v�Z���Ə��|���S���Ƃ̓����蔻�菈��
	int capsule_floor_triangles(XMFLOAT3* pos, float* dirY, int* jumpFlag, float speed, float radius, float height, float degree, int step) const 
	{ return 0; };
	
	//
	int segment_triangles(XMFLOAT3& sp, XMFLOAT3* ep, float correct) { return 0; };

	int select(const XMFLOAT3& sp, const XMFLOAT3& ep) { return 0; };
	
	//Getter
	XMFLOAT3 GetPosision() { return position_; }
	std::vector<Triangle*>& GetTriangles() { return Triangles; }

};

class Quad
{
	//�R���X�^���g�o�b�t�@�[
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal; //matW����Normal�p�ɉ���
	};

	//���_���
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR normal;
	};

	Transform transform_;

protected:
	std::vector<int> index_; //�C���f�b�N�X���
	int indexNum_;
	int vertexNum_;
	std::vector<VERTEX> vertices_;

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@

public:
	Quad();
	virtual ~Quad();

	void Release();
	HRESULT Initialize();
	void Draw();

	void SetTransform(Transform& _t) { transform_ = _t; };
	void SetPosition(XMFLOAT3& _t) { transform_.position_ = _t; };

private:
	//  Initialize����Ă΂��֐�-----------------
	virtual void InitVertexData(); //���_���̏���
	HRESULT CreateVertexBuffer();  //���_�o�b�t�@���쐬

	virtual void InitIndexData();  //�C���f�b�N�X��������
	HRESULT CreateIndexBuffer();  //�C���f�b�N�X�o�b�t�@���쐬

	HRESULT CreateConstantBuffer(); //�R���X�^���g�o�b�t�@���쐬

	// Draw�֐�����Ă΂��֐�--------------------
	void PassDataToCB(Transform transform);	//�R���X�^���g�o�b�t�@�Ɋe�����n��
	void SetBufferToPipeline();                         //�e�o�b�t�@
};