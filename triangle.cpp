#include "triangle.h"
#include "ray.h"

Triangle::Triangle(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3,
                   glm::dvec3 col, glm::dvec3 emis):
		m_p1(p1), m_p2(p2), m_p3(p3)
{
		m_col = col;
		m_emis = emis;
		m_norm = normalFromUV(p2-p1, p3-p1);

		glm::dvec3 edge1 = p2-p1;
		glm::dvec3 edge2 = p3-p1;
		m_area = glm::length(glm::cross(edge1, edge2)) * 0.5; // double facing

}

// eventually going to be normal mapping
glm::dvec3
Triangle::normal(const glm::dvec3& point, const glm::dvec3& dir) const {
		return dot(m_norm, dir) < 0.0 ? m_norm : -m_norm;
}

double Triangle::intersect(const Ray& ray) const {
		// the following uses moller trumbore (likely fastest)
		// woop et al. may potentially be watertight 
		// as named in moller & trumbore
		glm::dvec3 edge1, edge2, P, Q, T;
		double det;
		edge1 = m_p2-m_p1;
		edge2 = m_p3-m_p1;
		P = glm::cross(ray.D, edge2);
		det = dot(edge1, P);
		
		// triangle is parallel to ray
#ifdef ENABLE_CULL
		// u and v remain undivided by det
		if(det < EPS)
				return MAX_T;

		T = ray.O - m_p1;
		double u = dot(T, P);
		if(u < 0.0 || u > det)
				return MAX_T;

		Q = cross(T, edge1);
		double v = dot(ray.D, Q);
		if(v < 0.0 || u + v > det)
				return MAX_T;

		return dot(edge2, Q)/det;
#else
		if(det > -EPS && det < EPS)
				return MAX_T;
		double inv_det = 1.0/det;
		T = ray.O - m_p1;
		double u = dot(T, P) * inv_det;
		if(u < 0.0 || u > 1.0)
				return MAX_T;

		Q = cross(T, edge1);
		double v = dot(ray.D, Q) * inv_det;
		if(v < 0.0 || u + v > 1.0)
				return MAX_T;
		
		// det used to store t
		return (det=dot(edge2, Q)*inv_det) > MIN_T ? det : MAX_T;
#endif
}

[[nodiscard]] glm::dvec3
Triangle::sampleSurface(Ray& ray, double& dist,
                        double zeta1, double zeta2) const {
		const double t = std::sqrt(zeta1);
		const double u = 1-t;
		const double v = zeta2 * (1.-u);
		const double w = t*(1.-zeta2);

		const glm::dvec3 point = m_p1*u+
		                         m_p2*v+
		                         m_p3*w;
		ray.D = point-ray.O;
		dist = glm::length(ray.D);
		ray.D = glm::normalize(ray.D);
		return point;
}

