#pragma once
#include "CObjectBase.h"

// 武器のベースクラス
class CWeapon : public CObjectBase
{
public:
	// コンストラクタ
	CWeapon(ETag tag);
	// デストラクタ
	virtual ~CWeapon();

	// 武器の持ち主を設定
	void SetOwner(CObjectBase* owner);
	// 武器の持ち主を取得
	CObjectBase* GetOwner() const;

	// 武器をアタッチする行列を設定
	void SetAttachMtx(const CMatrix* mtx);

	// 行列を取得
	CMatrix Matrix() const override;

	// 行列を更新
	void UpdateMtx();

protected:
	CObjectBase* mpOwner;		// 武器の持ち主
	const CMatrix* mpAttachMtx;	// くっつける行列のポインター
	CMatrix mAttachMtx;			// くっつける行列の本体
};
