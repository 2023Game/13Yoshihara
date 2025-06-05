#pragma once
#include "CObjectBase.h"

class CModel;

class CSky : public CObjectBase
{
public:
	// コンストラクタ
	CSky(CModel* model,
		const CVector& pos, const CVector& scale, const CVector& rotation);
	// デストラクタ
	~CSky();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// 空のモデル
	CModel* mpModel;
};