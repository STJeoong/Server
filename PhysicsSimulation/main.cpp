#include <iostream>
#include <SFML/Graphics.hpp>
#include <Simplex.h>
#include <GameObject.h>
#include <queue>
#include <World.h>
#include <BoxCollider2D.h>
#include <Line.h>
#include <tuple>
#include <RigidBody2D.h>
#include <CircleCollider.h>
#include <MonoBehaviour.h>
class Test : public MonoBehaviour
{
    friend class GameObject;
public:
    virtual void start() override
    {
        //std::cout << "start\n";
    }
    virtual void update() override
    {
        //std::cout << "update\n";
        //this->enabled(true);
    }
    virtual void onCollisionEnter(const Collision2D& collision) override
    {
        //std::cout << "collision enter!\n";
    }
    virtual void onCollisionExit(const Collision2D& collision) override
    {
        //std::cout << "collision exit!\n";
    }
    virtual void onCollisionStay(const Collision2D& collision) override
    {
        //std::cout << "collision stay!\n";
    }
private:
    Test() = default;
    ~Test() = default;
    virtual Component* clone() override
    {
        return new Test();
    }
};
#define PI 3.14159265359f
int main()
{
    std::ios_base::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    GameObject* g = World::instantiate();
    RigidBody2D* ret = g->addComponent<RigidBody2D>();
    RigidBody2D* ret2 = g->addComponent<RigidBody2D>();
    RigidBody2D* ret3 = g->addComponent<RigidBody2D>();
    BoxCollider2D* ret6 = g->addComponent<BoxCollider2D>();
    g->removeComponent(ret6);
    g->removeComponent(ret);
    RigidBody2D* ret4 = g->addComponent<RigidBody2D>();
    RigidBody2D* ret5 = g->addComponent<RigidBody2D>();
    /*RigidBody2D* rigid = g->addComponent<RigidBody2D>();
    RigidBody2D* rigid2 = g->addComponent<RigidBody2D>();*/
 //   GameObject* g1 = World::instantiate();
 //   GameObject* g2 = World::instantiate();
 //   GameObject* g3 = World::instantiate(); // floor

 //   S_BoxDef def;
 //   def.halfSize = { 100.0f, 10.0f };
 //   BoxCollider2D* c3 = g3->addComponent<BoxCollider2D>(def);

 //   S_BoxDef def2;
 //   def2.halfSize = { 10.0f, 10.0f };
 //   def2.friction = 0.01f;
 //   //def2.bounciness = 0.8f;
 //   RigidBody2D* rigid1 = g1->addComponent<RigidBody2D>();
 //   BoxCollider2D* c1 = g1->addComponent<BoxCollider2D>(def2);
 //   g1->addComponent<Test>();

 //   /*S_BoxDef def3;
 //   def3.halfSize = { 10.0f, 10.0f };*/
 //   S_CircleDef circleDef;
 //   circleDef.radius = 10.0f;
 //   circleDef.bounciness = 0.5f;
 //   //g2->addComponent<RigidBody2D>();
 //   CircleCollider* c2 = g2->addComponent<CircleCollider>(circleDef);
 //   //BoxCollider2D* c2 = g2->addComponent<BoxCollider2D>(def3);


 //   g1->moveTo({ 500.0f, 400.0f });
 //   //g1->setRotation(1);
 //   rigid1->velocity({ -100,0 });
 //   g2->moveTo({400.0f, 100.0f});
 //   g3->moveTo({ 400.0f, 500.0f });
 //   sf::RectangleShape shape1({ 20.0f, 20.0f });
 //   shape1.setOrigin({ 10, 10 });
 //   sf::CircleShape shape2(10.0f);
 //   //sf::RectangleShape shape2({ 20.0f, 20.0f });
 //   shape2.setOrigin({ 10.0f,10.0f });
 //   sf::RectangleShape shape3({ 200.0f, 20.0f });
 //   shape3.setOrigin({ 100,10 });
 //   World::init({ 0.0f, 500.0f });

	//sf::RenderWindow window(sf::VideoMode(800, 600), "2D Physics Engine");
 //   sf::Clock clock;
 //   float dt = 0.0f;
 //   bool pause = false;
 //   float deg = 0.0f;
	//while (window.isOpen())
	//{
	//	sf::Event evt;
	//	while (window.pollEvent(evt))
	//	{
	//		if (evt.type == sf::Event::Closed)
	//			window.close();
 //           if (evt.type == sf::Event::KeyPressed)
 //               pause = !pause;
	//	}
 //       if (pause) continue;
 //       //Motion motion({ 0, 0 }, 0.001f);
 //       //Motion motion2({ 100.0f*dt, -100.0f*dt }, 0.001f);
 //       //g1->transform(motion);
 //       //g2->transform(motion2);
 //       //printf("g1:(x : %f, y : %f, degree : %f)\n", g1->transform().position().x(), g1->transform().position().y(), g1->transform().rotation().radian() * 180 / PI);



 //       //dt = clock.restart().asSeconds();
 //       World::step(0.00020f, 3, 0);
 //       // 화면 그리기
 //       window.clear(sf::Color::Black);
 //       shape1.setRotation(g1->transform().rotation().radian() * 180 / PI);
 //       shape1.setPosition({ g1->transform().position().x(), g1->transform().position().y() });
 //       shape2.setPosition({g2->transform().position().x(), g2->transform().position().y()});
 //       shape3.setPosition({ g3->transform().position().x(), g3->transform().position().y()});
 //       shape3.setRotation(g3->transform().rotation().radian() * 180 / PI);
 //       shape1.setFillColor(sf::Color::Red);
 //       shape2.setFillColor(sf::Color::Red);
 //       shape3.setFillColor(sf::Color::Yellow);
 //       window.draw(shape1);
 //       window.draw(shape2);
 //       window.draw(shape3);
 //       //body.draw(window);
 //       window.display();
	//}
	return 0;
}