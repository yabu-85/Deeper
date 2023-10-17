#pragma once
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"
#include "Engine/Transform.h"
#include <vector>
#include "Triangle.h"

class Cell
{
	//�������̍��W
	XMFLOAT3 position_;

	//cube�̈�ӂ̒���
	float length_ = 0;

	//���_�ʒu posision
	XMFLOAT3 verPos_[8];

	//����CELL�ɑ�����O�p�|���S��
	std::vector<Triangle*>Triangles;
	int NumTriangles = 0;

public:
	Cell();

	void SetPosLeng(XMFLOAT3 pos, float leng);

	//�O�p�|���S����AAB�ɐG��Ă����炱��CELL�ɓo�^
	bool SetTriangle(Triangle& t);

	//Setter
	void ResetTriangles();

	//Getter
	XMFLOAT3 GetPosision() { return position_; };
	std::vector<Triangle*>& GetTriangles() { return Triangles; };

};


class CellBox : public GameObject
{
	int hModel_ = -1;

public:
	CellBox(GameObject* parent) : GameObject(parent, "CollisionMap"), hModel_(-1) {};
	~CellBox() {};
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};
};


class CPolygon
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

	std::vector<int> index_; //�C���f�b�N�X���
	int indexNum_;
	int vertexNum_;
	std::vector<VERTEX> vertices_;

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@

public:
	CPolygon();
	virtual ~CPolygon();

	void Release();
	HRESULT Initialize(XMFLOAT3& ver1, XMFLOAT3& ver2, XMFLOAT3& ver3);
	void Draw();

	void SetVertex(XMFLOAT3& ver1, XMFLOAT3& ver2, XMFLOAT3& ver3);

private:
	//  Initialize����Ă΂��֐�-----------------
	virtual void InitVertexData(XMFLOAT3& ver1, XMFLOAT3& ver2, XMFLOAT3& ver3); //���_���̏���
	HRESULT CreateVertexBuffer(XMFLOAT3& ver1, XMFLOAT3& ver2, XMFLOAT3& ver3);  //���_�o�b�t�@���쐬

	virtual void InitIndexData();  //�C���f�b�N�X��������
	HRESULT CreateIndexBuffer();  //�C���f�b�N�X�o�b�t�@���쐬

	HRESULT CreateConstantBuffer(); //�R���X�^���g�o�b�t�@���쐬

	// Draw�֐�����Ă΂��֐�--------------------
	void PassDataToCB(Transform transform);	//�R���X�^���g�o�b�t�@�Ɋe�����n��
	void SetBufferToPipeline();                         //�e�o�b�t�@
};