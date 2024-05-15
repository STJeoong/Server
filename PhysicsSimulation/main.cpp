#include <iostream>
#include <SFML/Graphics.hpp>
#include <GameObject.h>
#include <World.h>
#include <BoxCollider2D.h>
#include <RigidBody2D.h>
#include <CircleCollider.h>
#define PI 3.14159265359f
sf::RenderWindow window(sf::VideoMode(800, 600), "2D Physics Engine");
#include "Renderer.h"

int main()
{
    std::ios_base::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);

#pragma region instantiate
    S_BoxDef def3;
    def3.halfSize = { 250.0f, 10.0f };
    def3.bouncinessThreshold = 50.0f;
    GameObject* g3 = World::instantiate(); // floor
    BoxCollider2D* c3 = g3->addComponent<BoxCollider2D>(def3);
    g3->addComponent<Renderder>(new sf::RectangleShape({ def3.halfSize.x() * 2, def3.halfSize.y() * 2 }), def3.halfSize, sf::Color::Yellow );

    S_BoxDef def4;
    def4.halfSize = { 1.0f, 1.0f };
    //def4.bounciness = 0.4f;
    GameObject* g4 = World::instantiate();
    RigidBody2D* rigid4 = g4->addComponent<RigidBody2D>();
    BoxCollider2D* c4 = g4->addComponent<BoxCollider2D>(def4);
    g4->addComponent<Renderder>(new sf::RectangleShape({ def4.halfSize.x() * 2, def4.halfSize.y() * 2 }), def4.halfSize, sf::Color::White);

    S_BoxDef def5;
    def5.halfSize = { 1.0f, 1.0f };
    //def5.bounciness = 0.5f;
    GameObject* g5 = World::instantiate();
    RigidBody2D* rigid5 = g5->addComponent<RigidBody2D>(); 
    BoxCollider2D* c5 = g5->addComponent<BoxCollider2D>(def5);
    g5->addComponent<Renderder>(new sf::RectangleShape({ def5.halfSize.x() * 2, def5.halfSize.y() * 2 }), def5.halfSize, sf::Color::Green);

    S_BoxDef def6;
    def6.halfSize = { 1.0f, 1.0f };
    //def6.bounciness = 0.6f;
    GameObject* g6 = World::instantiate();
    RigidBody2D* rigid6 = g6->addComponent<RigidBody2D>();
    BoxCollider2D* c6 = g6->addComponent<BoxCollider2D>(def6);
    g6->addComponent<Renderder>(new sf::RectangleShape({ def6.halfSize.x() * 2, def6.halfSize.y() * 2 }), def6.halfSize, sf::Color::Magenta);

    S_BoxDef def7;
    def7.halfSize = { 1.0f, 1.0f };
    //def6.bounciness = 0.6f;
    GameObject* g7 = World::instantiate();
    RigidBody2D* rigid7 = g7->addComponent<RigidBody2D>();
    BoxCollider2D* c7 = g7->addComponent<BoxCollider2D>(def7);
    g7->addComponent<Renderder>(new sf::RectangleShape({ def7.halfSize.x() * 2, def7.halfSize.y() * 2 }), def7.halfSize, sf::Color::Yellow);

    S_BoxDef def8;
    def8.halfSize = { 1.0f, 1.0f };
    //def6.bounciness = 0.6f;
    GameObject* g8 = World::instantiate();
    RigidBody2D* rigid8 = g8->addComponent<RigidBody2D>();
    BoxCollider2D* c8 = g8->addComponent<BoxCollider2D>(def8);
    g8->addComponent<Renderder>(new sf::RectangleShape({ def8.halfSize.x() * 2, def8.halfSize.y() * 2 }), def8.halfSize, sf::Color::Red);

    S_BoxDef def9;
    def9.halfSize = { 1.0f, 1.0f };
    //def6.bounciness = 0.6f;
    GameObject* g9 = World::instantiate();
    RigidBody2D* rigid9 = g9->addComponent<RigidBody2D>();
    BoxCollider2D* c9 = g9->addComponent<BoxCollider2D>(def9);
    g9->addComponent<Renderder>(new sf::RectangleShape({ def9.halfSize.x() * 2, def9.halfSize.y() * 2 }), def9.halfSize, sf::Color::White);

    S_CircleDef def10;
    def10.radius = 1.0f;
    GameObject* g10 = World::instantiate();
    RigidBody2D* rigid10 = g10->addComponent<RigidBody2D>();
    CircleCollider* c10 = g10->addComponent<CircleCollider>(def10);
    g10->addComponent<Renderder>(new sf::CircleShape(def10.radius), Vector2D{ def10.radius, def10.radius }, sf::Color::Red);
#pragma endregion

    S_BoxDef def1;
    def1.halfSize = { 1.0f, 1.0f };
    //def1.bounciness = 0.8f;
    //def1.bouncinessThreshold = 50.0f;
    GameObject* g1 = World::instantiate();
    RigidBody2D* rigid1 = g1->addComponent<RigidBody2D>();
    BoxCollider2D* c1 = g1->addComponent<BoxCollider2D>(def1);
    g1->addComponent<Renderder>(new sf::RectangleShape({ def1.halfSize.x() * 2, def1.halfSize.y() * 2 }), def1.halfSize, sf::Color::Red);
    g1->moveTo({ 399.0f, 300.0f });
    //g1->moveTo({ 398.0f, 450.0f });
    //g1->setRotation(1);
    //rigid1->velocity({ -400,0 });
    //rigid1->fixedRotation(true);
    /*g4->moveTo({ 401.0f, 250.0f });
    g5->moveTo({ 399.5f, 290.0f });
    g6->moveTo({ 399.0f, 295.0f });
    g7->moveTo({ 400.0f, 285.0f });
    g8->moveTo({ 400.5f, 275.0f });*/
    g4->moveTo({ 410.0f, 200.0f });
    g4->setRotation(0.5f);
    rigid4->velocity({ 0, 100 });
    g5->moveTo({ 400.0f, 250.0f });
    rigid5->angularVelocity(1.1f);
    g6->moveTo({ 405.0f, 230.0f });
    rigid6->angularVelocity(-1.1f);
    g7->moveTo({ 402.0f, 180.0f });
    g8->moveTo({ 415.0f, 260.0f });
    g9->moveTo({ 400.0f, 270.0f });
    g10->moveTo({ 403.0f, 300.0f });
    /*g4->moveTo({ 400.0f, 100.0f });
    g5->moveTo({ 402.0f, 200.0f });
    g6->moveTo({ 399.0f, 350.0f });
    g7->moveTo({ 401.0f, 400.0f });
    g8->moveTo({ 398.5f, 250.0f });
    g8->name() = "g8";
    g9->moveTo({ 397.0f, 300.0f });
    g9->name() = "g9";
    g10->move({ 400.0f, 325.0f });
    g10->name() = "circle";*/
    //rigid6->velocity({ -50,0 });
    //g1->setRotation(1);
    //rigid1->velocity({ -100,0 });
    g3->moveTo({ 400.0f, 330.0f });
    //g3->moveTo({ 400.0f, 500.0f });

    World::init({ 0.0f, 10.0f });
	
    sf::Clock clock;
    bool pause = false;
    float dt = 0.0f;
    sf::View originalView = window.getDefaultView();
    sf::View zoomedView = window.getDefaultView();
    zoomedView.zoom(1.0f / 12.0f);
    float zoomFactor = 2.0f;

	while (window.isOpen())
	{
		sf::Event evt;
		while (window.pollEvent(evt))
		{
			if (evt.type == sf::Event::Closed)
				window.close();
            if (evt.type == sf::Event::KeyPressed)
                pause = !pause;
		}
        if (pause) continue;
        dt += clock.restart().asSeconds();
        if (dt < 0.02f) continue;
        dt -= 0.02f;
        window.clear(sf::Color::Black);
        //dt = clock.restart().asSeconds();
        World::step(0.02f, 5, 3);
        window.setView(zoomedView);
        window.display();
	}
	return 0;
}