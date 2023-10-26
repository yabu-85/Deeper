#include "EnemyUi.h"
#include "EnemyBase.h"
#include "Aim.h"
#include "Engine/Camera.h"

namespace {
    const unsigned maxTime = 4294967295;
    const float maxSizeX = 5.0f;
}

EnemyUi::EnemyUi(EnemyBase* parent)
	:vHandle_{-1, -1}, pParent_(parent)
{
}

void EnemyUi::Initialize()
{
    data_.textureFileName = "Png/Gauge2.png";
    data_.delay = maxTime;
    data_.number = 1;
    data_.lifeTime = maxTime;
    data_.direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data_.directionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data_.speed = 0.0f;
    data_.speedRnd = 0.0f;
    data_.size = XMFLOAT2(maxSizeX / 2, 0.5f);
    data_.sizeRnd = XMFLOAT2(0.0f, 0.0f);
    data_.scale = XMFLOAT2(1.0f, 1.0f);
    data_.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
    data_.deltaColor = XMFLOAT4(0.0f, 0.0f , 0.0f, 0.0f);
    data_.isBillBoard = true;
	vHandle_[0] = VFX::Start(data_);	//�G�~�b�^�[��ݒu

    data_.textureFileName = "Png/GaugeFrame2.png";
    data_.delay = maxTime;
    data_.number = 1;
    data_.lifeTime = maxTime;
    data_.direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data_.directionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data_.speed = 0.0f;
    data_.speedRnd = 0.0f;
    data_.size = XMFLOAT2(maxSizeX, 0.5f);
    data_.sizeRnd = XMFLOAT2(0.0f, 0.0f);
    data_.scale = XMFLOAT2(1.0f, 1.0f);
    data_.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
    data_.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
    data_.isBillBoard = true;
    vHandle_[1] = VFX::Start(data_);	//�G�~�b�^�[��ݒu

}

void EnemyUi::Update()
{
    XMFLOAT3 pos = pParent_->GetPosition();
    pos.y += 3.0f;
    VFX::SetParticlePosition(vHandle_[1], pos);

    if (true) {
        //�I�t�Z�b�g�̒l�����߂Ă���Ƃ�����
        XMMATRIX mat;
        mat.r[0] = Camera::GetViewMatrix().r[0];
        mat.r[1] = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        mat.r[2] = Camera::GetViewMatrix().r[2];
        mat.r[3] = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

        const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
        XMVECTOR direction = XMVector3TransformNormal(forwardVector, mat);
        XMFLOAT4 directionTest;
        XMStoreFloat4(&directionTest, direction);
        XMFLOAT3 direc = XMFLOAT3(directionTest.x, directionTest.y, directionTest.z);

        OutputDebugString("UI : ");
        std::string strNumber = std::to_string(direc.x);
        OutputDebugStringA(strNumber.c_str());
        OutputDebugString(", ");

        strNumber = std::to_string(direc.z);
        OutputDebugStringA(strNumber.c_str());
        OutputDebugString("\n");
        
        pos.x -= direc.z * 1.5f;
        pos.z -= direc.x * 1.5f;
    }

    VFX::SetParticlePosition(vHandle_[0], pos);

}
