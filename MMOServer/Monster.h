#pragma once
#include "GameObject.h"
class Area;
class Monster : public GameObject
{
	friend class Map;
public:
	static void init();
private:


public:
	virtual E_ObjectType objectType() const override { return E_ObjectType::MONSTER; }
protected:
	Monster() = delete;
	Monster(const Monster&) = delete;
	Monster(Monster* copy) : GameObject(copy) {} // TODO : �����Ұ� ������ ����
	Monster(bool isActive, GameObject* parent) : GameObject(isActive, parent) {}
	Monster(bool isActive, Monster* copy, GameObject* parent) : GameObject(isActive, copy, parent) {}
	virtual ~Monster() = default; // TODO : �����Ұ� ������ ����
private:
	virtual GameObject* clone() override { return new Monster(this); }
};