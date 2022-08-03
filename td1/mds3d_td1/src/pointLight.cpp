#include "light.h"

class PointLight : public Light
{
public:
    PointLight(const PropertyList &propList)
        : Light(propList.getColor("intensity", Color3f(1.f)))
    {
        m_position = propList.getPoint("position", Point3f::UnitX());
    }

    Vector3f direction(const Point3f &x, float *dist = 0) const
    {
        /// TODO
        // m_position => position de la lumiere
        Vector3f direction = m_position - x;

        return direction;
    }

    Color3f intensity(const Point3f &x) const
    {
        /// TODO
        Vector3f dir = m_position - x;
        // Vector3f dir = direction(x, 0);
        Color3f intensity = m_intensity / dir.squaredNorm();
        return intensity;
    }

    std::string toString() const
    {
        return tfm::format(
            "PointLight[\n"
            "  intensity = %s\n"
            "  position = %s\n"
            "]",
            m_intensity.toString(),
            ::toString(m_position));
    }

protected:
    Point3f m_position;
};

REGISTER_CLASS(PointLight, "pointLight")
