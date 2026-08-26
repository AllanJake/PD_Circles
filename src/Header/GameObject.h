#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#pragma once

#include <vector>
#include <cmath>
#include <string>
#include "Vector2.h"

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();

    void SetParent(GameObject* newParent);
    void AddChild(GameObject* child);

    virtual void Update();
    virtual void Draw();

    Vec2 GetWorldPosition() const;
    float GetWorldRotation() const;

    bool IsCircleCollision();
    void SetRadius(float value);
    float GetCollisionRadius();
    Vec2 GetAABBMin();
    Vec2 GetAABBMax();

    
    Vec2 localPosition;
    float localRotation;
    std::vector<std::string> tags;
    
protected:
    GameObject* parent;
    std::vector<GameObject*> children;
    float M_PI = 3.14f;
    float DEG2RAD(float degrees);

private:
    bool isCircleCollision = true;
    float collisionRadius;
};  

#endif