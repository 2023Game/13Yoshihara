#pragma once
#include "CRideableObject.h"
#include "CObjectBase.h"

class CInteractUI3D;

// 調べるオブジェクトのベースクラス
class CInteractObject : public CObjectBase
{
public:
	// コンストラクタ
	CInteractObject(ETaskPriority prio=ETaskPriority::eDefault,
		int sortOrder=0,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// デストラクタ
	virtual ~CInteractObject();

	// オブジェクト削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

	// 更新
	void Update();

	// 調べる内容のテキストを返す
	std::string GetInteractStr() const;

	// 調べられる状態かどうか
	virtual bool CanInteract() const;
	// 調べる（継承先で実装）
	virtual void Interact() = 0;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自分のコライダー</param>
	/// <param name="other">衝突した相手のコライダ―</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

#if _DEBUG
	// デバッグ表示用の名前を取得
	std::string GetDebugName() const;
	// デバッグ表示用の名前を設定
	void SetDebugName(std::string name);
#endif

protected:
	std::string mInteractStr;	// 調べる内容のテキスト
	CInteractUI3D* mpInteractUI3D;	// インタラクトUI
	CVector mUIOffsetPos;			// UIのオフセット座標

#if _DEBUG
	std::string mDebugName;		// デバッグ表示用の名前
#endif
};