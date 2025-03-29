#include "triangle.h"
#include "ray.h"

Triangle::Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
                   glm::vec3 col, glm::vec3 emis):
		m_p1(p1), m_p2(p2), m_p3(p3)
{
		m_col = col;
		m_emis = emis;
		m_norm = normalFromUV(p2-p1, p3-p1);

		glm::vec3 edge1 = p2-p1;
		glm::vec3 edge2 = p3-p1;
		m_area = glm::length(glm::cross(edge1, edge2)) * 0.5f;

}

// eventually going to be normal mapping
glm::vec3
Triangle::normal(const glm::vec3& point, const glm::vec3& dir) const {
		return dot(m_norm, dir) < 0.0 ? m_norm : -m_norm;
}

float Triangle::intersect(const Ray& ray) const {
		// the following uses moller trumbore (likely fastest)
		// woop et al. may potentially be watertight 
		// as named in moller & trumbore
		glm::vec3 edge1, edge2, P, Q, T;
		float det;
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
		float u = dot(T, P);
		if(u < 0.f || u > det)
				return MAX_T;

		Q = cross(T, edge1);
		float v = dot(ray.D, Q);
		if(v < 0.f || u + v > det)
				return MAX_T;

		return dot(edge2, Q)/det;
#else
		if(det > -EPS && det < EPS)
				return MAX_T;
		float inv_det = 1.f/det;
		T = ray.O - m_p1;
		float u = dot(T, P) * inv_det;
		if(u < 0.f || u > 1.f)
				return MAX_T;

		Q = cross(T, edge1);
		float v = dot(ray.D, Q) * inv_det;
		if(v < 0.f || u + v > 1.f)
				return MAX_T;
		
		// det used to store t
		return (det=dot(edge2, Q)*inv_det) > MIN_T ? det : MAX_T;
#endif
}

[[nodiscard]] glm::vec3
Triangle::sampleSurface(Ray& ray, float& dist,
                        float zeta1, float zeta2) const {
		const float t = std::sqrt(zeta1);
		const float u = 1.f-t;
		const float v = zeta2 * (1.f-u);
		const float w = t*(1.f-zeta2);

		const glm::vec3 point = m_p1*u+
		                        m_p2*v+
		                        m_p3*w;
		ray.D = point-ray.O;
		dist = glm::length(ray.D);
		ray.D = glm::normalize(ray.D);
		return point;
}

Bound Triangle::getBound() const {
	return Union(Union(Bound(m_p1), Bound(m_p2)), Bound(m_p3));
}

