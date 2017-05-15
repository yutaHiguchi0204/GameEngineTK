//
// FollowCamera.cpp
// 自機追従カメラを制御するクラス
//

// ヘッダファイルのインクルード
#include "FollowCamera.h"

// 名前空間
using namespace DirectX;
using namespace DirectX::SimpleMath;

// 静的メンバの定義
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

// メンバ関数の定義

// コンストラクタ
FollowCamera::FollowCamera(int width, int height)
	: Camera(width, height)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
}

// 仮想デストラクタ
FollowCamera::~FollowCamera()
{

}

/// <summary>
/// 更新処理
/// </summary>
void FollowCamera::Update()
{
	// 視点、参照点座標
	Vector3 eyePos, refPos;

	// 自機の座標に追従させる
	refPos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);

	// ターゲット座標からカメラ座標への差分
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);

	// カメラの視点方向の逆方向に回転
	Matrix rotMat = Matrix::CreateRotationY(XMConvertToRadians(m_targetAngle));
	cameraV = Vector3::TransformNormal(cameraV, rotMat);

	// 視点座標を計算
	eyePos = refPos + cameraV;

	SetEyePos(eyePos);
	SetRefPos(refPos);

	// 基底クラスの更新
	Camera::Update();
}

/// <summary>
/// 自機を追従するための座標を設定
/// </summary>
/// <param name="pos">座標</param>
void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & pos)
{
	m_targetPos = pos;
}

/// <summary>
/// 自機を追従するための回転角を設定
/// </summary>
/// <param name="angle">回転角</param>
void FollowCamera::SetTargetAngle(float angle)
{
	m_targetAngle = angle;
}
