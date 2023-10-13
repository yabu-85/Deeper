#include "Cell.h"

Cell::Cell()
	:position_(0,0,0), length_(0)
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

void Cell::SetTriangle(Triangle& t)
{
	XMFLOAT3* tp = t.GetPosition();
	if (tp[0].x < verPos_[0].x && tp[1].x < verPos_[0].x && tp[2].x < verPos_[0].x ||
		tp[0].x < verPos_[1].x && tp[1].x < verPos_[2].x && tp[2].x < verPos_[3].x ||
		
		tp[0].y < verPos_[0].y && tp[1].y < verPos_[0].y && tp[2].y < verPos_[0].y ||
		tp[0].y < verPos_[1].y && tp[1].y < verPos_[2].y && tp[2].y < verPos_[3].y ||

		tp[0].z < verPos_[0].z && tp[1].z < verPos_[0].z && tp[2].z < verPos_[0].z ||
		tp[0].z < verPos_[0].z && tp[1].z < verPos_[0].z && tp[2].z < verPos_[0].z )
	{
		return;
	}

	Triangle *tri = new Triangle;
	tri->SetPosition(tp);

	XMVECTOR vec[3];
	for(int i = 0;i < 3;i++) vec[i] = XMLoadFloat3(&tp[i]);
	tri->CreatTriangle(vec[0], vec[1], vec[2]);

	Triangles.push_back(tri);

}

void Cell::ResetTriangles()
{
	for (Triangle* t : Triangles) {
		delete t;
	}

	Triangles.clear();
}


//---------------------------------------------------------------------------------------------


#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

Quad::Quad() :pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), vertexNum_(0), indexNum_(0)
{
}

Quad::~Quad()
{
	Quad::Release();
}

void Quad::Release()
{
	SAFE_RELEASE(pVertexBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pConstantBuffer_);
}

HRESULT Quad::Initialize()
{
	HRESULT hr;
	hr = CreateVertexBuffer(); //���_���̏���
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

void Quad::Draw()
{
	Direct3D::SetShader(Direct3D::SHADER_UNLIT);

	transform_.Calclation();//�g�����X�t�H�[�����v�Z
	PassDataToCB(transform_);
	SetBufferToPipeline();
	Direct3D::pContext_->DrawIndexed(index_.size(), 0, 0);

	Direct3D::SetShader(Direct3D::SHADER_3D);
}

void Quad::SetTransform(Transform& _t)
{
	transform_ = _t;
}

void Quad::SetPosition(XMFLOAT3& _t)
{
	transform_.position_ = _t;
}

void Quad::InitVertexData()
{
	// ���_���
	vertices_ =
	{
		{ XMVectorSet(0.0f, 0.0f, 10.0f, 0.0f), XMVectorSet(-1.0f, 1.0f, -1.0f, 0.0f)},    // �l�p�`�̒��_�i����j0
		{ XMVectorSet(10.0f, 0.0f, 10.0f, 0.0f), XMVectorSet(1.0f, 1.0f, -1.0f, 0.0f) },   // �l�p�`�̒��_�i�E��j1
		{ XMVectorSet(10.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f) },   // �l�p�`�̒��_�i�E���j2
		{ XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f) },   // �l�p�`�̒��_�i�����j3

		{ XMVectorSet(0.0f, 10.0f, 10.0f, 0.0f), XMVectorSet(-1.0f, 1.0f, -1.0f, 0.0f)},   // �l�p�`�̒��_ 0�̏�
		{ XMVectorSet(10.0f, 10.0f, 10.0f, 0.0f), XMVectorSet(1.0f, 1.0f, -1.0f, 0.0f) },  // �l�p�`�̒��_ 1�̏�
		{ XMVectorSet(10.0f, 10.0f, 0.0f, 0.0f), XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f) },  // �l�p�`�̒��_ 2�̏�
		{ XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f) },  // �l�p�`�̒��_ 3�̏�

	};

	vertexNum_ = vertices_.size();
}

HRESULT Quad::CreateVertexBuffer()
{
	InitVertexData();

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

void Quad::InitIndexData()
{
	//�C���f�b�N�X���
	index_ = { 0,2,3, 0,1,2, 4,1,0, 4,5,1, 5,2,1, 5,6,2, 6,3,2, 6,7,3, 7,0,3, 7,4,0, 4,6,7, 4,5,6 };

	indexNum_ = sizeof(index_);
}

HRESULT Quad::CreateIndexBuffer()
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

void Quad::PassDataToCB(Transform transform)
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

void Quad::SetBufferToPipeline()
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

HRESULT Quad::CreateConstantBuffer()
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