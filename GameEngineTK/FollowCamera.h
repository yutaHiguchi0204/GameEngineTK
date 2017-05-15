//
// FollowCamera.h
// ���@�Ǐ]�J�����𐧌䂷��N���X
//
#pragma once

// �w�b�_�t�@�C���̃C���N���[�h
#include "Camera.h"

// �N���X�̒�`
class FollowCamera : public Camera
{
private:
	DirectX::SimpleMath::Vector3 m_targetPos;		// �Ǐ]�Ώۂ̍��W
	float m_targetAngle;							// �Ǐ]�Ώۂ̉�]�p

public:
	FollowCamera(int width, int height);
	~FollowCamera();

	void Update();

	void SetTargetPos(const DirectX::SimpleMath::Vector3& pos);			// �Ǐ]�Ώۂ̍��W��ݒ�
	void SetTargetAngle(float angle);									// �Ǐ]�Ώۂ̉�]�p��ݒ�

	static const float CAMERA_DISTANCE;
};