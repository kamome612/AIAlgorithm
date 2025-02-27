#pragma once
#include "./Library/GameObject.h"
#include "./globals.h"
#include <vector>

class Enemy :
    public GameObject
{
    Point pos_;
    bool isAlive_;
    float speed_;
    Point nextPos_;
    DIR forward_;
    bool isRandom_;
    bool isRight_;
    std::vector<Point> route_;
    /*Point move_;
    bool moveNow_;
    Point dis;*/
public:
    Enemy();
    ~Enemy();

    void Update() override;
    void Draw() override;
    bool CheckHit(const Rect& me, const Rect& other);
    bool HitToChip(int x, int y);
    void XCloserMove();
    void YCloserMove();
    void XYCloserMove();
    void XYCloserMoveRandom();
    void RightHandMove();
    void LeftHandMove();
};

