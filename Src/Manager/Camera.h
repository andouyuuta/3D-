#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"

class Player;

class Camera
{
public:
	static constexpr float MOVE_SPEED = 5.0f;
	static constexpr int  ROT_SPEED_DEG = 1;

	// カメラの高さ
	static constexpr float HEIGHT = 200.0f;

	// 追従対象からカメラまでの距離
	static constexpr float DIS_FOLLOW2CAMERA = 500.0f;

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void GameUpdate(void);
	void SetBeforeDraw(void);
	void Draw(void);
	void Release(void);

	// 追従するプレイヤーの設定
	void SetPlayer(Player* _player);

	VECTOR GetPos(void);

	bool IsMove(VECTOR _moveVec);

	VECTOR GetCameraAngles(void);

	// カメラの座標を初期位置に戻す
	void ReSet(void);

private:
	// カメラの位置
	VECTOR pos_;

	// 追従対象からのローカル座標
	VECTOR localPosFormPlayer_;

	// カメラの角度(rad)
	VECTOR angles_;

	// 追従するプレイヤー
	Player* followPlayer_;

	// カーソルの位置
	Vector2 prevMousePos_;
	Vector2 nowMousePos_;
};

