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

  private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	// ゲームシーン用

	uint32_t textureHandle_ = 0;// テクスチャハンドル
	Model* model_ = nullptr;// 3Dモデル

	ViewProjection viewProjection_;// ビュープロジェクション

	//DebugCamera* debugCamera_ = nullptr;// デバッグカメラ

	Vector3 start[102];
	Vector3 end[102];
	Vector4 color;

	float viewAngle = 0.0f;// カメラ上方向の角度

	Player* player_ = nullptr;// 自キャラ

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