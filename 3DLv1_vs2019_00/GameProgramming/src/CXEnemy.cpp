#include "CXEnemy.h"

//コライダの初期化
CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)
{

}

//合成行列の設定
void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.Matrix(&mpCombinedMatrix[11]);
	//体
	mColSphereBody.Matrix(&mpCombinedMatrix[8]);
	//剣
	mColSphereSword.Matrix(&mpCombinedMatrix[21]);
}
