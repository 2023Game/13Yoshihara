#include "CTestField.h"

// コンストラクタ
CTestField::CTestField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TestField");

	// オブジェクトを生成
	CreateFieldObjects();
	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CTestField::~CTestField()
{
}

// フィールドのオブジェクトを生成
void CTestField::CreateFieldObjects()
{
}

// フィールドのコライダ―を生成
void CTestField::CreateCol()
{
	mpGroundColliderMesh = new CColliderMesh(this, ELayer::eGround, mpModel);
}
