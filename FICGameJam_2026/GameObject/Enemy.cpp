#include "Enemy.h"
#include "Player.h"
#include "../Game.h"
#include "../EffectManager.h"
#include<Dxlib.h>

namespace
{
	//移動速度
	constexpr float kSpeed = 1.0f;

	//マウスカーソルの半径
	constexpr float kCircleRadius = 100.0f;

	//経路のウェイポイントに到達したとみなす距離
	constexpr float kWayPointThreshold = 5.0f;

	//サークルに入って死ぬまでの時間
	constexpr int kInCircleDeathTime = 120;
}

Enemy::Enemy(Vector2 pos, Vector2 vel, float dir,float width,float height):
	Character(pos,vel,dir, width, height),
	handle_(-1)
{
	//プレイヤーの初期化
	pPlayer_ = std::weak_ptr<Player>();
}

Enemy::~Enemy()
{
	DeleteGraph(handle_);
	DeleteGraph(runHandle_);
	DeleteGraph(deathHandle_);
}

void Enemy::Init()
{
	handle_ = LoadGraph(L"data/Idle.png");
	runHandle_ = LoadGraph(L"data/Run.png");
	deathHandle_ = LoadGraph(L"data/Heal.png");

	state_ = AnimState::Idle;
	ChangeState(state_);
}

void Enemy::Update()
{
	if (spawnTimer_ > 0)
	{
		spawnTimer_--;
	}

	//死亡演出中は更新を行わない
	if (isDying_)
	{
		animation_.Update();
		if (animation_.GetIsEnd())
		{
			//死亡エフェクトの生成
			EffectManager::GetInstance().Play(L"death", pos_);
			Destroy();
		}
		return;
	}

	//プレイヤーの取得
	auto player = pPlayer_.lock();

	//プレイヤーが存在しない場合は何もしない
	if (!player)return;

	//マウス座標の取得
	int mx, my;
	GetMousePoint(&mx, &my);
	Vector2 mousePos = { (float)mx, (float)my };

	//敵とマウスカーソルの距離
	Vector2 toMouse = mousePos - pos_;
	float distToMouse = sqrtf(toMouse.x * toMouse.x + toMouse.y * toMouse.y);

	//マウスカーソルの円の中に入っている場合は停止
	if (distToMouse <= kCircleRadius)
	{
		//円の中ではIdleあにめーしょんをする
		if (state_ != AnimState::Idle)
		{
			state_ = AnimState::Idle;
			ChangeState(state_);
		}

		vel_ = Vector2(0.0f, 0.0f);
		inCircleTimer_++;

		//マウスカーソルの円の中に一定時間入っていたら消滅
		if (inCircleTimer_ >= kInCircleDeathTime)
		{
			//死亡アニメーションに遷移
			isDying_ = true;
			state_ = Death;
			ChangeState(state_);
		}
		return;
	}
	else
	{
		//時間のリセット
		inCircleTimer_ = 0;
	}

	//経路がない、または経路のインデックスが範囲外の場合は何もしない
	if (path_.empty() || pathIndex_ >= path_.size())
	{
		return;
	}

	//現在向かうべきウェイポイント
	Vector2 targetPos = path_[pathIndex_];
	Vector2 toTarget = targetPos - pos_;
	float distToTarget = toTarget.Length();

	//ウェイポイントに閾値まで近づいたら次のウェイポイントへ進める
	if (distToTarget <= kWayPointThreshold)
	{
		pathIndex_++;
		if (pathIndex_ >= path_.size())
		{
			vel_ = Vector2(0.0f, 0.0f);
			return;
		}
		targetPos = path_[pathIndex_];
		toTarget = targetPos - pos_;
		distToTarget = toTarget.Length();
	}

	//ウェイポイントに向かって移動する
	if (distToTarget > 0.0f)
	{
		toTarget.Normalize();
		vel_ = toTarget * kSpeed;
	}
	else
	{
		vel_ = Vector2(0.0f, 0.0f);
	}
	pos_ += vel_;

	//移動量から判定してアニメーションを切り替え
	AnimState newState = (vel_.Length() > 0.0f) ? Run : Idle;
	if (newState != state_)
	{
		state_ = newState;
		ChangeState(state_);
	}

	//アニメーションの更新
	animation_.Update();

	//コライダー座標の更新
	collider_.SetPos(pos_);
}

void Enemy::Draw()
{
	animation_.Draw(pos_, false);

#ifdef _DEBUG
	//デバッグ用プレイヤー表示
	float halfW = collider_.GetWidth() / 2.0f;
	float halfH = collider_.GetHeight() / 2.0f;
	DrawBox(pos_.x - halfW, pos_.y - halfH, pos_.x + halfW, pos_.y + halfH, 0xffff00, false);

	//デバッグ用:経路の可視化
	for (size_t i = 0; i + 1 < path_.size(); ++i)
	{
		DrawLine(
			static_cast<int>(path_[i].x), static_cast<int>(path_[i].y),
			static_cast<int>(path_[i + 1].x), static_cast<int>(path_[i + 1].y),
			0x00ff00);
	}
#endif
}

void Enemy::ChangeState(AnimState state)
{
	switch (state)
	{
	case Idle:
		animation_.Init(handle_, 0, Vector2{ 192,192 }, 7, 10, 0.75f, true);
		break;
	case Run:
		animation_.Init(runHandle_, 0, Vector2{ 192,192 }, 3, 10, 0.75f, true);
		break;
	case Death:
		animation_.Init(deathHandle_, 0, Vector2{ 192,192 }, 10, 10, 0.75f, false);
		break;
	default:
		break;
	}
}
