/****************************
 * �v���C���[�Z���N�g�N���X *
 ****************************/

/* ���dInclude�h�~ */
#ifndef RACE_PLAYER_SELECT_H_
#define RACE_PLAYER_SELECT_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "sprite.h"
#include "xinput.h"
#include "flow_base.h"

/********
 * �萔 *
 ********/

/****************************************
 * �v���C���[�̐l�����Ǘ�����N���X�ł� *
 ****************************************/
class PlayerSelect : public FlowBase
{
private:

	VOID SpriteDraw(VOID);
public:
	PlayerSelect(VOID);
	~PlayerSelect(VOID);

	BOOL Init(VOID);
	VOID ReSet(VOID);
	INT UpDate(VOID);
	VOID Draw(VOID);
	VOID Destroy(VOID);
};

#endif