#pragma once
#include "../Engine/GameObject.h"
#include <vector>

enum MAP {
    M_FLOAR,
    M_WALL,
};

struct IntersectData {
    int hModelNum;
	int hRayModelNum;
    XMFLOAT3 position;
    XMFLOAT3 scale;

    IntersectData() : hModelNum(-1), hRayModelNum(-1), position{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f } {}
    IntersectData(int num, XMFLOAT3 pos) : hModelNum(num), hRayModelNum(-1), position(pos), scale{1.0f, 1.0f, 1.0f} {}
    IntersectData(int num, int ray, XMFLOAT3 pos) : hModelNum(num), hRayModelNum(ray), position(pos), scale{1.0f, 1.0f, 1.0f} {}
    IntersectData(int num, int ray, XMFLOAT3 pos, XMFLOAT3 sca) : hModelNum(num), hRayModelNum(ray), position(pos), scale(sca) {}
};

class CreateStage {
public:
    enum StageNum {
        FLOAR = 0,
        WALL,
        SMALL_WALL,
		R_FLOAR,
		R_WALL,
		R_SMALL_WALL,
		MAX,
    };

private:
    int hModel_[MAX];
    int mapSizeX_;
    int mapSizeZ_;
    
    std::vector<IntersectData> intersectDatas_; //���[�|���̃f�[�^�W
    std::vector<std::vector<int>> mapData_;

	struct Cell	{
		int x, z;
		Cell() : x(0), z(0) {};
		Cell(int _x, int _z) : x(_x), z(_z) {};
	};

public:
    CreateStage();
    ~CreateStage();
	void Initialize();
    void Draw();

	//RayCast�p�̃f�[�^���擾
    std::vector<IntersectData> GetIntersectDatas() { return intersectDatas_; };

	//�}�b�v�f�[�^�̎擾
	std::vector<std::vector<int>> GetMapData() { return mapData_; };
    
    //���S�Ƀ����_���ȏ��̃|�W�V�������擾����
    XMFLOAT3 GetRandomFloarPosition(float size = 0.0f);
    
    //�v���C���[���猩������:Position������range�̋����ōœK�ȏꏊ���擾����
    XMFLOAT3 GetPositionPlayerDirection(XMFLOAT3 position, float range);

	//CSV��ǂݎ��Map�f�[�^�̍쐬
    void CreateStageData(std::string name);

};