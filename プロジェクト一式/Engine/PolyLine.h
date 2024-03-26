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
	float WIDTH_;		//����
	int LENGTH_;		//����
	int smooth_;		//���炩��
	int size_;			//���W�̐�
	bool first_;		//�ŏ��̃f�[�^��

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_;	            //�摜

	list<XMFLOAT3> positions_;	    //�ߋ�length_�񕪂̈ʒu
	list<XMFLOAT3> positionsSub_;	//���W���w�肵���o�[�W�����Ŏg�p

	float alpha_;                   //�����x
	bool  moveAlpha_;               //���X�ɓ����ɂ��Ă���

public:
	//�R���X�g���N�^
	PolyLine();

	//�S�Ẵ|�W�V���������Z�b�g
	void ResetPosition();

	//��Ԍ��̃f�[�^������
	void ClearLastPositions();

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
	void SetLength(int leng) { LENGTH_ = leng; }
	void SetWidth(float width) { WIDTH_ = width; }
	void SetSmooth(int smooth) { smooth_ = smooth; }

	//���
	void Release();

};

