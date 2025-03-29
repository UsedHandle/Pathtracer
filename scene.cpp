#include "scene.h"
#include <cstdio>

Scene::Scene(std::initializer_list<Shape*> objList,
             std::initializer_list<Shape*> lightList)
	: objects(std::move(objList)),
	  firstLightIndex(objList.size())
{
	objects.insert(
		objects.end(),
		std::make_move_iterator(lightList.begin()),
		std::make_move_iterator(lightList.end())
	);
}

Scene::~Scene(){
	/*
	for(Shape* shape : objects){
		delete shape;
	}*/
}

bool Scene::findIntersection(
	const Ray& ray, float& t,
	const Shape*& shapeptr) const
{
	t = MAX_T;
	for(uint32_t i = 0; i < objects.size(); ++i){
		const float temp_t = objects[i]->intersect(ray);
		if(temp_t < t){ t = temp_t; shapeptr = objects[i]; }
	}

	return t < MAX_T;
}

bool Scene::visibility(const Ray& ray, float t) const {
	for(uint32_t i = 0; i < objects.size(); ++i){
		const float temp_t = objects[i]->intersect(ray);
		if(temp_t + EPS < t) return false;
	}

	return true;
}
