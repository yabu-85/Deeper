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
	void SetTriangle(Triangle& t);

	//Setter
	void ResetTriangles();

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

	void SetTransform(Transform& _t);
	void SetPosition(XMFLOAT3& _t);

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