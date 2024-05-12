#pragma once
#define PI 3.14159265359f
#include <GameObject.h>
#include <MonoBehaviour.h>
#include <SFML/Graphics.hpp>
#include <Vector2D.h>
#include <iostream>
extern sf::RenderWindow window;
class Renderder : public MonoBehaviour
{
public:
    Renderder(sf::Shape* shape, const Vector2D& origin, const sf::Color& color) : _shape(shape)
    {
        _shape->setOrigin({ origin.x(), origin.y() });
        _shape->setFillColor(color);
    }
protected:
    virtual void update() override
    {
        _shape->setPosition({ this->gameObject()->transform().position().x(), this->gameObject()->transform().position().y() });
        _shape->setRotation(this->gameObject()->transform().rotation().radian() * 180 / PI);
        window.draw(*_shape);
    }
private:
    Renderder() = default;
    virtual ~Renderder() = default;
    virtual Component* clone() override { return new Renderder(); }
    sf::Shape* _shape = nullptr;
};