#pragma once
#include "CGameMenuBase.h"
#include "JobType.h"

class CJobUnlockMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="prevMenu">一つ前のメニュー</param>
	CJobUnlockMenu(CGameMenuBase* prevMenu);
	// デストラクタ
	~CJobUnlockMenu();

	// 更新
	void Update();
	
private:
	// [TRASH]クリック時のコールバック関数
	void OnClickTrash();
	// [DELIVERY]クリック時のコールバック関数
	void OnClickDelivery();

	// アンロック金額を減らせたか
	bool IncreaseMoney(EJobType jobType);
};