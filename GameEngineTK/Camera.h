//
// Camera.h
// カメラを制御するクラス
//
#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

// クラスの定義
class Camera
{
protected:
	// ビュー行列関係
	DirectX::SimpleMath::Matrix m_view;						// ビュー行列
	DirectX::SimpleMath::Vector3 m_eyePos;					// 視点
	DirectX::SimpleMath::Vector3 m_refPos;					// 注視点（参照点）
	DirectX::SimpleMath::Vector3 m_upVec;					// 上方向ベクトル

	// プロジェクション行列関係
	DirectX::SimpleMath::Matrix m_proj;						// プロジェクション行列
	float m_fovY;											// 垂直方向視野角
	float m_aspect;											// アスペクト比
	float m_nearClip;										// 手前の表示限界距離
	float m_farClip;										// 奥の表示限界距離

public:
	Camera(int width, int height);
	virtual ~Camera();

	virtual void Update();											// 更新処理

	const DirectX::SimpleMath::Matrix& GetViewMatrix();				// ビュー行列の取得
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix();		// プロジェクション行列の取得

	void SetEyePos(const DirectX::SimpleMath::Vector3& pos);		// 視点の設定
	void SetRefPos(const DirectX::SimpleMath::Vector3& pos);		// 注視点の設定
	void SetUpVec(const DirectX::SimpleMath::Vector3& vec);			// 上方向ベクトルの設定

	void SetFovY(float fovY);										// 垂直方向視野角の設定
	void SetAspect(float aspect);									// アスペクト比の設定
	void SetNearClip(float nearClip);								// 手前の表示限界距離の設定
	void SetFarClip(float farClip);									// 奥の表示限界距離の設定
};