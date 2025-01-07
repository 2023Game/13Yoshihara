#include "CSky.h"
#include "CModel.h"

// コンストラクタ
CSky::CSky(CModel* model,
	const CVector& pos, const CVector& scale, const CVector& rotation)
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mpModel(model)
{
	Position(pos);
	Scale(scale);
	Rotate(rotation);
}

// デストラクタ
CSky::~CSky()
{
}

// 更新
void CSky::Update()
{
}

// 描画
void CSky::Render()
{
	mpModel->Render(Matrix());
}
