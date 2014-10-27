/***********
 * Include *
 ***********/
#include "checkpoint.h"

/******************
 * コンストラクタ *
 ******************/
CheckPoint::CheckPoint(VOID)
{
	/* プレイヤーの数値のリセット */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		player_check_[i].ReSet();
		return_position_[i] = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	}
}

/****************
 * デストラクタ *
 ****************/
CheckPoint::~CheckPoint(VOID)
{
	Mesh::Release(my_mesh_);
	LayoutBase::Destroy();
}

/*********************************
 * 関数名 : Init関数             *
 * 内  容 : 初期化を行います     *
 * 引  数 : なし                 *
 * 戻り値 : 成功したらTRUEを返す *
 *********************************/
BOOL CheckPoint::Init(VOID)
{
	/* ローカル変数 */
	BOOL ret = TRUE;

	do
	{
		/* ファイルの読み込み */
		if (!FileCheck())
		{
			/* エラー */
			ErrorDialog("ファイルの読み込みの取得に失敗");
			ret = FALSE;
			break;
		}

		/* テクスチャの読み込み */
		if (!LayoutBase::Init())
		{
			/* エラー */
			ErrorDialog("テクスチャの情報取得に失敗");
			ret = FALSE;
			break;
		}

		/* 階層を移動する */
		_chdir("Resource_fast/Mesh/Block");

		/* メッシュの読み込み */
		//if ((my_mesh_ = Mesh::Load(kHitCheck)) == NULL)
		//{
		//	/* エラー */
		//	ErrorDialog("ブロックのメッシュ情報の取得に失敗");
		//	ret = FALSE;
		//	break;
		//}

		if ((my_mesh_ = Mesh::Load("wall.x")) == NULL)
		{
			ErrorDialog("エラー");
			ret = FALSE;
		}

		/* 階層を戻す */
		_chdir("../../../");

	} while (0);

	return ret;
}

/*********************************
 * 関数名 : ReSet関数            *
 * 内  容 : 初期化処理を行います *
 * 引  数 : なし                 *
 * 戻り値 : なし                 *
 *********************************/
VOID CheckPoint::ReSet(VOID)
{
	/* プレイヤー情報の初期化 */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		player_check_[i].ReSet();
	}
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
INT CheckPoint::UpDate(INT player_no_)
{
	///* ボタン入力保存用 */
	//ULONG push_button[kAllPlayer];

	///* ボタン入力を格納 */
	//push_button[player_no_] = XInput::ReturnPushButton(player_no_);

	///* 十字下キーが押されたら */
	//if (push_button[player_no_] == XINPUT_GAMEPAD_DPAD_DOWN)
	//{
	//	/* そのプレイヤーをゴールさせる */
	//	player_check_[player_no_].lap = kFinalLap + 1;
	//}

	///* 十字キー上が押されたら */
	//if (push_button[player_no_] == XINPUT_GAMEPAD_DPAD_UP)
	//{
	//	/* 全員をゴールさせる */
	//	for (INT i = 0; i < kAllPlayer; i++)
	//	{
	//		player_check_[i].lap = kFinalLap + 1;
	//	}
	//}

	return LapCheck();
}

/*********************************************
 * 関数名 : Draw関数                         *
 * 内  容 : チェックポイントの描画を行います *
 * 引  数 : INT player_no プレイヤー番号     *
 * 戻り値 : なし                             *
 *********************************************/
VOID CheckPoint::Draw(INT player_no)
{
	//it_matrix_ = check_matrix_.begin();

	//while (it_matrix_ != check_matrix_.end())
	//{
	//	Mesh::Draw(*it_matrix_, my_mesh_);

	//	++it_matrix_;
	//}
}

/*****************************************
 * 関数名 : TwoDraw関数                  *
 * 内  容 : 2人用の描画処理を行います    *
 * 引  数 : INT player_no プレイヤー番号 *
 * 戻り値 : なし                         *
 *****************************************/
VOID CheckPoint::TwoDraw(INT player_no)
{
	TwoLapDraw(player_no);      // 周回数の描画
	TwoRankingDraw(player_no);  // 順位の描画
}

/*****************************************
 * 関数名 : MultiDraw関数                *
 * 内  容 : 多人数用の描画処理を行います *
 * 引  数 : INT player_no プレイヤー番号 *
 * 戻り値 : なし                         *
 *****************************************/
VOID CheckPoint::MultiDraw(INT player_no)
{
	MultiLapDraw(player_no);      // 周回数の描画
	MultiRankingDraw(player_no);  // 順位の描画
}

/*********************************
 * 関数名 : Destroy関数          *
 * 内  容 : 情報の破棄を行います *
 * 引  数 : なし                 *
 * 戻り値 : なし                 *
 *********************************/
VOID CheckPoint::Destroy(VOID)
{
	Mesh::Release(my_mesh_);     // メッシュの解放
	Texture::Release(texture_);  // テクスチャの解放
}

/*****************************************
 * 関数名 : TwoLapDraw関数               *
 * 内  容 : 2人用の周回を描画します      *
 * 引  数 : INT player_no プレイヤー番号 *
 * 戻り値 : なし                         *
 *****************************************/
VOID CheckPoint::TwoLapDraw(INT player_no)
{
	/* 配置座標 */
	D3DXVECTOR3 kMultiRankingPosition[kAllPlayer] =
	{
		D3DXVECTOR3(10.0F,                  kMultiY, 0.0F),  // プレイヤー1
		D3DXVECTOR3(10.0F, kMultiWindow.y + kMultiY, 0.0F),  // プレイヤー2
	};

	/* 座標の作成 */
	D3DXVECTOR3 now_pos = kMultiRankingPosition[player_no];
	now_pos.x += kLapPosition;
	now_pos.y -= 15.0F;

	/* 座標の作成 */
	D3DXVECTOR3 pos = now_pos;
	pos.x += (kLapPosition / 2);

	/* 座標の作成 */
	D3DXVECTOR3 final_pos_ = now_pos;
	final_pos_.x += kLapPosition;

	/* 文字の描画 */
	DrawSet(kMultiRankingPosition[player_no], kLapRect, player_no);

	/* 現在の周回数と最終周回目標を描画 */
	CountSet(now_pos, player_check_[player_no].lap, MIDDLE, player_no, 1);

	pos.y += 10.0F;
	DrawSet(pos, kBarRect, player_no);
	CountSet(final_pos_, kFinalLap, MIDDLE, player_no, 1);

	/* イテレーターの設定 */
	it_matrix_ = check_matrix_.begin();       // ワールド座標

	//CountSet(D3DXVECTOR3(0.0F, 0.0F, 0.0F), player_check_[player_no].check_no, MIDDLE, player_no);
}

/*****************************************
 * 関数名 : MultiLapDraw関数             *
 * 内  容 : 多人数の周回を描画します     *
 * 引  数 : INT player_no プレイヤー番号 *
 * 戻り値 : なし                         *
 *****************************************/
VOID CheckPoint::MultiLapDraw(INT player_no)
{
	/* 配置座標 */
	D3DXVECTOR3 kMultiRankingPosition[kAllPlayer] =
	{
		D3DXVECTOR3(10.0F                 , kMultiY                 , 0.0F),  // プレイヤー1
		D3DXVECTOR3(10.0F + kMultiWindow.x, kMultiY                 , 0.0F),  // プレイヤー2
		D3DXVECTOR3(10.0F                 , kMultiWindow.y + kMultiY, 0.0F),  // プレイヤー3
		D3DXVECTOR3(10.0F + kMultiWindow.x, kMultiWindow.y + kMultiY, 0.0F)   // プレイヤー4
	};

	/* 座標の作成 */
	D3DXVECTOR3 now_pos = kMultiRankingPosition[player_no];
	now_pos.x += kLapPosition;
	now_pos.y -= 15.0F;

	/* 座標の作成 */
	D3DXVECTOR3 pos = now_pos;
	pos.x += (kLapPosition / 2);

	/* 座標の作成 */
	D3DXVECTOR3 final_pos_ = now_pos;
	final_pos_.x += kLapPosition;

	/* 文字の描画 */
	DrawSet(kMultiRankingPosition[player_no], kLapRect, player_no);
	
	/* 現在の周回数と最終周回目標を描画 */
	CountSet(now_pos, player_check_[player_no].lap, MIDDLE, player_no, 1);

	pos.y += 10.0F;
	DrawSet(pos, kBarRect, player_no);
	CountSet(final_pos_, kFinalLap, MIDDLE, player_no, 1);
}

/******************************************
 * 関数名 : TwoRankingDraw関数            *
 * 内  容 : 2人用のランキングを描画します *
 * 引  数 : INT player_no プレイヤー番号  *
 * 戻り値 : なし                          *
 ******************************************/
VOID CheckPoint::TwoRankingDraw(INT player_no)
{
	/* 配置座標 */
	D3DXVECTOR3 kMultiRankingPosition[kAllPlayer] =
	{
		D3DXVECTOR3(10.0F,                  kMultiY, 0.0F),  // プレイヤー1
		D3DXVECTOR3(10.0F, kMultiWindow.y + kMultiY, 0.0F),  // プレイヤー2
	};

	/* 座標の作成 */
	D3DXVECTOR3 position = kMultiRankingPosition[player_no] + kMultiRankingPos;
	D3DXVECTOR3 position2 = position + kMultiRankPos;

	/* 現在のランキングを描画 */
	CountSet(position, player_check_[player_no].ranking, MIDDLE, player_no, 1);
	DrawSet(position2, kRankingRect, player_no, 0);
}

/*******************************************
 * 関数名 : MultiRankingDraw関数           *
 * 内  容 : 多人数のランキングを描画します *
 * 引  数 : INT player_no プレイヤー番号   *
 * 戻り値 : なし                           *
 *******************************************/
VOID CheckPoint::MultiRankingDraw(INT player_no)
{
	/* 配置座標 */
	D3DXVECTOR3 kMultiRankingPosition[kAllPlayer] =
	{
		D3DXVECTOR3(10.0F                 ,                  kMultiY, 0.0F),  // プレイヤー1
		D3DXVECTOR3(10.0F + kMultiWindow.x,                  kMultiY, 0.0F),  // プレイヤー2
		D3DXVECTOR3(10.0F                 , kMultiWindow.y + kMultiY, 0.0F),  // プレイヤー3
		D3DXVECTOR3(10.0F + kMultiWindow.x, kMultiWindow.y + kMultiY, 0.0F)   // プレイヤー4
	};

	/* 座標の作成 */
	D3DXVECTOR3 position  = kMultiRankingPosition[player_no] + kMultiRankingPos;
	D3DXVECTOR3 position2 = position + kMultiRankPos;

	/* 現在のランキングを描画 */
	CountSet(position, player_check_[player_no].ranking, MIDDLE, player_no, 1);
	DrawSet(position2, kRankingRect, player_no, 0);
}

/******************************************
 * 関数名 : RankingSort関数               *
 * 内  容 : 順位をソートして返します      *
 * 引  数 : INT player_no_ プレイヤー番号 *
 * 戻り値 : INT ranking   順位            *
 ******************************************/
VOID CheckPoint::RankingSort(INT player_no, INT player_count)
{
	INT ranking = 1;

	/* ランキングをソートする */
	for (INT i = 0; i < player_count; i++)
	{
		/* 周回数が他のプレイヤーより少ないなら */
		if (player_check_[player_no].lap < player_check_[i].lap)
		{
			/* 順位を1増やす */
			ranking++;
		}
		/* チェックポイントが少ないなら */
		else if (player_check_[player_no].lap == player_check_[i].lap &&
			player_check_[player_no].check_no < player_check_[i].check_no)
		{
			/* 順位を1増やす */
			ranking++;
		}
		/* 距離が遠いなら */
		else if (player_check_[player_no].lap == player_check_[i].lap &&
				 player_check_[player_no].check_no == player_check_[i].check_no &&
			     player_check_[player_no].vector > player_check_[i].vector)
		{
			/* 順位を1増やす */
			ranking++;
		}
	}

	/* 順位を保存 */
	player_check_[player_no].ranking = ranking;   
}

/*********************************************************
 * 関数名 : LapCheck関数                                 *
 * 内  容 : プレイヤーのチェックポイント到達を管理します *
 * 引  数 : なし                                         *
 * 戻り値 : 現在の状態を返します                         *
 *********************************************************/
INT CheckPoint::LapCheck(VOID)
{
	INT point = NoPoint;

	/* イテレーターの設定 */
	it_matrix_    = check_matrix_.begin();       // ワールド座標
	it_position_  = check_position_.begin();     // 座標
	it_infomation = check_information_.begin();  // ブロック情報

	D3DXVECTOR3* pos = new D3DXVECTOR3(0.0F, 0.0F, 0.0F);

	/* 移動後の座標から移動前の座標を引いた値をレイとして判定する */
	D3DXVECTOR3  ray = D3DXVECTOR3(-1.0F, 0.0F, -1.0F);

	/* 終了まで続ける */
	while (it_matrix_ != check_matrix_.end())
	{
		/* 1週した場合 */
		if ((*it_infomation == 0) && (player_check_[player_no_].check_no == check_max_))
		{
			player_check_[player_no_].vector = D3DXVec3LengthSq(&(D3DXVECTOR3(*it_position_ - player_pos_[player_no_].player_pos_)));
		}
		else if (*it_infomation == player_check_[player_no_].check_no + 1)
		{
			player_check_[player_no_].vector = D3DXVec3LengthSq(&(D3DXVECTOR3(*it_position_ - player_pos_[player_no_].player_pos_)));
		}

		/* プレイヤーが近くにいるかの判定 */
		if (D3DXVec3Length(&(D3DXVECTOR3(*it_position_ - player_pos_[player_no_].player_pos_))) < kCheckCollision)
		{
			/* 当たり判定 */
			if (collision_.RayIntersect(my_mesh_, *it_matrix_, player_pos_[player_no_].origin_pos_, ray, pos))
			{
				return_position_[player_no_] = *it_position_;

				/* 1週した場合 */
				if ((*it_infomation == 0) && player_check_[player_no_].check_no == check_max_)
				{
					player_check_[player_no_].lap++;  // 周回を1増やす
					player_check_[player_no_].check_no = *it_infomation;

					/* 戻り値を返す */
					point = WeekPoint;

					/* SEの再生 */
					DSound::Restart(GOAL_SE);
					DSound::Play(GOAL_SE);
					break;
				}
				/* チェックポイントなら */
				else if (player_check_[player_no_].check_no + 1 == *it_infomation)
				{
					player_check_[player_no_].check_no = *it_infomation;
					point = MiddlePoint;
					break;
				}
			}
		}

		++it_matrix_;
		++it_position_;
		++it_infomation;
	}

	return point;
}

/*************************************
 * 関数名 : FileCheck関数            *
 * 内  容 : ファイルを開き管理します *
 * 引  数 : なし                     *
 * 戻り値 : 成功したらTRUEを返す     *
 *************************************/
BOOL CheckPoint::FileCheck(VOID)
{
	BOOL ret = TRUE;
	FILE* fp;

	INT   player    = 0;     // プレイヤー番号
	INT   check_no  = 0;     // チェックポイント番号
	FLOAT direction = 0.0F;  // 向きの指定
	CHAR  buf[NAME_LEN];     // データ格納用

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	D3DXMATRIX  world, rota, w_pos;

	/* 階層を移動する */
	_chdir(check_no_);

	do
	{
		/* ファイルの読み込み */
		fopen_s(&fp, kCheckCsv, "r");

		/* ファイルが読み込めたか確認 */
		if (!fp)
		{
			/* エラー */
			ErrorDialog("ブロックのファイル読み込みに失敗しました");
			ret = FALSE;
			break;
		}

		/* ファイルの終端まで文字を読み取り表示する */
		while (fgets(buf, NAME_LEN, fp) != NULL)
		{
			for (UINT i = 0; i < strlen(buf); i++)
			{
				/* 中身が数字なら */
				if (((buf[i] != ',') && (buf[i] != '\n') && (buf[i] != '-')))
				{
					/* 「*」ならスタート位置を設定 */
					if (buf[i] == 's')
					{
						pos.y = 0.0F;
						player_pos_[player++].start_position_ = pos;
					}
					else
					{
						switch (buf[i])
						{
							case 'h' : direction = 0.0F; continue;
							case 'w' : direction = (90.0F / 180.0F) * D3DX_PI; continue;
							default : break;
						}

						check_no = (INT)buf[i] - (INT)'0';

						/* 座標を保存する */
						D3DXMatrixIdentity(&world);  // 初期化
						D3DXMatrixRotationY(&rota, direction);  // 回転値
						D3DXMatrixTranslation(&w_pos, pos.x, pos.y, pos.z);  // 座標位置
						D3DXMatrixMultiply(&world, &rota, &w_pos);

						check_position_.push_back(pos);          // 座標を格納
						check_matrix_.push_back(world);          // ワールド座標を格納
						check_information_.push_back(check_no);  // int型にキャストして格納

						/* チェックポイントの最大値を取得 */
						if (check_max_ <= (check_no)) check_max_ = check_no;
					}
					i++;
				}
				pos.x += 1.0F;  // 座標を動かす
			}

			pos.x = 0.0F;  // 横軸の初期化
			pos.z -= 1.0F;  // 縦軸を動かす
		}

	} while (0);

	/* ファイルを閉じる */
	fclose(fp);

	/* 階層を移動する */
	_chdir("../../../");

	return ret;
}