/* =====================================================================
//! @param		�uEnemy�v�N���X�̃w�b�_�t�@�C��
//! @create		����@�T��
//! @date		17/06/01
===================================================================== */
#pragma once

// �w�b�_�t�@�C���̃C���N���[�h
#include <vector>
#include "CollisionNode.h"
#include "Obj3d.h"

// �N���X�̒�`
class Enemy
{
public:

	// �v���C���[�̃p�[�c
	enum ENEMY_PARTS
	{
		PARTS_BODY,
		PARTS_LEFTFOOT,
		PARTS_RIGHTFOOT,
		PARTS_LEFTARM,
		PARTS_RIGHTARM,
		PARTS_HEAD,
		PARTS_FLOWER,

		PARTS_NUM
	};

	// �v���C���[�X�e�[�g
	enum ENEMY_STATE
	{
		STATE_MOVE_FORWARD,
		STATE_MOVE_BACK,
		STATE_MOVE_LEFT,
		STATE_MOVE_RIGHT,
		STATE_TURN,
		STATE_FLOAT,
		STATE_SPLITS,

		STATE_NUM
	};

private:

	std::vector<Obj3d> m_parts;							// �G�̃p�[�c

	bool m_isState[STATE_NUM];							// �X�e�[�g

	float m_targetAngle;								// �`�h�ɂ��p�x

	int m_timer;										// �^�C�}�[

	SphereNode m_collisionNodeBullet;					// �e�ۗp�̓����蔻��

public:

	Enemy() {};											// �R���X�g���N�^
	~Enemy() {};										// �f�X�g���N�^

	void Initialize();									// ������
	void LoadPartsModel();								// �p�[�c�̃��f���ǂݍ���
	void SetPartsParent();								// �p�[�c�̐e�q�֌W��ݒ�
	void SetOffSet();									// �e����̍��W������ݒ�

	void SetState(ENEMY_STATE state, bool flg);			// �X�e�[�g�ݒ�

	void RotateFlower();								// �Ԃ���]������

	void Move();										// �ړ�
	void ForwardSomersault();							// �O�����Ԃ�
	void FloatFlowers();								// �Ԃ𕂂�����
	void Splits();										// �Ҋ���i�J�r�j

	Obj3d& GetParts(ENEMY_PARTS parts);					// �p�[�c���擾

	const SphereNode& GetCollisionNodeBullet() { return m_collisionNodeBullet; };

	void Update();										// �X�V����
	void Draw();										// �`�揈��
};