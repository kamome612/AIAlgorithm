#include "Enemy.h"
#include "./Stage.h"
#include "globals.h"
#include "time.h"
#include "ImGui/imgui.h"
#include "Player.h"

namespace
{
	Point nDir[4] = { {0,-1},{0,1},{-1,0},{1,0} };
}

Enemy::Enemy()
    :pos_({ 0,0 }), isAlive_(true),nextPos_({0,0})
{
   /* int rx = GetRand(STAGE_WIDTH * CHA_WIDTH);
    int ry = GetRand(STAGE_HEIGHT * CHA_HEIGHT);*/
	/*int rx = GetRand(STAGE_WIDTH - 2) * CHA_WIDTH;
	int ry = GetRand(STAGE_HEIGHT - 2) * CHA_HEIGHT;
    pos_ = { rx, ry };*/
	SRand((unsigned)time(NULL));
	int rx = 0;
	int ry = 0;
	while (rx % 2 == 0 || ry % 2 == 0)
	{
		rx = GetRand(STAGE_WIDTH - 1);
		ry = GetRand(STAGE_HEIGHT - 1);
	}
	pos_ = { rx * CHA_WIDTH, ry * CHA_HEIGHT };
	forward_ = RIGHT;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	static bool stop = false;

	if (!stop) {
		Point op = pos_;
		Point move = { nDir[forward_].x,nDir[forward_].y };
		Rect eRect = { pos_.x,pos_.y,CHA_WIDTH,CHA_HEIGHT };
		Stage* stage = (Stage*)FindGameObject<Stage>();
		pos_ = { pos_.x + move.x,pos_.y + move.y };
		for (auto& obj : stage->GetStageRects())
		{
			if (CheckHit(eRect, obj)) {
				Rect tmpRectX = { op.x,pos_.y,CHA_WIDTH,CHA_HEIGHT };
				Rect tmpRectY = { pos_.x,op.y,CHA_WIDTH,CHA_HEIGHT };
				if (!CheckHit(tmpRectX, obj))
				{
					pos_.x = op.x;
				}
				else if (!CheckHit(tmpRectY, obj))
				{
					pos_.y = op.y;
				}
				else
				{
					pos_ = op;
				}
				forward_ = (DIR)(GetRand(3));
				break;
			}
		}
	}
	int prgssx = pos_.x % (CHA_WIDTH);
	int prgssy = pos_.y % (CHA_HEIGHT);
	int cx = (pos_.x / (CHA_WIDTH)) % 2;
	int cy = (pos_.y / (CHA_HEIGHT)) % 2;
	if (prgssx == 0 && prgssy == 0 && cx && cy)
	{
		//forward_ = (DIR)(GetRand(3));
		//XYCloserMoveRandom();
		//XYCloserMove(); 
		RightHandMove();
	}
	//Point nDir[4] = { {1,0},{0,1},{-1,0},{0,-1} };
	//static int judg = 0;
	//if (!moveNow_) {
	//	move_ = nDir[GetRand(3)];
	//	moveNow_ = true;
	//	dis = {0,0};
	//	judg = (GetRand(3) + 1);
	//}
	//int sx, sy;
	//sx = pos_.x;
	//sy = pos_.y;
 //   //pos_ = { pos_.x + move.x - 1, pos_.y + move.y -1 };
	//if (moveNow_) {
	//	pos_ = { pos_.x + move_.x, pos_.y + move_.y };
	//	dis = { dis.x + move_.x,dis.y + move_.y };
	//	if (dis.x >= CHA_WIDTH * judg || dis.x <= (-CHA_WIDTH * judg) ||
	//		dis.y >= CHA_HEIGHT * judg || dis.y <= (-CHA_HEIGHT * judg))
	//	{
	//		moveNow_ = false;
	//	}
	//}
	//
	////マップチップとの当たり判定
	//if (HitToChip(pos_.x, pos_.y)) {
	//	if (!(HitToChip(sx, pos_.y))) {
	//		//x座標を元に戻したらぶつかっていない→x座標だけ元に戻す
	//		pos_.x = sx;
	//	}
	//	else if (!HitToChip(pos_.x, sy)) {
	//		//y座標を元に戻したらぶつかっていない→y座標だけ元に戻す
	//		pos_.y = sy;
	//	}
	//	else {
	//		//片方だけ戻してもやっぱりぶつかっている→両方もとに戻す
	//		pos_.x = sx;
	//		pos_.y = sy;
	//	}
	//	moveNow_ = false;
	//}
	//
	////プレイヤーとの当たり判定
	//Player* player = (Player*)FindGameObject<Player>();
	//Rect pRect = player->GetRect();
	//Rect myRect = { pos_.x,pos_.y,CHA_WIDTH,CHA_HEIGHT };
	//if (CheckHit(myRect, pRect))
	//{
	//	printfDx("!");
	//}
}

void Enemy::Draw()
{
    DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(80, 89, 10), TRUE);
	Point tp[4][3] = {
		{{pos_.x + CHA_WIDTH / 2,pos_.y},             {pos_.x,pos_.y + CHA_HEIGHT / 2},{pos_.x + CHA_WIDTH,pos_.y + CHA_HEIGHT / 2}},
		{{pos_.x + CHA_WIDTH / 2,pos_.y + CHA_HEIGHT},{pos_.x,pos_.y + CHA_HEIGHT / 2},{pos_.x + CHA_WIDTH,pos_.y + CHA_HEIGHT / 2}},
		{{pos_.x,pos_.y + CHA_HEIGHT / 2},            {pos_.x + CHA_WIDTH / 2,pos_.y}, {pos_.x + CHA_WIDTH / 2,pos_.y + CHA_HEIGHT}},
		{{pos_.x + CHA_WIDTH,pos_.y + CHA_HEIGHT / 2},{pos_.x + CHA_WIDTH / 2,pos_.y}, {pos_.x + CHA_WIDTH / 2,pos_.y + CHA_HEIGHT}}
	};
	DrawTriangle(tp[forward_][0].x, tp[forward_][0].y,
		         tp[forward_][1].x, tp[forward_][1].y,
		         tp[forward_][2].x, tp[forward_][2].y,
		         GetColor(255, 255, 255), TRUE);
}

bool Enemy::CheckHit(const Rect& me, const Rect& other)
{
	if (me.x < other.x + other.w &&
		me.x + me.w > other.x &&
		me.y < other.y + other.h &&
		me.y + me.h > other.y)
	{
		return true;
	}
	return false;
}

bool Enemy::HitToChip(int x, int y)
{
	Rect me = { x,y,CHA_WIDTH,CHA_HEIGHT };
	Stage* stage = (Stage*)FindGameObject<Stage>();
	for (auto& obj : stage->GetStageRects())
	{
		if (CheckHit(me, obj)) {
			return true;
		}
	}
	return false;
}

void Enemy::XCloserMove()
{
	Player* player = (Player*)FindGameObject<Player>();
	if (pos_.x > player->GetPos().x)
	{
		forward_ = LEFT;
	}
	else if (pos_.x < player->GetPos().x)
	{
		forward_ = RIGHT;
	}
}

void Enemy::YCloserMove()
{
	Player* player = (Player*)FindGameObject<Player>();
	if (pos_.y > player->GetPos().y)
	{
		forward_ = UP;
	}
	else if (pos_.y < player->GetPos().y)
	{
		forward_ = DOWN;
	}
}

void Enemy::XYCloserMove()
{
	Player* player = (Player*)FindGameObject<Player>();
	int xdis = abs(pos_.x - player->GetPos().x);
	int ydis = abs(pos_.y - player->GetPos().y);

	if (xdis > ydis) {
		if (pos_.x > player->GetPos().x)
		{
			forward_ = LEFT;
		}
		else if (pos_.x < player->GetPos().x)
		{
			forward_ = RIGHT;
		}
	}
	else
	{
		if (pos_.y > player->GetPos().y)
		{
			forward_ = UP;
		}
		else if (pos_.y < player->GetPos().y)
		{
			forward_ = DOWN;
		}
	}
}

void Enemy::XYCloserMoveRandom()
{
	Player* player = (Player*)FindGameObject<Player>();
	int rnum = GetRand(2);
	if (rnum == 0)
	{
		XYCloserMove();
	}
	else if (rnum == 1)
	{
		forward_ = (DIR)GetRand(3);
	}
}

void Enemy::RightHandMove()
{
	DIR myRight[4] = { RIGHT,LEFT,UP,DOWN };
	DIR myLeft[4] = { LEFT,RIGHT,DOWN,UP };
	Point nposF = { pos_.x + nDir[forward_].x,pos_.y + nDir[forward_].y };
	Point nposR = { pos_.x + nDir[myRight[forward_]].x,pos_.y + nDir[myRight[forward_]].y };
	Rect myRectF{ nposF.x,nposF.y,CHA_WIDTH,CHA_HEIGHT };
	Rect myRectR{ nposR.x,nposR.y,CHA_WIDTH,CHA_HEIGHT };
	Stage* stage = (Stage*)FindGameObject<Stage>();
	bool isRightOpen = true;
	bool isForwardOpen = true;
	for (auto& obj : stage->GetStageRects()) {
		if (CheckHit(myRectF, obj)) {
			isForwardOpen = false;
		}
		if (CheckHit(myRectR, obj)) {
			isRightOpen = false;
		}
	}

	if (isRightOpen) {
		forward_ = myRight[forward_];
	}
	else if (isRightOpen == false && isForwardOpen == false) {
		forward_ = myLeft[forward_];
	}
}
