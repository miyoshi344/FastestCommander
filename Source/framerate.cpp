/***********
 * Include *
 ***********/
#include "framerate.h"

DWORD             FrameRate::previous_time_;
DWORD             FrameRate::difference_;
CHAR              FrameRate::fps_string_[MAX_PATH];
LARGE_INTEGER     FrameRate::clock_counter_;
LONGLONG          FrameRate::previous_count_;
std::list<DOUBLE> FrameRate::list_;
BOOL              FrameRate::count_flag_;
DOUBLE            FrameRate::freq_;
DOUBLE            FrameRate::sum_times_;

/******************
 * �R���X�g���N�^ *
 ******************/
FrameRate::FrameRate(VOID)
{
	difference_ = 0L;
	previous_time_ = timeGetTime();
	sum_times_ = 0.0;
	strcpy_s(fps_string_, kWindowName);

	/* �T���v���� */
	list_.resize(kNumSamples, 0.0);

	/* QueryPerformanceCounter���g�p�\�� */
	if (QueryPerformanceCounter(&clock_counter_))
	{
		count_flag_ = TRUE; // QueryPerformanceCounter���g�p�\

		/* ���݂̃N���b�N���ƂP�b������̃N���b�N�����擾 */
		previous_count_ = clock_counter_.QuadPart;
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		freq_ = static_cast<DOUBLE>(freq.QuadPart);
	}
	else
	{
		count_flag_ = FALSE;  // QueryPerformanceCounter�͎g�p�ł��Ȃ�
		previous_count_ = timeGetTime();
	}
}

/*******************************************
 * �֐��� : GetDifferenceTime�֐�          *
 * ��  �e : �����������擾���܂�           *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �����������~���b�ɂ��ĕԂ��܂� *
 *******************************************/
DOUBLE FrameRate::GetDifferenceTime(VOID)
{
	DOUBLE current_difference = 0.0;

	/* QueryPerformanceCounter���g�p�ł��邩 */
	if (count_flag_)
	{
		/* ���݂̎������擾���č������v�Z */
		QueryPerformanceCounter(&clock_counter_);
		LONGLONG difference = clock_counter_.QuadPart - previous_count_;
		DOUBLE def = static_cast<DOUBLE>(difference);
		previous_count_ = clock_counter_.QuadPart;  // ���̎��Ԃ�O��̎��Ԃɂ���
		current_difference = def * 1000.0 / freq_;  // �������Z�o
	}
	else
	{
		/* QueryPerformanceCounter�͎g���Ȃ��̂�timeGetTime */
		DWORD current = timeGetTime();
		current_difference = static_cast<DOUBLE>(current - previous_count_);
		previous_count_ = current;
	}

	return current_difference;
}

/*************************************
 * �֐��� : Update�֐�               *
 * ��  �e : FPS���X�V���܂�          *
 * ��  �� :                          *
 * �߂�l : �t���[�����[�g��Ԃ��܂� *
 *************************************/
DOUBLE FrameRate::Update(CONST DOUBLE Difference)
{
	/* �P�Â��f�[�^���폜���Č��݂̒l��ǉ� */
	list_.pop_front();
	list_.push_back(Difference);

	/* FPS���Z�o */
	DOUBLE fps = 0.0;
	DOUBLE average = (sum_times_ + Difference) / kNumSamples;
	if (average) fps = 1000.0 / average;

	/* ���ϒl�X�V */
	sum_times_ += Difference - *list_.begin();

	return fps;
}

/*************************************
 * �֐��� : GetFps�֐�               *
 * ��  �e : FPS���擾���܂�          *
 * ��  �� : �Ȃ�                     *
 * �߂�l : �t���[�����[�g��Ԃ��܂� *
 *************************************/
DOUBLE FrameRate::GetFps(VOID)
{
	DOUBLE difference = GetDifferenceTime();

	/* 0�̏ꍇ�͌v�Z�s�� */
	if (!difference) return 0;

	return Update(difference);
}

/*******************************************************
 * �֐��� : Check�֐�                                  *
 * ��  �e : �t���[�����[�g�̒������s���܂�             *
 * ��  �� : �Ȃ�                                       *
 * �߂�l : ���C�����[�v�̏������s���ꍇTRUE��Ԃ��܂� *
 *******************************************************/
BOOL FrameRate::Check(VOID)
{
	BOOL ret = FALSE;


	/* 16�~���b�o�߂��Ă��邩�`�F�b�N */
	DWORD current_time = timeGetTime();
	DWORD judge = (current_time - previous_time_) + difference_;

	if (judge > kRate)
	{
		difference_ = judge % kRate;   // ���������Ԃ��Z�o
		previous_time_ = current_time; // ���̎��Ԃ�ۑ�
		ret = TRUE;
	}

	return ret;
}

/******************************************
 * �֐��� : Draw�֐�                      *
 * ��  �e : FPS���^�C�g���o�[�ɕ\�����܂� *
 * ��  �� : �Ȃ�                          *
 * �߂�l : �Ȃ�                          *
 ******************************************/
VOID FrameRate::Draw(VOID)
{
	CHAR str[MAX_PATH];	// �^�C�g�����p

	/* �^�C�g�������쐬���Đݒ� */
	sprintf_s(str, MAX_PATH, "%s�F%.1f", kWindowName, GetFps());
	SetWindowText(FindWindow(kClassName, fps_string_), str);

	/* �X�V�������O��ۑ� */
	strcpy_s(fps_string_, str);
}