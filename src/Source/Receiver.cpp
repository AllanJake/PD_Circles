#include "../Header/Receiver.h"

Receiver::Receiver(PlaydateAPI* pd)
    : Module(pd)
{

}

Receiver::~Receiver()
{
}

void Receiver::Update()
{
    GameObject::Update();
}

void Receiver::Draw()
{
    GameObject::Draw();
}

bool Receiver::TryGetImageSize(Vec2 &outVec)
{
    return false;
}
