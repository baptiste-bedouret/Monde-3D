#include "integrator.h"
#include "scene.h"

class NormalIntegrator : public Integrator
{
public:
    NormalIntegrator(const PropertyList &props)
    {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const
    {
        /** TODO : Find the surface that is visible in the requested direction
                   Return its ambiant color */

        Hit hit;
        scene->intersect(ray, hit);
        if (hit.foundIntersection())
        {
            float xbis = abs(hit.normal().x());
            float ybis = abs(hit.normal().y());
            float zbis = abs(hit.normal().z());
            Color3f colornormal(xbis, ybis, zbis);

            return colornormal;
        }
        else
        {
            return scene->backgroundColor();
        }
    }

    std::string
    toString() const
    {
        return "NormalIntegrator[]";
    }
};

REGISTER_CLASS(NormalIntegrator, "normals")
