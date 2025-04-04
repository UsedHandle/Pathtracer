#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <iostream>
#include <print>
#include <chrono>
#include <string>
using std::chrono::high_resolution_clock;

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec2;

#include "pixel.h"
#include "pathtracer.h"
#include "model.h"

#include "stb_image_write.h"

#include "triangle.h"
#include "sphere.h"

int main(int argc, char** argv) {
	uint32_t numSamples = 8;
	uint32_t numBounces = 5;

	uint32_t pixels_width = 1024;
	uint32_t pixels_height = 768;

	if(argc > 1){
		int val = std::stoi(argv[1]);
		if(val > 0)
			numSamples = static_cast<uint32_t>(std::stoi(argv[1]));
	}
	if(argc > 2){
		int val = std::stoi(argv[2]);

		if(val >= 0)
			numBounces = static_cast<uint32_t>(std::stoi(argv[2]));
	}
	if (argc > 4) {
		int val1 = std::stoi(argv[3]);
		int val2 = std::stoi(argv[4]);
		if(val1 > 0 && val2 > 0) {
			pixels_width = val1;
			pixels_height = val2;
		}

	}

	Pathtracer tracer(Sampler(1835), numBounces);
	glm::mat4 trans(1.0);
	trans = glm::translate(trans, glm::vec3(50., 40., 70.));
	trans = glm::rotate(trans, (float)PI / 7.f, glm::vec3(0., 1., 0.));

	trans = glm::rotate(trans, -(float)PI/2.f, glm::vec3(1.,0.,0.));
	trans = glm::scale(trans, glm::vec3(.25f));
	Model model("assets/stanforddragon.stl", glm::vec3(0.75, 0.5, 0.25), glm::vec3(0.f), trans);
	//Model model("assets/box.glb", glm::vec3(0.75), glm::vec3(0.f), trans);

	std::vector<Shape*> objectList = {
		// smallpt scene except light and walls with triangles instead of spheres
		new Triangle(vec3(1.,0.,0.),vec3(1.,81.6,0.),vec3(1.,0.,170.),vec3(.75,.25,.25),vec3(0.)),   // Left
		new Triangle(vec3(1.,81.6,170.),vec3(1.,0.,170.),vec3(1, 81.6,0.0),vec3(.75,.25,.25),vec3(0.)), // Left
		new Triangle(vec3(99.,0.0,0.0),vec3(99.,81.6,0.0),vec3(99.,0.,170.),vec3(.25,.25,.75),vec3(0.)),   // Right
		new Triangle(vec3(99.,81.6,170.),vec3(99.,0.,170.),vec3(99., 81.6,0.0),vec3(.25,.25,.75),vec3(0.)), // Right
		new Triangle(vec3(1.,0.,0.),vec3(1.,81.6,0.),vec3(99.,0.,0.),vec3(.75,.75,.75),vec3(0.)),   // Back
		new Triangle(vec3(99.,81.6,0.),vec3(99.,0.,0.),vec3(1.,81.6,0.),vec3(.75,.75,.75),vec3(0.)), // Back
		new Triangle(vec3(1.,0.,170.),vec3(1.,81.6,170.),vec3(99.,0.,170.),vec3(0.,0.,0.),vec3(0.)),   // Front
		new Triangle(vec3(99.,81.6,170.),vec3(99.,0.,170.),vec3(1.,81.6,170.),vec3(0.,0.,0.),vec3(0.)), // Front
		new Triangle(vec3(1.,0.,0.),vec3(1.,0.,170.),vec3(99.,0.,0.),vec3(.75,.75,.75),vec3(0.)),   // Bottom
		new Triangle(vec3(99.,0.,170.),vec3(1.,0.,170.),vec3(99.,0.,0.),vec3(.75,.75,.75),vec3(0.)), // Bottom
		new Triangle(vec3(1.,81.6,0.),vec3(1.,81.6,170.),vec3(99.,81.6,0.),vec3(.75,.75,.75),vec3(0.)),   // Top
		new Triangle(vec3(99.,81.6,170.),vec3(1.,81.6,170.),vec3(99.,81.6,0.),vec3(.75,.75,.75),vec3(0.)), // Top
		/*new Sphere(1e5,  vec3( 1e5+1,40.8,81.6),       vec3(.75,.25,.25),  vec3(0.0)),//Left */
		/*new Sphere(1e5,  vec3(-1e5+99,40.8,81.6),      vec3(.25,.25,.75),  vec3(0.0)),//Right */
		/*new Sphere(1e5,  vec3( 50.,40.8, 1e5),         vec3(.75,.75,.75),  vec3(0.0)),//Back */
		/*new Sphere(1e5,  vec3( 50.,40.8,-1e5+170),     vec3(0.0),          vec3(0.0)),//Front */
		/*new Sphere(1e5,  vec3( 50., 1e5, 81.6),        vec3(.75,.75,.75),  vec3(0.0)),//Bottom */
		/*new Sphere(1e5,  vec3( 50.,-1e5+81.6,81.6),    vec3(.75,.75,.75),  vec3(0.0)),//Top */
		new Sphere(16.5, vec3( 27.,16.5,47),           vec3(1.0)*.999f,     vec3(0.0)),//Mirror 
		new Sphere(16.5, vec3( 73.,16.5,78),           vec3(1.0)*.999f,     vec3(0.0)),//Glass
	};
	objectList.reserve(objectList.size() + model.m_triangles.size());
	for (const Triangle& Tri : model.m_triangles) {
		objectList.push_back(new Triangle(Tri));
		//std::print("{} {} {}\n", Tri.m_p1.x, Tri.m_p1.y, Tri.m_p1.z);
	}

	const float centerx = 50.f;
	const float centerz = 79.1f;
	const float width = 20.f;
	const float height = 20.f;
	const float minx = centerx - width/2.;
	const float maxx = centerx + width/2.;
	const float y    = 81.6f - 9;
	const float minz = centerz - height/2.;
	const float maxz = centerz + height/2.;
	std::vector<Shape*> lightList = {
		/*new Sphere(600., vec3( 50.,681.6-.27,81.6), vec3(1.0),vec3(12.)) //Light */
		/*new Sphere(5.,vec3( 50.,81.6-6.,81.6), vec3(1.0), vec3(12.)), //Light*/
		/* Sphere(20.,vec3( 50.,51.6-6.,81.6), vec3(1.0), vec3(12.)) //Light */
		new Triangle(vec3(maxx,y,maxz),vec3(minx,y,minz),vec3(minx,y,maxz), vec3(1.0), vec3(50.)), //Light
		new Triangle(vec3(maxx,y,minz),vec3(minx,y,minz),vec3(maxx,y,maxz), vec3(1.0), vec3(50.)), //Light
	};

	Scene cornellBox(objectList, lightList);

	std::vector<pixel> pixels(pixels_height * pixels_width);

	// std::tan() is not constexpr
	constexpr float FOV = 75.f;
	const float transVal =  std::tan(glm::radians(FOV/2.f));

	float aspect_ratio =
		float(pixels_width)/float(pixels_height);

	/* // Ray ray(vec3(0.0), vec3(0.0, 0.0, 1.0)); */
	vec3 Left =    vec3( 1.f,  0.f,  0.f);
	vec3 Up   =    vec3( 0.f,  1.f,  0.f);
	vec3 D    =    vec3( 0.f,  0.f,  1.f);

	D          =  rotateY(D,      glm::radians(180.f));
	//Up         =  rotateY(Up,     glm::radians(180.f));
	Left       =  rotateY(Left,   glm::radians(180.f));
	
	Ray ray(vec3(50.0, 50.0, 150.0), D);
   
	auto start = high_resolution_clock::now();

	for(uint32_t i = 0; i < pixels_height; ++i){
		// does not mess with stdout, so there
		// is no flushing of text like log info
		fprintf(stderr, "\rRendering... %.1f%%",100.0 *
				static_cast<float>((i+1)*pixels_width)/
				static_cast<float>(pixels_width*pixels_height));
	   
		for (uint32_t j = 0; j < pixels_width; ++j) {
			vec2 uv = vec2(
				float(j) / float(pixels_width - 1),
				1.0 - float(i) / float(pixels_height - 1));

			uv = uv * 2.f * transVal - transVal;
			uv.x *= aspect_ratio;

			ray.D = D;
			ray.D += uv.y * Up;
			ray.D += -uv.x * Left;
			ray.D = normalize(ray.D);


			vec3 col = vec3(0.f);

			
			for(uint32_t k = 0; k < numSamples; ++k)
				col += tracer.radiance(ray, &cornellBox);

			col *= 1.f/static_cast<float>(numSamples);
			



			pixels[i*pixels_width +j] = toPixel(col);
		}
	}
  
	printf("\n");

	auto end = high_resolution_clock::now();
	auto time = (end - start)/std::chrono::milliseconds(1);
	printf("time: %.3f\n", static_cast<float>(time)*1e-3);
	
	stbi_write_png("outimage.png",
	               pixels_width, pixels_height, 3,
	               pixels.data(), pixels_width * sizeof(pixel));

}
