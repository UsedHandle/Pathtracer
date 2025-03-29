#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <iostream>
#include <chrono>
#include <string>
using std::chrono::high_resolution_clock;

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
using glm::vec3;
using glm::dvec2;
using glm::dmat3;

#include "pixel.h"
#include "pathtracer.h"

#include "stb_image_write.h"

int main(int argc, char** argv) {
	uint32_t numSamples = 200;
	uint32_t numBounces = 2;
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
	
#define MID_GRAPHICS
#ifdef HIGH_GRAPHICS
	const uint32_t pixels_width   =  1024;
	const uint32_t pixels_height  =  768;
#endif
#ifdef VGA_GRAPHICS
	const uint32_t pixels_width  =	640;
	const uint32_t pixels_height =	480;
#endif

#ifdef MID_GRAPHICS
	const uint32_t pixels_width   =  256;
	const uint32_t pixels_height  =  256;
#endif

#ifdef LOW_GRAPHICS
	const uint32_t pixels_width   =  100;
	const uint32_t pixels_height  =  100;
#endif

	Pathtracer tracer(Sampler(1835), numBounces);
	
	// eventually would get replaced with BVH with unique ptr
	Scene cornellBox({
		// smallpt scene except light
		new Sphere(1e5,  vec3( 1e5+1,40.8,81.6),       vec3(.75,.25,.25),  vec3(0.0)),//Left 
		new Sphere(1e5,  vec3(-1e5+99,40.8,81.6),      vec3(.25,.25,.75),  vec3(0.0)),//Right 
		new Sphere(1e5,  vec3( 50.,40.8, 1e5),         vec3(.75,.75,.75),  vec3(0.0)),//Back 
		new Sphere(1e5,  vec3( 50.,40.8,-1e5+170),     vec3(0.0),          vec3(0.0)),//Front 
		new Sphere(1e5,  vec3( 50., 1e5, 81.6),        vec3(.75,.75,.75),  vec3(0.0)),//Bottom 
		new Sphere(1e5,  vec3( 50.,-1e5+81.6,81.6),    vec3(.75,.75,.75),  vec3(0.0)),//Top 
		new Sphere(16.5, vec3( 27.,16.5,47),           vec3(1.0)*.999,     vec3(0.0)),//Mirror 
		new Sphere(16.5, vec3( 73.,16.5,78),           vec3(1.0)*.999,     vec3(0.0)),//Glass
	},
	{
		/*new Sphere(600., vec3( 50.,681.6-.27,81.6), vec3(1.0),vec3(12.)) //Light */
		/*new Sphere(5.,vec3( 50.,81.6-6.,81.6), vec3(1.0), vec3(12.)), //Light*/
		/* Sphere(20.,vec3( 50.,51.6-6.,81.6), vec3(1.0), vec3(12.)) //Light */
		new Triangle(vec3( 50.,81.6-9.,81.6),vec3( 42.,81.6-9.,76.6),vec3( 42.,81.6-9.,81.6), vec3(1.0), vec3(50.)), //Light
		new Triangle(vec3( 50.,81.6-9.,76.6),vec3( 42.,81.6-9.,76.6),vec3( 50.,81.6-9.,81.6), vec3(1.0), vec3(50.)), //Light
	});

	pixel pixels[pixels_height][pixels_width];

	// std::tan() is not constexpr
	constexpr float FOV = 75.0;
	const float transVal =  std::tan(glm::radians(FOV/2.0));

	constexpr float aspect_ratio =
		float(pixels_width)/float(pixels_height);

	/* // Ray ray(vec3(0.0), vec3(0.0, 0.0, 1.0)); */
	vec3 Left =    vec3( 1.0,  0.0,  0.0);
	vec3 Up   =    vec3( 0.0,  1.0,  0.0);
	vec3 D    =    vec3( 0.0,  0.0,  1.0);

	D          =  rotateY(D,      glm::radians(180.0));
	Up         =  rotateY(Up,     glm::radians(180.0));
	Left       =  rotateY(Left,   glm::radians(180.0));
	
	Ray ray(vec3(50.0, 50.0, 150.0), D);
   
	auto start = high_resolution_clock::now();
	
	/* #pragma omp parallel for schedule(dynamic) reduction(+:n_inside) */
	for(uint32_t i = 0; i < pixels_height; ++i){
		// does not mess with stdout so there
		// is no flushing text like log info
		fprintf(stderr, "\rRendering... %.1f%%",100.0 *
				static_cast<float>((i+1)*pixels_width)/
				static_cast<float>(pixels_width*pixels_height));
	   
		for(uint32_t j = 0; j < pixels_width; ++j){
			dvec2 uv = dvec2(
						  float(j)/float(pixels_width-1),
					1.0 - float(i)/float(pixels_height-1));

			uv = uv * 2.0*transVal - transVal;
			uv.x *= aspect_ratio;
			
			ray.D = D;
			ray.D +=  uv.y*Up;
			ray.D += -uv.x*Left;
			ray.D = normalize(ray.D);
			

			vec3 col = vec3(0.0);
			
			for(uint32_t k = 0; k < numSamples; ++k)
				col += tracer.radiance(ray, &cornellBox);
			/*float t;*/
			/*if((t=Triangle(vec3( 50.,81.6-9.,-81.6),vec3( 51.,81.6-9.,81.6),vec3( 42.,81.6-9.,81.6), vec3(1.0), vec3(12.)).intersect(ray)) < MAX_T){*/
			/*	col = vec3(0.0, 1.0, 0.0);*/
			/*	printf("%lf\n", t); */
			/*} else {*/
			/*	col = vec3(0.0);*/
			/*}*/
			//
			col *= float(1.0/numSamples);
			
			pixels[i][j] = toPixel(col);
		}
	}
  
	printf("\n");

	auto end = high_resolution_clock::now();
	auto time = (end - start)/std::chrono::milliseconds(1);
	printf("time: %.3f\n", static_cast<float>(time)*1e-3);
	
	stbi_write_png("outimage.png",
	               pixels_width, pixels_height, 3,
	               pixels, pixels_width*sizeof(pixel));

}
