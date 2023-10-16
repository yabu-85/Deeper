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
	XMFLOAT3 GetPosision() { return position_; }
	std::vector<Triangle*>& GetTriangles() { return Triangles; }

};

class Quad
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

protected:
	std::vector<int> index_; //インデックス情報
	int indexNum_;
	int vertexNum_;
	std::vector<VERTEX> vertices_;

	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

public:
	Quad();
	virtual ~Quad();

	void Release();
	HRESULT Initialize();
	void Draw();

	void SetTransform(Transform& _t);
	void SetPosition(XMFLOAT3& _t);

private:
	//  Initializeから呼ばれる関数-----------------
	virtual void InitVertexData(); //頂点情報の準備
	HRESULT CreateVertexBuffer();  //頂点バッファを作成

	virtual void InitIndexData();  //インデックス情報を準備
	HRESULT CreateIndexBuffer();  //インデックスバッファを作成

	HRESULT CreateConstantBuffer(); //コンスタントバッファを作成

	// Draw関数から呼ばれる関数--------------------
	void PassDataToCB(Transform transform);	//コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline();                         //各バッファ
};