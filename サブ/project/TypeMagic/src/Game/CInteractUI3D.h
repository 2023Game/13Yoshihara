#pragma once
#include "CObjectBase.h"

class CImage3D;

// 3D空間に配置するインタラクトUIのイメージクラス
class CInteractUI3D : public CObjectBase
{
public:
	// コンストラクタ
	CInteractUI3D(CObjectBase* owner);
	// デストラクタ
	~CInteractUI3D();

	// 持ち主を設定
	void SetOwner(CObjectBase* owner);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CObjectBase* mpOwner;		// インタラクトUIの持ち主

	CImage3D* mpInteractUIImg;	// インタラクトUIのイメージ
};