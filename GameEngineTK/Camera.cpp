//
// Camera.cpp
// カメラを制御するクラス
//
#include "Camera.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

// コンストラクタ
Camera::Camera(int w, int h)
{
	// ビュー行列関係の初期設定
	m_eyePos = Vector3(0.0f, 0.0f, 5.0f);
	m_refPos = Vector3(0.0f, 0.0f, 0.0f);
	m_upVec = Vector3(0.0f, 1.0f, 0.0f);
	m_view = GetViewMatrix();

	// プロジェクション行列関係の初期設定
	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = (float)w / h;
	m_nearClip = 0.1f;
	m_farClip = 1000.0f;
}

// 仮想デストラクタ
Camera::~Camera()
{

}

/// <summary>
/// 更新処理
/// </summary>
void Camera::Update()
{
	// ビュー行列の取得
	m_view = Matrix::CreateLookAt(m_eyePos, m_refPos, m_upVec);

	// プロジェクション行列の取得
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}

/// <summary>
/// ビュー行列の取得
/// </summary>
/// <returns>行列</returns>
DirectX::SimpleMath::Matrix Camera::GetViewMatrix()
{
	return m_view;
}

/// <summary>
/// プロジェクション行列の取得
/// </summary>
/// <returns>行列</returns>
DirectX::SimpleMath::Matrix Camera::GetProjectionMatrix()
{
	return m_proj;
}

/// <summary>
/// 視点の設定
/// </summary>
/// <param name="pos">視点</param>
void Camera::SetEyePos(DirectX::SimpleMath::Vector3 pos)
{
	m_eyePos = pos;
}

/// <summary>
/// 注視点の設定
/// </summary>
/// <param name="pos">注視点</param>
void Camera::SetRefPos(DirectX::SimpleMath::Vector3 pos)
{
	m_refPos = pos;
}

/// <summary>
/// 上方向ベクトルの設定
/// </summary>
/// <param name="vec">上方向ベクトル</param>
void Camera::SetUpVec(DirectX::SimpleMath::Vector3 vec)
{
	m_upVec = vec;
}

/// <summary>
/// 垂直方向視野角の設定
/// </summary>
/// <param name="fovY">垂直方向視野角</param>
void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}

/// <summary>
/// アスペクト比の設定
/// </summary>
/// <param name="aspect">アスペクト比</param>
void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

/// <summary>
/// 手前の表示限界距離の設定
/// </summary>
/// <param name="nearClip">手前の表示限界距離</param>
void Camera::SetNearClip(float nearClip)
{
	m_nearClip = nearClip;
}

/// <summary>
/// 奥の表示限界距離の設定
/// </summary>
/// <param name="farClip">奥の表示限界距離</param>
void Camera::SetFarClip(float farClip)
{
	m_farClip = farClip;
}