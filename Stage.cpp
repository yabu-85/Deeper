#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include <vector>

namespace {
    bool drawCell = true;
    const float floarSize = 5.0f;

}

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), startPos_(0,0,0), mapSizeX_(0), mapSizeZ_(0)
{
    for (int i = 0; i < MAX + RMAX; i++) hModel_[i] = -1;
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    std::string fileName[] = { "StageT1", "StageT2", "RayStageT1", "RayStageT2" };
    for (int i = 0; i < MAX + RMAX; i++) {
        hModel_[i] = Model::Load("Model/Stage/" + fileName[i] + ".fbx");
        assert(hModel_[i] >= 0);
    }
    
    mapSizeX_ = 30;
    mapSizeZ_ = 50;

    startPos_ = XMFLOAT3( (float)(rand() % mapSizeX_), 0.0f, (float)(mapSizeZ_ - 1));

    CreatStage();
}

void Stage::Update()
{
    if (Input::IsKeyDown(DIK_E)) drawCell = !drawCell;

}

void Stage::Draw()
{
    for (int i = 0; i < intersectDatas_.size(); i++) {
        int handle = intersectDatas_.at(i).hModelNum - MAX;
        Transform trans;
        trans.position_ = intersectDatas_.at(i).position;
        if(handle == T1) trans.scale_ = XMFLOAT3(5.0f / 10.0f, 1.0f, 5.0f / 10.0f);
        Model::SetTransform(handle, trans);
        Model::Draw(handle, 0);
    }

    //Ray�p�̃��C���[�\��
    if (drawCell) {
        for (int i = 0; i < intersectDatas_.size(); i++) {
            int handle = intersectDatas_.at(i).hModelNum;
            Transform trans;
            trans.position_ = intersectDatas_.at(i).position;
            if (handle == RT1) trans.scale_ = XMFLOAT3(5.0f / 10.0f, 1.0f, 5.0f / 10.0f);
            Model::SetTransform(handle, trans);
            Model::Draw(handle, 2);
        }
    }

}

void Stage::Release()
{
}

void Stage::ResetStage()
{
}

//�E�H�[�t���[���̃}�b�v����������Č��悤���Ǝv���i�J�[�r�B�f�B�X�J�o���[�̃}�b�v��������肽����ˁF�ނ����������ǋL�������ς�����j
void Stage::CreatStage()
{
    //�G�̏o���ʒu�̃R���g���[���͈ȉ��̂悤�ɍs���܂��B
    //(1)�e�p�[�c�Ƀv���C���[���ړ��\�ȏꏊ���w�肳��A�}�b�v���g�ݏオ�������ɁA1�{�̃��C���Ńv���C���[�������\�������郋�[�g���C�����`�����
    //(2)�Q�[�����̓v���C���[������͈͂Ń��C���ɉ����ĉe���̓}�b�v����������A���͈͓̔��œG����������v���C���[���P��������

    //�G�̏o���̓^�C�����C���ɂ���Ɓu���[���[�R�[�X�^�[�̂悤�Ɂv���䂳��Ă��āA�ŏ���1�̂̓G�������̂��A���X�ɓG�̏P�����������Ȃ�
    //������s�[�N�ɒB���A�������|����Ǝb���͕��a���K���c�c�Ƃ������ɂȂ�܂��B
    //1�̃X�e�[�W�̏I���܂łɓ|���ׂ��G�̐������߂��Ă��āA����̕��𒴂���ƃv���C���[�̓S�[���ɒH�蒅�������o���܂��B

    
    //��邱�Ɓ[���܂��}�b�v��������[�����[�g���Ȃ��Ł[�����̏ꏊ�̉e���͂ɂ���ēG���X�|�[�������

    //�}�b�v
    //�܂��S�[���܂ł̃��[�g�Ƀp�X����
    //�S�[���܂ł̃p�X�𓱂��񂶂�Ȃ��āA�Ƃ肠�����p�X��������x�̏ꏊ�܂ň����čŏI�����̓����������I��goalPos�ɂȂ�
    
    //�}�b�v�̃T�C�Y���̂Q�����z���p�ӂ��āA��������ƂɃp�X���Ȃ��Ă���
    //�܂�������ԉ����㍡��͉��ōs�������ɃX�^�[�g�|�W�V������u����
    
    //�}�X��z�u���Ă�
    //�P�o�����̕������v�Z���A�󂢂Ă���e�ʂ��v�Z���āA�{�b�N�X�̃T�C�Y���v�Z���z�u�E���̕����Q�����z��ɓ���Ă�
    //���Ƃ����āA�o����z�u����}�X���v�Z

    mapData_.resize(mapSizeZ_);
    for (int i = 0; i < mapSizeZ_; i++)
    {
        mapData_[i].resize(mapSizeX_, NONE);
    }

    int StartsizeX = 5;
    int StartsizeZ = 3;
    XMFLOAT3 staPos = XMFLOAT3(startPos_.x, 0.0f, startPos_.z - StartsizeZ);
    CreatSquare(staPos, StartsizeX, StartsizeZ);
    //�X�^�[�g�̏o���͌Œ�ł������ǁA���̂��Ƃ̃}�X�ڂ͂Ȃ񂩂��ďo���}�X���v�Z���Ă���
    //
    

    //�󔒂̏ꏊ�͒��Ŗ��߂Ă݂�
    for (int z = 0; z < mapSizeZ_ -1; z++) {
        for (int x = 0; x < mapSizeX_ -1; x++) {
            if (mapData_[z][x] == NONE) {
                intersectDatas_.push_back({ hModel_[MAX + RT2], XMFLOAT3(5.0f * x, 0.0f, 5.0f * z) });
            }
        }
    }

}

XMFLOAT3 Stage::GetPlayerStartPos()
{
    return startPos_;
}

void Stage::CreatSquare(XMFLOAT3 start, int _x, int _z)
{
    for (int x = start.x; x < start.x + _x; x++) {
        for (int z = start.z; z < start.z + _z; z++) {
            intersectDatas_.push_back({ hModel_[MAX + RT1], XMFLOAT3(floarSize * (float)x , start.y, floarSize * (float)z) });
            mapData_[z][x] = FLOAR;
        }
    }
}
