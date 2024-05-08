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
	int length_;			//����
	int smooth_;			//���炩��
	int size_;				//���W�̐�
	float width_;			//����
	float alpha_;			//�����x

	bool moveAlpha_;		//���X�ɓ����ɂ��Ă���
	bool first_;			//�ŏ��̃f�[�^��
	bool clear_;			//�����Ă�����
	bool allClearReset_;	//���ׂď�������Clear����������

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_;	            //�摜

	list<XMFLOAT3> positions_;	    //�ߋ�length_�񕪂̈ʒu
	list<XMFLOAT3> positionsSub_;	//���W���w�肵���o�[�W�����Ŏg�p
public:
	PolyLine();
	void Update();
	void Draw();
	void Release();

	//�����FfileName	�摜�t�@�C����
	HRESULT Load(std::string fileName);
	
	//�S�Ẵ|�W�V���������Z�b�g
	void ResetPosition();

	//��Ԍ��̃f�[�^������
	void ClearLastPositions();

	//Clear��ture�ɁA������AllClear���ꂽ��Clear�t���O��false�ɂ��邩�ǂ���
	void SetClear(bool allClear);

	//Clear��������
	void ClearCancel();

	//���݂̈ʒu���L��������
	//�����Fpos1, pos2 �L��������ʒu
	void AddPosition(XMFLOAT3 pos1, XMFLOAT3 pos2);

	//���݂̈ʒu���L��������
	//�����Fpos	���݂̈ʒu
	void AddPosition(XMFLOAT3 pos);

	//���X�ɓ����ɂȂ�悤�ɐݒ�
	void SetMoveAlphaFlag() { moveAlpha_ = true; }
	void SetLength(int leng) { length_ = leng; }
	void SetWidth(float width) { width_ = width; }
	void SetSmooth(int smooth) { smooth_ = smooth; }

};

