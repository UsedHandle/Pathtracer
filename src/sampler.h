#pragma once

#include <cstdint>
#include <concepts>
#include <algorithm>

#include <glm/glm.hpp>

#include "ray.h"

struct Sampler {
	uint32_t state{};
	
	Sampler(){};
	Sampler(uint32_t seed) : state(seed) {	}
	Sampler(const Sampler& b) = default;
	Sampler(Sampler&& b) noexcept = default;
	Sampler& operator=(const Sampler& b) = default;
	Sampler& operator=(Sampler&& b) noexcept = default;

	inline void seed(uint32_t seed){ state=seed; }

	// xorshift
	inline void nextState(){
		state ^= state << 13;
		state ^= state >> 17;
		state ^= state << 5;
	}

	inline uint32_t getNextState(){
		nextState();
		return state;
	}

	float uniform_dist();

	template<std::integral T>
	T randInt(T min, T max){
		const T size = max-min+1;
		const T rand = static_cast<T>(uniform_dist()*static_cast<float>(size));
		// floating point inaccuracies can lead to rand being equal to the size
		const T out = std::min<T>(rand, size-1);
		return static_cast<T>(out) + min; 
	}
 
	glm::vec3 uniHemisphere();
	glm::vec3 cosHemisphere();
};
