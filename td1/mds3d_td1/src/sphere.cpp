#include "sphere.h"
#include <iostream>

Sphere::Sphere(float radius)
    : m_radius(radius)
{
}

Sphere::Sphere(const PropertyList &propList)
{
    m_radius = propList.getFloat("radius", 1.f);
    m_center = propList.getPoint("center", Point3f(0, 0, 0));
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const Ray &ray, Hit &hit) const
{
    /// TODO: compute ray-sphere intersection

    Point3f o = ray.origin;
    Vector3f d = ray.direction;
    float B = 2 * o.dot(d) - 2 * m_center.dot(d);
    float A = d.squaredNorm();
    float C = -2 * o.dot(m_center) + o.squaredNorm() + m_center.squaredNorm() - m_radius * m_radius;

    /* Calcul du discriminant b^2-4ac */
    float D = pow(B, 2) - 4.0 * A * C;

    if (D == 0) /* b^2-4ac = 0 */
    {
        float t0 = (-B / 2 * A);
        hit.setT(t0);
        hit.setShape(this);
        const Normal3f normal = o + t0 * d - m_center;
        hit.setNormal(normal);
        return true;
    }
    else if (D > 0) /* b^2-4ac > 0 */
    {
        float t1 = (-B + sqrt(D)) / (2 * A);
        float t2 = (-B - sqrt(D)) / (2 * A);
        if (t1 > 0 && t1 < t2)
        {
            hit.setT(t1);
            Normal3f normal = o + t1 * d - m_center;
            normal = normal.normalized();
            hit.setNormal(normal);
        }
        if (t2 > 0 && t2 < t1)
        {
            Normal3f normal = o + t2 * d - m_center;
            normal = normal.normalized();
            hit.setNormal(normal);
            hit.setT(t2);
        }
        hit.setShape(this);

        return true;
    }
    else
    {
        return false;
    }
}

REGISTER_CLASS(Sphere, "sphere")
