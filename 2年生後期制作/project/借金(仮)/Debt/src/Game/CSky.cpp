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

// どのオブジェクトより後ろに描画する
void CSky::Render()
{
	glDepthRange(0.99, 1.0); // 遠い位置に描画
	
	mpModel->Render(Matrix());

	glDepthRange(0.0, 1.0); // 通常の描画設定に戻す
}
