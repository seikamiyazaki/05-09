#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ファイルを指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();// 3Dモデルの生成

	worldTransform_.Initialize();// ワールドトランスフォームの初期化
	viewProjection_.Initialize();// ビュープロジェクションの初期化

	debugCamera_ = new DebugCamera(50, 50);// デバッグカメラの生成

	AxisIndicator::GetInstance()->SetVisible(true);// 軸方向表示を有効にする
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	// ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	color = Vector4(200, 200, 200, 1);

	for (int i = 0; i < 102; i++) {
		if (i < 51) {
			start[i] = Vector3((i * 2) - 50, 0, -50);
			end[i] = Vector3((i * 2) - 50, 0, 50);
		}
		else {
			start[i] = Vector3(-50, 0, (i * 2) - 152);
			end[i] = Vector3(50, 0, (i * 2) - 152);
		}
	}
}

void GameScene::Update() {
	debugCamera_->Update();// デバッグカメラの更新
}

void GameScene::Draw() {

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
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	for (int i = 0; i < 102; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(start[i], end[i], color);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
