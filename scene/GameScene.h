#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <random>
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	GameScene();// コンストクラタ
	~GameScene();// デストラクタ
	void Initialize();// 初期化
	void Update();// 毎フレーム処理
	void Draw();// 描画

	// 衝突判定と応答
	void CheckAllCollision();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	// ゲームシーン用
	uint32_t textureHandle_ = 0;// テクスチャハンドル
	Model* model_ = nullptr;// 3Dモデル
	ViewProjection viewProjection_;// ビュープロジェクション

	float viewAngle = 0.0f;// カメラ上方向の角度

	//std::list<std::unique_ptr<Player>> player_;
	//std::list<std::unique_ptr<Enemy>> enemy_;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Skydome* skydome_ = nullptr;

	// 3Dモデル
	Model* modelSkydome_ = nullptr;

public:
	enum PartId {
		kRoot,
		kSpine,
		kChest,
		kHead,
		kArmL,
		kArmR,
		kHip,
		kLegL,
		kLegR,
		kNumPartId
	};
};
