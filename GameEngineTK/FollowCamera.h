//
// FollowCamera.h
// 自機追従カメラを制御するクラス
//
#pragma once

// ヘッダファイルのインクルード
#include "Camera.h"
#include <Keyboard.h>
#include "Player.h"

// クラスの定義
class FollowCamera : public Camera
{
private:
	DirectX::SimpleMath::Vector3 m_targetPos;						// 追従対象の座標
	float m_targetAngle;											// 追従対象の回転角

	DirectX::Keyboard* m_keyboard;									// キーボード
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;		// キーボードトラッカー
	bool isThirdPersonFlg;											// FPSかTPSか

	Player* m_pPlayer;												// プレイヤー
	float m_cameraY;												// 手動回転させた分の角度

public:
	FollowCamera(int width, int height);
	~FollowCamera();

	void Update();

	void SetTargetPos(const DirectX::SimpleMath::Vector3& pos);			// 追従対象の座標を設定
	void SetTargetAngle(float angle);									// 追従対象の回転角を設定

	void SetKeyboard(DirectX::Keyboard* keyboard);						// キーボード設定

	void SetPlayer(Player* player) { m_pPlayer = player; };				// プレイヤーをセット

	static const float CAMERA_DISTANCE;
};