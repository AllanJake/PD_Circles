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
    int lineWidth = defaultLineWidth;
    switch (circleState)
    {
        default:
        case STATE::UNSELECTED:
        {
            pd->graphics->drawEllipse(pos.x - radius, pos.y - radius, radius * 2, radius * 2, lineWidth, 0, 360, kColorBlack);
            break;
        }
        case STATE::SELECTED:
        {
            lineWidth = 7;
            int sizeDiff = lineWidth - defaultLineWidth;
            pd->graphics->drawEllipse(pos.x - radius, pos.y - radius, (radius * 2) + (sizeDiff / 2), (radius * 2) + (sizeDiff / 2), lineWidth, 0, 360, kColorBlack);
            break;
        }
        case STATE::LOCKED:
        {
            lineWidth = 1;
            int majorRad = radius + 2;
            int minorRad = radius - 2;
            pd->graphics->drawEllipse(pos.x - minorRad, pos.y - minorRad, minorRad * 2, minorRad * 2, lineWidth, 0, 360, kColorBlack);
            pd->graphics->drawEllipse(pos.x - majorRad, pos.y - majorRad, majorRad * 2, majorRad * 2, lineWidth, 0, 360, kColorBlack);
            break;
        }
    }

    

    GameObject::Draw();
}