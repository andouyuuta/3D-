#pragma once
#include "SceneBase.h"

class GameOver : public SceneBase
{
public:
	// コンストラクタ
	GameOver(void);

	// デストラクタ
	~GameOver(void) override;

	void Init(void) override;		// 初期化
	void Update(void) override;		// 更新	
	void Draw(void) override;		// 描画
	void Release(void) override;	// 解放

private:
	// 画像
	int hightLight_;

	// カーソルが範囲内にあるかどうか
	bool gameFlg_;
	bool titleFlg_;

	// マウス座標
	Vector2 mousePos_;
};