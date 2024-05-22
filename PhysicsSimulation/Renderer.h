#pragma once
#define PI 3.14159265359f
#include <GameObject.h>
#include <MonoBehaviour.h>
#include <SFML/Graphics.hpp>
#include <Vector2D.h>
#include <iostream>
#include <BoxCollider2D.h>
#include <Contact2D.h>
extern sf::RenderWindow window;
class ContactDebug : public MonoBehaviour
{
public:
    virtual void start() override
    {
        _collider = this->gameObject()->getComponent<Collider2D>();
    }
    virtual void update() override
    {
        const std::vector<Collision2D*> collisions = _collider->collisions();
        for (int i = 0; i < collisions.size(); ++i)
        {
            const std::vector<Contact2D*> contacts = collisions[i]->contacts();
            for (int j = 0; j < contacts.size(); ++j)
            {
                const Contact2D& c = *(contacts[j]);
                {
                    sf::CircleShape point(0.2f);
                    point.setOrigin({ 0.2f,0.2f });
                    point.setFillColor(sf::Color::Red);
                    point.setPosition({ c.contactA().x(), c.contactA().y() });
                    window.draw(point);
                }
                {
                    sf::CircleShape point(0.2f);
                    point.setOrigin({ 0.2f,0.2f });
                    point.setFillColor(sf::Color::Blue);
                    point.setPosition({ c.contactB().x(), c.contactB().y() });
                    window.draw(point);
                }
                //sf::Vertex point({ c.contactA().x(), c.contactA().y() }, sf::Color::White);
                //window.draw(&point, 1, sf::Points);
            }
        }
    }
private:
    virtual Component* clone() override
    {
        return new ContactDebug();
    }

    Collider2D* _collider = nullptr;
};
class Renderder : public MonoBehaviour
{
public:
    Renderder(sf::Shape* shape, const Vector2D& origin) : _shape(shape)
    {
        _shape->setOrigin({ origin.x(), origin.y() });
    }
    void fillColor(const sf::Color& color)
    {
        _shape->setFillColor(color);
    }
    void outlineColor(const sf::Color& color)
    {
        _shape->setOutlineColor(color);
    }
    void outlineThickness(float t)
    {
        _shape->setOutlineThickness(t);
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