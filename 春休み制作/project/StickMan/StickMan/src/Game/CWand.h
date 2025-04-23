#pragma once
#include "CWeapon.h"

class CModel;
class CConnectPoint;

// 杖のクラス
class CWand : public CWeapon
{
public:
	// コンストラクタ
	CWand(CObjectBase* owner, ETag tag);
	// デストラクタ
	~CWand();

	// 更新
	void Update() override;

	// 描画
	void Render() override;

	// 衝突判定を行うコライダーをリストに追加
	void AddCollider(CCollider* col);
	// 衝突判定を行うコライダーをリストから取り除く
	void RemoveCollider(CCollider* col);
	
	/// <summary>
	/// 設定されているコライダーと衝突判定を行う
	/// </summary>
	/// <param name="hitPos">衝突した一番近い位置</param>
	/// <returns>衝突したか</returns>
	bool Ray(CVector& hitPos);

	// 接続部の有効無効を設定
	void SetConnect(bool isOnOff);
	// 接続されているか
	bool GetConnect();

private:
	CModel* mpModel;		// 杖のモデル
	CConnectPoint* mpPoint;	// 接続部

	// 視点方向のレイで衝突判定するコライダーのリスト
	std::list<CCollider*> mColliders;
};
