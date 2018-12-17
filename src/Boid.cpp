#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include "Boid.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h> /// methods to create primitives - torus, sphere, and built in prims
#include <ngl/ShaderLib.h> /// another singleton class, ways of managing itself
#include <ngl/NGLStream.h> /// implements all ostream operators, for printing
#include <iostream>
#include <ngl/Random.h>
#include <ngl/fmt/format.h>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <ngl/NGLMessage.h>


void Boid::updateCollection()
{
    for(auto &c : m_collection)
        c.clear();

    for(auto &m : m_meshes) /// need memory otherwise we dont get a draw - not a copy
    {
        switch(m.type)
        {
            case Boid::BoidMeshType::CONE : m_collection[0].push_back(&m); break;
        }
    }
}

void Boid::update()
{

    for(size_t i=0; i<m_collection.size(); ++i)
    {

         for(auto &_m:m_collection[i])
         {
            _m->vel.set(0,2,0);
            _m->pos  += _m->vel;

            std::cout << "pos......." << _m->pos << '\n';

         }
    }
}
