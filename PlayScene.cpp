#include "PlayScene.h"
#include "GameManager.h"
#include "Engine/SceneManager.h"

#include "Engine/Input.h"

#define EFFEKSEER 1

#if EFFEKSEER
#include <EffekseerForDXLib.h>

int effectResourceHandle = -1;
int grBackgroundHandle = -1;
int grFrontHandle = -1;

// ���Ԃ�����������(����I�ɃG�t�F�N�g���Đ����邽��)
int time_ = 0;
float fTime_ = 0.0f;

// �t���X�N���[���؂�ւ��p�t���O��ݒ肷��B(F1�AF2�ŃE�C���h�E�A�t���X�N���[����؂�ւ����悤�ɂ���B)
bool isFullScreen = false;

// �G�t�F�N�g�̕\������ʒu��ݒ肷��B
float position_x = 0.0f;
float position_y = 0.0f;

// �Đ����̃G�t�F�N�g�̃n���h��������������B
int playingEffectHandle = -1;
#endif

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
}

void PlayScene::Initialize()
{
	GameManager::Initialize(this);

#if EFFEKSEER
	// DX���C�u�����̕\�����@���E�B���h�E���[�h�ɕύX����B
	ChangeWindowMode(true);

	//SetUserWindow(Direct3D::hWnd_);
	SetUserWindowMessageProcessDXLibFlag(0);

	//�`���𗠉�ʂɕύX����B
	SetDrawScreen(DX_SCREEN_BACK);

	// DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// DX���C�u����������������B
	if (DxLib_Init() == -1) {
		return;
	}

	// Effekseer������������B
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if (Effekseer_Init(5000) == -1)
	{
		DxLib_End();
		return;
	}

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// �G�t�F�N�g���\�[�X��ǂݍ��ށB
	// �ǂݍ��ގ��ɑ傫�����w�肷��B
	effectResourceHandle = LoadEffekseerEffect("Effect/ToonWater.efkefc", 1.0f);

	// ���ł������̂ŉ摜��ǂݍ��ށB
	grBackgroundHandle = LoadGraph(_T("Texture/Background.png"));
	grFrontHandle = LoadGraph(_T("Texture/Front.png"));

	// Z�o�b�t�@��L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetUseZBuffer3D(TRUE);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetWriteZBuffer3D(TRUE);
#endif
}

void PlayScene::Update()
{
	GameManager::Update();

	if (Input::IsKeyDown(DIK_C)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}

#if EFFEKSEER
	if (!ProcessMessage() && !ClearDrawScreen() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		// DX���C�u�����̃J������ݒ肷��B
		SetCameraPositionAndTarget_UpVecY(VGet(0, 10, -80), VGet(0, 0, 0));
		SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
		SetCameraNearFar(1.0f, 150.0f);

		// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
		Effekseer_Sync3DSetting();

		// ����I�ɃG�t�F�N�g���Đ�����
		if (time_ % 1 == 0)
		{
			// �G�t�F�N�g���Đ�����B
			if (Input::IsKey(DIK_E))
			playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);
		}

		// ���ł������̂ŉ摜��`�悷��B
		// �������ĕ`�悵����łȂ��ƁAEffekseer�͕`��ł��Ȃ��B
		DrawGraph(0, 0, grBackgroundHandle, TRUE);

		// �Đ����̃G�t�F�N�g���ړ�����B
		position_x = 30.0f * cos(fTime_);
		position_y = 30.0f * sin(fTime_);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle, position_x, position_y, 0);
		fTime_ += 0.02f;

		// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
		UpdateEffekseer3D();

		// 3D��\������B
		DrawCapsule3D(VGet(0.0f, 100.0f, 0.0f), VGet(0.0f, -100.0f, 0.0f), 6.0f, 16, GetColor(100, 100, 100), GetColor(255, 255, 255), TRUE);

		// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
		DrawEffekseer3D();

		// �G�t�F�N�g�̏�ɂ��摜��`��ł���B
		//DrawGraph(0, 0, grFrontHandle, TRUE);

		// �X�N���[�������ւ���B
		ScreenFlip();

		// ���Ԃ��o�߂�����B
		time_++;

	}
#endif

}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
	GameManager::Release();

#if EFFEKSEER
	// �G�t�F�N�g���\�[�X���폜����B(Effekseer�I�����ɔj�������̂ō폜���Ȃ��Ă�����)
	DeleteEffekseerEffect(effectResourceHandle);

	// Effekseer���I������B
	Effkseer_End();

	// DX���C�u�������I������B
	DxLib_End();
#endif

}
