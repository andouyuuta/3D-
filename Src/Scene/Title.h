#pragma once
#include <DxLib.h>
#include <string>
#include "SceneBase.h"
#include "../Common/Vector2.h"


class Title : public SceneBase
{
public:

	//　テキスト関連
	//　ゲームスタート
	static constexpr float GAMESTART_TEXT_X = 1000.0f;							// 左上 X座標
	static constexpr float GAMESTART_TEXT_Y = 550.0f;							//		Y座標
	static constexpr float GAMESTART_TEXT_X_END = GAMESTART_TEXT_X + 350.0f;	// 右下 X座標
	static constexpr float GAMESTART_TEXT_Y_END = GAMESTART_TEXT_Y + 60.0f;		//		Y座標

	// タイトル関係の構造体
	struct text
	{
		Vector2 textLeftUpPos_;		// 文字の左上の座標
		Vector2 textRightDownPos_;	// 文字の右上の座標
		bool textFlg_;				// 文字選択フラグ
	};

	// コンストラクタ
	Title(void);

	// デストラクタ
	~Title(void) override;

	void Init(void) override;		// 初期化
	void Update(void) override;		// 更新	
	void Draw(void) override;		// 描画
	void Release(void) override;	// 解放

private:
	// 画像

	// カーソルが範囲内にあるかどうか
	bool mouseFlg_;

	// マウス座標
	Vector2 mousePos_;
};