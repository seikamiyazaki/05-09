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
	textureHandle_ = TextureManager::Load("mario.jpg");// ファイルを指定してテクスチャを読み込む
	model_ = Model::Create();// 3Dモデルの生成

	//re:02_03
	std::random_device seed_gen;// 乱数シード生成器
	std::mt19937_64 engine(seed_gen());// メルセンヌ・ツイスターの乱数エンジン
	std::uniform_real_distribution<float> dist(-10.0f, 10.0f);// 乱数範囲の指定
	std::uniform_real_distribution<float> rotation(-10.0f, 10.0f);

	//re:02_02
	for (WorldTransform& worldTransform : worldTransforms_) {

		worldTransform.Initialize();// ワールドトランスフォームの初期化

		worldTransform.scale_ = { 1.0f,1.0f,1.0f };// Ｘ、Ｙ、Ｚ方向のスケーリングを設定
		Matrix4 matScale;// スケーリング行列を宣言
		matScale = MathUtility::Matrix4Identity();// スケーリング倍率を行列に設定する
		matScale.m[0][0] = worldTransform.scale_.x;
		matScale.m[1][1] = worldTransform.scale_.y;
		matScale.m[2][2] = worldTransform.scale_.z;

		worldTransform.rotation_ = { rotation(engine),rotation(engine) ,rotation(engine) };// Ｘ、Ｙ、Ｚ方向の回転角を設定
		Matrix4 matRot;// 各軸用回転行列を宣言
		Matrix4 matRotZ, matRotX, matRotY;// 各軸用回転行列を宣言
		matRot = MathUtility::Matrix4Identity();
		matRotZ = MathUtility::Matrix4Identity();
		matRotX = MathUtility::Matrix4Identity();
		matRotY = MathUtility::Matrix4Identity();
		matRotZ.m[0][0] = cos(worldTransform.rotation_.z);// Z軸回転行列の各要素を設定する
		matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
		matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
		matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
		matRotX.m[1][1] = cos(worldTransform.rotation_.x);// X軸回転行列の各要素を設定する
		matRotX.m[1][2] = sin(worldTransform.rotation_.x);
		matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
		matRotX.m[2][2] = cos(worldTransform.rotation_.x);
		matRotY.m[0][0] = cos(worldTransform.rotation_.x);// Y軸回転行列の各要素を設定する
		matRotY.m[0][2] = -sin(worldTransform.rotation_.x);
		matRotY.m[2][0] = sin(worldTransform.rotation_.x);
		matRotY.m[2][2] = cos(worldTransform.rotation_.x);
		matRot *= matRotZ;// 各軸の回転行列を合成
		matRot *= matRotX;
		matRot *= matRotY;

		worldTransform.translation_ = { dist(engine),dist(engine),dist(engine) };// Ｘ、Ｙ、Ｚ方向の平行移動を設定
		Matrix4 matTrans = MathUtility::Matrix4Identity();// 平行移動行列を宣言
		matTrans.m[3][0] = worldTransform.translation_.x;// 移動量を行列に設定する
		matTrans.m[3][1] = worldTransform.translation_.y;
		matTrans.m[3][2] = worldTransform.translation_.z;

		worldTransform.matWorld_ = MathUtility::Matrix4Identity();// 単位行列を代入する
		worldTransform.matWorld_ *= matScale;
		worldTransform.matWorld_ *= matRot;
		worldTransform.matWorld_ *= matTrans;

		worldTransform.TransferMatrix();// 行列の転送
	}
	//viewProjection_.eye = { 0,0,-10 };// カメラ視点座標を設定
	viewProjection_.target = { 10,0,0 };// カメラ視点座標を設定
	viewProjection_.up = { cosf(PI / 4.0f),sinf(PI / 4.0f),0.00f };// カメラ視点座標を設定
	viewProjection_.Initialize();// ビュープロジェクションの初期化

	debugCamera_ = new DebugCamera(50, 50);// デバッグカメラの生成
	AxisIndicator::GetInstance()->SetVisible(true);// 軸方向表示を有効にする
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	//re:02_01
	color = Vector4(1, 0, 0, 1);
	for (int i = 0; i < 102; i++)
		if (i < 51) {
			start[i] = Vector3((i * 2) - 50, 0, -50);
			end[i] = Vector3((i * 2) - 50, 0, 50);
		}
		else {
			start[i] = Vector3(-50, 0, (i * 2) - 152);
			end[i] = Vector3(50, 0, (i * 2) - 152);
		}
}

void GameScene::Update() {

	debugCamera_->Update();// デバッグカメラの更新

	Vector3 move = { 0,0,0 };// 視点移動ベクトル
	const float kEyeSpeed = 0.2f;// 視点の移動速さ
	if (input_->PushKey(DIK_W)) move = { 0,0,kEyeSpeed };
	else if (input_->PushKey(DIK_S)) move = { 0,0, -kEyeSpeed };

	Vector3 move1 = { 0,0,0 };// 注視点移動ベクトル
	const float kTargetSpeed = 0.2f;// 注視点の移動速さ
	if (input_->PushKey(DIK_LEFT)) move = { kTargetSpeed,0,0 };
	else if (input_->PushKey(DIK_RIGHT)) move = { -kTargetSpeed,0,0 };

	const float kUpRotSpeed = 0.05f;// 上方向の回転速さ
	if (input_->PushKey(DIK_SPACE)) {
		viewAngle += kUpRotSpeed;
		viewAngle = fmodf(viewAngle, PI * 2.0f);// 2πを超えたら元に戻す
	}

	viewProjection_.eye += move;// 視点移動
	viewProjection_.target += move1;// 注視点移動
	viewProjection_.up = { cosf(viewAngle), sinf(viewAngle), 0.0f };// 上方向ベクトルを計算
	viewProjection_.UpdateMatrix();// 行列の再計算

	// デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
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
	for (WorldTransform& worldTransform : worldTransforms_)
		model_->Draw(worldTransform, viewProjection_, textureHandle_);

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
