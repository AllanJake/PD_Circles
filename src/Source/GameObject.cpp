#include "../Header/GameObject.h"

GameObject::GameObject()
    : localPosition{0, 0}, localRotation(0), parent(nullptr)
{

}

GameObject::~GameObject()
{

}

void GameObject::SetParent(GameObject* newParent)
{
    if (parent) {
        // remove from old parent
    }

    parent = newParent;
    if (parent) parent->AddChild(this);
}

void GameObject::AddChild(GameObject* child)
{
    children.push_back(child);
}

Vec2 GameObject::GetWorldPosition() const {
    if (!parent) return localPosition;

    float radians = parent->GetWorldRotation() * M_PI / 180.0f;
    Vec2 parentPos = parent->GetWorldPosition();

    float x = cosf(radians) * localPosition.x - sinf(radians) * localPosition.y;
    float y = sinf(radians) * localPosition.x + cosf(radians) * localPosition.y;

    return { parentPos.x + x, parentPos.y + y };
}

float GameObject::GetWorldRotation() const {
    if (!parent) return localRotation;
    return parent->GetWorldRotation() + localRotation;
}

void GameObject::Update() {
    for (auto* child : children) {
        child->Update();
    }
}

void GameObject::Draw() {
    for (auto* child : children) {
        child->Draw();
    }
}

bool GameObject::IsCircleCollision()
{
    return isCircleCollision;
}

void GameObject::SetRadius(float value)
{
    collisionRadius = value;
}

float GameObject::GetCollisionRadius()
{
    return collisionRadius;
}

float GameObject::DEG2RAD(float degrees)
{
    return degrees * (M_PI / 180);
}
