#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#pragma once

#include <vector>
#include <cmath>

struct Vec2 {
    float x;
    float y;
};

class GameObject
{
public:
    GameObject();
    ~GameObject();

    void SetParent(GameObject* newParent);
    void AddChild(GameObject* child);

    virtual void Update();
    virtual void Draw();

    Vec2 GetWorldPosition() const;
    float GetWorldRotation() const;

    Vec2 localPosition;
    float localRotation;

private:
    GameObject* parent;
    std::vector<GameObject*> children;
    float M_PI = 3.14;

};

#endif