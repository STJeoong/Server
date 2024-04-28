#include <iostream>
#include <SFML/Graphics.hpp>
class Body {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
    float invMass;

    Body(sf::Vector2f pos, float m = 1.0f) : position(pos), mass(m) {
        velocity = sf::Vector2f(0.0f, 0.0f);
        if (m == 0.0f)
            invMass = FLT_MAX;
        else if (m == FLT_MAX)
            invMass = 0.0f;
        else
            invMass = 1 / mass;
    }

    void applyForce(sf::Vector2f force) {
        sf::Vector2f acceleration = force * invMass;
        velocity += acceleration;
    }

    void update(float dt) {
        position += velocity * dt;
    }

    void draw(sf::RenderWindow& window) {
        sf::CircleShape shape(10.0f);
        sf::RectangleShape s2({10.0f, 10.0f});
        s2.setPosition(position);
        s2.setFillColor(sf::Color::Red);
        window.draw(s2);
    }
};
#include <GameObject.h>
#include <BoxCollider2D.h>
class A;
class B
{
public:
    void append(A* a) { v.push_back(a); }
private:
    std::vector<A*> v;
};
B b;
class A
{
public:
    A()
    {
        b.append(this);
    }
};
int main()
{
    GameObject* g1 = GameObject::instantiate();
    GameObject* g2 = GameObject::instantiate(nullptr, g1);
    GameObject* g3 = GameObject::instantiate(nullptr, g2);
    g1->transform({ 400.0f, 300.0f });
    g2->transform({100.0f, 100.0f});
    g3->transform({ 50.0f, 0.0f });
    sf::CircleShape shape1(10.0f);
    sf::CircleShape shape2(10.0f);
    sf::CircleShape shape3(10.0f);

	sf::RenderWindow window(sf::VideoMode(800, 600), "2D Physics Engine");
    Body body(sf::Vector2f(400.0f, 300.0f));
    sf::Clock clock;
    float dt = 0.0f;
	while (window.isOpen())
	{
		sf::Event evt;
		while (window.pollEvent(evt))
		{
			if (evt.type == sf::Event::Closed)
				window.close();
		}

        dt = clock.restart().asSeconds();

        // 중력 적용
        //body.applyForce(sf::Vector2f(0.0f, 0.098f * body.mass));

        Motion motion({ 0, 0 }, 0.001f);
        Motion motion2({ -100.0f*dt, -100.0f*dt }, 0.01f);
        g1->transform(motion);
        g2->transform(motion2);
        printf("g1:(x : %f, y: %f), g2:(x: %f, y: %f)\n", g1->transform().position().x(), g1->transform().position().y(),
                                                        g2->transform().position().x(), g2->transform().position().y());
        // 업데이트
        //body.update(dt);

        // 화면 그리기
        window.clear(sf::Color::Black);
        shape1.setPosition({g1->transform().position().x(), g1->transform().position().y()});
        shape2.setPosition({g2->transform().position().x(), g2->transform().position().y()});
        shape3.setPosition({g3->transform().position().x(), g3->transform().position().y()});
        shape1.setFillColor(sf::Color::Red);
        shape2.setFillColor(sf::Color::Blue);
        shape3.setFillColor(sf::Color::Yellow);
        window.draw(shape1);
        window.draw(shape2);
        window.draw(shape3);
        //body.draw(window);
        window.display();
	}
	return 0;
}