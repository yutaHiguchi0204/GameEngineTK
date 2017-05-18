//
// FollowCamera.cpp
// ���@�Ǐ]�J�����𐧌䂷��N���X
//

// �w�b�_�t�@�C���̃C���N���[�h
#include "FollowCamera.h"

// ���O���
using namespace DirectX;
using namespace DirectX::SimpleMath;

// �ÓI�����o�̒�`
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

// �����o�֐��̒�`

// �R���X�g���N�^
FollowCamera::FollowCamera(int width, int height)
	: Camera(width, height)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
	m_keyboard = nullptr;

	isThirdPersonFlg = false;
}

// ���z�f�X�g���N�^
FollowCamera::~FollowCamera()
{

}

/// <summary>
/// �X�V����
/// </summary>
void FollowCamera::Update()
{
	// �L�[�{�[�h�̏�Ԃ��擾
	Keyboard::State keyboardState = m_keyboard->GetState();
	m_keyboardTracker.Update(keyboardState);

	// �b�L�[���������Ƃ�FPS/TPS��؂�ւ���
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::C))
	{
		// �t���O�̐؂�ւ�
		isThirdPersonFlg = !isThirdPersonFlg;
	}

	// ���_�A�Q�Ɠ_���W
	Vector3 eyePos, refPos;

	if (isThirdPersonFlg)
	{
		// ���@�̍��W�ɒǏ]������
		refPos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);

		// �^�[�Q�b�g���W����J�������W�ւ̍���
		Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);

		// �J�����̎��_�����̋t�����ɉ�]
		Matrix rotMat = Matrix::CreateRotationY(XMConvertToRadians(m_targetAngle));
		cameraV = Vector3::TransformNormal(cameraV, rotMat);

		// ���_���W���v�Z
		eyePos = refPos + cameraV;

		// ���_�����݈ʒu�����Ԃ���
		eyePos = m_eyePos + (eyePos - m_eyePos) * 0.05f;

		// �Q�Ɠ_�����݈ʒu�����Ԃ���
		refPos = m_refPos + (refPos - m_refPos) * 0.2f;
	}
	else
	{
		// ���@�̖ڂ̈ʒu����������_�Ƃ���
		Vector3 position;

		// ���@�̍��W�ɒǏ]������
		position = m_targetPos + Vector3(0.0f, 0.2f, 0.0f);

		// �^�[�Q�b�g���W����J�������W�ւ̍���
		Vector3 cameraV(0.0f, 0.0f, -CAMERA_DISTANCE);

		// �J�����̎��_�����̋t�����ɉ�]
		Matrix rotMat = Matrix::CreateRotationY(XMConvertToRadians(m_targetAngle));
		cameraV = Vector3::TransformNormal(cameraV, rotMat);

		eyePos = position + cameraV * 0.1f;
		refPos = eyePos + cameraV;
	}

	SetEyePos(eyePos);
	SetRefPos(refPos);

	// ���N���X�̍X�V
	Camera::Update();
}

/// <summary>
/// ���@��Ǐ]���邽�߂̍��W��ݒ�
/// </summary>
/// <param name="pos">���W</param>
void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & pos)
{
	m_targetPos = pos;
}

/// <summary>
/// ���@��Ǐ]���邽�߂̉�]�p��ݒ�
/// </summary>
/// <param name="angle">��]�p</param>
void FollowCamera::SetTargetAngle(float angle)
{
	m_targetAngle = angle;
}

/// <summary>
/// �L�[�{�[�h�ݒ�
/// </summary>
/// <param name="keyboard">�L�[�{�[�h</param>
void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}