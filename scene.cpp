#include "scene.h"
#include <cstdio>

Scene::Scene(const std::initializer_list<Shape*>& objList,
             const std::initializer_list<Shape*>& lightList)
	: objects(objList),
	  lights(lightList)
{
	objects.insert(
		objects.end(),
		std::make_move_iterator(lightList.begin()),
		std::make_move_iterator(lightList.end())
	);

	bvh = BVH(objects);
}

Scene::~Scene(){
	for(Shape* shape : objects){
		delete shape;
	}
}