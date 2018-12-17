#ifndef BOID_H_
#define BOID_H_

#include <ngl/Vec3.h>
#include "WindowParams.h"
#include <ngl/Transformation.h> // pos rot and scale
#include <ngl/Mat4.h>
#include <ngl/AbstractVAO.h>
#include "NGLScene.h"

// this must be included after NGL includes else we get a clash with gl libs
#include <QOpenGLWindow>

class Boid
{
public:


    enum class BoidMeshType : char {CONE};

    WinParams m_win;

    struct BoidData
    {
        ngl::Vec3 pos;   // no need _ cuz of lite data structures
        ngl::Vec3 vel;
        ngl::Vec3 acc;

        ngl::Vec3 dir;
        ngl::Vec3 scale;
        ngl::Vec3 rot;    // all this is a pod type
        ngl::Vec4 colour;
        BoidMeshType type;
        float distance;

        BoidData(const ngl::Vec3 &_pos, const ngl::Vec3 &_scale, // constructor intialisation
                 const ngl::Vec3 &_rot, const ngl::Vec4 &_colour,
                 BoidMeshType _type) :
            pos(_pos), scale(_scale), rot(_rot), colour(_colour),
            type(_type){}
        BoidData() = default;
        BoidData(const BoidData &) = default;
        ~BoidData() = default;

    };

    std::vector<std::vector<BoidData *>> m_collection; // 2D structure --- 2D ARRAY LIST
    std::vector<BoidData> m_meshes; /// making object or instance ---- LIST

    void addBoidClick(BoidMeshType _m);

    void updateCollection();
    void update();

    ngl::Vec3 cohesion();
    ngl::Vec3 separation();
    ngl::Vec3 allignment();

};


#endif
