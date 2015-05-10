#ifndef EDAM_WORLD
#define EDAM_WORLD

//3d viewer

#include <qgl.h>
#include <vector>
using std::vector;

#include <iostream>

//geometric defaults
//default viewpoint in 3d space
//this is the distance back from the origin
//it shows up as an initial negative z-shift, so bear that when choosing the other init values below
//transforms are applied in the order in which they appear here
#define DEFORIG 10000
//default rotation of default viewpoint
#define INITRX -90
#define INITRY 0
#define INITRZ 0
//shifts following those rotations
//the x shift is left-right, the y up-down and the z forward-back
//they are applied not along the original axes but from the perspective of the viewer
#define INITX 0
#define INITY 0
#define INITZ 0

//macros for squaring.  This comes up a lit
#define SQUARE(x) ((x)*(x))

//a translation or rotation by the user
class Move
{
public:
  Move(void)
  {
    is_rot=false;
    x=y=z=0;
  }
  bool is_rot;
  float x;
  float y;
  float z;
};

//action on mouse input
enum Action
  {
    MOVE=0,
    ZOOM=1,
    PAN=2,
    ORIGIN=3,
  };

//useful 3-vector class for various purposes
class ThreeVector
{
public:
  ThreeVector()
  {
    x=y=z=0;
  }
  double& operator[](int i)
  {
    if (i<1)
      return x;
    else if (i>1)
      return z;
    else
      return y;
  }
  double x, y, z;
};

//chunk of detector geometry pulled from vrml file
class Shape
{
public:
  Shape(QString n, int re)
  {
    //the name from the comment immediately preceeding the shape definition
    name=n;
    //and the replica number
    rep=re;
    //shape made of faces?  defaults to no indicating lines
    faces=false;
    gl_wire=gl_full=-1;
    r=g=b=0;
    //default to show shape
    show=true;
    

  }

public:
  //gl handle for wire-frame rendering
  GLint gl_wire;
  //gl handle for surface rendering
  GLint gl_full;
  //the name from the comment immediately preceeding the shape definition
  QString name;
  //replica number from the same
  int rep;
  //colour;
  float r,g,b;
  //some vrml objects do not have surfaces, only lines
  bool faces;
  //the points encompassing the surface
  vector<ThreeVector> points;
  //a map of between which points to draw lines
  //closely linked to the vrml representation
  vector<int> index;
  //show this shape or not
  bool show;
};

class ZOrder
{
public:
  ZOrder(void)
  {
    id=0;
    z=0;
  }
  ZOrder(int sid, int sz)
  {
    id=sid;
    z=sz;
  }
  bool operator<(const ZOrder& other) const
  {
    return other.z<z;
  }
  int id;
  unsigned z;
};

//3d image widget derived from GL capable widget
class World : public QGLWidget
{
  Q_OBJECT
  
public:
  World(QWidget*);

private:
  //handles for gl constructions
  //crosshair which is seen in Move naviation moce
  GLint crosshair;
  //sphwre representing centre in Origin/Centre mode
  GLint m_origin;
  //dummy sphere to trick gl into behaving when no other spheres are present
  GLint m_nothing;
  //view changes by the user
  //these have to be accumulated for navigation to work properly
  vector<Move> m_moves;
  //size of central vertex sphere
  float m_origsize;
  //its distance from the viewer
  float m_origposn;
  //current mouse co-ordinates on screen
  int mousex, mousey;
  //what to do with user mouse action
  Action m_action;
  //detector geometry
  vector<Shape> detector;
  //order in which detector objects should be drawn
  vector<ZOrder> detorder;
  //tracks
  vector<Shape> tracks;
  //what to show
  bool m_showtracks;
  bool m_showfulldet;
  bool m_showwireframe;
  //what is the mouse pointing at right now
  QString mouseinfo;
  //a number between 0 and 1 to indicate shape opacity
  float m_opacity;
  //background colour
  float m_bgr, m_bgg, m_bgb;

private:
  //set up GL lighting and other basics
  void initializeGL(void);
  //window resized
  void resizeGL(int, int);
  //window needs re-painting
  void paintGL(void);
  //mouse action by user
  void mousePressEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent*);
  void mouseDoubleClickEvent(QMouseEvent*);
  //find location where right click took place
  int clickfindGL(int, int);
  //order shapes by z in current rotation/translation frame
  void zorderGL(void);
  //key press
  void keyPressEvent(QKeyEvent*);

private:
  //construct sphere out of triangles
  void blob(void);
  //a cross in the centre of the view
  void docrosshair(void);

public:
  //effect of mouse action by user
  Action get_action(void);
  void set_action(Action);
  //add a volume or track to internal array
  void add(Shape);
  //perform gl magic to render a shape or track
  void render_shape(Shape&);
  //shift the viewpoint from outside this class
//   void set_rotation(GLfloat, GLfloat, GLfloat);
//   void set_position(GLfloat, GLfloat, GLfloat);
  //go to default viewpoint
  void reset(bool=true);
  //apply initial transforms
  void init(void);
  //forget current set of shape and tracks and go to default viewpoint
  void clear(void);
  //change in geometry
  void geomchange(void);
  //what to show
  bool show_fulldet(void);
  void show_fulldet(bool);
  bool show_wireframe(void);
  void show_wireframe(bool);
  bool show_tracks(void);
  void show_tracks(bool);
  //move to focus on a specified point
  void seek(float, float, float);
  //add a move to the list of moves of the geometry
  void addmove(int, int);
  //access shape list - use with caution
  //access shape list
  int get_numshapes(void);
  const Shape& get_shape(int);
  //set shape visibility
  void set_shapevis(int, bool, bool=true);
  //from outside, force refresh
  void refresh(void);
  //change surface opacity
  void opacity(float);
  //return shape opacity
  float opacity(void);
  //set background colour
  void background(float, float, float);
  float background_red(void);
  float background_green(void);
  float background_blue(void);

signals:
  //navigation mode changed
  void modechange(Action);
  //right click on a volume
  void select(int);
  //double-click on a volume
  void hide(int);
};

#endif //ifndef EDAM_WORLD
