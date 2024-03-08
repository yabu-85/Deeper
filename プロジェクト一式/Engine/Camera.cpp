#include "camera.h"
#include "Direct3D.h"
#include "Global.h"

XMFLOAT3 _position;
XMFLOAT3 _target;
XMMATRIX _view;
XMMATRIX _proj;
XMMATRIX _billBoard;

//�������i�v���W�F�N�V�����s��쐬�j
void Camera::Initialize()
{
	_position = XMFLOAT3(0, 3, -10);	//�J�����̈ʒu
	_target = XMFLOAT3( 0, 0, 0);	//�J�����̏œ_

	//�v���W�F�N�V�����s��
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
}

//�X�V�i�r���[�s��쐬�j
void Camera::Update()
{
	//�r���[�s��
	_view = XMMatrixLookAtLH(XMVectorSet(_position.x, _position.y, _position.z, 0),
		XMVectorSet(_target.x, _target.y, _target.z, 0), XMVectorSet(0, 1, 0, 0));


	//�r���{�[�h�s��
	//�i��ɃJ�����̕��������悤�ɉ�]������s��B�p�[�e�B�N���ł����g��Ȃ��j
	//http://marupeke296.com/DXG_No11_ComeOnBillboard.html
	_billBoard = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMLoadFloat3(&_target)- XMLoadFloat3(&_position), XMVectorSet(0, 1, 0, 0));
	_billBoard = XMMatrixInverse(nullptr, _billBoard);

}

//�œ_��ݒ�
void Camera::SetTarget(XMFLOAT3 target) { _target = target; }

//�ʒu��ݒ�
void Camera::SetPosition(XMFLOAT3 position) { _position = position; }

//�œ_���擾
XMFLOAT3 Camera::GetTarget() { return _target; }

//�ʒu���擾
XMFLOAT3 Camera::GetPosition() { return _position; }

//�r���[�s����擾
XMMATRIX Camera::GetViewMatrix() { return _view; }

//�v���W�F�N�V�����s����擾
XMMATRIX Camera::GetProjectionMatrix() { return _proj; }

//�r���{�[�h�p��]�s����擾
XMMATRIX Camera::GetBillboardMatrix(){	return _billBoard; }

bool Camera::IsPositionWithinScreen(XMFLOAT3 pos)
{
	XMVECTOR v2 = XMVector3TransformCoord(XMLoadFloat3(&pos), Camera::GetViewMatrix());
	v2 = XMVector3TransformCoord(v2, Camera::GetProjectionMatrix());
	float x = XMVectorGetX(v2);
	float y = XMVectorGetY(v2);
	
	if (x >= 1.0f || y >= 1.0f || x <= -1.0f || y <= -1.0f) return false;
	return true;
}

bool Camera::IsPositionWithinVector(XMFLOAT3 pos)
{
	XMFLOAT3 cPos = GetPosition();
	XMFLOAT3 cTar = GetTarget();

	//CameraPosition��CameraTarget�̋������CameraPosition��pos�Ƃ̋������Z���Ȃ�͈͓��Ƃ���
	float dist1 = DistanceCalculation(cPos, cTar);
	float dist2 = DistanceCalculation(cPos, pos);
	if (dist2 <= dist1) return true;

	//���όv�Z����
	XMVECTOR vec = XMVector3Normalize(XMLoadFloat3(&cTar) - XMLoadFloat3(&cPos));
	XMVECTOR tVec = XMVector3Normalize(XMLoadFloat3(&pos) - XMLoadFloat3(&cTar));
	float dot = XMVectorGetX(XMVector3Dot(tVec, vec));
	if (dot > 0.0f) return true;
	return false;
}
