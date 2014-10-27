/* Include */
#include "field.h"

/******************
 * コンストラクタ *
 ******************/
Field::Field(VOID)
{
	/* 初期化 */
	for (INT i = 0; i < BLOCK_MAX; i++)
	{
		block[i] = NULL;
		block[i] = new FieldObject;
	}
}

/****************
 * デストラクタ *
 ****************/
Field::~Field(VOID)
{
	/* 破棄を行う */
	for (INT i = 0; i < BLOCK_MAX; i++)
	{
		block[i]->Destroy();
		SafeDelete(block[i]);
	}
}

/***********************************
 * 関数名 : Init関数               *
 * 内  容 : 初期化処理を行います   *
 * 引  数 : なし                   *
 * 戻り値 : 初期化に成功したらTRUE *
 ***********************************/
BOOL Field::Init(VOID)
{
	BOOL ret = TRUE;    // 判定用

	do
	{
		/* スカイボール初期化 */
		if (!SkyInit())
		{
			/* エラー */
			ErrorDialog("スカイボールの初期化に失敗しました");
			ret = FALSE;
			break;
		}

		/* ブロック初期化 */
		if (!BlockInit())
		{
			/* エラー */
			ErrorDialog("ブロックの初期化に失敗しました");
			ret = FALSE;
		}

		/* マップ初期化 */
		if (!MapFileSet())
		{
			/* エラー */
			ErrorDialog("ファイルの読み込みに失敗しました");
			ret = FALSE;
		}

	} while (0);

	return ret;
}

/***********************************
 * 関数名 : ReSet関数              *
 * 内  容 : 状態の初期化を行います *
 * 引  数 : なし                   *
 * 戻り値 : なし                   *
 ***********************************/
VOID Field::ReSet(VOID)
{
	information_list_ = info_list_;
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Field::UpDate(VOID)
{
	/* 当たり判定の取得 */
	PlayerHitCheck();
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Field::Draw(VOID)
{
	/* フィールドの描画 */
	FieldDraw();

	/* スカイボールの描画 */
	SkyDraw();
}

/*******************************
 * 関数名 : Destroy関数        *
 * 内  容 : 破棄処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Field::Destroy(VOID)
{
	collision_.Destroy();

	for (INT i = 0; i < BLOCK_MAX; i++)
	{
		block[i]->Destroy();
	}
}

/***********************************************************
 * 関数名 : BlockCheck関数                                 *
 * 内  容 : プレイヤーが現在触れているブロックを取得します *
 * 引  数 : なし                                           *
 * 戻り値 : 当たったブロックの属性を返します               *
 ***********************************************************/
INT Field::BlockCheck(VOID)
{
	/* イテレータ */
	it_player_pos_        = player_pos_list_.begin();
	it_player_matrix_     = player_list_.begin();
	it_player_infomation_ = player_information_.begin();
	it_radius_            = player_radius_list_.begin();
	it_player_end_        = player_list_.end();

	D3DXVECTOR3* pos = new D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	D3DXVECTOR3 ray = player_pos_[player_] - origin_pos_[player_];
	ray.y = 0.1F;

	/* リストの最後まで判定する */
	while (it_player_matrix_ != it_player_end_)
	{
		if (!(*it_player_infomation_ == SLOPE || *it_player_infomation_ == WALL))
		{
			/* 当たり判定を取得します */
			if (collision_.RayIntersect(block[*it_player_infomation_]->ReturnMesh(), *it_player_matrix_, origin_pos_[player_], ray, pos))
			{
				/* チェックポイントなら */
				if (*it_player_infomation_ == CHECK) return GROUND;

				return *it_player_infomation_;
			}
		}
		else
		{
			if(collision_.SphereHitCheck(
				player_pos_[player_],
				*it_player_pos_,
				player_radius_[player_],
				*it_radius_))
			{
				return *it_player_infomation_;
			}
		}

		/* イテレータを進める */
		++it_player_matrix_; 
		++it_player_infomation_;
		++it_radius_;
	}

	return BLOCK_MAX;
}

/***********************************************
 * 関数名 : BulletCheck関数                    *
 * 内  容 : 弾の当たったブロック判定を行います *
 * 引  数 : なし                               *
 * 戻り値 : 当たったらTRUEを返します           *
 ***********************************************/
BOOL Field::BulletCheck(INT PlayerColor)
{
	/* フィールドの座標を更新 */
	it_bullet_matrix_     = matrix_list_.begin();
	it_bullet_position_   = pos_list_.begin();
	it_bullet_infomation_ = information_list_.begin();
	it_bullet_end_        = matrix_list_.end();

	D3DXVECTOR3* pos = new D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	D3DXVECTOR3 ray = bullet_pos_[player_] - origin_bullet_[player_];

	/* 当たり判定をする範囲を判定する */
	while (it_bullet_matrix_ != it_bullet_end_)
	{
		if (D3DXVec3Length(&(D3DXVECTOR3(*it_bullet_position_ - bullet_pos_[player_]))) < kFieldCollision)
		{
			/* 当たり判定を取得します */
			if (collision_.SphereHitCheck(
					*it_bullet_position_,
					bullet_pos_[player_],
					radius_,
					bullet_radius_[player_]))
			{
				/* 色がつかないブロックなら色を変えずに返す */
				if ((*it_bullet_infomation_ == WALL)  ||
					(*it_bullet_infomation_ == SLOPE) ||
					(*it_bullet_infomation_ == DUMMY))
				{
					return TRUE;
				}

				/* 変えられるブロックなら色を変える */
				*it_bullet_infomation_ = PlayerColor;
				return TRUE;
			}
		}

		/* イテレータを進める */
		++it_bullet_matrix_;
		++it_bullet_position_;
		++it_bullet_infomation_;
	}

	return FALSE;
}

/*********************************************
 * 関数名 : SkyInit関数                      *
 * 内  容 : スカイボールの初期化を行います   *
 * 引  数 : なし                             *
 * 戻り値 : 初期化に成功したらTRUEを返します *
 *********************************************/
BOOL Field::SkyInit(VOID)
{
	BOOL ret = TRUE;

	/* フォルダ階層を移動する */
	_chdir("Resource_fast/Mesh");

	do
	{
		/* キャラクターのメッシュの取得 */
		if ((my_mesh_ = Mesh::Load(kSkyBall)) == NULL)
		{
			/* プレイヤーメッシュの情報取得に失敗 */
			ErrorDialog("メッシュの情報取得に失敗しました");
			ret = FALSE;
			break;
		}

		/* 階層を移動する */
		_chdir("../../");

	} while (0);

	return ret;
}

/*********************************************
 * 関数名 : BlockInit関数                    *
 * 内  容 : ブロックの初期化を行います       *
 * 引  数 : なし                             *
 * 戻り値 : 初期化に成功したらTRUEを返します *
 *********************************************/
BOOL Field::BlockInit(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* ステージごとの読み込みメッシュを取得 */
		switch (field_no_)
		{
			case GRASS:
				block[GROUND]->SetMeshName(kGrassGround);  // 草のブロック
				break;

			case SNOW:
				block[GROUND]->SetMeshName(kSnowGround);   // 雪のブロック
				break;

			case MOUNTAIN:
				block[GROUND]->SetMeshName(kRockyGround);  // 岩のブロック
				break;

			case BEACH:
				block[GROUND]->SetMeshName(kSandGround);   // 砂のブロック
				break;
		}

		/* 共通のメッシュを取得 */
		block[THORN]->SetMeshName(kThorn);
		block[SLOPE]->SetMeshName(kSlope);
		block[CHECK]->SetMeshName(kCheck);
		block[WALL]->SetMeshName(kWall);
		block[DUMMY]->SetMeshName(kWall);

		/* 各色のメッシュを取得 */
		block[RED_BLOCK]->SetMeshName(kRedBlock);
		block[GREEN_BLOCK]->SetMeshName(kGreenBlock);
		block[YELLOW_BLOCK]->SetMeshName(kYellowBlock);
		block[BLUE_BLOCK]->SetMeshName(kBlueBlock);

		/* 初期化 */
		for (INT i = 0; i < BLOCK_MAX; i++)
		{
			/* 各ブロックの初期化処理 */
			if (!block[i]->Init())
			{
				/* ブロックの情報取得に失敗 */
				ErrorDialog("ブロックのメッシュ情報取得に失敗しました");
				ret = FALSE;
				break;
			}
		}

	} while (0);

	return ret;
}

/************************************************************
 * 関数名 : MapFileSet関数                                  *
 * 内  容 : マップ用のcsvファイルを展開し、情報を格納します *
 * 引  数 : なし                                            *
 * 戻り値 : なし                                            *
 ************************************************************/
BOOL Field::MapFileSet(VOID)
{
	D3DXVECTOR3 pos_ = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	FLOAT       direction = 0.0F;  // ブロックの向き
	FLOAT       wall      = 0.0F;  // ブロックの高さ
	INT         loop      = 0;     // ブロックの上に積む個数
	INT         block_type;        // ブロックの属性
	INT         count = 0;
	FILE*       fp;            // ファイルポインタ
	CHAR        buf[NAME_LEN]; // 文字格納用
	D3DXMATRIX  pos, rota;     // 情報格納用
	BOOL ret = TRUE;           // 戻り値

	/* フォルダ階層を移動する */
	_chdir(map_no_);

	do
	{
		/* ファイルの読み込み */
		fopen_s(&fp, kFieldMap, "r");

		/* 取得できたかどうか */
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
				if ((buf[i] != ',') && (buf[i] != '\n'))
				{
					/* CHAR型からINT型に変換 */
					block_type = ((INT)buf[i] - (INT)'0');  // その他

					switch (buf[i])
					{
						case '*': pos_.y += 1.0F; count++; continue;  // 高さを上げる
						case '/': pos_.y -= 1.0F; count--; continue;  // 高さを下げる
						case 'a': pos_.y += 0.5F; continue;  // 坂の補正上げ
						case 'i': pos_.y -= 0.5F; continue;  // 坂の補正下げ
						case 'l': direction = (kSlopeDirection / 180.0F)     * D3DX_PI; continue; // 左を向く
						case 't': direction = (kSlopeDirection * 2 / 180.0F) * D3DX_PI; continue; // 上を向く
						case 'r': direction = (kSlopeDirection * 3 / 180.0F) * D3DX_PI; continue; // 右を向く
						case 'd': direction = (kSlopeDirection * 4 / 180.0F) * D3DX_PI; continue; // 下を向く
						case '3': loop = 1; break;
						case '4': loop = kDammyLoop; break;  // ダミー
						default : loop = 1; 
					}

					/* 高さを保存しておく */
					wall = pos_.y;
					if (buf[i] == '3' || buf[i] == '4') wall += 0.5F;

					/* 指定された高さ分のループを行う */
					for (INT n = 0; n < loop; n++)
					{
						/* 座標を保存する */
						pos_list_.push_back(pos_);

						/* 座標をセットする */
						D3DXMatrixIdentity(&world_);
						D3DXMatrixRotationY(&rota, direction);
						D3DXMatrixTranslation(&pos, pos_.x, wall, pos_.z);

						D3DXMatrixMultiply(&world_, &rota, &pos);
						matrix_list_.push_back(world_);  // 座標を格納する

						if (buf[i] == '4' && n == 0)
						{
							information_list_.push_back(GROUND);
							wall -= 1.0F;
						}
						else information_list_.push_back(block_type); // ブロックの属性を格納

						wall += 1.0F;
					}

					i++;
				}

				pos_.x += 1.0F;    // 座標を動かす
				direction = 0.0F;  // 向きを初期化
			}

			pos_.x  = 0.0F;  // 横軸の初期化
			pos_.y  = 0.0F;  // 高さの初期化
			pos_.z -= 1.0F;  // 縦軸を動かす
		}

		info_list_ = information_list_;

	}while (0);

	/* フォルダ階層を戻す */
	_chdir("../../../");

	/* ファイルを閉じる */
	fclose(fp);

	return ret;
}

/***************************************************
 * 関数名 : PlayerHitCheck関数                     *
 * 内  容 : プレイヤーの近くのブロックを判定します *
 * 引  数 : なし                                   *
 * 戻り値 : なし                                   *
 ***************************************************/
VOID Field::PlayerHitCheck(VOID)
{
	/* フィールドの座標を更新 */
	it_matrix_     = matrix_list_.begin();
	it_position_   = pos_list_.begin();
	it_infomation_ = information_list_.begin();

	/* 終了位置 */
	it_end_position_ = pos_list_.end();

	/* 初期化 */
	draw_matrix_[player_].clear();
	draw_pos_[player_].clear();
	draw_information_[player_].clear();

	player_pos_list_.clear();
	player_list_.clear();
	player_information_.clear();
	player_radius_list_.clear();

	player_len_list_.clear();

	/* 当たり判定をする範囲を判定する */
	while (it_position_ != it_end_position_)
	{
		/* プレイヤーの座標の近くを判定する */
		if (D3DXVec3Length(&(D3DXVECTOR3(*it_position_ - player_draw_pos_[player_]))) < kFieldDraw)
		{
			/* 座標をセットする */
			draw_pos_[player_].push_back(*it_position_);
			draw_matrix_[player_].push_back(*it_matrix_);  // 座標を格納する
			draw_information_[player_].push_back(*it_infomation_);

			len_ = max_ - min_;

			player_len_list_.push_back(len_);
		}

		/* プレイヤーの座標の近くを判定する */
		if (D3DXVec3Length(&(D3DXVECTOR3(*it_position_ - player_pos_[player_]))) < kFieldCollision)
		{
			player_pos_list_.push_back(*it_position_);
			player_list_.push_back(*it_matrix_);  // 座標を格納する
			player_information_.push_back(*it_infomation_);

			player_radius_list_.push_back(collision_.SphereCreate(block[*it_infomation_]->ReturnMesh()));
		}

		/* イテレータを進める */
		++it_matrix_;
		++it_position_;
		++it_infomation_;
	}
}

/***************************************
 * 関数名 : FieldDraw関数              *
 * 内  容 : フィールドの描画を行います *
 * 引  数 : なし                       *
 * 戻り値 : なし                       *
 ***************************************/
VOID Field::FieldDraw(VOID)
{
	/* イテレータ */
	it_draw_pos_        = draw_pos_[player_].begin();
	it_draw_matrix_     = draw_matrix_[player_].begin();
	it_draw_infomation_ = draw_information_[player_].begin();
	it_draw_end_        = draw_matrix_[player_].end();
	it_len_list_ = player_len_list_.begin();

	/* 全体の描画を行う */
	while (it_draw_matrix_ != it_draw_end_)
	{
		if (*it_draw_infomation_ <= BLOCK_MAX)
		{
			/* 描画を行う */
			block[*it_draw_infomation_]->SetMatrix(*it_draw_matrix_);
			block[*it_draw_infomation_]->SetPosition(*it_draw_pos_);

			/* 隠し通路なら */
			if (*it_draw_infomation_ == DUMMY)
			{
				block[WALL]->Draw();
			}

			//collision_.BoxMesh(block[*it_draw_infomation_]->ReturnMesh(),
			//	*it_draw_pos_,
			//	*it_len_list_);

			block[*it_draw_infomation_]->Draw();
		}

		/* 更新 */
		++it_draw_pos_;
		++it_draw_matrix_;
		++it_draw_infomation_;
	}
}

/*****************************************
 * 関数名 : SkyDraw関数                  *
 * 内  容 : スカイボールの描画を行います *
 * 引  数 : なし                         *
 * 戻り値 : なし                         *
 *****************************************/
VOID Field::SkyDraw(VOID)
{
	/* ワールド座標系を作成 */
	D3DXMatrixIdentity(&world_);
	D3DXMatrixTranslation(&world_, player_pos_[player_].x,
		player_pos_[player_].y - 50.0F,
		player_pos_[player_].z);

	/* スカイボールを描画 */
	mesh_.Draw(world_, my_mesh_);
}