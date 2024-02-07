#pragma once
#include <list>
#include "Direct3D.h"
#include "Texture.h"
#include <DirectXMath.h>
#include <string>
#include "BillBoard.h"

using namespace DirectX;
using namespace std;

class PolyLine
{
	const float WIDTH_;		//����
	const int LENGTH_;		//����

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_;	            //�摜

	struct PolyList {
		XMFLOAT3 position1;
		XMFLOAT3 position2;
	};
	list<PolyList> polyList_;
	list<XMFLOAT3> positions_;	    //�ߋ�length_�񕪂̈ʒu

	float alpha_;                   //�����x
	bool  moveAlpha_;               //���X�ɓ����ɂ��Ă���

public:
	//�R���X�g���N�^
	PolyLine();

	void ResetPosition();

	//���݂̈ʒu���L��������
	//�����Fpos1, pos2 �L��������ʒu
	void AddPosition(XMFLOAT3 pos1, XMFLOAT3 pos2);

	//���݂̈ʒu���L��������
	//�����Fpos	���݂̈ʒu
	void AddPosition(XMFLOAT3 pos);

	//���[�h
	//�����FfileName	�摜�t�@�C����
	//�ߒl�F����/���s
	HRESULT Load(std::string fileName);

	//�`��
	void Draw();

	//���X�ɓ����ɂȂ�悤�ɐݒ�
	void SetMoveAlphaFlag() { moveAlpha_ = true; }

	//���
	void Release();

};

