/* =====================================================================
//! @param		�uPlayer�v�N���X�̃\�[�X�t�@�C��
//! @create		����@�T��
//! @date		17/06/01
===================================================================== */

// �w�b�_�t�@�C���̃C���N���[�h
#include "Player.h"
#include <SimpleMath.h>

// ���O���
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

bool Player::m_isFire;

/* =====================================================================
//! ���@�e		������
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::Initialize()
{
	// �p�[�c�̃��f���ǂݍ���
	LoadPartsModel();

	// �p�[�c�̐e�q�֌W��ݒ�
	SetPartsParent();

	// �e����̍��W������ݒ�
	SetOffSet();

	// ���̕����iBODY�j�̍��W�𒲐��i������ɂ�����j
	m_parts[PARTS_BODY].SetTranslate(Vector3(0.0f, 0.6f, 0.0f));

	// �X�e�[�g�̏������i��������Ă��Ȃ���ԁj
	for (int i = 0; i < STATE_NUM; i++)
	{
		m_isState[i] = false;
	}

	m_isFire = false;
}

/* =====================================================================
//! ���@�e		�p�[�c�̃��f���ǂݍ���
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::LoadPartsModel()
{
	// �p�[�c�����̗̈���m��
	m_parts.resize(PARTS_NUM);

	// ���f���ǂݍ���
	m_parts[PARTS_BODY		].LoadModel(L"Resources/bodyPik.cmo");
	m_parts[PARTS_LEFTFOOT	].LoadModel(L"Resources/leftFootPik.cmo");
	m_parts[PARTS_RIGHTFOOT	].LoadModel(L"Resources/rightFootPik.cmo");
	m_parts[PARTS_LEFTARM	].LoadModel(L"Resources/leftArmPik.cmo");
	m_parts[PARTS_RIGHTARM	].LoadModel(L"Resources/rightArmPik.cmo");
	m_parts[PARTS_HEAD		].LoadModel(L"Resources/pikmin.cmo");
	m_parts[PARTS_FLOWER	].LoadModel(L"Resources/flowerPik.cmo");
}

/* =====================================================================
//! ���@�e		�p�[�c�̐e�q�֌W��ݒ�
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::SetPartsParent()
{
	// �e���Z�b�g
	m_parts[PARTS_LEFTFOOT	].SetParent(&m_parts[PARTS_BODY]);
	m_parts[PARTS_RIGHTFOOT	].SetParent(&m_parts[PARTS_BODY]);
	m_parts[PARTS_LEFTARM	].SetParent(&m_parts[PARTS_BODY]);
	m_parts[PARTS_RIGHTARM	].SetParent(&m_parts[PARTS_BODY]);
	m_parts[PARTS_HEAD		].SetParent(&m_parts[PARTS_BODY]);
	m_parts[PARTS_FLOWER	].SetParent(&m_parts[PARTS_HEAD]);
}

/* =====================================================================
//! ���@�e		�e����̍��W������ݒ�
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::SetOffSet()
{
	m_parts[PARTS_LEFTFOOT	].SetTranslate(Vector3(-0.2f, 0.0f, 0.0f));
	m_parts[PARTS_RIGHTFOOT	].SetTranslate(Vector3(0.2f, 0.0f, 0.0f));
	m_parts[PARTS_LEFTARM	].SetTranslate(Vector3(-0.25f, 0.6f, 0.2f));
	m_parts[PARTS_RIGHTARM	].SetTranslate(Vector3(0.25f, 0.6f, 0.2f));
	m_parts[PARTS_HEAD		].SetTranslate(Vector3(0.0f, 0.7f, 0.0f));
	m_parts[PARTS_FLOWER	].SetTranslate(Vector3(0.0f, 1.2f, 0.1f));
}

/* =====================================================================
//! ���@�e		�X�e�[�g��ݒ�
//! ���@��		�X�e�[�g�iPLAYER_STATE�j�A�X�e�[�g��ԁibool�j
//! �߂�l		�Ȃ�
===================================================================== */
void Player::SetState(PLAYER_STATE state, bool flg)
{
	m_isState[state] = flg;
}

/* =====================================================================
//! ���@�e		�Ԃ���]������
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::RotateFlower()
{
	// ���݂̊p�x��ۑ�
	float flowerAngle = m_parts[PARTS_FLOWER].GetRotate().z;

	// ��]
	m_parts[PARTS_FLOWER].SetRotate(Vector3(0.0f, 0.0f, flowerAngle + 0.1f));
}

/* =====================================================================
//! ���@�e		�ړ�
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::Move()
{
	// �O��ړ�
	if (m_isState[STATE_MOVE_FORWARD] || m_isState[STATE_MOVE_BACK])
	{
		// �y���̓���������ݒ�
		Vector3 moveV = Vector3(0.0f, 0.0f, -0.1f);

		// ��ނ̏ꍇ�͐i�ޕ����i�y���j�𔽓]
		if (m_isState[STATE_MOVE_BACK])
		{
			moveV.z *= -1.0f;
		}

		// �ړ��ʃx�N�g�������@�̊p�x����]������
		float angle = m_parts[PARTS_BODY].GetRotate().y;
		Matrix rotMat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotMat);

		// ���s�ړ��s��̌v�Z
		Vector3 pos = m_parts[PARTS_BODY].GetTranslate();
		m_parts[PARTS_BODY].SetTranslate(pos + moveV);

		// �O��ړ��̃X�e�[�g�����Z�b�g
		m_isState[STATE_MOVE_FORWARD] = false;
		m_isState[STATE_MOVE_BACK] = false;
	}

	// ����
	if (m_isState[STATE_MOVE_LEFT] || m_isState[STATE_MOVE_RIGHT])
	{
		// �x���̉�]������p�x��ݒ�
		float turningAngle = 0.03f;

		// �E����̏ꍇ�͉�]������p�x�i�x���j�𔽓]
		if (m_isState[STATE_MOVE_RIGHT])
		{
			turningAngle *= -1.0f;
		}

		// ���@�̉�]�p�x��ۑ�
		float angleX = m_parts[PARTS_BODY].GetRotate().x;
		float angleY = m_parts[PARTS_BODY].GetRotate().y;
		m_parts[PARTS_BODY].SetRotate(Vector3(angleX, angleY + turningAngle, 0.0f));
	}
}

/* =====================================================================
//! ���@�e		�O�����Ԃ�
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::ForwardSomersault()
{
	// ���@�̉�]�p�x�ƍ��W��ۑ�
	float angleX = m_parts[PARTS_BODY].GetRotate().x;
	float angleY = m_parts[PARTS_BODY].GetRotate().y;
	Vector3 transMat = m_parts[PARTS_BODY].GetTranslate();

	// �O��
	if (angleX > -XM_2PI)
	{
		m_parts[PARTS_BODY].SetRotate(Vector3(angleX - 0.1f, angleY, 0.0f));
		m_parts[PARTS_BODY].SetTranslate(Vector3(transMat.x, transMat.y - sinf(angleX) * 0.2f, transMat.z));
	}
	else
	{
		// �p�x�������i�����j�ɖ߂�
		m_parts[PARTS_BODY].SetRotate(Vector3(0.0f, angleY, 0.0f));

		// �X�e�[�g�̑O����Ԃ��I����
		m_isState[STATE_TURN] = false;
	}
}

/* =====================================================================
//! ���@�e		�Ԃ𕂂�����
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::FloatFlowers()
{
	// ���݂̉Ԃ̈ʒu��ۑ�
	Vector3 pos = m_parts[PARTS_FLOWER].GetTranslate();

	if (m_isState[STATE_FLOAT])
	{
		// �Ԃ𕂂���
		if (pos.y < 3.0f)
		{
			m_parts[PARTS_FLOWER].SetTranslate(pos + Vector3(0.0f, 0.05f, 0.0f));
		}
	}
	else
	{
		// �Ԃ����Ƃ̈ʒu�ɖ߂�
		if (pos.y > 1.2f)
		{
			m_parts[PARTS_FLOWER].SetTranslate(pos + Vector3(0.0f, -0.05f, 0.0f));
		}
	}
}

/* =====================================================================
//! ���@�e		�Ҋ���
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::Splits()
{
	// ���݂̈ʒu�Ƒ��̊p�x��ۑ�
	Vector3 pos = m_parts[PARTS_BODY].GetTranslate();
	Vector3 angleLeft = m_parts[PARTS_LEFTFOOT].GetRotate();
	Vector3 angleRight = m_parts[PARTS_RIGHTFOOT].GetRotate();

	if (m_isState[STATE_SPLITS])
	{
		// �҂��J��
		if (pos.y > 0.15f)
		{
			m_parts[PARTS_BODY].SetTranslate(pos + Vector3(0.0f, -0.02f, 0.0f));
			m_parts[PARTS_LEFTFOOT].SetRotate(angleLeft + Vector3(0.0f, 0.0f, -0.03f));
			m_parts[PARTS_RIGHTFOOT].SetRotate(angleRight + Vector3(0.0f, 0.0f, 0.03f));
		}
	}
	else
	{
		// �҂�߂�
		if (pos.y < 0.6f)
		{
			m_parts[PARTS_BODY].SetTranslate(pos + Vector3(0.0f, 0.02f, 0.0f));
			m_parts[PARTS_LEFTFOOT].SetRotate(angleLeft + Vector3(0.0f, 0.0f, 0.03f));
			m_parts[PARTS_RIGHTFOOT].SetRotate(angleRight + Vector3(0.0f, 0.0f, -0.03f));
		}
	}
}

/* =====================================================================
//! ���@�e		�p�[�c�𔭎˂�����
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::FireBurret()
{
	m_isFire = false;

	// ���˂���p�[�c�̃��[���h�s����擾
	Matrix worldM = m_parts[PARTS_FLOWER].GetWorldMatrix();

	// ���[���h�s�񂩂�e�v�f�𒊏o����
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;

	worldM.Decompose(scale, rotate, translate);

	// ���˃p�[�c��e���番�������ēƗ�
	m_parts[PARTS_FLOWER].SetParent(nullptr);
	m_parts[PARTS_FLOWER].SetScale(scale);
	m_parts[PARTS_FLOWER].SetRotateQ(rotate);
	m_parts[PARTS_FLOWER].SetTranslate(translate);

	// �e�ۂ̑��x��ݒ�
	m_bulletVel = Vector3(0.0f, 0.0f, -0.5f);
	m_bulletVel = Vector3::Transform(m_bulletVel, rotate);

	m_isFire = true;
}

/* =====================================================================
//! ���@�e		���˂����p�[�c�����Z�b�g����
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::ResetBurret()
{
	if (!m_isFire) return;

	// �e�q�֌W��߂�
	m_parts[PARTS_FLOWER].SetParent(&m_parts[PARTS_HEAD]);

	// ���ꂼ��̗v�f��ݒ�
	m_parts[PARTS_FLOWER].SetScale(Vector3(1.0f, 1.0f, 1.0f));
	m_parts[PARTS_FLOWER].SetRotate(Vector3(0.0f, 0.0f, 0.0f));
	m_parts[PARTS_FLOWER].SetTranslate(Vector3(0.0f, 1.2f, 0.1f));
}

/* =====================================================================
//! ���@�e		�p�[�c���擾����
//! ���@��		�p�[�c�iPLAYER_PARTS�j
//! �߂�l		�p�[�c�I�u�W�F�N�g�iObj3d*�j
===================================================================== */
Obj3d& Player::GetParts(PLAYER_PARTS parts)
{
	return m_parts[parts];
}

/* =====================================================================
//! ���@�e		�X�V����
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::Update()
{
	// �e�p�[�c�̍X�V
	for (vector<Obj3d>::iterator itr = m_parts.begin(); itr != m_parts.end(); itr++)
	{
		(*itr).Update();
	}

	// �X�e�[�g�ɂ���ď������s��
	{
		// �ړ�
		Move();

		// �O�����Ԃ�
		if (m_isState[STATE_TURN])
		{
			ForwardSomersault();
		}

		// �Ԃ𕂂�����
		FloatFlowers();

		// �Ҋ��肷��
		Splits();
	}

	// ���˂��ꂽ�p�[�c�̏���
	if (m_isFire)
	{
		Vector3 pos = m_parts[PARTS_FLOWER].GetTranslate();
		m_parts[PARTS_FLOWER].SetTranslate(pos + m_bulletVel);
	}

	// �Ԃ���]������
	RotateFlower();
}

/* =====================================================================
//! ���@�e		�`�揈��
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Player::Draw()
{
	// �e�p�[�c�̍X�V
	for (vector<Obj3d>::iterator itr = m_parts.begin(); itr != m_parts.end(); itr++)
	{
		(*itr).Draw();
	}
}