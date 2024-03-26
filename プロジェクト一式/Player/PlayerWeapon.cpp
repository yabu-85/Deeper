#include "PlayerWeapon.h"
#include "Player.h"
#include "PlayerData.h"
#include "../InputManager.h"
#include "../Weapon/WeaponBase.h"
#include "../Weapon/MainSwordWeapon.h"
#include "../Weapon/NormalGunWeapon.h"
#include "../Weapon/StoneArmWeapon.h"
#include "../Engine/Direct3D.h"

#include "../Engine/Text.h"

namespace {
    const XMFLOAT2 WEAPON_BOX_POSITION1 = { 0.5f, -0.7f };
    const XMFLOAT2 WEAPON_BOX_POSITION2 = { 0.8f, -0.7f };
    const XMFLOAT2 WEAPON_DURANCE_POSITION = { 0.03f, 0.08f };
    const float WEAPON_PNG_SIZE = 0.8f;

    Text* pText = nullptr;
}

PlayerWeapon::PlayerWeapon(Player* pPlayer)
    : pPlayer_(pPlayer), pMainWeapon_(nullptr), pSubWeapon_{nullptr, nullptr}, currentSubIndex_(0), drawPictWeapon_{-1, -1}
{
    const std::string fileName1[] = { "WeaponBoxSV", "WeaponBoxSIV", "WeaponBoxNSV", "WeaponBoxNSIV" };
    for (int i = 0; i < SELECT_MAX; i++) {
        hPictSelect_[i] = Image::Load("Image/" + fileName1[i] + ".png");
        assert(hPictSelect_[i] >= 0);
    }
    drawPictSelect_[0] = SELECT_VALID;
    drawPictSelect_[1] = NO_SELECT_VALID;

    const std::string fileName2[] = { "StoneArmImage", "RedBoxImage", "RedBoxImage" };
    for (int i = 0; i < (int)WeaponObjectManager::WEAPON_TYPE::WT_MAX; i++) {
        hPictWeapon_[i] = Image::Load("Image/" + fileName2[i] + ".png");
        assert(hPictWeapon_[i] >= 0);
    }

    float scrSizeX = static_cast<float>(Direct3D::screenWidth_);
    float scrSizeY = static_cast<float>(Direct3D::screenHeight_);
    boxTrans_[0].position_ = {WEAPON_BOX_POSITION1.x, WEAPON_BOX_POSITION1.y, 0.f};
    boxTrans_[1].position_ = {WEAPON_BOX_POSITION2.x, WEAPON_BOX_POSITION2.y, 0.f};

    pText = new Text();
    pText->Initialize();

}

void PlayerWeapon::DrawWeapon()
{
    for (int i = 0; i < 2; i++) {
        //�Z���N�g�{�b�N�X�̕\��
        Image::SetTransform(hPictSelect_[drawPictSelect_[i]], boxTrans_[i]);
        Image::Draw(hPictSelect_[drawPictSelect_[i]]);

        //�����Ă镐���Image�̕\��
        if (drawPictWeapon_[i] != -1) {
            Image::SetTransform(hPictWeapon_[drawPictWeapon_[i]], weaponTrans_[i]);
            Image::Draw(hPictWeapon_[drawPictWeapon_[i]]);
            
            XMFLOAT2 pos = { weaponTrans_[i].position_.x, weaponTrans_[i].position_.y };
            pos.x = (pos.x + 1.0f) / 2.0f;
            pos.y = 1.0f - (pos.y + 1.0f) / 2.0f;
            pos.x += WEAPON_DURANCE_POSITION.x;
            pos.y += WEAPON_DURANCE_POSITION.y;
            pos.x = (pos.x * (float)Direct3D::screenWidth_);
            pos.y = (pos.y * (float)Direct3D::screenHeight_);
            pText->Draw((int)pos.x, (int)pos.y, pSubWeapon_[i]->GetDurability());
        }
    }
}

void SceneTransitionInitialize() {
    pText = new Text();
    pText->Initialize();
}

void PlayerWeapon::SetPlayerDataWeapon()
{
    pMainWeapon_ = Instantiate<MainSwordWeapon>(pPlayer_);

    for (int i = 0; i < 2; i++) {
        //�ǂ̕��킩��ǂݍ���
        int type = PlayerData::GetWeaponData(i).type_;
        WeaponBase* weapon = nullptr;
        if(type == PlayerData::STONE_ARM_WEAPON) SetWeapon(weapon = Instantiate<StoneArmWeapon>(pPlayer_));
        else if (type == PlayerData::NORMAL_BULLET_WEAPON) SetWeapon(weapon = Instantiate<NormalGunWeapon>(pPlayer_));
        
        //�ϋv�n�̓ǂݍ���
        if (weapon) weapon->SetDurability(PlayerData::GetWeaponData(i).durability_);
    }
}

void PlayerWeapon::SetWeapon(WeaponBase* weapon)
{
    //�󂢂Ă�����ǉ�����
    if (pSubWeapon_[0] == nullptr) {
        SetWeaponData(0, weapon);
        return;
    }
    else if (pSubWeapon_[1] == nullptr) {
        SetWeaponData(1, weapon);
        return;
    }

    //�㏑������
    pSubWeapon_[currentSubIndex_]->KillMe();
    SetWeaponData(currentSubIndex_, weapon);

}

void PlayerWeapon::WeaponChangeIndex()
{
    if (InputManager::IsCmd(InputManager::WEAPON_UP)) {
        currentSubIndex_ = 0;
        pMainWeapon_->Invisible();
        if (pSubWeapon_[0]) pSubWeapon_[0]->Visible();
        if (pSubWeapon_[1]) pSubWeapon_[1]->Invisible();
        drawPictSelect_[0] = (int)SELECT_VALID;
        drawPictSelect_[1] = (int)NO_SELECT_VALID;
    }
    else if (InputManager::IsCmd(InputManager::WEAPON_DOWN)) {
        currentSubIndex_ = 1;
        pMainWeapon_->Invisible();
        if (pSubWeapon_[1]) pSubWeapon_[1]->Visible();
        if (pSubWeapon_[0]) pSubWeapon_[0]->Invisible();
        drawPictSelect_[0] = (int)NO_SELECT_VALID;
        drawPictSelect_[1] = (int)SELECT_VALID;
    }
}

void PlayerWeapon::SubWeaponRemove()
{
    pSubWeapon_[currentSubIndex_]->KillMe();
    pSubWeapon_[currentSubIndex_] = nullptr;

    //�摜���\����
    drawPictWeapon_[currentSubIndex_] = -1;

}

//--------------------------------------------------------------------------

void PlayerWeapon::SetWeaponData(int index, WeaponBase* weapon)
{
    pSubWeapon_[index] = weapon;
    
    //���I�����Ă�index�EMainWeapon����\���Ȃ�T�u�����\����Ԃ�
    if(index == currentSubIndex_ && !pMainWeapon_->IsVisibled()) pSubWeapon_[index]->Visible();

    //�\������摜���Z�b�g
    if (weapon->GetWeaponType() == WeaponObjectManager::WEAPON_TYPE::WT_STONE) drawPictWeapon_[index] = (int)WeaponObjectManager::WEAPON_TYPE::WT_STONE;
    else if(weapon->GetWeaponType() == WeaponObjectManager::WEAPON_TYPE::WT_THROW) drawPictWeapon_[index] = (int)WeaponObjectManager::WEAPON_TYPE::WT_THROW;
    else if(weapon->GetWeaponType() == WeaponObjectManager::WEAPON_TYPE::WT_MELEE) drawPictWeapon_[index] = (int)WeaponObjectManager::WEAPON_TYPE::WT_MELEE;

    //box�̃T�C�Y�ɍ��킹��
    XMFLOAT3 size1 = Image::GetTextureSize(hPictSelect_[0]);
    XMFLOAT3 size2 = Image::GetTextureSize(hPictWeapon_[drawPictWeapon_[index]]);
    float size2Max = (size2.x > size2.y) ? size2.x : size2.y;
    weaponTrans_[index] = boxTrans_[index];
    weaponTrans_[index].scale_ = { size1.x / size2Max * WEAPON_PNG_SIZE, size1.y / size2Max * WEAPON_PNG_SIZE, 1.0f };

}
