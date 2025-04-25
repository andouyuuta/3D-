#pragma once
#include "SceneBase.h"

class GameClear : public SceneBase
{
public:
	// コンストラクタ
	GameClear(void);

	// デストラクタ
	~GameClear(void) override;

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