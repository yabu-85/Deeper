#include "Cell.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

namespace {
	bool boxDraw = false;
}

Cell::Cell()
	:position_(0,-99999,0), length_(0)
{
	for (int i = 0; i < 8; i++) verPos_[i] = XMFLOAT3(0, 0, 0);
}

void Cell::SetPosLeng(XMFLOAT3 pos, float leng)
{
	position_ = pos;
	length_ = leng;
	
	//�����v��荶��X�^�[�g�i�ォ�王�_�j
	verPos_[0] = XMFLOAT3(pos.x, pos.y, pos.z + leng);
	verPos_[1] = XMFLOAT3(pos.x + leng, pos.y, pos.z + leng);
	verPos_[2] = XMFLOAT3(pos.x + leng, pos.y, pos.z);
	verPos_[3] = XMFLOAT3(pos.x, pos.y, pos.z);

	//�㎞�v���
	verPos_[4] = XMFLOAT3(pos.x, pos.y + leng, pos.z + leng);
	verPos_[5] = XMFLOAT3(pos.x + leng, pos.y + leng, pos.z + leng);
	verPos_[6] = XMFLOAT3(pos.x + leng, pos.y + leng, pos.z);
	verPos_[7] = XMFLOAT3(pos.x, pos.y + leng, pos.z);

}

bool Cell::SetTriangle(Triangle& t)
{
	XMFLOAT3* tp = t.GetPosition();

	//�S���_�� Cell�̒��_�ʒu���O���ɂ���ꍇ�́A�֐����I���
	if (tp[0].x < verPos_[7].x && tp[1].x < verPos_[7].x && tp[2].x < verPos_[7].x || //�S���_���E
		tp[0].x > verPos_[6].x && tp[1].x > verPos_[6].x && tp[2].x > verPos_[6].x || //�S���_����
		
		tp[0].y < verPos_[3].y && tp[1].y < verPos_[3].y && tp[2].y < verPos_[3].y || //�S���_����
		tp[0].y > verPos_[7].y && tp[1].y > verPos_[7].y && tp[2].y > verPos_[7].y || //�S���_����

		tp[0].z < verPos_[7].z && tp[1].z < verPos_[7].z && tp[2].z < verPos_[7].z || //�S���_����
		tp[0].z > verPos_[4].z && tp[1].z > verPos_[4].z && tp[2].z > verPos_[4].z )  //�S���_���O
	{
		return false;
	}

	Triangle* tri = new Triangle;
	tri->SetPosition(tp);

	XMVECTOR vec[3];
	for (int i = 0; i < 3; i++) vec[i] = XMLoadFloat3(&tp[i]);
	tri->CreatTriangle(vec[0], vec[1], vec[2]);

	Triangles.push_back(tri);

	return true;
}

void Cell::ResetTriangles()
{
	for (Triangle* t : Triangles) {
		delete t;
	}

	Triangles.clear();
}

void CellBox::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Model/BoxCollider.fbx");
	assert(hModel_ >= 0);

}

void CellBox::Update()
{
	if (Input::IsKeyDown(DIK_R)) boxDraw = !boxDraw;

}

void CellBox::Draw()
{
	if (boxDraw) {
		Transform t = transform_;
		t.position_ = {
			transform_.position_.x + (transform_.scale_.x / 2),
			transform_.position_.y + (transform_.scale_.y / 2),
			transform_.position_.z + (transform_.scale_.z / 2) };

		Model::SetTransform(hModel_, t);
		Model::Draw(hModel_, 2);
	}

}




//---------------------------------------------------------------------------------------------


#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

CPolygon::CPolygon() :pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), vertexNum_(0), indexNum_(0)
{
}

CPolygon::~CPolygon()
{
	CPolygon::Release();
}

void CPolygon::Release()
{
	SAFE_RELEASE(pVertexBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pConstantBuffer_);
}

HRESULT CPolygon::Initialize(XMFLOAT3& ver1, XMFLOAT3& ver2, XMFLOAT3& ver3)
{
	HRESULT hr;
	hr = CreateVertexBuffer(ver1, ver2, ver3); //���_���̏���
	if (FAILED(hr)) {
		MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	hr = CreateIndexBuffer();  //�C���f�b�N�X�o�b�t�@���쐬
	if (FAILED(hr)) {
		MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	hr = CreateConstantBuffer(); //�R���X�^���g�o�b�t�@���쐬
	if (FAILED(hr)) {
		MessageBox(nullptr, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

void CPolygon::Draw()
{
	Direct3D::SetShader(Direct3D::SHADER_OUTLINE);

	transform_.Calclation();//�g�����X�t�H�[�����v�Z
	PassDataToCB(transform_);
	SetBufferToPipeline();
	Direct3D::pContext_->DrawIndexed(index_.size(), 0, 0);

	Direct3D::SetShader(Direct3D::SHADER_3D);
}

void CPolygon::SetVertex(XMFLOAT3& ver1, XMFLOAT3& ver2, XMFLOAT3& ver3)
{
	Initialize(ver1, ver2, ver3);
}

void CPolygon::InitVertexData(XMFLOAT3& ver1, XMFLOAT3& ver2, XMFLOAT3& ver3)
{
	// ���_���
	vertices_ =
	{
		{ XMVectorSet(ver1.x, ver1.y, ver1.z, 0.0f), XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f) },   // �l�p�`�̒��_�i����j0
		{ XMVectorSet(ver2.x, ver2.y, ver2.z, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) },   // �l�p�`�̒��_�i�E��j1
		{ XMVectorSet(ver3.x, ver3.y, ver3.z, 0.0f), XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) },   // �l�p�`�̒��_�i�E���j2
	};

	vertexNum_ = vertices_.size();
}

HRESULT CPolygon::CreateVertexBuffer(XMFLOAT3& ver1, XMFLOAT3& ver2, XMFLOAT3& ver3)
{
	InitVertexData(ver1, ver2, ver3);

	// ���_�f�[�^�p�o�b�t�@�̐ݒ�
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
	return S_OK;
}

void CPolygon::InitIndexData()
{
	//�C���f�b�N�X���
	index_ = { 0,1,2, 0,2,1 };

	indexNum_ = sizeof(index_);
}

HRESULT CPolygon::CreateIndexBuffer()
{
	InitIndexData();

	// �C���f�b�N�X�o�b�t�@�𐶐�����
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index_.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

void CPolygon::PassDataToCB(Transform transform)
{
	//�R���X�^���g�o�b�t�@�ɓn�����
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matNormal = XMMatrixTranspose(transform.matRotate_ * XMMatrixInverse(nullptr, transform.matScale_));

	//�R���X�^���g�o�b�t�@�̍X�V
	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�
	Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//�ĊJ
}

void CPolygon::SetBufferToPipeline()
{
	//���_�o�b�t�@
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//�R���X�^���g�o�b�t�@
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p
}

HRESULT CPolygon::CreateConstantBuffer()
{
	//�R���X�^���g�o�b�t�@�쐬
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	HRESULT hr;
	// �R���X�^���g�o�b�t�@�̍쐬
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK); //
		return hr;
	}
	return S_OK;
}