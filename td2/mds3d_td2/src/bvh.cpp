
#include "bvh.h"
#include "mesh.h"
#include <iostream>

void BVH::build(const Mesh *pMesh, int targetCellSize, int maxDepth)
{
    // store a pointer to the mesh
    m_pMesh = pMesh;
    // allocate the root node
    m_nodes.resize(1);

    if (m_pMesh->nbFaces() <= targetCellSize)
    { // only one node
        m_nodes[0].box = pMesh->AABB();
        m_nodes[0].first_face_id = 0;
        m_nodes[0].is_leaf = true;
        m_nodes[0].nb_faces = m_pMesh->nbFaces();
        m_faces.resize(m_pMesh->nbFaces());
        for (int i = 0; i < m_pMesh->nbFaces(); ++i)
        {
            m_faces[i] = i;
        }
    }
    else
    {
        // reserve space for other nodes to avoid multiple memory reallocations
        m_nodes.reserve(std::min<int>(2 << maxDepth, std::log(m_pMesh->nbFaces() / targetCellSize)));

        // compute centroids and initialize the face list
        m_centroids.resize(m_pMesh->nbFaces());
        m_faces.resize(m_pMesh->nbFaces());
        for (int i = 0; i < m_pMesh->nbFaces(); ++i)
        {
            m_centroids[i] = (m_pMesh->vertexOfFace(i, 0).position + m_pMesh->vertexOfFace(i, 1).position + m_pMesh->vertexOfFace(i, 2).position) / 3.f;
            m_faces[i] = i;
        }

        // recursively build the BVH, starting from the root node and the entire list of faces
        buildNode(0, 0, m_pMesh->nbFaces(), 0, targetCellSize, maxDepth);
    }
}

bool BVH::intersect(const Ray &ray, Hit &hit) const
{
    // compute the intersection with the root node
    float tMin, tMax;
    Normal3f n;
    ::intersect(ray, m_nodes[0].box, tMin, tMax, n);

    // TODO
    // vérifier si on a bien une intersection (en fonction de tMin, tMax, et hit.t()), et si oui appeler intersecNode...
    if ((::intersect(ray, m_nodes[0].box, tMin, tMax, n)) || tMin < hit.t() || tMax > 0)
    {
        return intersectNode(0, ray, hit);
    }
    return false;
}

bool BVH::intersectNode(int nodeId, const Ray &ray, Hit &hit) const
{
    // TODO, deux cas: soit mNodes[nodeId] est une feuille (il faut alors intersecter les triangles du noeud),
    // soit c'est un noeud interne (il faut visiter les fils (ou pas))

    float tMin_right, tMax_right, tMin_left, tMax_left;
    Normal3f normal;
    int son_right, son_left;
    if (m_nodes[nodeId].first_child_id)
    {

        if (::intersect(ray, m_nodes[0].box, tMin_right, tMax_right, normal) || tMin_right < hit.t())
        {
            son_right = intersectNode(m_nodes[nodeId].first_child_id, ray, hit);
            return son_right;
        }
        else if (::intersect(ray, m_nodes[0].box, tMin_left, tMax_left, normal) || tMin_left < hit.t())
        {
            son_left = intersectNode(m_nodes[nodeId].first_child_id + 1, ray, hit);
            return son_left;
        }
    }

    if (m_nodes[nodeId].is_leaf)
    {
        // nombre de triangle du noeud-> m_nodes[nodeId].nb_faces
        for (int i = 0; i < m_nodes[nodeId].nb_faces; i++)
        {
            // intersecter les triangles du noeud:
            Hit h;
            m_pMesh->intersectFace(ray, h, m_faces[i]);
            if (h.t() < hit.t() && h.t() > 0)
            {
                // hit = h;
                hit.setT(h.t());
                hit.setNormal(h.normal());
                hit.setShape(h.shape());
            }
        }
        return true;
    }
    return false;
}

/** Sorts the faces with respect to their centroid along the dimension \a dim and spliting value \a split_value.
 * \returns the middle index
 */
int BVH::split(int start, int end, int dim, float split_value)
{
    int l(start), r(end - 1);
    while (l < r)
    {
        // find the first on the left
        while (l < end && m_centroids[l](dim) < split_value)
            ++l;
        while (r >= start && m_centroids[r](dim) >= split_value)
            --r;
        if (l > r)
            break;
        std::swap(m_centroids[l], m_centroids[r]);
        std::swap(m_faces[l], m_faces[r]);
        ++l;
        --r;
    }
    return m_centroids[l][dim] < split_value ? l + 1 : l;
}

void BVH::buildNode(int nodeId, int start, int end, int level, int targetCellSize, int maxDepth)
{
    Node &node = m_nodes[nodeId];

    // étape 1 : calculer la boite englobante des faces indexées de m_faces[start] à m_faces[end]
    // (Utiliser la fonction extend de Eigen::AlignedBox3f et la fonction mpMesh->vertexOfFace(int) pour obtenir les coordonnées des sommets des faces)

    std::vector<std::vector<Point3f>> vertice;
    for (int i = start; i < end; i++)
    {
        // donner la position de chaque sommet
        vertice[i][0] = m_pMesh->vertexOfFace(m_faces[i], 0).position;
        vertice[i][1] = m_pMesh->vertexOfFace(m_faces[i], 1).position;
        vertice[i][2] = m_pMesh->vertexOfFace(m_faces[i], 2).position;
        node.box.extend(vertice[i][0]);
        node.box.extend(vertice[i][1]);
        node.box.extend(vertice[i][2]);
    }

    // étape 2 : déterminer si il s'agit d'une feuille (appliquer les critères d'arrêts)

    // Si c'est une feuille, finaliser le noeud et quitter la fonction

    // Si c'est un noeud interne :

    // level =profondeur actuelle?
    if (maxDepth <= level)
    {
        node.is_leaf = true;
        node.first_face_id = start;
        return;
    }
    else if (node.first_child_id)
    {
        // étape 3 : calculer l'index de la dimension (x=0, y=1, ou z=2) et la valeur du plan de coupe
        // (on découpe au milieu de la boite selon la plus grande dimension)
        // étape 4 : appeler la fonction split pour trier (partiellement) les faces et vérifier si le split a été utile
        // étape 5 : allouer les fils, et les construire en appelant buildNode...

        Vector3f sizes = node.box.sizes();
        int coord;
        if (sizes.x() > sizes.y() && sizes.x() > sizes.z())
        {
            coord = 0;
        }
        else if (sizes.y() > sizes.x() && sizes.y() > sizes.z())
        {
            coord = 1;
        }
        else if (sizes.z() > sizes.x() && sizes.z() > sizes.y())
        {
            coord = 2;
        }
        // la valeur du plan de coupe:
        float splitvalue = coord / 2;

        int Split = split(start, end, coord, splitvalue);

        // fils droit et fils gauche:
        buildNode(node.first_child_id, start, end, level + 1, targetCellSize, maxDepth);
        buildNode(node.first_child_id + 1, start, end, level + 1, targetCellSize, maxDepth);
    }
}
