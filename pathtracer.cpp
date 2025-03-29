#include "pathtracer.h"
#include "sampler.h"

Pathtracer::Pathtracer(Sampler new_sampler, uint32_t new_numBounces)
	: sampler(new_sampler), numBounces(new_numBounces)
{
	direct	  = std::make_unique<glm::dvec3[]>( numBounces   ); 
	emission  = std::make_unique<glm::dvec3[]>( numBounces+1 ); 
	col	  = std::make_unique<glm::dvec3[]>( numBounces   ); 
	LiWeight  = std::make_unique<double[]>    ( numBounces   ); 
}


glm::dvec3 Pathtracer::radiance(
		Ray indirectRay,
		const Scene* const scene)
{
	using glm::dvec3;
	using glm::dmat3;

	for(uint32_t i = 0; i < numBounces; ++i){
		direct[i]    =  glm::dvec3(0.0);
		emission[i]  =  glm::dvec3(0.0);
		col[i]       =  glm::dvec3(0.0);
		LiWeight[i]  =  0.0;
	}

	emission[numBounces] = glm::dvec3(0.0);
	double brdf_pdf = 1.0;
	for(uint32_t depth = 0; depth < numBounces+1; ++depth){
		double t;
		const Shape* obj;
	
		if(!scene->findIntersection(indirectRay, t, obj)){
			break;
		}



		emission[depth] = obj->m_emis;
		// if depth is the last bounce
		if(depth == numBounces)
		   break;
		
		// brdf sampling
		dvec3 x = indirectRay.O + indirectRay.D*t;
		dvec3 w = obj->normal(x, indirectRay.D);
	   

		dmat3 orthonormalMat = orthonormalZ(w);

		dvec3 sample = sampler.cosHemisphere();

		const double next_brdf_pdf = sample.z/PI;

		dvec3 d = orthonormalMat * sample;
		col[depth] = obj->m_col;
		indirectRay = Ray(x, d);

		// Next event estimation
		Ray directRay(x, dvec3(0.0));
		double directDist;
		
		// no light edge case
		if(scene->firstLightIndex == scene->objects.size()){
			brdf_pdf = next_brdf_pdf;
			break;
		}

		std::size_t randLightIndex =
			sampler.randInt(scene->firstLightIndex, scene->objects.size()-1);
		const Shape* const sampleLight = scene->objects[randLightIndex];
		dvec3 point = sampleLight->sampleSurface(
				directRay, directDist,
				sampler.uniform_dist(),
				sampler.uniform_dist()
		);
	 
	   
		const dvec3 pointNormal =
			sampleLight->normal(point, directRay.D);
		const double light_cos_theta = dot(pointNormal, -directRay.D);

		// last check makes sure the pdf is not zero for example when a point is sampled
		// on a triangle from the same triangle
		if(scene->visibility( directRay, directDist ) && light_cos_theta > EPS){
			const double surfaceArea	 =	sampleLight->m_area;
			const double R2			 =	directDist*directDist;
			const double nee_pdfA = 1./(surfaceArea);
			const double nee_pdfw = R2/(light_cos_theta) * nee_pdfA;	   
			const double cos_theta = std::abs(dot(directRay.D, w));

			const dvec3 BRDF = obj->m_col/PI;
			const dvec3 light = sampleLight->m_emis;

			LiWeight[depth] = (nee_pdfw*nee_pdfw)/(nee_pdfw*nee_pdfw + brdf_pdf*brdf_pdf);
			/*printf("%lf %lf %lf %lf\n", R2, light_cos_theta, nee_pdfA, LiWeight[depth]);*/
			const double cosprod = std::abs(cos_theta*light_cos_theta);

			direct[depth] =
				BRDF * light * cosprod/R2  * 1.0/nee_pdfA;
		}

		brdf_pdf = next_brdf_pdf;
	}
	
	dvec3 light_out = emission[numBounces];

	// uint goes to its upper limit when it goes below 0
	for(uint32_t i = numBounces-1; i < numBounces; --i){
		light_out = (1.-LiWeight[i])*emission[i] + LiWeight[i]*direct[i] + col[i]*light_out;
		/*light_out = (emission[i] + col[i]*light_out);*/
	}
	return light_out;
}
