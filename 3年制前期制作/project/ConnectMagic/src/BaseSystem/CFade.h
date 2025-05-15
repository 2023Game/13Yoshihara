#pragma once
#include "CTask.h"
#include "CColor.h"
#include "CImage.h"
#include "FadeType.h"

#define DEFAULT_FADE_TIME 0.4f

// 画面のフェード処理
class CFade : public CTask
{
public:
	// インスタンスを取得
	static CFade* Instance(EFadeType type = EFadeType::eBlackOut);

	/// <summary>
	/// フェードカラーを設定
	/// </summary>
	/// <param name="color">設定する色</param>
	static void SetFadeColor(const CColor& color);

	/// <summary>
	/// フェードイン開始
	/// </summary>
	/// <param name="time">フェード時間</param>
	static void FadeIn(float time = DEFAULT_FADE_TIME);
	/// <summary>
	/// フェードアウト開始
	/// </summary>
	/// <param name="time">フェード時間</param>
	static void FadeOut(float time = DEFAULT_FADE_TIME);

	/// <summary>
	/// フェード中かどうか
	/// </summary>
	/// <returns>trueならば、フェード中</returns>
	static bool IsFading();

	// フェードの種類を変更する
	void ChangeFadeType(EFadeType type);

private:
	// フェードの種類
	EFadeType mType;

	// 暗転フェードの初期設定
	void InitBlackOut();
	// 渦状フェードの初期設定
	void InitVortex();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">フェードの種類</param>
	CFade(EFadeType type);
	// デストラクタ
	~CFade();

	/// <summary>
	/// フェード開始
	/// </summary>
	/// <param name="time">フェード時間</param>
	/// <param name="isFadeIn">フェードインかどうか</param>
	void StartFade(float time, bool isFadeIn);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// フェード中の処理
	void Fade();
	// フェード終了の処理
	void FadeEnd();

	// フェードクラスのインスタンス
	static CFade* ms_instance;

	CImage* mpFadeImage;	// フェード画像
	CColor mFadeColor;		// フェードカラー
	float mFadeTime;		// フェード時間
	float mElapsedTime;		// 経過時間
	bool mIsFadeIn;			// フェードインかどうか
	bool mIsFading;			// フェード中かどうか
};