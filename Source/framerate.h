/***********
 * FPS�Ǘ� *
 ***********/

/* ���dInclude�h�~ */
#ifndef RACE_FRAME_RATE_H_
#define RACE_FRAME_RATE_H_

/***********
 * Include *
 ***********/
#include "common.h"

/********
 * �萔 *
 ********/
CONST INT kRate       = 16;  // 60�t���[�������p
CONST INT kNumSamples = 10;  // �t���[�����[�g�Z�o�p�T���v����

/******************************
 * �t���[�����[�g���Ǘ����܂� *
 ******************************/
class FrameRate
{
private:

	static DWORD previous_time_;             // �P�t���[���O�̎���
	static DWORD difference_;                // �O�̎��ԂƂ̌덷
	static CHAR  fps_string_[MAX_PATH];      // �t���[��������

	static LARGE_INTEGER     clock_counter_;   // �N���b�N�J�E���^�[
	static LONGLONG          previous_count_;  // �ȑO�̃N���b�N�J�E���g��
	static std::list<DOUBLE> list_;            // �T���v���i�[�p
	static BOOL              count_flag_;      // �v�����@
	static DOUBLE            freq_;            // 1�b������̃N���b�N�J�E���g��
	static DOUBLE            sum_times_;       // ���v�l

	static DOUBLE Update(CONST DOUBLE Difference); // FPS���X�V���܂�
	static DOUBLE GetFps(VOID);                    // FPS���擾���܂�

public:
	FrameRate(VOID);  // �R���X�g���N�^

	static BOOL Check(VOID); // 60�t���[���X�V���m�F���܂�
	static VOID Draw(VOID);  // FPS��\�����܂�

	static DOUBLE GetDifferenceTime(VOID);         // �����������擾���܂�
};

#endif