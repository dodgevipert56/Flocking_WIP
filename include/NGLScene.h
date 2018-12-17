#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include <ngl/Vec3.h>
#include "WindowParams.h"
#include <ngl/Transformation.h> /// pos rot and scale
#include <ngl/Mat4.h>
#include <ngl/AbstractVAO.h>
/// this must be included after NGL includes else we get a clash with gl libs
#include <QOpenGLWindow>
#include <QTime>
///----------------------------------------------------------------------------------------------------------------------
//// @file NGLScene.h
//// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
//// @author Jonathan Macey
//// @version 1.0
//// @date 10/9/13
//// Revision History :
//// This is an initial version used for the new NGL6 / Qt 5 demos
//// @class NGLScene
//// @brief our main glwindow widget for NGL applications all drawing elements are
//// put in this file
///----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
  public:
    NGLScene();
    ///----------------------------------------------------------------------------------------------------------------------
    //// @brief ctor for our NGL drawing class
    //// @param [in] parent the parent window to the class
    ///----------------------------------------------------------------------------------------------------------------------
    NGLScene(size_t _numMeshes);
    ///----------------------------------------------------------------------------------------------------------------------
    //// @brief dtor must close down ngl and release OpenGL resources
    ///----------------------------------------------------------------------------------------------------------------------
    ~NGLScene() override;
    ///----------------------------------------------------------------------------------------------------------------------
    //// @brief the initialize class is called once when the window is created and we have a valid GL context
    //// use this to setup any default GL stuff
    ///----------------------------------------------------------------------------------------------------------------------
    void initializeGL() override;
    ///----------------------------------------------------------------------------------------------------------------------
    //// @brief this is called everytime we want to draw the scene
    ///----------------------------------------------------------------------------------------------------------------------
    void paintGL() override;
    void timerEvent(QTimerEvent *_event) override;
    ///----------------------------------------------------------------------------------------------------------------------
    //// @brief this is called everytime we resize the window
    ///----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h) override;





private:

    ///----------------------------------------------------------------------------------------------------------------------
    //// @brief Qt Event called when a key is pressed
    //// @param [in] _event the Qt event to query for size etc
    ///----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event) override;
    ///----------------------------------------------------------------------------------------------------------------------
    //// @brief this method is called every time a mouse is moved
    //// @param _event the Qt Event structure
    ///----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event ) override;
    ///----------------------------------------------------------------------------------------------------------------------
    //// @brief this method is called everytime the mouse button is pressed
    //// inherited from QObject and overridden here.
    //// @param _event the Qt Event structure
    ///----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event) override;
    ///----------------------------------------------------------------------------------------------------------------------
    //// @brief this method is called everytime the mouse button is released
    //// inherited from QObject and overridden here.
    //// @param _event the Qt Event structure
    ///----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event ) override;

    ///----------------------------------------------------------------------------------------------------------------------
    //// @brief this method is called everytime the mouse wheel is moved
    //// inherited from QObject and overridden here.
    //// @param _event the Qt Event structure
    ///----------------------------------------------------------------------------------------------------------------------
    void wheelEvent(QWheelEvent *_event) override;
    //// @brief windows parameters for mouse control etc.
    WinParams m_win;
    //// position for our model
    ngl::Vec3 m_modelPos;

    ngl::Mat4 m_view;
    ngl::Mat4 m_project;
    void loadMatrixToShader(const ngl::Mat4 &_tx, const ngl::Vec4 &_colour);

    /// no need to use smart pointers since its a pod type therefore can construct and destruct
    ///std::vector<std::vector<Boid::BoidData *>> m_collection; /// 2D structure --- 2D ARRAY LIST

    std::unique_ptr<ngl::AbstractVAO> m_vao; /// use this to populate data and draw


    struct Vertex /// represent vertex
    {
        ngl::Vec3 pos;
        ngl::Vec4 colour;
    };

    void spawnBoids(); /// not const becuz it a mutate
   // void updateCollection();
    void prune();


/// My implementations ------------------------------------------------------------------------
    //std::vector<Boid::BoidData> getNeighbours(Boid _boids, float _radius);





};



#endif
