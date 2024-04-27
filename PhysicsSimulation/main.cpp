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
#include <World.h>
#include <BoxCollider2D.h>
int main()
{
    //obj->~MyClass();
    //delete collider;
	//sf::RenderWindow window(sf::VideoMode(800, 600), "2D Physics Engine");
 //   Body body(sf::Vector2f(400.0f, 300.0f));
 //   sf::Clock clock;
 //   float dt = 0.0f;
	//while (window.isOpen())
	//{
	//	sf::Event evt;
	//	while (window.pollEvent(evt))
	//	{
	//		if (evt.type == sf::Event::Closed)
	//			window.close();
	//	}

 //       dt = clock.restart().asSeconds();

 //       // 중력 적용
 //       body.applyForce(sf::Vector2f(0.0f, 0.98f * body.mass));

 //       // 업데이트
 //       printf("dt : %f\n", dt);
 //       body.update(dt);

 //       // 화면 그리기
 //       window.clear(sf::Color::Black);
 //       body.draw(window);
 //       window.display();
	//}
	return 0;
}