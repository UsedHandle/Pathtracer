#include "sphere.h"

Sphere::Sphere(float r,
               glm::vec3 pos,
               glm::vec3 col,
               glm::vec3 emis): m_r(r), m_pos(pos)
{
	m_area=4.0*PI*r*r;
	m_pos=pos;
	m_col=col;
	m_emis=emis;
}

glm::vec3
Sphere::normal(const glm::vec3& point, const glm::vec3& dir) const {
	glm::vec3 norm = (point - m_pos)/m_r;

	// negate the normal if D is facing 
	// at the same direction at the normal
	// (when D is inside the sphere)
	return dot(norm, dir) < 0.0 ? norm : -norm;
}

float Sphere::intersect(const Ray& ray) const {
	// vec3 osubp = ray.O - pos;
	const glm::vec3 negative_op = m_pos - ray.O;
	
	const float b = dot(ray.D, negative_op);

	float discrim = b*b - dot(negative_op, negative_op) + m_r*m_r;
	if(discrim < 0.0) return MAX_T;
	else discrim = std::sqrt(discrim);

	float root;
	return ( root=b-discrim)>MIN_T ? root :
	       ((root=b+discrim)>MIN_T ? root : MAX_T);
}

[[nodiscard]] glm::vec3
Sphere::sampleSurface(Ray& ray, float& dist, float zeta1, float zeta2) const
{
	const float phi = 2.0 * PI * zeta1;
	const float theta	 = std::acos(1.0 - 2.0 * zeta2);
	const float sin_theta = std::sin(theta);
   
	const glm::vec3 point = glm::vec3(
		sin_theta * std::cos(phi),
		sin_theta * std::sin(phi),
		std::cos(theta)
	) * m_r + m_pos;

	dist = length(point - ray.O);
	ray.D = (point-ray.O)/dist;

	return point;
}
