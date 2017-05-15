//
// Camera.h
// �J�����𐧌䂷��N���X
//
#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

// �N���X�̒�`
class Camera
{
protected:
	// �r���[�s��֌W
	DirectX::SimpleMath::Matrix m_view;						// �r���[�s��
	DirectX::SimpleMath::Vector3 m_eyePos;					// ���_
	DirectX::SimpleMath::Vector3 m_refPos;					// �����_�i�Q�Ɠ_�j
	DirectX::SimpleMath::Vector3 m_upVec;					// ������x�N�g��

	// �v���W�F�N�V�����s��֌W
	DirectX::SimpleMath::Matrix m_proj;						// �v���W�F�N�V�����s��
	float m_fovY;											// ������������p
	float m_aspect;											// �A�X�y�N�g��
	float m_nearClip;										// ��O�̕\�����E����
	float m_farClip;										// ���̕\�����E����

public:
	Camera(int width, int height);
	virtual ~Camera();

	virtual void Update();											// �X�V����

	const DirectX::SimpleMath::Matrix& GetViewMatrix();				// �r���[�s��̎擾
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix();		// �v���W�F�N�V�����s��̎擾

	void SetEyePos(const DirectX::SimpleMath::Vector3& pos);		// ���_�̐ݒ�
	void SetRefPos(const DirectX::SimpleMath::Vector3& pos);		// �����_�̐ݒ�
	void SetUpVec(const DirectX::SimpleMath::Vector3& vec);			// ������x�N�g���̐ݒ�

	void SetFovY(float fovY);										// ������������p�̐ݒ�
	void SetAspect(float aspect);									// �A�X�y�N�g��̐ݒ�
	void SetNearClip(float nearClip);								// ��O�̕\�����E�����̐ݒ�
	void SetFarClip(float farClip);									// ���̕\�����E�����̐ݒ�
};