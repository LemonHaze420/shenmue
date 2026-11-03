#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/pati_physics.hpp"
double sqrt(double value);
void AdvancePatiBall(PatiBallWork *ball) {
    int iteration;
    PatiPhysicsCopyVector(&ball->segment.position, &ball->previous.position);
    PatiPhysicsCopyVector(&ball->segment.velocity, &ball->previous.velocity);
    ball->remainingTime = 1;
    ball->soundFlags = 0;
    iteration = 0;
    ball->collision = 0;
    ball->collisionIndex = 0;
    ball->collisionTime = -1;
    do {
        IntegratePatiBall(ball);
        ball->collision = 0;
        ball->collisionIndex = 0;
        ball->collisionTime = -1;
        FindPatiCollision(ball);
        if (!ball->collision)
            break;
        if (!iteration)
            MarkPatiCollisionSound(ball);
        ResolvePatiCollision(ball);
        ball->remainingTime *= ball->turningFraction * ball->collisionTime;
        if (ball->remainingTime < 0.00030000001f)
            break;
        PatiPhysicsCopyVector(&ball->segment.position, &ball->current.position);
        PatiPhysicsCopyVector(&ball->segment.velocity, &ball->current.velocity);
        if (++iteration == 3)
            FTBL.EventAssertNullsub(__FILE__, 204);
    } while (ball->collision);
    PlayPatiCollisionSounds(ball);
    UpdatePatiBallSettled(ball);
    FollowPatiBall(ball);
    ++ball->soundFrames;
    PatiPhysicsCopyVector(&ball->previous.position, &ball->current.position);
    PatiPhysicsCopyVector(&ball->previous.velocity, &ball->current.velocity);
}

void IntegratePatiBall(PatiBallWork *ball) {
    int gravity = 1, index;
    float speedY, minX, maxX, minY, maxY;
    ball->turningFraction = 0;
    if (__builtin_fabs(ball->segment.velocity.x) < 0.000099999997f)
        ball->segment.velocity.x = 0;
    if (__builtin_fabs(ball->segment.velocity.y) < 0.000099999997f)
        ball->segment.velocity.y = 0;
    if (ball->segment.position.y < ball->bottom) {
        ball->segment.position.y = ball->bottom;
        if (ball->segment.velocity.y < 0) {
            ball->segment.velocity.y = 0;
            gravity = 0;
        }
    }
    if (ball->segment.position.x > ball->right)
        ball->segment.position.x = ball->right;
    if (ball->segment.position.x < ball->left)
        ball->segment.position.x = ball->left;
    for (index = 0; index + 1 < ball->pocketCount; ++index) {
        minX = ball->dividers[index].x + ball->dividerWidth;
        maxX = ball->dividers[index].x;
        minY = ball->dividers[index].y;
        maxY = ball->dividers[index].y + ball->radius;
        if (ball->segment.position.x > minX && maxX > ball->segment.position.x
            && ball->segment.position.y > minY && maxY > ball->segment.position.y) {
            ball->segment.position.y = maxY;
        }
        minX = ball->dividers[index].x + ball->dividerWidth;
        maxX = (ball->dividers[index].x + ball->dividerWidth) + ball->radius;
        minY = ball->dividers[index].y - ball->dividerHeight;
        maxY = ball->dividers[index].y;
        if (ball->segment.position.x > minX && maxX > ball->segment.position.x
            && ball->segment.position.y > minY && maxY > ball->segment.position.y) {
            ball->segment.position.x = maxX;
        }
        minX = ball->dividers[index].x - ball->radius;
        maxX = ball->dividers[index].x;
        if (ball->segment.position.x > minX && maxX > ball->segment.position.x
            && ball->segment.position.y > minY && maxY > ball->segment.position.y) {
            ball->segment.position.x = minX;
        }
    }
    if (ball->option == 8) {
        for (index = 0; index < 6; ++index) {
            if (!ball->centerPockets[index].type)
                continue;
            minX = (ball->centerPockets[index].x - ball->centerPockets[index].halfWidth);
            maxX = (ball->centerPockets[index].x - ball->centerPockets[index].halfWidth) - ball->dividerWidth;
            minY = (ball->centerPockets[index].y + ball->centerPockets[index].height);
            maxY = (ball->centerPockets[index].y + ball->centerPockets[index].height) + ball->radius;
            if (ball->segment.position.x > minX && maxX > ball->segment.position.x
                && ball->segment.position.y > minY && maxY > ball->segment.position.y) {
                ball->segment.position.y = maxY;
            }
            minX = (ball->centerPockets[index].x + ball->centerPockets[index].halfWidth);
            maxX = (ball->centerPockets[index].x + ball->centerPockets[index].halfWidth) + ball->dividerWidth;
            if (ball->segment.position.x > minX && maxX > ball->segment.position.x
                && ball->segment.position.y > minY && maxY > ball->segment.position.y) {
                ball->segment.position.y = maxY;
            }
            minX = (ball->centerPockets[index].x - ball->centerPockets[index].halfWidth);
            maxX = (ball->centerPockets[index].x - ball->centerPockets[index].halfWidth) + ball->radius;
            minY = ball->centerPockets[index].y;
            maxY = (ball->centerPockets[index].y + ball->centerPockets[index].height);
            if (ball->segment.position.x > minX && maxX > ball->segment.position.x
                && ball->segment.position.y > minY && maxY > ball->segment.position.y) {
                ball->segment.position.x = maxX;
            }
            minX = (ball->centerPockets[index].x + ball->centerPockets[index].halfWidth);
            maxX = (ball->centerPockets[index].x + ball->centerPockets[index].halfWidth) - ball->radius;
            if (ball->segment.position.x > minX && maxX > ball->segment.position.x
                && ball->segment.position.y > minY && maxY > ball->segment.position.y) {
                ball->segment.position.x = maxX;
            }
            minX = ((ball->centerPockets[index].x - ball->centerPockets[index].halfWidth) - ball->dividerWidth) - ball->radius;
            maxX = ((ball->centerPockets[index].x - ball->centerPockets[index].halfWidth) - ball->dividerWidth);
            minY = (ball->centerPockets[index].y - ball->dividerWidth);
            if (ball->segment.position.x > minX && maxX > ball->segment.position.x
                && ball->segment.position.y > minY && maxY > ball->segment.position.y) {
                ball->segment.position.x = minX;
            }
            minX = ((ball->centerPockets[index].x + ball->centerPockets[index].halfWidth) + ball->dividerWidth) + ball->radius;
            maxX = ((ball->centerPockets[index].x + ball->centerPockets[index].halfWidth) + ball->dividerWidth);
            if (ball->segment.position.x > minX && maxX > ball->segment.position.x
                && ball->segment.position.y > minY && maxY > ball->segment.position.y) {
                ball->segment.position.x = minX;
            }
            minX = (ball->centerPockets[index].x - ball->centerPockets[index].halfWidth);
            maxX = (ball->centerPockets[index].x + ball->centerPockets[index].halfWidth);
            minY = ball->centerPockets[index].y;
            maxY = ball->centerPockets[index].y + ball->radius;
            if (ball->segment.position.x > minX && maxX > ball->segment.position.x
                && ball->segment.position.y > minY && maxY > ball->segment.position.y) {
                ball->segment.position.y = maxY;
                if (ball->segment.velocity.y < 0) {
                    ball->segment.velocity.y = 0;
                    gravity = 0;
                }
            }
            minX = ((ball->centerPockets[index].x - ball->centerPockets[index].halfWidth) - ball->dividerWidth);
            maxX = ((ball->centerPockets[index].x + ball->centerPockets[index].halfWidth) + ball->dividerWidth);
            minY = (ball->centerPockets[index].y - ball->dividerWidth) - ball->radius;
            maxY = (ball->centerPockets[index].y - ball->dividerWidth);
            if (ball->segment.position.x > minX && maxX > ball->segment.position.x
                && ball->segment.position.y > minY && maxY > ball->segment.position.y) {
                ball->segment.position.y = minY;
            }
        }
    }
    PatiPhysicsCopyVector(&ball->current.position, &ball->segment.position);
    PatiPhysicsCopyVector(&ball->current.velocity, &ball->segment.velocity);
    speedY = __builtin_fabs(ball->segment.velocity.y);
    if (speedY < 0.00000011920929f && ball->segment.position.y == ball->bottom) {
        gravity = 0;
        ball->soundFlags |= 8;
        ball->current.velocity.x = (1 - ball->surfaceFriction * ball->remainingTime) * ball->current.velocity.x;
    } else if (ball->option == 8 && __builtin_fabs(ball->segment.velocity.y) < 0.00000011920929f) {
        for (index = 0; index < 6; ++index) {
            if (ball->centerPockets[index].type && ball->segment.position.y == ball->centerPockets[index].y + ball->radius) {
                float minX, maxX;
                minX = ball->centerPockets[index].x - ball->centerPockets[index].halfWidth;
                maxX = ball->centerPockets[index].x + ball->centerPockets[index].halfWidth;
                if (ball->segment.position.x > minX && maxX > ball->segment.position.x) {
                    ball->soundFlags |= 8;
                    ball->current.velocity.x = (1 - ball->surfaceFriction * ball->remainingTime) * ball->current.velocity.x;
                    gravity = 0;
                }
            }
        }
    }
    if (gravity) {
        ball->current.velocity.y = ball->gravity * ball->remainingTime + ball->current.velocity.y;
        if (ball->segment.velocity.y > 0.001f && ball->current.velocity.y < -0.001f) {
            float next = ball->current.velocity.y;
            ball->turningFraction = next / (next - ball->segment.velocity.y);
            ball->current.velocity.y = 0;
        }
    }
    if (__builtin_fabs(ball->current.velocity.x) < 0.000099999997f)
        ball->current.velocity.x = 0;
    if (__builtin_fabs(ball->current.velocity.y) < 0.000099999997f)
        ball->current.velocity.y = 0;
    ball->current.position.x = ball->segment.velocity.x * ball->remainingTime * (1 - ball->turningFraction) + ball->current.position.x;
    ball->current.position.y = ball->segment.velocity.y * ball->remainingTime * (1 - ball->turningFraction) + ball->current.position.y;
}

void FindPatiCollision(PatiBallWork *ball) {
    FindPatiFloorCollision(ball);
    FindPatiLeftCollision(ball);
    FindPatiRightCollision(ball);
    FindPatiDividerCollision(ball);
    FindPatiCenterCollision(ball);
    FindPatiNailCollision(ball);
}

float PatiCrossingTime(float previous, float current, float edge) {
    edge -= current;
    previous -= current;
    return __builtin_fabs(previous) < 0.001f ? 0.0f : edge / previous;
}

float ProbePatiPoint(PatiBallWork *ball, const Vector3 *point) {
    Vector3 delta, from, to;
    float length, dot, c, discriminant, root, reciprocal, time;
    delta.x = ball->current.position.x - ball->segment.position.x;
    delta.y = ball->current.position.y - ball->segment.position.y;
    delta.z = 0;
    from.x = point->x - ball->segment.position.x;
    from.y = point->y - ball->segment.position.y;
    from.z = 0;
    to.x = point->x - ball->current.position.x;
    to.y = point->y - ball->current.position.y;
    to.z = 0;
    if (FTBL.GetVector3DotProduct(&ball->current.velocity, &from) <= 0)
        return -1;
    length = delta.x * delta.x + delta.y * delta.y;
    dot = to.x * delta.x + to.y * delta.y;
    c = to.x * to.x + to.y * to.y - ball->radius * ball->radius;
    discriminant = dot * dot - length * c;
    if (discriminant < 0)
        return -2;
    if (__builtin_fabs(length) < 0.00000011920929f)
        return -3;
    root = sqrt(discriminant);
    reciprocal = 1.0f / length;
    time = (-dot + root) * reciprocal;
    if (!(time < 0) && !(time > 1))
        return time;
    time = (-dot - root) * reciprocal;
    if (!(time < 0) && !(time > 1))
        return time;
    return -4;
}

void FindPatiFloorCollision(PatiBallWork *ball) {
    float time;
    if (ball->current.velocity.y < 0 && ball->current.position.y < ball->bottom) {
        time = PatiCrossingTime(ball->segment.position.y, ball->current.position.y, ball->bottom);
        if (time > ball->collisionTime) {
            ball->collisionTime = time;
            ball->collision = 1;
        }
    }
}

void FindPatiRightCollision(PatiBallWork *ball) {
    float time;
    if (ball->current.velocity.x > 0 && ball->current.position.x > ball->right) {
        time = PatiCrossingTime(ball->segment.position.x, ball->current.position.x, ball->right);
        if (time > ball->collisionTime) {
            ball->collisionTime = time;
            ball->collision = 2;
        }
    }
}

void FindPatiLeftCollision(PatiBallWork *ball) {
    float time;
    if (ball->current.velocity.x < 0 && ball->current.position.x < ball->left) {
        time = PatiCrossingTime(ball->segment.position.x, ball->current.position.x, ball->left);
        if (time > ball->collisionTime) {
            ball->collisionTime = time;
            ball->collision = 3;
        }
    }
}

void FindPatiDividerCollision(PatiBallWork *ball) {
    Vector3 divider, topRight, topLeft, rightTop, rightBottom, leftTop, leftBottom;
    short index;
    float time;
    for (index = 0; index + 1 < ball->pocketCount; ++index) {
        divider.x = ball->dividers[index].x;
        divider.y = ball->dividers[index].y;
        divider.z = ball->dividers[index].z;
        PatiPhysicsSetVector(&topRight, divider.x + ball->dividerWidth, divider.y + ball->radius, divider.z);
        PatiPhysicsSetVector(&topLeft, divider.x, divider.y + ball->radius, divider.z);
        if (ball->current.velocity.y < 0 && IntersectPatiSegments(&ball->current.position, &ball->segment.position, &topRight, &topLeft) == 1) {
            time = PatiCrossingTime(ball->segment.position.y, ball->current.position.y, topRight.y);
            if (time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 4;
                ball->collisionIndex = index;
            }
        }
        PatiPhysicsSetVector(&rightTop, divider.x + ball->dividerWidth + ball->radius, divider.y, divider.z);
        PatiPhysicsSetVector(&rightBottom, divider.x + ball->dividerWidth + ball->radius, divider.y - ball->dividerHeight, divider.z);
        if (ball->current.velocity.x < 0 && IntersectPatiSegments(&ball->current.position, &ball->segment.position, &rightTop, &rightBottom) == 1) {
            time = PatiCrossingTime(ball->segment.position.x, ball->current.position.x, rightTop.x);
            if (time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 5;
                ball->collisionIndex = index;
            }
        }
        PatiPhysicsSetVector(&leftTop, divider.x - ball->radius, divider.y, divider.z);
        PatiPhysicsSetVector(&leftBottom, divider.x - ball->radius, divider.y - ball->dividerHeight, divider.z);
        if (ball->current.velocity.x > 0 && IntersectPatiSegments(&ball->current.position, &ball->segment.position, &leftTop, &leftBottom) == 1) {
            time = PatiCrossingTime(ball->segment.position.x, ball->current.position.x, leftTop.x);
            if (time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 6;
                ball->collisionIndex = index;
            }
        }
        PatiPhysicsSetVector(&topRight, divider.x + ball->dividerWidth, divider.y, divider.z);
        time = ProbePatiPoint(ball, &topRight);
        if (time >= 0 && time > ball->collisionTime) {
            ball->collisionTime = time;
            ball->collision = 7;
            ball->collisionIndex = index;
        }
        PatiPhysicsSetVector(&topLeft, divider.x, divider.y, divider.z);
        time = ProbePatiPoint(ball, &topLeft);
        if (time >= 0 && time > ball->collisionTime) {
            ball->collisionTime = time;
            ball->collision = 8;
            ball->collisionIndex = index;
        }
    }
}

void FindPatiCenterCollision(PatiBallWork *ball) {
    Vector3 center, vertices[16];
    short index;
    float halfWidth, height, time;
    if ((short)ball->option != 8) return;
    for (index = 0; index < 6; ++index) {
        if (!ball->centerPockets[index].type) continue;
        center.x = ball->centerPockets[index].x;
        center.y = ball->centerPockets[index].y;
        center.z = 0;
        halfWidth = ball->centerPockets[index].halfWidth;
        height = ball->centerPockets[index].height;
        PatiPhysicsSetVector(&vertices[0], center.x - halfWidth, height + center.y + ball->radius, center.z);
        PatiPhysicsSetVector(&vertices[1], center.x - halfWidth - ball->dividerWidth, height + center.y + ball->radius, center.z);
        if (ball->current.velocity.y < 0 && IntersectPatiSegments(&ball->current.position, &ball->segment.position, &vertices[0], &vertices[1]) == 1) {
            time = PatiCrossingTime(ball->segment.position.y, ball->current.position.y, vertices[0].y);
            if (time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 9;
                ball->collisionIndex = index;
            }
        }
        PatiPhysicsSetVector(&vertices[2], center.x - halfWidth + ball->radius, center.y, center.z);
        PatiPhysicsSetVector(&vertices[3], center.x - halfWidth + ball->radius, height + center.y, center.z);
        if (ball->current.velocity.x < 0 && IntersectPatiSegments(&ball->current.position, &ball->segment.position, &vertices[2], &vertices[3]) == 1) {
            time = PatiCrossingTime(ball->segment.position.x, ball->current.position.x, vertices[2].x);
            if (time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 13;
                ball->collisionIndex = index;
            }
        }
        PatiPhysicsSetVector(&vertices[4], center.x - halfWidth - ball->dividerWidth - ball->radius, center.y - ball->dividerWidth, center.z);
        PatiPhysicsSetVector(&vertices[5], center.x - halfWidth - ball->dividerWidth - ball->radius, height + center.y, center.z);
        if (ball->current.velocity.x > 0 && IntersectPatiSegments(&ball->current.position, &ball->segment.position, &vertices[4], &vertices[5]) == 1) {
            time = PatiCrossingTime(ball->segment.position.x, ball->current.position.x, vertices[4].x);
            if (time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 12;
                ball->collisionIndex = index;
            }
        }
        PatiPhysicsSetVector(&vertices[6], center.x + halfWidth + ball->dividerWidth, height + center.y + ball->radius, center.z);
        PatiPhysicsSetVector(&vertices[7], center.x + halfWidth, height + center.y + ball->radius, center.z);
        if (ball->current.velocity.y < 0 && IntersectPatiSegments(&ball->current.position, &ball->segment.position, &vertices[6], &vertices[7]) == 1) {
            time = PatiCrossingTime(ball->segment.position.y, ball->current.position.y, vertices[6].y);
            if (time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 9;
                ball->collisionIndex = index;
            }
        }
        PatiPhysicsSetVector(&vertices[8], center.x + halfWidth + ball->dividerWidth + ball->radius, center.y - ball->dividerWidth, center.z);
        PatiPhysicsSetVector(&vertices[9], center.x + halfWidth + ball->dividerWidth + ball->radius, height + center.y, center.z);
        if (ball->current.velocity.x < 0 && IntersectPatiSegments(&ball->current.position, &ball->segment.position, &vertices[8], &vertices[9]) == 1) {
            time = PatiCrossingTime(ball->segment.position.x, ball->current.position.x, vertices[8].x);
            if (time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 13;
                ball->collisionIndex = index;
            }
        }
        PatiPhysicsSetVector(&vertices[10], center.x + halfWidth - ball->radius, center.y, center.z);
        PatiPhysicsSetVector(&vertices[11], center.x + halfWidth - ball->radius, height + center.y, center.z);
        if (ball->current.velocity.x > 0 && IntersectPatiSegments(&ball->current.position, &ball->segment.position, &vertices[10], &vertices[11]) == 1) {
            time = PatiCrossingTime(ball->segment.position.x, ball->current.position.x, vertices[10].x);
            if (time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 12;
                ball->collisionIndex = index;
            }
        }
        PatiPhysicsSetVector(&vertices[2], center.x - halfWidth, center.y + ball->radius, center.z);
        PatiPhysicsSetVector(&vertices[10], center.x + halfWidth, center.y + ball->radius, center.z);
        if (ball->current.velocity.y < 0 && IntersectPatiSegments(&ball->current.position, &ball->segment.position, &vertices[2], &vertices[10]) == 1) {
            time = PatiCrossingTime(ball->segment.position.y, ball->current.position.y, vertices[2].y);
            if (time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 10;
                ball->collisionIndex = index;
            }
        }
        PatiPhysicsSetVector(&vertices[4], center.x - halfWidth - ball->dividerWidth, center.y - ball->dividerWidth - ball->radius, center.z);
        PatiPhysicsSetVector(&vertices[8], center.x + halfWidth + ball->dividerWidth, center.y - ball->dividerWidth - ball->radius, center.z);
        if (ball->current.velocity.y < 0 && IntersectPatiSegments(&ball->current.position, &ball->segment.position, &vertices[4], &vertices[8]) == 1) {
            time = PatiCrossingTime(ball->segment.position.y, ball->current.position.y, vertices[4].y);
            if (time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 11;
                ball->collisionIndex = index;
            }
        }
        PatiPhysicsSetVector(&vertices[0], center.x - halfWidth, height + center.y, center.z);
        time = ProbePatiPoint(ball, &vertices[0]);
        if (time >= 0 && time > ball->collisionTime) {
            ball->collisionTime = time;
            ball->collision = 17;
            ball->collisionIndex = index;
        }
        PatiPhysicsSetVector(&vertices[1], center.x - halfWidth - ball->dividerWidth, height + center.y, center.z);
        time = ProbePatiPoint(ball, &vertices[1]);
        if (time >= 0 && time > ball->collisionTime) {
            ball->collisionTime = time;
            ball->collision = 16;
            ball->collisionIndex = index;
        }
        PatiPhysicsSetVector(&vertices[6], center.x + halfWidth + ball->dividerWidth, height + center.y, center.z);
        time = ProbePatiPoint(ball, &vertices[6]);
        if (time >= 0 && time > ball->collisionTime) {
            ball->collisionTime = time;
            ball->collision = 19;
            ball->collisionIndex = index;
        }
        PatiPhysicsSetVector(&vertices[7], center.x + halfWidth, height + center.y, center.z);
        time = ProbePatiPoint(ball, &vertices[7]);
        if (time >= 0 && time > ball->collisionTime) {
            ball->collisionTime = time;
            ball->collision = 18;
            ball->collisionIndex = index;
        }
        PatiPhysicsSetVector(&vertices[4], center.x - halfWidth - ball->dividerWidth, center.y - ball->dividerWidth, center.z);
        time = ProbePatiPoint(ball, &vertices[4]);
        if (time >= 0 && time > ball->collisionTime) {
            ball->collisionTime = time;
            ball->collision = 14;
            ball->collisionIndex = index;
        }
        PatiPhysicsSetVector(&vertices[8], center.x + halfWidth + ball->dividerWidth, center.y - ball->dividerWidth, center.z);
        time = ProbePatiPoint(ball, &vertices[8]);
        if (time >= 0 && time > ball->collisionTime) {
            ball->collisionTime = time;
            ball->collision = 15;
            ball->collisionIndex = index;
        }
    }
}

void FindPatiNailCollision(PatiBallWork *ball) {
    Vector3 point;
    unsigned short index;
    float time;
    for (index = 0; index < 512; ++index) {
        point.x = ball->nails->nails[index].position.x;
        point.y = ball->nails->nails[index].position.y;
        point.z = ball->nails->nails[index].position.z;
        if (point.y > 0) {
            time = ProbePatiPoint(ball, &point);
            if (time >= 0 && time > ball->collisionTime) {
                ball->collisionTime = time;
                ball->collision = 20;
                ball->collisionIndex = index;
            }
        }
    }
}

unsigned char IntersectPatiSegments(const Vector3 *current, const Vector3 *previous, const Vector3 *first, const Vector3 *second) {
    float cross[4];
    cross[0] = (first->x - current->x) * (previous->y - first->y) - (first->y - current->y) * (previous->x - first->x);
    cross[1] = (previous->x - first->x) * (second->y - previous->y) - (previous->y - first->y) * (second->x - previous->x);
    cross[2] = (second->x - previous->x) * (current->y - second->y) - (second->y - previous->y) * (current->x - second->x);
    cross[3] = (current->x - second->x) * (first->y - current->y) - (current->y - second->y) * (first->x - current->x);
    if (cross[0] >= 0) {
        if (cross[1] >= 0 && cross[2] >= 0 && cross[3] >= 0) return 1;
        return 2;
    } else {
        if (cross[1] < 0 && cross[2] < 0 && cross[3] < 0) return 1;
        return 2;
    }
}

void MarkPatiCollisionSound(PatiBallWork *ball) {
    int collision = ball->collision;
    if (!collision)
        return;
    switch (collision) {
    case 1:
    case 10:
        if (__builtin_fabs(ball->segment.velocity.y) > 0)
            ball->soundFlags |= 1;
        break;
    case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
    case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19:
        ball->soundFlags |= 4;
        break;
    case 20:
        ball->soundFlags |= 2;
        break;
    default:
        FTBL.EventAssertNullsub(__FILE__, 878);
        break;
    }
}

void PlayPatiCollisionSounds(PatiBallWork *ball) {
    if (ball->soundFlags & 1) PlayPatiCollisionSound(ball, 0);
    if (ball->soundFlags & 2) PlayPatiCollisionSound(ball, 1);
    if (ball->soundFlags & 4) PlayPatiCollisionSound(ball, 2);
    if (ball->soundFlags & 8) PlayPatiCollisionSound(ball, 3);
}

void ResolvePatiCollision(PatiBallWork *ball) {
    float remainder;
    if (!ball->collision)
        return;
    PatiPhysicsCopyVector(&ball->beforeCollision.position, &ball->current.position);
    PatiPhysicsCopyVector(&ball->beforeCollision.velocity, &ball->current.velocity);
    remainder = 1.0f - ball->collisionTime;
    ball->current.position.x = ball->segment.position.x * ball->collisionTime + remainder * ball->segment.position.x;
    ball->current.position.y = ball->segment.position.y * ball->collisionTime + remainder * ball->segment.position.y;
    ball->current.velocity.x = ball->segment.velocity.x * ball->collisionTime + remainder * ball->segment.velocity.x;
    ball->current.velocity.y = ball->segment.velocity.y * ball->collisionTime + remainder * ball->segment.velocity.y;
    switch (ball->collision) {
    case 1: BouncePatiFloor(ball); break;
    case 2: BouncePatiLeft(ball); break;
    case 3: BouncePatiRight(ball); break;
    case 4: BouncePatiDivider(ball); break;
    case 5: BouncePatiDividerLeft(ball); break;
    case 6: BouncePatiDividerRight(ball); break;
    case 7: BouncePatiDividerRightPoint(ball); break;
    case 8: BouncePatiDividerLeftPoint(ball); break;
    case 9: BouncePatiPocketFloor(ball); break;
    case 10: BouncePatiCenterFloor(ball); break;
    case 11: BouncePatiCenterTop(ball); break;
    case 13: BouncePatiCenterLeft(ball); break;
    case 12: BouncePatiCenterRight(ball); break;
    case 14: BouncePatiCenterLowerLeft(ball); break;
    case 15: BouncePatiCenterLowerRight(ball); break;
    case 16: BouncePatiCenterOuterUpperLeft(ball); break;
    case 17: BouncePatiCenterUpperLeft(ball); break;
    case 18: BouncePatiCenterUpperRight(ball); break;
    case 19: BouncePatiCenterOuterUpperRight(ball); break;
    case 20: BouncePatiNail(ball); break;
    default:
        FTBL.EventAssertNullsub(__FILE__,975);
        break;
    }
}

void BouncePatiFloor(PatiBallWork *ball) {
    ball->current.velocity.x *= ball->tangentialRetention;
    ball->current.velocity.y *= -ball->wallRestitution;
    if (ball->current.velocity.y <= 0 || (ball->current.velocity.y > 0 && ball->current.velocity.y + ball->gravity < 0)) {
        ball->current.position.y = ball->bottom;
        ball->current.velocity.y = ball->segment.velocity.y = 0;
    }
    if (ball->current.position.y < ball->bottom)
        ball->current.position.y = ball->bottom;
}

void BouncePatiLeft(PatiBallWork *ball) {
    ball->current.velocity.x = -ball->wallRestitution * ball->segment.velocity.x;
}

void BouncePatiRight(PatiBallWork *ball) {
    ball->current.velocity.x = -ball->wallRestitution * ball->segment.velocity.x;
}

void BouncePatiDivider(PatiBallWork *ball) {
    ball->current.velocity.x *= ball->tangentialRetention;
    ball->current.velocity.y *= -ball->wallRestitution;
    if (__builtin_fabs(ball->current.velocity.x) < 0.001f) {
        ball->current.velocity.x = ball->current.velocity.y * 0.03125f;
        if (__builtin_fabs(ball->current.velocity.x) < 0.001f)
            ball->current.velocity.x = ball->current.velocity.x > 0 ? 0.001f : -0.001f;
    }
}

void BouncePatiDividerLeft(PatiBallWork *ball) {
    ball->current.velocity.x = -ball->wallRestitution * ball->current.velocity.x;
}

void BouncePatiDividerRight(PatiBallWork *ball) {
    ball->current.velocity.x = -ball->wallRestitution * ball->current.velocity.x;
}

void BouncePatiDividerRightPoint(PatiBallWork *ball) {
    Vector3 divider, point;
    divider.x = ball->dividers[ball->collisionIndex].x;
    divider.y = ball->dividers[ball->collisionIndex].y;
    divider.z = ball->dividers[ball->collisionIndex].z;
    PatiPhysicsSetVector(&point, divider.x + ball->dividerWidth, divider.y, 0);
    BouncePatiPoint(ball, &point, ball->wallRestitution);
}

void BouncePatiDividerLeftPoint(PatiBallWork *ball) {
    Vector3 divider, point;
    divider.x = ball->dividers[ball->collisionIndex].x;
    divider.y = ball->dividers[ball->collisionIndex].y;
    divider.z = ball->dividers[ball->collisionIndex].z;
    PatiPhysicsSetVector(&point, divider.x, divider.y, 0);
    BouncePatiPoint(ball, &point, ball->wallRestitution);
}

void BouncePatiPocketFloor(PatiBallWork *ball) {
    ball->current.velocity.x *= ball->tangentialRetention;
    ball->current.velocity.y *= -ball->wallRestitution;
    if (__builtin_fabs(ball->current.velocity.x) < 0.001f) {
        ball->current.velocity.x = ball->current.velocity.y * 0.03125f;
        if (__builtin_fabs(ball->current.velocity.x) < 0.001f)
            ball->current.velocity.x = ball->current.velocity.x > 0 ? 0.001f : -0.001f;
    }
}

void BouncePatiCenterFloor(PatiBallWork *ball) {
    ball->current.velocity.x *= ball->tangentialRetention;
    ball->current.velocity.y *= -ball->wallRestitution;
    if (ball->current.velocity.y <= 0 || (ball->current.velocity.y > 0 && ball->current.velocity.y + ball->gravity < 0)) {
        ball->current.position.y = ball->centerPockets[ball->collisionIndex].y + ball->radius;
        ball->current.velocity.y = ball->segment.velocity.y = 0;
    }
    if (ball->current.position.y < ball->centerPockets[ball->collisionIndex].y + ball->radius)
        ball->current.position.y = ball->centerPockets[ball->collisionIndex].y + ball->radius;
}

void BouncePatiCenterTop(PatiBallWork *ball) {
    ball->current.velocity.x *= ball->tangentialRetention;
    ball->current.velocity.y = -ball->wallRestitution * ball->current.velocity.y;
}

void BouncePatiCenterLeft(PatiBallWork *ball) {
    ball->current.velocity.x = -ball->wallRestitution * ball->current.velocity.x;
}

void BouncePatiCenterRight(PatiBallWork *ball) {
    ball->current.velocity.x = -ball->wallRestitution * ball->current.velocity.x;
}

void BouncePatiCenterLowerLeft(PatiBallWork *ball) {
    Vector3 center, point;
    float halfWidth, height;
    center.x = ball->centerPockets[ball->collisionIndex].x;
    center.y = ball->centerPockets[ball->collisionIndex].y;
    center.z = 0;
    halfWidth = ball->centerPockets[ball->collisionIndex].halfWidth;
    PatiPhysicsSetVector(&point, center.x - halfWidth - ball->dividerWidth,
        center.y - ball->dividerWidth, 0);
    BouncePatiPoint(ball, &point, ball->wallRestitution);
}

void BouncePatiCenterLowerRight(PatiBallWork *ball) {
    Vector3 center, point;
    float halfWidth, height;
    center.x = ball->centerPockets[ball->collisionIndex].x;
    center.y = ball->centerPockets[ball->collisionIndex].y;
    center.z = 0;
    halfWidth = ball->centerPockets[ball->collisionIndex].halfWidth;
    PatiPhysicsSetVector(&point, center.x + halfWidth + ball->dividerWidth,
        center.y - ball->dividerWidth, 0);
    BouncePatiPoint(ball, &point, ball->wallRestitution);
}

void BouncePatiCenterOuterUpperLeft(PatiBallWork *ball) {
    Vector3 center, point;
    float halfWidth, height;
    center.x = ball->centerPockets[ball->collisionIndex].x;
    center.y = ball->centerPockets[ball->collisionIndex].y;
    center.z = 0;
    halfWidth = ball->centerPockets[ball->collisionIndex].halfWidth;
    height = ball->centerPockets[ball->collisionIndex].height;
    PatiPhysicsSetVector(&point, center.x - halfWidth - ball->dividerWidth,
        center.y + height, 0);
    BouncePatiPoint(ball, &point, ball->wallRestitution);
}

void BouncePatiCenterUpperLeft(PatiBallWork *ball) {
    Vector3 center, point;
    float halfWidth, height;
    center.x = ball->centerPockets[ball->collisionIndex].x;
    center.y = ball->centerPockets[ball->collisionIndex].y;
    center.z = 0;
    halfWidth = ball->centerPockets[ball->collisionIndex].halfWidth;
    height = ball->centerPockets[ball->collisionIndex].height;
    PatiPhysicsSetVector(&point, center.x - halfWidth,
        center.y + height, 0);
    BouncePatiPoint(ball, &point, ball->wallRestitution);
}

void BouncePatiCenterUpperRight(PatiBallWork *ball) {
    Vector3 center, point;
    float halfWidth, height;
    center.x = ball->centerPockets[ball->collisionIndex].x;
    center.y = ball->centerPockets[ball->collisionIndex].y;
    center.z = 0;
    halfWidth = ball->centerPockets[ball->collisionIndex].halfWidth;
    height = ball->centerPockets[ball->collisionIndex].height;
    PatiPhysicsSetVector(&point, center.x + halfWidth,
        center.y + height, 0);
    BouncePatiPoint(ball, &point, ball->wallRestitution);
}

void BouncePatiCenterOuterUpperRight(PatiBallWork *ball) {
    Vector3 center, point;
    float halfWidth, height;
    center.x = ball->centerPockets[ball->collisionIndex].x;
    center.y = ball->centerPockets[ball->collisionIndex].y;
    center.z = 0;
    halfWidth = ball->centerPockets[ball->collisionIndex].halfWidth;
    height = ball->centerPockets[ball->collisionIndex].height;
    PatiPhysicsSetVector(&point, center.x + halfWidth + ball->dividerWidth,
        center.y + height, 0);
    BouncePatiPoint(ball, &point, ball->wallRestitution);
}

void BouncePatiNail(PatiBallWork *ball) {
    unsigned int oldest = 0;
    int oldestIndex = 0, index;
    Vector3 point;
    for (index = 0; index < 8; ++index) {
        if (ball->contacts[index].age < 99999)
            ++ball->contacts[index].age;
        if (ball->contacts[index].age >= oldest) {
            oldest = ball->contacts[index].age;
            oldestIndex = index;
        }
    }
    index = 0;
    while (index < 8 && ball->contacts[index].nail != ball->collisionIndex)
        ++index;
    if (index < 8) {
        ball->contacts[index].age = 0;
        ++ball->contacts[index].count;
    } else {
        index = oldestIndex;
        ball->contacts[index].nail = ball->collisionIndex;
        ball->contacts[index].age = 0;
        ball->contacts[index].count = 1;
    }
    point.x = ball->nails->nails[ball->collisionIndex].position.x;
    point.y = ball->nails->nails[ball->collisionIndex].position.y;
    point.z = 0;
    BouncePatiPoint(ball, &point, ball->nailRestitution);
}

void BouncePatiPoint(PatiBallWork *ball, const Vector3 *point, float restitution) {
    Vector3 normal;
    float impulse, y;
    normal.x = point->x - ball->current.position.x;
    normal.y = point->y - ball->current.position.y;
    normal.z = 0;
    FTBL.Vector3_NormalizeInPlace(&normal);
    impulse = (restitution + 1) * FTBL.GetVector3DotProduct(&normal, &ball->current.velocity);
    ball->current.velocity.x -= impulse * normal.x;
    y = ball->current.velocity.y - impulse * normal.y;
    ball->current.velocity.y = y;
    if (ball->current.position.y > point->y && __builtin_fabs(ball->current.position.x - point->x) < 0.0049999999f
        && __builtin_fabs(ball->current.velocity.x) < 0.001f && __builtin_fabs(ball->segment.velocity.x) < 0.001f) {
        ball->current.velocity.x = (__builtin_fabs(y) * (ball->current.position.x > point->x ? 1 : -1)) * 0.03125f;
    }
    if (ball->current.position.y > point->y && __builtin_fabs(ball->current.velocity.x) < 0.001f
        && __builtin_fabs(ball->current.velocity.y) < 0.0050000004f)
        ball->current.velocity.x = ball->current.position.x > point->x ? 0.001f : -0.001f;
}

void UpdatePatiBallSettled(PatiBallWork *ball) {
    if (IsPatiBallSettled(ball)) {
        if (!ball->settled)
            ball->settled = 1;
    }
}

int IsPatiBallSettled(PatiBallWork *ball) {
    int window = 45;
    Vector3 difference;
    float movement, distance;
    unsigned int contacts;
    int index;
    if ((float)ball->frames++ > 2700.0f)
        return 1;
    if (window > 90)
        window = 90;
    difference.x = ball->history[ball->historyCursor].x - ball->current.position.x;
    difference.y = ball->history[ball->historyCursor].y - ball->current.position.y;
    difference.z = 0;
    movement = FTBL.GetVector3DotProduct(&difference, &difference);
    if (++ball->historyCursor >= window)
        ball->historyCursor = 0;
    difference.x = ball->history[ball->historyCursor].x - ball->current.position.x;
    difference.y = ball->history[ball->historyCursor].y - ball->current.position.y;
    difference.z = 0;
    distance = FTBL.GetVector3DotProduct(&difference, &difference);
    ball->history[ball->historyCursor].x = ball->current.position.x;
    ball->history[ball->historyCursor].y = ball->current.position.y;
    ball->movement[ball->historyCursor] = movement;
    movement = 0;
    for (index = 0; index < window; ++index)
        movement += ball->movement[index];
    if (movement < 0.000045000001f && distance < 0.0000030000001f)
        return 1;
    contacts = 0;
    for (index = 0; index < 8; ++index) {
        if (ball->contacts[index].count > contacts)
            contacts = ball->contacts[index].count;
    }
    if (contacts > 20) return 1;
    return 0;
}

void PlayPatiCollisionSound(PatiBallWork *ball, short material) {
    Vector3 origin, velocity;
    int sound;
    short period;
    float loud, quiet;
    PatiPhysicsSetVector(&origin, 0, 0, 0);
    PatiPhysicsCopyVector(&velocity, &ball->current.velocity);
    switch (material) {
    case 0: quiet = 0; sound = 283; loud = 0.0049999999f; period = 5; break;
    case 1: quiet = 0; sound = 289; loud = 0.0099999998f; period = 2; break;
    case 2: sound = 293; loud = 0.0080000004f; quiet = 0.0049999999f; period = 5; break;
    default: sound = 295; quiet = loud = 0; period = 60; break;
    }
    if (ball->previousSound != sound)
        ball->soundFrames = 0;
    if (ball->soundFrames >= period)
        ball->soundFrames = 0;
    if (ball->soundFrames < period) {
        if (ball->soundFrames <= 0) {
            if (__builtin_fabs(FTBL.GetSquaredDistanceXZ(&origin, &velocity)) > loud)
                FTBL.Loop3DSoundEffect(sound);
            else if (__builtin_fabs(FTBL.GetSquaredDistanceXZ(&origin, &velocity)) > quiet)
                FTBL.Loop3DSoundEffect(sound + 1);
            else
                FTBL.Loop3DSoundEffect(sound + 2);
        }
        ball->previousSound = sound;
    }
}

void FollowPatiBall(PatiBallWork *ball) {


    struct {
        Vector3 position, current;
        Vector3i rotation;
        unsigned char retained[12];
    } frame;
    FTBL.GetCharaPosition(SHEN_NAME4('C','M','D','M'), -1, &frame.current, 0);
    if (ball->current.position.y > ball->top - 0.13f) {
        if (frame.current.y > ball->top - 0.13f || frame.current.y < 0.25999999f)
            PatiPhysicsSetVector(&frame.position, 0, ball->top - 0.13f, 0);
        else
            PatiPhysicsSetVector(&frame.position, 0, frame.current.y, 0);
    } else if (ball->current.position.y > 0.25999999f && ball->top - 0.13f > ball->current.position.y) {
        if (ball->current.position.y > frame.current.y)
            PatiPhysicsSetVector(&frame.position, 0, frame.current.y, 0);
        else
            PatiPhysicsSetVector(&frame.position, 0, ball->current.position.y, 0);
    } else
        PatiPhysicsSetVector(&frame.position, 0, 0.25999999f, 0);
    {
        Vector3i *rotation = &frame.rotation;
        rotation->x = 0;
        rotation->y = ball->boardRotation.y;
        rotation->z = 0;
        FTBL.EV_AT_SetPos(SHEN_NAME4('C','M','D','M'), 0x38000000, &frame.position);
        FTBL.EV_AT_SetAngY_0(SHEN_NAME4('C','M','D','M'), 0x78000000, rotation, 0);
        switch ((short)ball->boardSize) {
        case 3: FTBL.EVS_LoopCamera(994, SHEN_NAME4('C','M','D','M'), SHEN_NAME4('C','M','D','M')); break;
        case 4: FTBL.EVS_LoopCamera(997, SHEN_NAME4('C','M','D','M'), SHEN_NAME4('C','M','D','M')); break;
        default: FTBL.EVS_LoopCamera(991, SHEN_NAME4('C','M','D','M'), SHEN_NAME4('C','M','D','M')); break;
        }
    }
}

void PatiPhysicsCopyVector(Vector3 *target, const Vector3 *source) {
    target->x = source->x;
    target->y = source->y;
    target->z = source->z;
}

void PatiPhysicsSetVector(Vector3 *target, float x, float y, float z) {
    target->x = x;
    target->y = y;
    target->z = z;
}

#endif
