#include "scene.h"
#include <cstdio>

Scene::Scene(const std::vector<Shape*>& objList,
             const std::vector<Shape*>& lightList)
	: objects(objList),
	  lights(lightList)
{
	objects.insert(
		objects.end(),
		lightList.begin(),
		lightList.end()
	);

	bvh = BVH(objects);
}

Scene::~Scene(){
	for(Shape* shape : objects)
		delete shape;
}