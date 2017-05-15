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
	// ���_�A�Q�Ɠ_���W
	Vector3 eyePos, refPos;

	// ���@�̍��W�ɒǏ]������
	refPos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);

	// �^�[�Q�b�g���W����J�������W�ւ̍���
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);

	// �J�����̎��_�����̋t�����ɉ�]
	Matrix rotMat = Matrix::CreateRotationY(XMConvertToRadians(m_targetAngle));
	cameraV = Vector3::TransformNormal(cameraV, rotMat);

	// ���_���W���v�Z
	eyePos = refPos + cameraV;

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
