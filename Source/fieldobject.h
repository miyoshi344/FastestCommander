/**************************************
 * �t�B�[���h�ɕ\������u���b�N�N���X *
 **************************************/

/* ���dInclude�h�~ */
#ifndef RACE_FIELDOBJECT_H_
#define RACE_FIELDOBJECT_H_

/***********
 * Include *
 ***********/
#include "objectbase.h"

/********
 * �萔 *
 ********/

/* ���ʂ̃��b�V�� */
CONST LPCSTR kThorn       = "boxhalf_obj.x";
CONST LPSTR  kSlope       = "slope_a_obj.x";
CONST LPCSTR kWall        = "kibako.x";
CONST LPCSTR kCheck       = "check.x";

/* �X�e�[�W���Ƃ̃��b�V�� */
CONST LPCSTR kGrassGround = "kusahara.x";
CONST LPCSTR kSnowGround  = "snow.x";
CONST LPCSTR kRockyGround = "iwaba.x";
CONST LPCSTR kSandGround  = "sand.x";

/* �e�F�̃��b�V�� */
CONST LPCSTR kRedBlock    = "boxhalf_red.x";
CONST LPCSTR kGreenBlock  = "boxhalf_green.x";
CONST LPCSTR kBlueBlock   = "boxhalf_blue.x";
CONST LPCSTR kYellowBlock = "boxhalf_yellow.x";

/****************************************************
 * �t�B�[���h�ɕ`�悷��u���b�N���Ǘ�����N���X�ł� *
 ****************************************************/
class FieldObject : public ObjectBase
{
protected:
	LPCSTR mesh_name_;

	BOOL ObjectInit(VOID);
public:
	BOOL Init(VOID); // ������
	VOID UpDate(VOID);  // �X�V����
	VOID Draw(VOID);    // �`�揈��
	VOID Destroy(VOID); // �j��

	VOID SetMeshName(LPCSTR mesh_name){ mesh_name_ = mesh_name; }
};

#endif