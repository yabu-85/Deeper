#pragma once
#include "Engine/GameObject.h"
#include <vector>

struct IntersectData {
    int hModelNum;
    XMFLOAT3 position;
    XMFLOAT3 scale;

    IntersectData() : hModelNum(-1), position{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f } { }
    IntersectData(int num, XMFLOAT3 pos) : hModelNum(num), position(pos), scale{1.0f, 1.0f, 1.0f} { }
    IntersectData(int num, XMFLOAT3 pos, XMFLOAT3 sca) : hModelNum(num), position(pos), scale(sca) { }
};

//�f�o�b�O�p
const float floarSize = 5.0f;
const float smallSize = 1.0f;

class CreateStage
{
public:
    enum StageNum {
        FLOAR = 0,
        WALL,
		WARP,
        MAX,
    };

    enum RayStageNum {
        R_FLOAR = 0,
        R_WALL,
		R_WARP,
		R_MAX,
    };

private:
    int hModel_[MAX + R_MAX];
    std::vector<IntersectData> intersectDatas_; //���[�|���̃f�[�^�W

    int mapSizeX_;
    int mapSizeZ_;
    XMFLOAT3 startPos_;
    std::vector<std::vector<int>> mapData_;

public:
    enum MAP {
        M_FLOAR,
        M_WALL,
    };

    CreateStage();
    ~CreateStage();
	void Initialize();
    void Update();
    void Draw();
    void Release();

    std::vector<IntersectData> GetIntersectDatas() { return intersectDatas_; };
    std::vector<std::vector<int>> GetMapData() { return mapData_; };
    XMFLOAT3 GetPlayerStartPos();
    
    //���S�Ƀ����_���ȏ��̃|�W�V�������擾����
    XMFLOAT3 GetRandomFloarPosition();
    
    //�v���C���[���猩������:Position������range�̋����ōœK�ȏꏊ���擾����
    XMFLOAT3 GetFloarPosition(XMFLOAT3 position, float range);

    void CreateStageData(std::string name);

};

/*
//�e�V�[���̃X�e�[�W����Ă����
class CreateStage
{
	//�X�e�[�W�쐬�ɕK�v�ȏ��
	struct CreateStageInfo
	{
		GameObject* parent;         //�e�I�u�W�F�N�g
		std::string ModelPathName;  //���f���p�X�l�[��
		std::string inName;         //�I�u�W�F�N�g�̖��O
		Transform   t;              //�g�����X�t�H�[��
		XMFLOAT3    camPos;         //�J�����̃|�W�V����(�K�v�ȃI�u�W�F�N�g�ƕK�v����Ȃ��I�u�W�F�N�g������)
	};

	//���郉�C�g�̃R���g���[���[����x�������Ȃ��̂�flag�ō�������ǂ����m�F����
	bool createShineController_;

	//�t�@�C�����[�h���ē�������ۑ����Ă���(LoadFile�Ƃ����֐��Ŏg��)
	//�D���ȃ^�C�~���O�ŃX�e�[�W����ł���悤��
	std::vector<CreateStageInfo> info_;

	//�쐬�����X�e�[�W�̃I�u�W�F�N�g�S�ĕۑ����Ă���
	std::vector<GameObject*> createStageAllObject_;

	//�e�X�e�[�W�̃u���b�N�̃g�����X�t�H�[����ۑ�
	//std::vector<Block*> tBlock_;

public:

	//�R���X�g���N�^
	CreateStage();

	//�I�u�W�F�N�g�쐬
	void CreateObject(GameObject* parent, std::string ModelPathName, std::string inName, Transform t, XMFLOAT3 camPos);

	/// <summary>
	/// �e�X�e�[�W�̃t�@�C�����[�h���X�e�[�W���쐬���Ă����
	/// </summary>
	/// <param name="parent">�쐬����X�e�[�W�̐e�I�u�W�F�N�g</param>
	/// <param name="filename">���[�h�������t�@�C��</param>
	void LoadFileCreateStage(GameObject* parent, std::string filename);

	/// <summary>
	/// �e�X�e�[�W�̃t�@�C�����[�h�������Ă����
	/// </summary>
	/// <param name="parent">�쐬����X�e�[�W�̐e�I�u�W�F�N�g</param>
	/// <param name="filename">���[�h�������t�@�C��</param>
	void LoadFile(GameObject* parent, std::string filename);

	/// <summary>
	/// LoadFile���g���ă��[�h�����t�@�C�������ɃX�e�[�W���쐬
	/// </summary>
	void LoadFileBasedCreateStage();

	/// <summary>
	/// �쐬�����X�e�[�W�����ׂč폜
	/// </summary>
	void AllCreateStageDelete();

	/// <summary>
	/// �쐬�����X�e�[�W���ׂĕ`��ƍX�V���Ȃ��悤��
	/// </summary>
	void AllCreateStageInvisibleAndLeave();

	/// <summary>
	/// �쐬�����X�e�[�W���ׂĕ`��ƍX�V����悤��
	/// </summary>
	void AllCreateStageVisibleAndEnter();

	/// <summary>
	/// �e�u���b�N�̃|�C���^�Q�b�g
	/// </summary>
	/// <returns>�e�u���b�N�̃|�C���^�ۊǂ��Ă���vector</returns>
	//std::vector<Block*> GetTransformBlock() { return tBlock_; }

	/// <summary>
	/// ���ʌ^�̐���(�摜��Mob���p�����Ă��Ȃ��Q�[���I�u�W�F�N�g)
	/// </summary>
	/// <typeparam name="T">�����������N���X�̖��O</typeparam>
	/// <param name="pParent">�e</param>
	template <class T>
	T* InstantiateNormal(GameObject* pParent, std::string modelPath, std::string name, Transform t);

	//���ʌ^�̐���(���O�ƃ��f���p�X�w�肵�Ȃ�)
	template <class T>
	T* InstantiateNormal(GameObject* pParent, Transform t);

	//Mob���p������3D�I�u�W�F����
	template <class T>
	T* InstantiateMob3D(GameObject* pParent, std::string modelPath, std::string name, Transform t);

	//�{�^���̐���
	template <class T>
	T* InstantiateButton(GameObject* pParent, std::string modelPath, std::string name, Transform t);

	//�u���b�N�̐���
	template <class T>
	T* InstantiateBlock(GameObject* pParent, std::string modelPath, std::string name, Transform t);

	//���̐���
	template <class T>
	T* InstantiatePlanet(GameObject* pParent, std::string modelPath, std::string name, Transform t);
};

*/
