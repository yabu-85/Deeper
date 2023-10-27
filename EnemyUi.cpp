#include "EnemyUi.h"
#include "EnemyBase.h"
#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/BillBoard.h"
#include "Engine/Texture.h"

namespace {
    const unsigned maxTime = 4294967295;
    const float maxSizeX = 5.0f;

}

EnemyUi::EnemyUi(EnemyBase* parent)
	:vHandle_{-1, -1}, pParent_(parent), size{0.0f, 0.0f}, parcent(1.0f)
{
}

EnemyUi::~EnemyUi()
{
//    VFX::End(vHandle_[0]);

}

void EnemyUi::Initialize()
{
    size = XMFLOAT2(maxSizeX, 0.5f);
    
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
//	vHandle_[0] = VFX::Start(data_);	//エミッターを設置

}

void EnemyUi::Update()
{
    XMFLOAT3 pos = pParent_->GetPosition();
    pos.y += 3.0f;
    size.x = maxSizeX * parcent;
    
//    VFX::SetParticleSize(vHandle_[0], size);
//    VFX::SetParticlePosition(vHandle_[0], pos);

}
