#include <DxLib.h>
#include "Camera.h"
#include "../Manager/InputManager.h"
#include "../Application.h"
#include "../Object/Player.h"

Camera::Camera(void)
{
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// カメラの初期位置
	pos_ = { 0.0f, 0.0f, 0.0f };

	// 追従対象からのローカル座標設定
	localPosFormPlayer_ = { 0.0f, HEIGHT, -DIS_FOLLOW2CAMERA };

	// カメラの初期角度
	angles_ = { 0.0f, 0.0f, 0.0f };

	// 直前のマウス座標
	prevMousePos_ = { 0, 0 };

	// 現在のマウス座標
	nowMousePos_ = { 0, 0 };
}

void Camera::Update(void)
{
}

void Camera::GameUpdate(void)
{
	// 入力制御取得
	InputManager& ins = InputManager::GetInstance();

	// クリップ距離を設定する
	SetCameraNearFar(5.0f, 30000.0f);

	// カメラの設定(位置と角度による制御)
	VECTOR UP_VECTOR = { 0.0f, 1.0f, 0.0f };
	VECTOR followPos = followPlayer_->GetPlayerPos();
	VECTOR Localpos_ = VAdd(followPos, localPosFormPlayer_);
	SetCameraPositionAndTargetAndUpVec(
		Localpos_,
		followPos,
		UP_VECTOR
	);

	// マウス感度を設定
	float mouseSensi = 0.0008f;  // マウス感度の調整値

	// 角度の制限（特にX軸の垂直方向の回転を制限)
	const float MAX_PITCH = DX_PI_F / 2.0f - 0.01f; // +90度まで
	const float MIN_PITCH = -DX_PI_F / 2.0f + 0.01f; // -90度まで

	// 画面の中央座標（マウスをここにリセットする）
	VECTOR screenCenter = VGet(Application::HALF_SCREEN_SIZE_X, Application::HALF_SCREEN_SIZE_Y, 0.0f);

	// マウス座標
	prevMousePos_ = nowMousePos_;
	nowMousePos_ = ins.GetMousePos();

	// マウスの差分を計算
	float deltaX = nowMousePos_.x - screenCenter.x;
	float deltaY = nowMousePos_.y - screenCenter.y;

	// マウスの差分を使った回転制御
	angles_.y += deltaX * mouseSensi;  // 水平回転
	angles_.x += deltaY * mouseSensi;  // 垂直回転

	// ピッチ（X軸回転）の制限を行う（垂直方向のみ制限）
	if (angles_.x > MAX_PITCH) angles_.x = MAX_PITCH;
	if (angles_.x < MIN_PITCH) angles_.x = MIN_PITCH;

	// マウスを画面中央にリセット
	SetMousePoint(screenCenter.x, screenCenter.y);

	// 単位行列を取得
	MATRIX mat = MGetIdent();

	// XYの回転行列をそれぞれ取得
	MATRIX matRotX = MGetRotX(angles_.x);
	MATRIX matRotY = MGetRotY(angles_.y);

	// 単位行列に合成していく
	mat = MMult(mat, matRotX);
	mat = MMult(mat, matRotY);

	// ローカル座標の回転
	VECTOR localPos = VTransform(localPosFormPlayer_, mat);

	// 追従対象のローカル座標を追加してワールド座標にする
	pos_ = VAdd(followPos, localPos);

	// カメラの位置と角度を設定
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,  // 垂直回転
		angles_.y,  // 水平回転
		angles_.z   // Z軸の回転（必要であれば）
	);
}

// カメラ設定（毎フレーム実行）
void Camera::SetBeforeDraw(void)
{
	// クリップ距離を設定する
	SetCameraNearFar(10.0f, 30000.0f);

	// カメラの設定（位置と角度による制御）
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);
}

void Camera::Draw(void)
{
	InputManager& ins = InputManager::GetInstance();
	Vector2 mousePos_ = ins.GetMousePos();

	DrawFormatString(20, 40, GetColor(0xff, 0xff, 0xff), "MousePosX = %d", mousePos_.x);
	DrawFormatString(20, 60, GetColor(0xff, 0xff, 0xff), "MousePosY = %d", mousePos_.y);
}

void Camera::Release(void)
{
}

void Camera::SetPlayer(Player* _player)
{
	followPlayer_ = _player;
}

VECTOR Camera::GetPos(void)
{
	return pos_;
}

bool Camera::IsMove(VECTOR _moveVec)
{
	// XYZすべての座標の移動量の絶対値をとる
	float absX = abs(_moveVec.x);
	float absY = abs(_moveVec.y);
	float absZ = abs(_moveVec.z);

	// 移動ベクトルが X・Y・Z すべて移動されていなければ
	bool isNoMoveX = absX < FLT_EPSILON;
	bool isNoMoveY = absY < FLT_EPSILON;
	bool isNoMoveZ = absZ < FLT_EPSILON;

	// 座標更新をせず抜ける
	if (isNoMoveX && isNoMoveY && isNoMoveZ)
	{
		return false;
	}
	else
	{
		return true;
	}
}

VECTOR Camera::GetCameraAngles(void)
{
	return angles_;
}

void Camera::ReSet(void)
{
	// カメラの位置
	pos_ = { 0.0f, 0.0f, 0.0f };

	// カメラの角度
	angles_ = { 0.0f, 0.0f, 0.0f };
}
