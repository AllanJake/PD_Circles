#include "../Header/Laser.h"
#include <cmath>
#include "../Header/Raycast.h"

void Laser::DrawLaser(Vec2 origin, float rotation)
{
    _origin = origin;
    float closestIntersectDistance = _laserLength;

    // Calculate Right Vector
    float angleRad = rotation * (M_PI / 180.0f);
    float x = sinf(angleRad);
    float y = -cosf(angleRad);
    Vec2 rightVector = {x, y};

    RaycastHit hit;
    if (Raycast::LineTrace(_origin, rightVector, _laserLength, hit)) {
        int tagSize = static_cast<int>(hit.hitObject->tags.size());

        // Only modules have tags, so start module checks
        if (tagSize > 0) {
            bool moduleHit = std::count(hit.hitObject->tags.begin(), hit.hitObject->tags.end(), "module") > 0;
            bool emitterHit = std::count(hit.hitObject->tags.begin(), hit.hitObject->tags.end(), "emitter") > 0;
            bool receiverHit = std::count(hit.hitObject->tags.begin(), hit.hitObject->tags.end(), "receiver") > 0;
            
            if (moduleHit) {
                closestIntersectDistance = hit.distance;
                Module* hitModule = dynamic_cast<Module*>(hit.hitObject);
                if (receiverHit) {
                    hitModule->SetModuleActive(true);
                    lastModule = hitModule;
                }
            }             
        }
    }
    else {
        if (lastModule != nullptr) {
            lastModule->SetModuleActive(false);
            lastModule = nullptr;
        }
    }
    
    Vec2 destination = {_origin.x + rightVector.x * closestIntersectDistance, _origin.y + rightVector.y * closestIntersectDistance};
    pd->graphics->drawLine(origin.x, origin.y, destination.x, destination.y, 2.5f, kColorBlack);
}
