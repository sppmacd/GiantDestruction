#include "Player.h"
#include "Settings.h"
#include "config.hpp"

Player::Player(b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.awake = true;
    bodyDef.active = true;
    bodyDef.userData = this;
    bodyDef.position = b2Vec2(0.f, 0.f);
    bodyDef.type = b2_dynamicBody;

    b2CircleShape head;
    b2PolygonShape base,rhand,lhand,rleg,lleg;
    head.m_p = b2Vec2(0.f, 1.73f);
    head.m_radius = 0.27f;
    base.SetAsBox(0.44f,0.74f, b2Vec2(0.50f,1.08f), 0.f);
    lhand.SetAsBox(0.26f, 0.74f, b2Vec2(0.86f,1.08f),0.f);
    rhand.SetAsBox(1.74f, 0.74f, b2Vec2(0.14f,1.08f),0.f);
    lleg.SetAsBox(0.21f, 0.69f, b2Vec2(0.38f, 0.36f), 0.f);
    rleg.SetAsBox(0.79f, 0.69f, b2Vec2(0.62f, 0.36f), 0.f);

    b2FixtureDef fixHead,fixBase,fixLHand,fixRHand,fixLLeg,fixRLeg;
    fixHead.shape = &head;
    fixBase.shape = &base;
    fixLHand.shape = &lhand;
    fixRHand.shape = &rhand;
    fixLLeg.shape = &lleg;
    fixRLeg.shape = &rleg;

    b2Body* handler = world->CreateBody(&bodyDef);
    handler->CreateFixture(&fixHead);
    handler->CreateFixture(&fixBase);
    handler->CreateFixture(&fixLHand);
    handler->CreateFixture(&fixRHand);
    handler->CreateFixture(&fixLLeg);
    handler->CreateFixture(&fixRLeg);

    b2Handler = handler;
}

Vector2f Player::getScreenPosition() //inverts Y!
{
    return Vector2f(0.f,ScreenSettings::windowSize.y) - Vector2f(b2Handler->GetPosition().x, b2Handler->GetPosition().y)*ScreenSettings::getBlockSize();
}
