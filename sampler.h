#pragma once

#include <cstdint>
#include <concepts>

#include <glm/glm.hpp>

#include "ray.h"

struct Sampler {
	uint32_t state;
	
	Sampler(){ }
	Sampler(uint32_t seed) : state(seed) {	}
	Sampler(const Sampler& b) : state(b.state) {  }
	Sampler& operator=(const Sampler& b);

	inline void seed(uint32_t seed){ state=seed; }

	// xorshift
	inline void nextState(){
		state ^= state << 13;
		state ^= state >> 17;
		state ^= state << 5;
	}

	[[nodiscard]] inline
	uint32_t getNextState(){
		nextState();
		return state;
	}

	[[nodiscard]] double uniform_dist();

	template<std::integral T>
	[[nodiscard]] T randInt(T min, T max){
		double rand_in_range = uniform_dist() * static_cast<double>(max-min+1);
		return static_cast<T>(rand_in_range) + min; 
	}
 
	[[nodiscard]] glm::dvec3 uniHemisphere();
	[[nodiscard]] glm::dvec3 cosHemisphere();
};
