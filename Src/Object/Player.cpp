#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Player.h"

Player::Player(void)
{
	list.modelid_ = -1;

	VECTOR pos_ = { 0.0f,0.0f,0.0f };
	VECTOR rot_ = { 0.0f,0.0f,0.0f };
	VECTOR scale_ = { 0.0f,0.0f,0.0f };
	float moveSpeed_ = 0.0f;
	int moveDir_ = 0;
	int moveKind_ = 0;

	VECTOR moveVec_ = { 0.0f,0.0f,0.0f };
	VECTOR oldMoveVec_ = { 0.0f,0.0f,0.0f };
	VECTOR moveVecRad_ = { 0.0f,0.0f,0.0f };

	VECTOR localRot_ = { 0.0f,0.0f,0.0f };

	int animindex_ = 0;
	int  animAttachNo_ = 0;
	float animTotalTime_ = 0.0f;
	float currentAnimTime_ = 0.0f;
	bool animlockflg_ = false;

	bool isdead_ = false;
	bool crouchflg_ = false;
	bool crouchattachflg_ = false;
	bool weaponflg_ = false;
}

Player::~Player(void)
{
}

void Player::SystemInit(void)
{
}

void Player::GameInit(void)
{
	// モデルの読み込み
	list.modelid_ = MV1LoadModel("Data/Model/PlayerModel.mv1");

	// プレイヤーの初期位置設定
	list.pos_ = { 0.0f, 0.0f, 0.0f };
	MV1SetPosition(list.modelid_, list.pos_);

	// プレイヤーの角度設定
	list.rot_ = { 0.0f, 0.0f, 0.0f };
	MV1SetRotationXYZ(list.modelid_, list.rot_);

	// 移動ベクトルが作成する角度
	list.moveVecRad_ = { 0.0f, 0.0f, 0.0f };

	// 直前の移動ベクトル
	list.oldMoveVec_ = { 0.0f, 0.0f, 0.0f };

	// プレイヤーの角度
	list.localRot_ = INIT_MODEL_ROT_OFFSET;

	// プレイヤーの移動速度
	list.moveSpeed_ = MOVE_SPEED_WALK;

	// プレイヤーの移動状態
	list.moveKind_ = 0;

	//死んでいるか
	list.isdead_ = false;

	//しゃがんでいるか
	list.crouchflg_ = false;

	//しゃがみ攻撃しているか
	list.crouchattackflg_ = false;		

	//武器出しているか
	list.weaponflg_ = false;

	//攻撃しているか
	list.attackflg_ = false;

	//攻撃段階
	list.comboflg_ = false;
	list.combostep_ = 0;

	//ブレンド管理
	list.blendAnimNo_ = -1;
	list.blendCurrentTime_ = 0.0f;
	list.blendMaxTime_ = 0.0f;
	list.blendflg_ = false;

	//アニメーション関連
	list.animindex_ = 1;
	list.animAttachNo_ = MV1AttachAnim(list.modelid_, list.animindex_);
	list.animTotalTime_ = MV1GetAttachAnimTotalTime(list.modelid_, list.animAttachNo_);
	list.currentAnimTime_ = 0.0f;
	list.animlockflg_ = false;
	MV1SetAttachAnimTime(list.modelid_, list.animAttachNo_, list.currentAnimTime_);

	list.currentratio_ = 0.0f;
	list.remainingtime_ = 0.0f;
}

void Player::Update(void)
{
	// アニメーション処理
 	PlayAnimation();
	//DebugAnimation();
	//移動処理
	UpdateMove();
	SetRotation();
}

void Player::UpdateMove(void)
{
	// 入力制御取得
	InputManager& ins = InputManager::GetInstance();

	//死亡時にも移動させない
	if (list.isdead_)
	{
		list.moveSpeed_ = MOVE_SPEED_STOP;
		ChangeAnimation(Death);
		return;
	}

	if (ins.IsTrgMouseRight())
	{
		list.weaponflg_ = !list.weaponflg_;
		if (list.weaponflg_)
		{
			ChangeAnimation(WeaponOut, true);					//武器取り出し
		}
		else {
			ChangeAnimation(Sheach, true);						//武器しまう
		}
		return;
	}

	//武器の状態で移動アニメーション切り替え
	if (!list.weaponflg_)
	{
		PlayerMove(NoWeaponIdle, NoWeaponWalk, NoWeaponRun);	//武器持っていないときの動き
	}
	else
	{
		PlayerMove(WeaponIdle, WeaponWalk, WeaponRun);			//武器持っているときの動き
	}
}

void Player::Draw(void)
{
	// モデルの描画
	MV1DrawModel(list.modelid_);

	// モデルの移動方向
	if (IsMove(list.moveVec_))
	{
		// 移動ベクトルを正規化
		VECTOR debugMoveVec = VNorm(list.moveVec_);

		// 線の長さを設定
		constexpr float DEBUG_MOVE_LINE_LENGTH = 100.0f;
		debugMoveVec = VScale(debugMoveVec, DEBUG_MOVE_LINE_LENGTH);

		// 線の終端座標を設定
		VECTOR debugMoveVecLineEndPos = VAdd(list.pos_, debugMoveVec);

		// 移動方向に線を描画
		DrawLine3D(list.pos_, debugMoveVecLineEndPos, 0xffffff);
	}

	// プレイヤー座標表示
	DrawFormatString(20, 120, GetColor(0xff, 0xff, 0xff), "プレイヤーの座標 : (X, Y, Z) = (% 1.2lf, % 1.2lf, % 1.2lf)", list.pos_.x, list.pos_.y, list.pos_.z);
}

void Player::Release(void)
{
	MV1DeleteModel(list.modelid_);
}

bool Player::IsMove(VECTOR _moveVec)
{
	// XYZすべての座標の移動量の絶対値をとる
	float absX = abs(_moveVec.x);
	float absY = abs(_moveVec.y);
	float absZ = abs(_moveVec.z);

	// 移動ベクトルが X・Y・Z すべて移動されてなければ
	bool isNoMoveX = absX < FLT_EPSILON;
	bool isNoMoveY = absY < FLT_EPSILON;
	bool isNoMoveZ = absZ < FLT_EPSILON;

	// 座標更新せずに抜ける
	if (isNoMoveX && isNoMoveY && isNoMoveZ)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Player::SetRotation(void)
{
	//------------------------------------
	// 回転行列を使用した角度設定
	//------------------------------------
	
	// 単位行列を設定する
	MATRIX mat = MGetIdent();

	// モデル自体のY軸回転行列を作成する
	MATRIX mGetRotY = MGetRotY(list.rot_.y);

	// モデルの補正用Y軸回転行列を作成する
	MATRIX mGetLocalRotY = MGetRotY(list.localRot_.y);

	// 行列を合成
	mat = MMult(mat, mGetRotY);
	mat = MMult(mat, mGetLocalRotY);

	// 行列を使用してモデルの角度を設定
	MV1SetRotationMatrix(list.modelid_, mat);
}

void Player::PlayerMove(int idle, int walk, int run)
{
	InputManager& ins = InputManager::GetInstance();

	if (list.weaponflg_)
	{
		if (AttackUpdate())
		{
			return;
		}
		if (CrouchUpdate())
		{
			return;
		}
	}

	// WASDでプレイヤー移動
	list.moveVec_ = { 0.0f, 0.0f, 0.0f };

	// 左・右・手前・奥のベクトルを作成する
	VECTOR RIGHT_MOVE_VEC = { 1.0f,  0.0f,  0.0f };
	VECTOR LEFT_MOVE_VEC = { -1.0f,  0.0f,  0.0f };
	VECTOR FRONT_MOVE_VEC = { 0.0f,  0.0f,  1.0f };
	VECTOR BACK_MOVE_VEC = { 0.0f,  0.0f, -1.0f };

	// 入力方向に移動ベクトルを追加する
	if (ins.IsNew(KEY_INPUT_W)) { list.moveVec_ = VAdd(list.moveVec_, FRONT_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_A)) { list.moveVec_ = VAdd(list.moveVec_, LEFT_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_S)) { list.moveVec_ = VAdd(list.moveVec_, BACK_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_D)) { list.moveVec_ = VAdd(list.moveVec_, RIGHT_MOVE_VEC); }

	// ベクトルの移動が行われていたら座標更新
	if (IsMove(list.moveVec_))
	{
		// 移動状態の設定
		list.moveKind_ = 1;

		// カメラ角度分設定する
		VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetCameraAngles();
		MATRIX cameraMatY = MGetRotY(cameraAngles.y);
		list.moveVec_ = VTransform(list.moveVec_, cameraMatY);

		// スタミナが切れているかどうか
		bool spFlg_ = false;

		if (spFlg_)
		{
			// スタミナない状態
			list.moveSpeed_ = MOVE_SPEED_STOP;
		}
		else
		{
			if (ins.IsNew(KEY_INPUT_LSHIFT))
			{
				// ダッシュ状態
				list.moveSpeed_ = MOVE_SPEED_RUN;
				ChangeAnimation(run);
			}
			else
			{
				// 歩き状態
				list.moveSpeed_ = MOVE_SPEED_WALK;
				ChangeAnimation(walk);
			}
		}

		// 座標更新
		list.moveVec_ = VNorm(list.moveVec_);
		list.moveVec_ = VScale(list.moveVec_, list.moveSpeed_);
		list.pos_ = VAdd(list.pos_, list.moveVec_);
		list.oldMoveVec_ = list.moveVec_;

		// 方向を角度に変換する( XZ平面 Y軸)
		list.moveVecRad_.y = atan2f(list.moveVec_.x, list.moveVec_.z);

		// シンプルに計算角度を設定してみる
		list.rot_.y = list.moveVecRad_.y;

		// 座標設定
		MV1SetPosition(list.modelid_, list.pos_);
	}
	else
	{
		// 移動状態の設定
		list.moveKind_ = 0;

		//アニメーションロックされていないとき待機
		if (!list.animlockflg_)
		{
			ChangeAnimation(idle);
		}
	}
}

bool Player::CrouchUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();

	//防御(しゃがみ)時には移動させない
	if (ins.IsNew(KEY_INPUT_LCONTROL))
	{
		if (!list.crouchflg_)
		{
			//しゃがみ状態に入るとき
			list.crouchflg_ = true;
			list.crouchattackflg_ = false;
			list.moveSpeed_ = MOVE_SPEED_STOP;		//動きを止める
			ChangeAnimation(Crouch, true);			//しゃがみ再生
			return true;
		}

		if (!list.animlockflg_ && !list.crouchattackflg_)
		{
			ChangeAnimation(Crouch_Idle);			//しゃがみ待機
		}

		//しゃがんでいるときに左クリック押したら
		if (!list.crouchattackflg_ && ins.IsTrgMouseLeft())
		{
			ChangeAnimation(Crouch_Attack);			//しゃがみ攻撃
			list.crouchattackflg_ = true;
		}
		return true;	//しゃがみ中
	}
	else if (list.crouchflg_)
	{
		//左コントロールを離したときにしゃがみ解除
		ChangeAnimation(Crouch_Cancel, true);		//しゃがみ解除
		list.crouchflg_ = false;
		list.crouchattackflg_ = false;
	}
	return false;		//しゃがみ中ではない
}

//攻撃更新
bool Player::AttackUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (!list.attackflg_ && ins.IsTrgMouseLeft())
	{
		list.attackflg_ = true;
		list.combostep_ = 1;
		list.comboflg_ = false;
		ChangeAnimation(Attack_1, true);
		return true;
	}

	AttackCombo(1, Attack_3, 2, 0.5f, 5.0f);
	AttackCombo(2, Attack_4, 3, 0.5f, 15.0f);
	AttackCombo(3, Attack_2, 4, 0.5f, 15.0f);

	//アニメーションが終わったら攻撃解除
	if (!list.animlockflg_ && list.attackflg_)
	{
		list.attackflg_ = false;
		list.combostep_ = 0;
		list.comboflg_ = false;
	}

	return list.attackflg_;		//攻撃中か
}

//攻撃コンボ(今のコンボ、次のアニメーション、次の段階、入力受付時間(後ろからの割合)、アニメーションスキップ時間(後ろから))
void Player::AttackCombo(int nowcombo, int nextanimidx, int nextstep, float reception, float remainingtime)
{
	InputManager& ins = InputManager::GetInstance();

	//攻撃１中にアニメーションが半分以上進んでいたら攻撃２
	if (list.attackflg_ && list.combostep_ == nowcombo)
	{
		//現在のアニメーション時間(割合)
		list.currentratio_ = list.currentAnimTime_ / list.animTotalTime_;

		//アニメーションが半分以上＋左クリック押したとき
		if (list.currentratio_ >= reception && ins.IsTrgMouseLeft())
		{
			list.comboflg_ = true;
		}

		//アニメーションの残り時間
		list.remainingtime_ = list.animTotalTime_ - list.currentAnimTime_;
		if (list.remainingtime_ <= remainingtime && list.comboflg_)
		{
			ChangeAnimation(nextanimidx, true);
			list.combostep_ = nextstep;
			list.comboflg_ = false;
		}
	}
}



void Player::PlayAnimation(void)
{
	DebugAnimation();			//数字キーでアニメーション切り替え

	switch (list.animindex_)
	{
		//ループさせる
	case 0:			//武器なし待機
	case 1:			//武器なし歩く
	case 2:			//武器なし走る
	case 5:			//武器あり待機
	case 6:			//武器あり歩く
	case 7:			//武器あり走る
	case 8:			//武器あり横歩き
	case 12:		//しゃがみ待機	
		list.currentAnimTime_ += ANIM_SPEED;
		if (list.currentAnimTime_ > list.animTotalTime_)
		{
			list.currentAnimTime_ = 0.0f;
		}
		list.animlockflg_ = false;
		break;
		//ループさせない
	case 3:			//武器取り出し
	case 4:			//武器戻す
		list.currentAnimTime_ += ANIM_SPEED;
		if (list.currentAnimTime_ >= list.animTotalTime_)
		{
			list.currentAnimTime_ = list.animTotalTime_;
			list.animlockflg_ = false;
		}
		break;
	case 9:			//ジャンプ
	case 13:		//防御したとき
	case 15:		//攻撃１
	case 16:		//攻撃２
	case 17:		//攻撃３
	case 18:		//攻撃４
		list.currentAnimTime_ += ATTACK_ANIM_SPEED;
		if (list.currentAnimTime_ >= list.animTotalTime_)
		{
			list.currentAnimTime_ = list.animTotalTime_;
			list.animlockflg_ = false;
		}
		break;
	case 19:		//死亡
		list.currentAnimTime_ += DEAD_ANIM_SPEED;
		if (list.currentAnimTime_ >= list.animTotalTime_)
		{
			list.currentAnimTime_ = list.animTotalTime_;
			list.animlockflg_ = false;
		}
		break;
	case 10:		//しゃがみ(防御)
		list.currentAnimTime_ += CROUCH_ANIM_SPEED;
		if (list.currentAnimTime_ >= list.animTotalTime_)
		{
			list.currentAnimTime_ = list.animTotalTime_;
			list.animlockflg_ = false;

			ChangeAnimation(12);		//しゃがみ待機
		}
		break;
	case 11:		//しゃがみ解除
		list.currentAnimTime_ += CROUCH_ANIM_SPEED;
		if (list.currentAnimTime_ >= list.animTotalTime_)
		{
			list.currentAnimTime_ = list.animTotalTime_;
			list.animlockflg_ = false;
		}
		break;
	case 14:		//しゃがみ攻撃
		list.currentAnimTime_ += CROUCH_ATTACK_SPEED;
		if (list.currentAnimTime_ >= list.animTotalTime_)
		{
			list.currentAnimTime_ = list.animTotalTime_;
			list.animlockflg_ = false;
			list.crouchattackflg_ = false;	//しゃがみ攻撃終了

			ChangeAnimation(Crouch_Idle);			//しゃがみ待機
		}
		break;
	default:
		break;
	}
	MV1SetAttachAnimTime(list.modelid_, list.animAttachNo_, list.currentAnimTime_);
	MV1SetPosition(list.modelid_, list.pos_);

	//攻撃用アニメーションブレンド
	if (list.blendflg_)
	{
		list.blendCurrentTime_ += ANIM_SPEED;
		float rate = list.blendCurrentTime_ / list.blendMaxTime_;

		if (rate > 1.0f)rate = 1.0f;

		//古いアニメーションの影響を減らす
		MV1SetAttachAnimBlendRate(list.modelid_, list.blendAnimNo_, 1.0f - rate);
		//新しいアニメーションの影響を増やす
		MV1SetAttachAnimBlendRate(list.modelid_, list.animAttachNo_, rate);
		if (rate >= 1.0f)
		{
			MV1SetAttachAnimBlendRate(list.modelid_, list.animAttachNo_, 1.0f);
			MV1SetAttachAnimBlendRate(list.modelid_, list.blendAnimNo_, 0.0f);
			MV1DetachAnim(list.modelid_, list.blendAnimNo_);		//古いアニメーション削除
			list.blendflg_ = false;
		}
	}
}

//矢印キーでアニメーション切り替え
void Player::DebugAnimation(void)
{
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_0)) {
		ChangeAnimation(19);
	}
	if (ins.IsTrgDown(KEY_INPUT_1)) {
		ChangeAnimation(1);
	}
	if (ins.IsTrgDown(KEY_INPUT_2)) {
		ChangeAnimation(2);
	}
	if (ins.IsTrgDown(KEY_INPUT_3)) {
		ChangeAnimation(3);
	}
	if (ins.IsTrgDown(KEY_INPUT_4)) {
		ChangeAnimation(4);
	}
}

//アニメーション切り替え
void Player::ChangeAnimation(int idx, bool lock)
{
	// すでにそのアニメーションなら処理不要
	if (list.animindex_ == idx)
		return;

	float blendTime = 0.2f;

	// 古いアニメを保存
	list.oldAttackNo_ = list.animAttachNo_;

	// 新しいアニメを設定
	list.animindex_ = idx;
	list.animAttachNo_ = MV1AttachAnim(list.modelid_, idx);
	list.animTotalTime_ = MV1GetAttachAnimTotalTime(list.modelid_, list.animAttachNo_);
	list.currentAnimTime_ = 0.0f;
	MV1SetAttachAnimTime(list.modelid_, list.animAttachNo_, list.currentAnimTime_);

	// Blendの初期化（全アニメ対象）
	MV1SetAttachAnimBlendRate(list.modelid_, list.oldAttackNo_, 1.0f);
	MV1SetAttachAnimBlendRate(list.modelid_, list.animAttachNo_, 0.0f);

	list.blendAnimNo_ = list.oldAttackNo_;
	list.blendCurrentTime_ = 0.0f;
	list.blendMaxTime_ = blendTime;
	list.blendflg_ = true;

	list.animlockflg_ = lock;
}

