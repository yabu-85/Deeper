#pragma once
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"
#include "Engine/Transform.h"
#include <vector>
#include "Triangle.h"

class Cell
{
	//左奥下の座標
	XMFLOAT3 position_;

	//cubeの一辺の長さ
	float length_ = 0;

	//頂点位置 posision
	XMFLOAT3 verPos_[8];

	//このCELLに属する三角ポリゴン
	std::vector<Triangle*>Triangles;
	int NumTriangles = 0;

public:
	Cell();

	void SetPosLeng(XMFLOAT3 pos, float leng);

	//三角ポリゴンがAABに触れていたらこのCELLに登録
	bool SetTriangle(Triangle& t);

	//Setter
	void ResetTriangles();

	//Getter
	XMFLOAT3 GetPosision() { return position_; };
	std::vector<Triangle*>& GetTriangles() { return Triangles; };

};


class CellBox : public GameObject
{
	int hModel_ = -1;

public:
	CellBox(GameObject* parent) : GameObject(parent, "CollisionMap"), hModel_(-1) {};
	~CellBox() {};
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};
};


class CPolygon
{
	//コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal; //matWからNormal用に改名
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR normal;
	};

	Transform transform_;

	std::vector<int> index_; //インデックス情報
	int indexNum_;
	int vertexNum_;
	std::vector<VERTEX> vertices_;

	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

public:
	CPolygon();
	virtual ~CPolygon();

	void Release();
	HRESULT Initialize(XMFLOAT3& ver1, XMFLOAT3& ver2, XMFLOAT3& ver3);
	void Draw();

private:
	//  Initializeから呼ばれる関数-----------------
	virtual void InitVertexData(XMFLOAT3& ver1, XMFLOAT3& ver2, XMFLOAT3& ver3); //頂点情報の準備
	HRESULT CreateVertexBuffer(XMFLOAT3& ver1, XMFLOAT3& ver2, XMFLOAT3& ver3);  //頂点バッファを作成

	virtual void InitIndexData();  //インデックス情報を準備
	HRESULT CreateIndexBuffer();  //インデックスバッファを作成

	HRESULT CreateConstantBuffer(); //コンスタントバッファを作成

	// Draw関数から呼ばれる関数--------------------
	void PassDataToCB(Transform transform);	//コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline();                         //各バッファ
};