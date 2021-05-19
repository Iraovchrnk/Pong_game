#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <bitset>
#include <array>
#include "Game.h"

class Component;
class Entity;


using ComponentId = std::size_t;

inline ComponentId getComponentTypeId() {
	static ComponentId lastId = 0;
	return lastId++;
}


template <typename T> inline ComponentId getComponentTypeId() noexcept {
	static ComponentId typeId = getComponentTypeId();
	return typeId;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component {

public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity {
private:
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
public:
	void update() {
		for (auto& c : components) {
			c->update();
		}

	}

	void draw() {
		for (auto& c : components) {
			c->draw();
		}
	}

	bool isActive() const {
		return active;
	}

	void destroy() {
		active = false;
	}

	template <typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeId<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeId<T>()] = c;
		componentBitSet[getComponentTypeId<T>()] = true;

		c->init();
		return *c;
	}

	template <typename T> T& getComponent() const {
		auto ptr(componentArray[getComponentTypeId<T>()]);
		return *static_cast<T*>(ptr);
	}

};


class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;
public:
	void update() {
		for (auto& entity : entities) {
			entity->update();
		}
	}
	void draw() {
		for (auto& entity : entities) {
			entity->draw();
		}
	}
	void refresh() {
		entities.erase(std::remove_if(entities.begin(), entities.end(),
			[](const std::unique_ptr<Entity> &mEntity) {
			return !mEntity->isActive();
		}),
			entities.end());
	}

	Entity& addEntity() {
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));

		return *e;
	}
};