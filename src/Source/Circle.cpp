#include "../Header/Circle.h"

Circle::Circle(PlaydateAPI* pd, int radius)
    : pd(pd), radius(radius)
{
    
}

Circle::~Circle()
{

}

void Circle::Update()
{
    GameObject::Update();
}

void Circle::Draw() {
    Vec2 pos = GetWorldPosition();

    pd->graphics->drawEllipse(pos.x - radius, pos.y - radius, radius * 2, radius * 2, 5, 0, 360, kColorBlack);

    GameObject::Draw();
}