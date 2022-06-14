#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"

GameScene::GameScene() {}
GameScene::~GameScene()
{
	delete model_;
	delete player_;
	delete enemy_;
}

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");// ファイルを指定してテクスチャを読み込む
	model_ = Model::Create();// 3Dモデルの生成

	viewProjection_.Initialize();// ビュープロジェクションの初期化

	//debugCamera_ = new DebugCamera(50, 50);// デバッグカメラの生成
	AxisIndicator::GetInstance()->SetVisible(true);// 軸方向表示を有効にする
	// 軸方向表示が参照するビュープロジェクションを指定（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// ライン描画が参照するビュープロジェクションを指定（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	Vector3 velocity(0, 0, 0.5f);
	enemy_ = new Enemy();
	enemy_->Initialize(model_, velocity);

	//std::unique_ptr<Player> newPlayer = std::make_unique<Player>();
	//newPlayer->Initialize(model_, textureHandle_);
	//std::unique_ptr<Enemy> newEney = std::make_unique<Enemy>();
	//newEney->Initialize(model_, position, velocity);
}

void GameScene::Update()
{
	player_->Update();// 自キャラの更新
	enemy_->Update();

	viewProjection_.UpdateMatrix();// 行列の再計算

	// デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに背景スプライトの描画処理を追加できる

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// ここに3Dオブジェクトの描画処理を追加できる
	// 3Dモデル描画
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	// ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに前景スプライトの描画処理を追加できる

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
