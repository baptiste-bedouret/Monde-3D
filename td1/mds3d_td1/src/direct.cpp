#include "integrator.h"
#include "scene.h"

class DirectIntegrator : public Integrator
{
public:
    DirectIntegrator(const PropertyList &props)
    {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const
    {

        Color3f R;
        Hit hit;
        float distance;
        scene->intersect(ray, hit);
        if (hit.foundIntersection())
        {
            for (auto light : scene->lightList())
            {
                Point3f x = ray.at(hit.t());
                Color3f brdf = hit.shape()->material()->brdf(ray.direction, -light->direction(x), hit.normal());
                Color3f p = std::max(light->direction(x).dot(hit.normal()), float(0)) * light->intensity(x);

                // Ombres portées:
                Ray new_ray = Ray(x, light->direction(x));
                Hit new_hit;
                scene->intersect(new_ray, new_hit);
                // Aucun objet ne bloque la lumière
                if (!new_hit.foundIntersection() && new_hit.t() > distance)
                {
                    R += brdf * p;
                }
            }
            return R;
        }

        return scene->backgroundColor();
    }

    std::string
    toString() const
    {
        return "DirectIntegrator[]";
    }
};

REGISTER_CLASS(DirectIntegrator, "direct")
