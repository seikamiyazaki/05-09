#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"

#define PI 3.141592653589793

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

	//re:02_01
	color = Vector4(1, 0, 0, 1);

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

	//re:02_02
	worldTransform_.scale_ = { 5.0f,5.0f,5.0f };// Ｘ、Ｙ、Ｚ方向のスケーリングを設定
	Matrix4 matScale;// スケーリング行列を宣言
	matScale = MathUtility::Matrix4Identity();// スケーリング倍率を行列に設定する
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;

	worldTransform_.rotation_ = { PI / 4,PI / 4 ,0 };// Ｘ、Ｙ、Ｚ方向の回転角を設定

	Matrix4 matRot;// 各軸用回転行列を宣言
	Matrix4 matRotZ, matRotX, matRotY;// 各軸用回転行列を宣言

	matRot= MathUtility::Matrix4Identity();
	matRotZ = MathUtility::Matrix4Identity();
	matRotX = MathUtility::Matrix4Identity();
	matRotY = MathUtility::Matrix4Identity();

	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);// Z軸回転行列の各要素を設定する
	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);// X軸回転行列の各要素を設定する
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);

	matRotY.m[0][0] = cos(worldTransform_.rotation_.x);// Y軸回転行列の各要素を設定する
	matRotY.m[0][2] = -sin(worldTransform_.rotation_.x);
	matRotY.m[2][0] = sin(worldTransform_.rotation_.x);
	matRotY.m[2][2] = cos(worldTransform_.rotation_.x);

	matRot *= matRotZ;// 各軸の回転行列を合成
	matRot *= matRotX;
	matRot *= matRotY;

	worldTransform_.translation_ = { 10,10,10 };// Ｘ、Ｙ、Ｚ方向の平行移動を設定

	Matrix4 matTrans = MathUtility::Matrix4Identity();// 平行移動行列を宣言
	matTrans.m[3][0] = worldTransform_.translation_.x;// 移動量を行列に設定する
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();// 単位行列を代入する
	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

	worldTransform_.TransferMatrix();// 行列の転送
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

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	// ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	for (int i = 0; i < 102; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(start[i], end[i], color);
	}

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
