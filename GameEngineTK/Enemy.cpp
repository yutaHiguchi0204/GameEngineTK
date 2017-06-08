/* =====================================================================
//! @param		�uEnemy�v�N���X�̃\�[�X�t�@�C��
//! @create		����@�T��
//! @date		17/06/05
===================================================================== */

// �w�b�_�t�@�C���̃C���N���[�h
#include "Enemy.h"
#include <SimpleMath.h>

// ���O���
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

/* =====================================================================
//! ���@�e		������
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Enemy::Initialize()
{
	// �p�[�c�̃��f���ǂݍ���
	LoadPartsModel();

	// �p�[�c�̐e�q�֌W��ݒ�
	SetPartsParent();

	// �e����̍��W������ݒ�
	SetOffSet();

	// ���̕����iBODY�j�̍��W�𒲐��i������ɂ�����j
	m_parts[PARTS_BODY].SetTranslate(Vector3(0.0f, 0.6f, 0.0f));

	// �����ʒu�������_���Ō���
	m_parts[PARTS_BODY].SetTranslate(Vector3((rand() % 15 + 5) * (rand() % 2 * 2 - 1), m_parts[PARTS_BODY].GetTranslate().y, (rand() % 15 + 5) * (rand() % 2 * 2 - 1)));

	// �X�e�[�g�̏������i��������Ă��Ȃ���ԁj
	for (int i = 0; i < STATE_NUM; i++)
	{
		m_isState[i] = false;
	}
	m_isState[STATE_MOVE_FORWARD] = true;

	// �`�h�p�x�̏�����
	m_targetAngle = 0.0f;

	// �^�C�}�[�̏�����
	m_timer = 0;
}

/* =====================================================================
//! ���@�e		�p�[�c�̃��f���ǂݍ���
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Enemy::LoadPartsModel()
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
void Enemy::SetPartsParent()
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
void Enemy::SetOffSet()
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
void Enemy::SetState(ENEMY_STATE state, bool flg)
{
	m_isState[state] = flg;
}

/* =====================================================================
//! ���@�e		�Ԃ���]������
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Enemy::RotateFlower()
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
void Enemy::Move()
{
	// �O��ړ�
	if (m_isState[STATE_MOVE_FORWARD] || m_isState[STATE_MOVE_BACK])
	{
		// �y���̓���������ݒ�
		Vector3 moveV = Vector3(0.0f, 0.0f, -0.03f);

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
		//m_isState[STATE_MOVE_FORWARD] = false;
		m_isState[STATE_MOVE_BACK] = false;
	}

	// �O�D�T�b���Ƃɐ���i���񂵂ĂȂ��Ƃ��j
	if (!m_isState[STATE_MOVE_LEFT] && !m_isState[STATE_MOVE_RIGHT])
	{
		if (m_timer % 30 == 0)
		{
			int num = rand() % 2 + STATE_MOVE_LEFT;
			m_isState[num] = true;

			m_targetAngle = XMConvertToRadians(rand() % 360);
		}
	}

	// ����
	if (m_isState[STATE_MOVE_LEFT] || m_isState[STATE_MOVE_RIGHT])
	{
		// �x���̉�]������p�x��ݒ�
		float turningAngle = 0.01f;

		// �E����̏ꍇ�͉�]������p�x�i�x���j�𔽓]
		if (m_isState[STATE_MOVE_RIGHT])
		{
			turningAngle *= -1.0f;
		}

		// ���@�̉�]�p�x��ۑ�
		float angleX = m_parts[PARTS_BODY].GetRotate().x;
		float angleY = m_parts[PARTS_BODY].GetRotate().y;
		m_parts[PARTS_BODY].SetRotate(Vector3(angleX, angleY + turningAngle, 0.0f));

		if		(m_isState[STATE_MOVE_LEFT] && angleY > m_targetAngle)	m_isState[STATE_MOVE_LEFT] = false;
		else if (m_isState[STATE_MOVE_RIGHT] && angleY < m_targetAngle)	m_isState[STATE_MOVE_RIGHT] = false;
	}
}

/* =====================================================================
//! ���@�e		�O�����Ԃ�
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Enemy::ForwardSomersault()
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
void Enemy::FloatFlowers()
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
void Enemy::Splits()
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
//! ���@�e		�p�[�c���擾����
//! ���@��		�p�[�c�iPLAYER_PARTS�j
//! �߂�l		�p�[�c�I�u�W�F�N�g�iObj3d*�j
===================================================================== */
Obj3d& Enemy::GetParts(ENEMY_PARTS parts)
{
	return m_parts[parts];
}

/* =====================================================================
//! ���@�e		�X�V����
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Enemy::Update()
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
		else
		{
			m_isState[STATE_TURN] = true;
		}

		// �Ԃ𕂂�����
		FloatFlowers();

		// �Ҋ��肷��
		Splits();
	}

	// �Ԃ���]������
	RotateFlower();

	// �^�C�}�[�v��
	m_timer++;
}

/* =====================================================================
//! ���@�e		�`�揈��
//! ���@��		�Ȃ�
//! �߂�l		�Ȃ�
===================================================================== */
void Enemy::Draw()
{
	// �e�p�[�c�̍X�V
	for (vector<Obj3d>::iterator itr = m_parts.begin(); itr != m_parts.end(); itr++)
	{
		(*itr).Draw();
	}
}