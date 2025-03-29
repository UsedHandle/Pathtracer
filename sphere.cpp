#include "sphere.h"

Sphere::Sphere(double r,
               glm::dvec3 pos,
               glm::dvec3 col,
               glm::dvec3 emis): m_r(r), m_pos(pos)
{
	m_area=4.0*PI*r*r;
	m_pos=pos;
	m_col=col;
	m_emis=emis;
}

glm::dvec3
Sphere::normal(const glm::dvec3& point, const glm::dvec3& dir) const {
	glm::dvec3 norm = (point - m_pos)/m_r;

	// negate the normal if D is facing 
	// at the same direction at the normal
	// (when D is inside the sphere)
	return dot(norm, dir) < 0.0 ? norm : -norm;
}

double Sphere::intersect(const Ray& ray) const {
	// dvec3 osubp = ray.O - pos;
	const glm::dvec3 negative_op = m_pos - ray.O;
	
	const double b = dot(ray.D, negative_op);

	double discrim = b*b - dot(negative_op, negative_op) + m_r*m_r;
	if(discrim < 0.0) return MAX_T;
	else discrim = std::sqrt(discrim);

	double root;
	return ( root=b-discrim)>MIN_T ? root :
	       ((root=b+discrim)>MIN_T ? root : MAX_T);
}

[[nodiscard]] glm::dvec3
Sphere::sampleSurface(Ray& ray, double& dist, double zeta1, double zeta2) const
{
	const double phi = 2.0 * PI * zeta1;
	const double theta	 = std::acos(1.0 - 2.0 * zeta2);
	const double sin_theta = std::sin(theta);
   
	const glm::dvec3 point = glm::dvec3(
		sin_theta * std::cos(phi),
		sin_theta * std::sin(phi),
		std::cos(theta)
	) * m_r + m_pos;

	dist = length(point - ray.O);
	ray.D = (point-ray.O)/dist;

	return point;
}
