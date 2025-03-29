#include "pathtracer.h"
#include "sampler.h"

Pathtracer::Pathtracer(Sampler new_sampler, uint32_t new_numBounces)
	: sampler(new_sampler), numBounces(new_numBounces)
{
	direct	  = std::make_unique<glm::vec3[]>( numBounces   ); 
	emission  = std::make_unique<glm::vec3[]>( numBounces+1 ); 
	col	  = std::make_unique<glm::vec3[]>( numBounces   ); 
	LiWeight  = std::make_unique<float[]>    ( numBounces   ); 
}


glm::vec3 Pathtracer::radiance(
		Ray indirectRay,
		const Scene* const scene)
{
	using glm::vec3;
	using glm::dmat3;

	for(uint32_t i = 0; i < numBounces; ++i){
		direct[i]    =  glm::vec3(0.0);
		emission[i]  =  glm::vec3(0.0);
		col[i]       =  glm::vec3(0.0);
		LiWeight[i]  =  0.0;
	}

	emission[numBounces] = glm::vec3(0.0);
	float brdf_pdf = 1.0;
	for(uint32_t depth = 0; depth < numBounces+1; ++depth){
		float t;
		const Shape* obj;
	
		if(!scene->findIntersection(indirectRay, t, obj)){
			break;
		}



		emission[depth] = obj->m_emis;
		// if depth is the last bounce
		if(depth == numBounces)
		   break;
		
		// brdf sampling
		vec3 x = indirectRay.O + indirectRay.D*t;
		vec3 w = obj->normal(x, indirectRay.D);
	   

		dmat3 orthonormalMat = orthonormalZ(w);

		vec3 sample = sampler.cosHemisphere();

		const float next_brdf_pdf = sample.z/PI;

		vec3 d = orthonormalMat * sample;
		col[depth] = obj->m_col;
		indirectRay = Ray(x, d);

		// Next event estimation
		Ray directRay(x, vec3(0.0));
		float directDist;
		
		// no light edge case
		if(scene->lights.size() == 0){
			brdf_pdf = next_brdf_pdf;
			break;
		}

		std::size_t randLightIndex =
			sampler.randInt((size_t)0, scene->lights.size()-1);
		const Shape* const sampleLight = scene->objects[randLightIndex];
		vec3 point = sampleLight->sampleSurface(
				directRay, directDist,
				sampler.uniform_dist(),
				sampler.uniform_dist()
		);
	 
	   
		const vec3 pointNormal =
			sampleLight->normal(point, directRay.D);
		const float light_cos_theta = dot(pointNormal, -directRay.D);

		// last check makes sure the pdf is not zero for example when a point is sampled
		// on a triangle from the same triangle
		if(scene->visibility( directRay, directDist ) && light_cos_theta > EPS){
			const float surfaceArea	 =      sampleLight->m_area;
			const float R2           =      directDist*directDist;
			const float nee_pdfA     =      1.f/(surfaceArea);
			const float nee_pdfw     =      R2/(light_cos_theta) * nee_pdfA;	   
			const float cos_theta    =      std::abs(dot(directRay.D, w));

			const vec3 BRDF = obj->m_col/PI;
			const vec3 light = sampleLight->m_emis;

			LiWeight[depth] = (nee_pdfw*nee_pdfw)/(nee_pdfw*nee_pdfw + brdf_pdf*brdf_pdf);
			/*printf("%lf %lf %lf %lf\n", R2, light_cos_theta, nee_pdfA, LiWeight[depth]);*/
			const float cosprod = std::abs(cos_theta*light_cos_theta);

			direct[depth] =
				BRDF * light * cosprod/R2  * 1.f/nee_pdfA;
		}

		brdf_pdf = next_brdf_pdf;
	}
	
	vec3 light_out = emission[numBounces];

	// uint goes to its upper limit when it goes below 0
	for(uint32_t i = numBounces-1; i < numBounces; --i){
		light_out = (1.f-LiWeight[i])*emission[i] + LiWeight[i]*direct[i] + col[i]*light_out;
		/*light_out = (emission[i] + col[i]*light_out);*/
	}
	return light_out;
}
