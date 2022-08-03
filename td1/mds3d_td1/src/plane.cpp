#include "plane.h"

Plane::Plane()
{
}

Plane::Plane(const PropertyList &propList)
{
    m_position = propList.getPoint("position", Point3f(0, 0, 0));
    m_normal = propList.getVector("normal", Point3f(0, 0, 1));
}

Plane::~Plane()
{
}

bool Plane::intersect(const Ray &ray, Hit &hit) const
{
    /// TODO
    Point3f o = ray.origin;
    Vector3f d = ray.direction;
    float t = ((-o + m_position).dot(m_normal)) / (d.dot(m_normal));

    if ((m_position - o).dot(m_normal) == 0 || d.dot(m_normal) == 0 || t == 0)
    {
        return false;
    }
    else if (t > 0)
    {
        hit.setT(t);
        hit.setShape(this);
        const Normal3f normal = o + t * d - m_position;
        hit.setNormal(normal);
        return true;
    }
    else
    {
        return false;
    }
}

REGISTER_CLASS(Plane, "plane")
