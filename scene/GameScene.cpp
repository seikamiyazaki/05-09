#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"

GameScene::GameScene() {}
GameScene::~GameScene() {
	delete model_;
	//delete debugCamera_;
	delete player_;
}

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");// ファイルを指定してテクスチャを読み込む
	model_ = Model::Create();// 3Dモデルの生成

	////re:02_03
	//std::random_device seed_gen;// 乱数シード生成器
	//std::mt19937_64 engine(seed_gen());// メルセンヌ・ツイスターの乱数エンジン
	//std::uniform_real_distribution<float> dist(-10.0f, 10.0f);// 乱数範囲の指定
	//std::uniform_real_distribution<float> rotation(-10.0f, 10.0f);

	viewProjection_.Initialize();// ビュープロジェクションの初期化

	//debugCamera_ = new DebugCamera(50, 50);// デバッグカメラの生成
	AxisIndicator::GetInstance()->SetVisible(true);// 軸方向表示を有効にする
	// 軸方向表示が参照するビュープロジェクションを指定（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// ライン描画が参照するビュープロジェクションを指定（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	player_ = new Player();// 自キャラの生成
	player_->Initilaize(model_, textureHandle_);// 自キャラの初期化
}

void GameScene::Update() {
	player_->Update();// 自キャラの更新

	//debugCamera_->Update();// デバッグカメラの更新
	viewProjection_.UpdateMatrix();// 行列の再計算

	// デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//debugText_->SetPos(50, 70);
	//debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	//debugText_->SetPos(50, 90);
	//debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	//debugText_->SetPos(50, 110);
	//debugText_->Printf("fovAngleY(Degree):%f", PI / 180 * (viewProjection_.fovAngleY));
	//debugText_->SetPos(50, 130);
	//debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
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

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	// ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	//for (int i = 0; i < 102; i++)
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(start[i], end[i], color);

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