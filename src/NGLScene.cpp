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

NGLScene::NGLScene(size_t _numMeshes)
{
  /// re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle(fmt::format("Blank NGL Num Meshes {0}", _numMeshes).c_str()); ///fmt - lib python and c style formats with place holder {0} or %d stuff
  Boid test;

  test.m_meshes.resize(_numMeshes);
  spawnBoids(); /// call spawnBoids()
  test.m_collection.resize(4); /// only one object "CONE" by default needed was 4

  ///boidUpdate();
  startTimer(10); /// ----------- Timer
}

void Boid::addBoidClick(Boid::BoidMeshType _m)
{
    Boid test;

    ngl::Random *rng = ngl::Random::instance();
    Boid::BoidData m;
    m.pos = rng->getRandomPoint(70, 70, 70);
    m.rot.m_y =rng->randomPositiveNumber(360.0f);
    m.scale.set(1.0f, 1.0f, 1.0f);
    m.colour = rng->getRandomColour4();
    m.type = _m;
    test.m_meshes.push_back(m);
}

void NGLScene::spawnBoids() /// not const becuz it a mutate
{

    Boid test;

    ngl::Random *rng = ngl::Random::instance(); /// rand class
    for (auto &m : test.m_meshes) /// its going to mutate the value in there and need a copy
    {
        m.pos = rng->getRandomPoint(70, 70, 70); /// on a plane the position with given range 40 in this case
        m.rot.m_y =rng->randomPositiveNumber(360.0f); /// 0 to 360
        m.scale.set(1.0f, 1.0f, 1.0f);
        m.colour = rng->getRandomColour4();
        int type = static_cast<int>(rng->randomPositiveNumber(1));
        switch(type)
        {
            case 0 : m.type = Boid::BoidMeshType::CONE; break;
        }
    }
}

NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}


void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_project = ngl::perspective(45.0f, static_cast<float>(_w)/_h,
                               0.5f, 400.0f); ///FOV , last are near and far clipping planes

}

constexpr auto *ColourShader = "ColourShader"; /// In compile time replaced COLOUR SHADER

void NGLScene::initializeGL()
{
      /// we need to initialise the NGL lib which will load all of the OpenGL functions, this must
      /// be done once we have a valid GL context but before we call any GL commands. If we dont do
      /// this everything will crash
      ngl::NGLInit::instance();
      glClearColor(0.8f, 0.8f, 0.8f, 1.0f);			   /// Grey Background- BACKGROUND
      /// enable depth testing for drawing
      glEnable(GL_DEPTH_TEST);
      /// enable multisampling for smoother drawing
      glEnable(GL_MULTISAMPLE);

    ngl::VAOPrimitives::instance()->createCone("cone", 1.0f, 2.5, 10, 1); /// initialise cone with descriptions
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  shader->loadShader(ColourShader, "shaders/ColourVertex.glsl", ///loading the shaders from the ones we created
                      "shaders/ColourFragment.glsl");



  m_view = ngl::lookAt({0.0f, 100.0f, 50.0f},  ///gen a func to simulate glu lookat, 4*4 matrix
                       ngl::Vec3::zero(),   /// return a 0 matrix
                       ngl::Vec3::up());    /// return a 0 matrix

}

void NGLScene::loadMatrixToShader(const ngl::Mat4 &_tx, const ngl::Vec4 &_colour) /// getMatrix is a const cant be mutable or something like that
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ColourShader); ///activate shader

    shader->setUniform("MVP", m_project*m_view*_tx); /// using proj and view identity matrices from NGLScene - draw objects

    shader->setUniform("vertColour", _colour); /// give color
}


void NGLScene::paintGL()
{

  Boid test;

  /// clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);

  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  ngl::Mat4 mouseRotation;
  rotX.rotateX(m_win.spinXFace); /// rot around x axis and in angles according to mouse press down
  rotY.rotateY(m_win.spinYFace);
  mouseRotation = rotY*rotX;

  ngl::Transformation tx;
  auto *prim = ngl::VAOPrimitives::instance();
  for(auto m : test.m_meshes)
  {
      tx.setPosition(m.pos);  /// set Position for paint
      tx.setRotation(m.rot);
      tx.setScale(m.scale);
      loadMatrixToShader(mouseRotation*tx.getMatrix(), m.colour);

      switch(m.type)
      {
        case Boid::BoidMeshType::CONE : prim->draw("cone"); break;
      }
   }


}


void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  /// this method is called every time the main window recives a key event.
  /// we then switch on the key value and set the camera in the GLWindow
    Boid test;

  switch (_event->key())
  {
  /// escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());

  break;
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break; /// wireframe draw
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break; /// solid draw
  case Qt::Key_C : if(test.m_meshes.size()<=0) test.m_meshes.resize(100); spawnBoids(); break; /// draw each time, even after all gone

  ///case Qt:: : addMesh(MeshType::CONE); break;

  default : break;
  }
  /// finally update the GLWindow and re-draw

    update();
}

///All the Movement happens here
void NGLScene::timerEvent(QTimerEvent *_event)
{
    ///ngl::NGLMessage::addMessage("timer", Colours::YELLOW, TimeFormat::TIMEDATE);
    Boid test;
    test.updateCollection();
    test.update();

    std::cout << "Timer event called......" << '\n';
    const float pi = 3.14f;

    ngl::Vec3 center;


    update();
}

//// timer calculations -- get time like Delta time
/*
    QTime myTimer;
    myTimer.start();
    /// do something..
    int dTime = myTimer.elapsed();
*/


/*
    for(size_t i=0; i<m_collection.size(); ++i)
    {
        for(auto _m :m_collection[i]) /// all of the cones
        {
            center += _m->pos;
        }

        center /= m_collection[i].size(); /// doing this gives the center

        for(auto &m:m_collection[i])
        {
            m->dir =center - m->pos; ///length between the two objects
            m->distance = m->dir.length(); ///overall distance
            std::cout << "Length: " << m->distance << "\n";

            m->dir.normalize();
            m->pos+=m->dir * 0.25f; ///speed

            auto yaw = std::atan2(m->dir.m_x, m->dir.m_z); ///finding the look at angle based on the direction headed
            yaw = yaw * (180.0f/pi);

            float xz = sqrtf((m->dir.m_x * m->dir.m_x) + (m->dir.m_z * m->dir.m_z)); ///getting the xz plane

            auto pitch = std::atan( m->dir.m_y / xz);
            pitch = pitch * (180.0f/pi);

            std::cout << pitch << ".........PITCH_OLD" << '\n';

            m->rot.m_y = yaw;    ///------- YAW
            m->rot.m_x = -pitch; ///------- PITCH

            std::cout << m->rot.m_x << ".........PITCH_NEW" << '\n';

        }
    }

*/
