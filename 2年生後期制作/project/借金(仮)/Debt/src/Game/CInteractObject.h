#pragma once
#include "CRideableObject.h"
#include "CColliderSphere.h"
class CBillBoardUI;
class CObjectBase;

class CInteractObject : public CObjectBase
{
public:
	CInteractObject(float radius, std::string objectName);
	virtual ~CInteractObject();

	// インタラクト判定
	void Interact();
	
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自分のコライダー</param>
	/// <param name="other">衝突した相手のコライダ―</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

protected:
	CColliderSphere* mpColliderSphere;
	CBillBoardUI* mpBillBoardUI;
	CObjectBase* mpPlayer;
	bool mIsInteract;		// インタラクトしているかどうか
	bool mIsInteractArea;	// インタラクトエリア内かどうか
	std::vector<std::string> mMenuItemPathList;	// メニューのアイテム用画像のパスリスト
	std::string mMenuSelectPath;	// メニューのセレクト画像のパス
};