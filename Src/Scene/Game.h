#pragma once
#include "SceneBase.h"

class Camera;
class Grid;
class Player;
class Enemy;
class EnemyManager;

class Game : public SceneBase
{
public:
	// コンストラクタ
	Game(void);

	// デストラクタ
	~Game(void) override;

	void Init(void) override;		// 初期化
	void Update(void) override;		// 更新	
	void Draw(void) override;		// 描画
	void Release(void) override;	// 解放

private:
	// レティクル
	int dot_;

	// グリッド線
	Grid* grid_;

	// カメラ
	Camera* camera_;

	// プレイヤー
	Player* player_;

	//敵
	Enemy* enemy_;
	EnemyManager* enemymng_;

	// 画像

};