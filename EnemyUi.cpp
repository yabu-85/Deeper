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
	:vHandle_{-1, -1}, pParent_(parent), size{0.0f, 0.0f}, parcent(1.0f), height_(0.0f)
{
}

EnemyUi::~EnemyUi()
{
}

void EnemyUi::Initialize(float height)
{
    data_.textureFileName = "Png/Gauge2.png";
    data_.delay = 0;
    data_.number = 1;
    data_.lifeTime = 1;
    data_.direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data_.directionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data_.speed = 0.0f;
    data_.speedRnd = 0.0f;
    data_.size = XMFLOAT2(maxSizeX, 0.3f);
    data_.sizeRnd = XMFLOAT2(0.0f, 0.0f);
    data_.scale = XMFLOAT2(1.0f, 1.0f);
    data_.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
    data_.deltaColor = XMFLOAT4(0.0f, 0.0f , 0.0f, 0.0f);
    data_.isBillBoard = true;
	vHandle_[0] = VFX::Start(data_);	//�G�~�b�^�[��ݒu

    size = XMFLOAT2(maxSizeX, 0.3f); 
    height_ = height;
}

void EnemyUi::Update()
{
    XMFLOAT3 pos = pParent_->GetPosition();
    pos.y += height_;
    size.x = maxSizeX * parcent;

    data_.position = pos;
    data_.size = size;
    vHandle_[0] = VFX::Start(data_);	//�G�~�b�^�[��ݒu
    
}
