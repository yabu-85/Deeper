#pragma once
#include "Image.h"
#include "Direct3D.h"

class Text
{
	int hPict_;

	unsigned int width_, height_;	//1�������̕��ƍ���
	char fileName_[MAX_PATH];
	unsigned int rowLength_;

public:
	Text();
	~Text();

	//�������i�f�t�H���g�̐ݒ�j
	//�ߒl�F�����^���s
	HRESULT Initialize();

	//�������i�I���W�i���̉摜���g���������j
	//�����FfileName	�摜�t�@�C����
	//�����FcharWidth	�摜�̂P�����̕��i�s�N�Z���j
	//�����FcharHeight	�摜�̂P�����̍����i�s�N�Z���j
	//�����FrowLength	�摜�̒��ɉ��ɉ���������ł��邩
	//�ߒl�F�����^���s
	HRESULT Initialize(const char* fileName, const unsigned int charWidth, const unsigned int charHeight, const unsigned int rowLength);


	//�`��i������j
	//�����Fx, y	�\���ʒu�i����j
	//�����Fstr		�\��������������
	void Draw(int x, int y, const char* str, Direct3D::BLEND_MODE blend = Direct3D::BLEND_MODE::BLEND_DEFAULT);

	//�`��i�����l�j
	//�����Fx, y	�\���ʒu�i����j
	//�����Fvalue	�\���������l
	void Draw(int x, int y, int value, Direct3D::BLEND_MODE blend = Direct3D::BLEND_MODE::BLEND_DEFAULT);

	//���
	void Release();
};

