#pragma once
#include <DirectXMath.h>
using namespace DirectX;

//�ړI�n�܂ł܂ł̍œK�Ȍo�H��T�����A���ɐi�ވʒu������

//�E�H�[�t���[���̃}�b�v�������������^AI�炵������Q�l�ɂ����炦�����Ȃ���

class NavigationAI
{

public:
	void Navi(XMFLOAT3 target, XMFLOAT3 pos);
	void Navi(XMFLOAT3& target);

};

