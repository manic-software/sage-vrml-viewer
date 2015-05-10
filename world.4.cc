#include "world.4.hh"

#ifdef QT4
#include <QMouseEvent>
#endif

#include <qtimer.h>
#include <q3textstream.h>
#include <qfile.h>
#include <qapplication.h>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <cmath>
#include <algorithm>
#include <iomanip>

#include <GL/glu.h>

//the shperes are drawn a standard size and then scaled in paintGL
//the tracks are not done this way, since they do not have enough symmetry
#define BLOB_SIZE 50.
//culling depth for objects
//do not display objects closer than this
#define DEPTH_MIN 1.
//or further away than this
#define DEPTH_MAX 100000.
//numberical id's for opengl objects
//lowest id's are used for tracks
//after that, there are offsets
//#define HIT_OFFSET (100000)

//3d image widget derived from GL capable widget
World::
World(QWidget* parent):
  QGLWidget(parent)
{
  //by default, mouse does x-y shift
  m_action=MOVE;
  //origin sphere default size
  m_origsize=500;
  //and position
  m_origposn=DEFORIG;
  //not yet generated
  m_origin=m_nothing=-1;

  //what to show
  m_showtracks=true;
  m_showfulldet=false;
  m_showwireframe=true;

  //background colour
  m_bgr=m_bgg=m_bgb=0.6;

  //accept focus to pick up keyboard events
  //(these are picked up by Gui
#ifdef QT4
  setFocusPolicy(Qt::StrongFocus);
#else
  setFocusPolicy(QWidget::StrongFocus);
#endif
  //catch mouse movement event if no button is pressed down
  setMouseTracking(true);

  //apply initial conditions
  init();
}

//mouse action by user
//press button
void World::
mousePressEvent(QMouseEvent* e)
{
  //if holding down left button
  if (e->button()==Qt::LeftButton)
    {
      //note current mouse position
      mousex=e->x();
      mousey=e->y();
    }
  //if the right button, we are going to want to see if it was on a track
  else if (e->button()==Qt::RightButton)
    {
      int track=clickfindGL(e->x(), e->y());
      if (track<=detector.size() && track >0)
	{
	  //tell any listening functions that track selection has taken place
	  //if the click was outside the tracks, "track" will be zero, otherwise track is number
	  emit select(track-1);
	}
    }

}
//move mouse
void World::
mouseMoveEvent(QMouseEvent* e)
{
  //if holding down left button
  if (e->state()==Qt::LeftButton)
    {
      //this is where we have moved to
      int newx=e->x();
      int newy=e->y();
      int deltax=newx-mousex;
      int deltay=newy-mousey;
      //add move to list of geometry moves
      addmove(deltax, deltay);
      //note current mouse position for next move
      mousex=newx;
      mousey=newy;
    }
  //no mouse button is down
  else if (e->state()==Qt::NoButton)
    {
      //create new "information" string for bottom right of display
      QString newinfo;
      //if pointing at a detector component, the name
      int track=clickfindGL(e->x(), e->y());
      if (track>0)
	newinfo=detector[track-1].name;
      //if this information has changed since last time
      if (newinfo!=mouseinfo)
	{
	  //display the new information
	  mouseinfo=newinfo;
	  paintGL();
	  updateGL();
	}
    }
}
 void World::
   mouseDoubleClickEvent(QMouseEvent* e)
 {
   //if the right button, we are going to want to see if it was on a track
   if (e->button()==Qt::RightButton)
    {
      int track=clickfindGL(e->x(), e->y());
      if (track<=detector.size() && track >0)
	{
	  //tell any listening functions that track selection has taken place
	  //if the click was outside the tracks, "track" will be zero, otherwise track is number
	  emit hide(track-1);
	}
    }
 }

//find location where right click took place
int World::
clickfindGL(int x, int y)
{
  GLuint buff[5120]={0};
  GLint hits, viewport[4];
  //get the display width/height etc
  glGetIntegerv(GL_VIEWPORT, viewport);
  //create buffer to hold information on objects consistent with click location
  glSelectBuffer(5120, buff);
  //switch to gl mode for finding objects
  glRenderMode(GL_SELECT);
  //initialise name list so that object numbers will be included in buffer
  glInitNames();
  //re-initialise the perspective etc as in resizeGL
  glMatrixMode(GL_PROJECTION);
  //back-up the normal display settings
  glPushMatrix();
  //go to gl standard display settings
  glLoadIdentity();
  //define region to look for objects in
  gluPickMatrix(GLdouble(x), GLdouble(viewport[3]-y-1), 1.0, 1.0, viewport);
  //gluPickMatrix(GLdouble(width()/2.), GLdouble(height()/2.), 30.0, 30.0, viewport);
  //re-initialise the perspective etc as in resizeGL
  GLfloat w = (float) width() / (float) height();
  GLfloat h = 1.0;
  glFrustum(-w, w, -h, h, DEPTH_MIN, DEPTH_MAX);
  //perform standard rendering of objects exactly as when displaying on screen
  paintGL();
  //revert to display settings from back-up (glPushMatrix above)
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  //switch back to drawing mode
  //as we do so, opengl gives us the number of shapes it thinks we may have clicked on, and fills buff with the details
  hits=glRenderMode(GL_RENDER);
//   cout << hits << " hits\n";
//look for the front track from the list of candidates
  int track=0;
  unsigned zmin;
  GLuint* ptr=buff;
  //go through all cansisates
  for (int i=0; i<hits; i++)
    //if this is a track and either it's the first, or it's closer than the one we have
    //    if (buff[i*4+3]>0)
      {
	//assume that this is the track we're after
	int num=*ptr;
	ptr++;
	int z=*ptr;
	ptr+=2;
	if (num>0)
	  {
	    int t=*ptr;
	    if (t>0 && (!track || z<zmin))
	      {
		zmin=z;
		track=t;
	      }
	    ptr+=num;
	  }
      }
  return track;

}
//find location where right click took place
void World::
zorderGL(void)
{
  //  timeval tv1, tv2;
  //gettimeofday(&tv1, NULL);
  //detector object ordering map
  detorder.clear();
  //set up gl buffer
  GLuint buff[102400]={0};
  GLint hits, viewport[4];
  //get the display width/height etc
  //glGetIntegerv(GL_VIEWPORT, viewport);
  //create buffer to hold information on objects consistent with click location
  glSelectBuffer(102400, buff);
  //switch to gl mode for finding objects
  glRenderMode(GL_SELECT);
  //initialise name list so that object numbers will be included in buffer
  glInitNames();
  //re-initialise the perspective etc as in resizeGL
  //glMatrixMode(GL_PROJECTION);
  //back-up the normal display settings
  //glPushMatrix();
  //go to gl standard display settings
  //glLoadIdentity();
  //define region to look for objects in
  //int x=376;
  //int y=119;
  //x=300;y=200;
  //gluPickMatrix(GLdouble(x), GLdouble(y), 200., 200., viewport);
  //gluPickMatrix(GLdouble(width()/2.), GLdouble(height()/2.), 30.0, 30.0, viewport);
  //re-initialise the perspective etc as in resizeGL
  //GLfloat w = (float) width() / (float) height();
  //GLfloat h = 1.0;
  //glFrustum(-w, w, -h, h, DEPTH_MIN, DEPTH_MAX);
  //perform standard rendering of objects exactly as when displaying on screen
  paintGL();
  //revert to display settings from back-up (glPushMatrix above)
  //glMatrixMode(GL_PROJECTION);
  //glPopMatrix();
  //switch back to drawing mode
  //as we do so, opengl gives us the number of shapes it thinks we may have clicked on, and fills buff with the details
  hits=glRenderMode(GL_RENDER);
  //cout << hits << " hits\n";
//look for the front track from the list of candidates
  int track=-1;
  unsigned zmin;
  GLuint* ptr=buff;
  //go through all cansisates
  for (int i=0; i<hits; i++)
    //if this is a track and either it's the first, or it's closer than the one we have
    //    if (buff[i*4+3]>0)
      {
	//assume that this is the track we're after
	zmin=buff[i*4+1];
	int track=buff[i*4+3];
	//	cout << "hit " << i << endl;
	int names=*ptr;
	//	cout << "   #names " << (*ptr) << endl;
	ptr++;
	//	cout << "   z1 " << ((float)(*ptr)/0x7fffffff) << endl;
	unsigned z=*ptr;
	ptr++;
	//	cout << "   z2 " << ((float)(*ptr)/0x7fffffff) << endl;
	ptr++;
	//	cout << "   name list:" << endl;
	for (int j=0; j<names; j++)
	  {
	    if (!j && (*ptr)>0)
	      detorder.push_back(ZOrder((*ptr)-1, z));
	    //	    cout << "      " << (*ptr) << endl;
	    ptr++;
	  }
	//	cout << endl;
      }
  std::sort(detorder.begin(), detorder.end());
  //  gettimeofday(&tv2, NULL);
  //   cout << "z ordering took " << (tv2.tv_usec-tv1.tv_usec) << " microseconds\n";
//     cout << "detector ordering contains " << detorder.size() << " points\n";
//    cout << "number of detector components is " << detector.size() << endl;
//   cout << "detector ordering is as follows\n";
  //  for (int i=0; i<detorder.size(); i++)
  //    cout << detorder[i].id << "   " << detorder[i].z << endl;
}
//key press
void World::
keyPressEvent(QKeyEvent* e)
{
  //if shift, ctrl, alt etc
  if (e->state()!=Qt::NoButton && e->state()!=Qt::Keypad)
    //ignore
    return;
  //calibration - match to a mouse move of this many pixels
  const int KEY2PIXELS=10;

  //arrow keys, same effect as mouse
  if (e->key()==Qt::Key_Left)
    {
      //add appropriate move
      addmove(-KEY2PIXELS, 0);
    }
  else if (e->key()==Qt::Key_Right)
    {
      //add appropriate move
      addmove(KEY2PIXELS, 0);
    }
  else if (e->key()==Qt::Key_Up)
    {
      //add appropriate move
      addmove(0, -KEY2PIXELS);
    }
  else if (e->key()==Qt::Key_Down)
    {
      //add appropriate move
      addmove(0, KEY2PIXELS);
    }
  //different keys will apply different navigation steps 
  //for these keys match the navigation mode to the key
  else if (e->key()==Qt::Key_A)
    {
      //a will be move left
      //set navigation mode accordingly
      set_action(MOVE);
      emit modechange(MOVE);
      //add appropriate move
      addmove(KEY2PIXELS, 0);
    }
  else if (e->key()==Qt::Key_D)
    {
      //move right
      //set navigation mode accordingly
      emit modechange(MOVE);
      set_action(MOVE);
      //add appropriate move
      addmove(-KEY2PIXELS, 0);
    }
  else if (e->key()==Qt::Key_S)
    {
      //move down
      //set navigation mode accordingly
      emit modechange(MOVE);
      set_action(MOVE);
      //add appropriate move
      addmove(0, -KEY2PIXELS);
    }
  else if (e->key()==Qt::Key_W)
    {
      //move up
      //set navigation mode accordingly
      emit modechange(MOVE);
      set_action(MOVE);
      //add appropriate move
      addmove(0, KEY2PIXELS);
    }
  else if (e->key()==Qt::Key_Y)
    {
      //zoom in
      //set navigation mode accordingly
      emit modechange(ZOOM);
      set_action(ZOOM);
      //add appropriate move
      addmove(0, -KEY2PIXELS);
    }
  else if (e->key()==Qt::Key_H)
    {
      //zoom out
      //set navigation mode accordingly
      emit modechange(ZOOM);
      set_action(ZOOM);
      //add appropriate move
      addmove(0, KEY2PIXELS);
    }
  else if (e->key()==Qt::Key_F)
    {
      //pan down
      //set navigation mode accordingly
      emit modechange(PAN);
      set_action(PAN);
      //add appropriate move
      addmove(0, KEY2PIXELS);
    }
  else if (e->key()==Qt::Key_R)
    {
      //pan up
      //set navigation mode accordingly
      emit modechange(PAN);
      set_action(PAN);
      //add appropriate move
      addmove(0, -KEY2PIXELS);
    }
  else if (e->key()==Qt::Key_E)
    {
      //pan left
      //set navigation mode accordingly
      emit modechange(PAN);
      set_action(PAN);
      //add appropriate move
      addmove(-KEY2PIXELS, 0);
    }
  else if (e->key()==Qt::Key_T)
    {
      //pan right
      //set navigation mode accordingly
      emit modechange(PAN);
      set_action(PAN);
      //add appropriate move
      addmove(KEY2PIXELS, 0);
    }
  else if (e->key()==Qt::Key_G)  //american for full stop
    {
      //twist left
      //set navigation mode accordingly
      emit modechange(ZOOM);
      set_action(ZOOM);
      //add appropriate move
      addmove(-KEY2PIXELS, 0);
    }
  else if (e->key()==Qt::Key_J) //proper slash, not backslash
    {
      //twist right
      //set navigation mode accordingly
      emit modechange(ZOOM);
      set_action(ZOOM);
      //add appropriate move
      addmove(KEY2PIXELS, 0);
    }
  else if (e->key()==Qt::Key_I)
    {
      //move origin out
      //set navigation mode accordingly
      emit modechange(ORIGIN);
      set_action(ORIGIN);
      //add appropriate move
      addmove(0, -KEY2PIXELS);
    }
  else if (e->key()==Qt::Key_K)
    {
      //move origin in
      //set navigation mode accordingly
      emit modechange(ORIGIN);
      set_action(ORIGIN);
      //add appropriate move
      addmove(0, KEY2PIXELS);
    }
  else if (e->key()==Qt::Key_O) //square bracket
    {
      //mke origin blob bigger
      //set navigation mode accordingly
      emit modechange(ORIGIN);
      set_action(ORIGIN);
      //add appropriate move
      addmove(KEY2PIXELS, 0);
    }
  else if (e->key()==Qt::Key_U)  //square bracket
    {
      //make origin blob smaller
      //set navigation mode accordingly
      emit modechange(ORIGIN);
      set_action(ORIGIN);
      //add appropriate move
      addmove(-KEY2PIXELS, 0);
    }
}


//set up GL lighting and other basics
void World::
initializeGL(void)
{
  //background colour
  glClearColor(m_bgr, m_bgg, m_bgb, 0.0);
  //enable transparency
  glEnable (GL_BLEND); 
  //transparency algorithm
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //disable backface culling
  glDisable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  //fire up basic surface lighting
  glEnable(GL_LIGHTING);

  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  //glEnable(GL_COLOR_MATERIAL);

  // Create light components
  //first 3 numbers are colout, last is kind of strength (0-1)
  GLfloat ambientLight[] = { 0.0f, 0.0f, 0.0f, 0.5f };
  GLfloat diffuseLight[] = { 0.05f, 0.05f, 0.05f, 0.05f };
  //  GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 0.5f };
  //position of light: x, y, z, be positional (1 for true)
  GLfloat position[] = { -100000., 0., 0., 1.};

  //lighting
  //this is a bit of a dark art
  // Assign created components to GL_LIGHT0
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  //  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHT0);
  position[0]=0.;
  position[2]=100000.;
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
  //  glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT1, GL_POSITION, position);
  glEnable(GL_LIGHT1);

  //ensure that foreground objects appear in front of background ones
  glEnable( GL_DEPTH_TEST );

  //prepare cross-hair
  //just 2 lines in the middle of the screen
  GLfloat crosshaircol[]={0., 0., 0., 1.};
  //a new opengl object, gets an integer address
  crosshair=glGenLists(1);
  glNewList(crosshair, GL_COMPILE);
  //set colour
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, crosshaircol);
  //see function, this draws the lines
  docrosshair();
  //end of this gl object
  glEndList();

  //similarly generate origin sphere
  //the blob which appears when navigation 
  GLfloat centrecol[4] = {0.3, 0.3, 0.3, 1.0 };
  m_origin=glGenLists(1);
  glNewList(m_origin, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, centrecol);
  //see function
  blob();
  glEndList();	

  //this is so that the system has something to draw at startup otherwise gl mis-behaves
  GLfloat nothingcol[4] = {0.0, 0.0, 0.0, 0.0 };
  m_nothing=glGenLists(1);
  glNewList(m_nothing, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, nothingcol);
  blob();
  glEndList();	


}

//window resized
void World::
resizeGL( int width, int height )
{
    // setup the size of the window to draw in... we want to use the entire window
    glViewport(0, 0, width, height);
    
    // setup the projection matrix.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //perspective and depth of field
    GLfloat w = (float) width / (float) height;
    GLfloat h = 1.0;
    glFrustum(-w, w, -h, h, DEPTH_MIN, DEPTH_MAX);
    glEnable(GL_DEPTH);    


}

//window needs re-painting
void World::
paintGL(void)
{
  //reset painter
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //switch to drawing mode
  glMatrixMode(GL_MODELVIEW);
  //reset translation matrix
  glLoadIdentity();

    //track net translations
    Move tot;
    memset(&tot, 0, sizeof(Move));
//     tot.x+=INITX;
//     tot.y+=INITY;
//     tot.z+=INITZ;

    //move into position for crosshair
    glTranslatef(0, 0, -100);
    //draw croshair if in x-y translation mode
    if (m_action==MOVE)
      glCallList(crosshair);
    //move back to effect chosen position of rotation vertex
    glTranslatef(0, 0, -(m_origposn-100));
        
    //if we are working on moving the origin
    //draw origin
    if (m_action==ORIGIN)
      {
	//adjust scale to effect proper shpere size
	glScalef(m_origsize/BLOB_SIZE, m_origsize/BLOB_SIZE, m_origsize/BLOB_SIZE);
	//draw standard sphere
	glCallList(m_origin); 
	//put scale back
	glScalef(BLOB_SIZE/m_origsize, BLOB_SIZE/m_origsize, BLOB_SIZE/m_origsize);
      }
    else
      {
	//draw the invisible sphere mentioned earlier to avoid lighting problem when there are no spheres present
	glScalef(1e-3, 1e-3, 1e-3);
	glCallList(m_nothing);
	glScalef(1e3, 1e3, 1e3);
      }

    //in order to properly compound rotations and the translations interspersed between them, the rotation axes must be changed after each rotation and following motion must be applied to the rotated axes
    //this is a bitch
    double xax[3]={1,0,0};
    double yax[3]={0,1,0};
    double zax[3]={0,0,1};
    //go through each rotation and translation
    for (vector<Move>::iterator it=m_moves.begin(); it!=m_moves.end(); it++)
      {
	//if this one is a rotation
	if (it->is_rot)
	  {
	    double cang;
	    double sang;
	    double r[3][3];
	    double n[3];
	    //if it is a rotation about the x axis
	    if (fabs(it->x)>fabs(it->y) && fabs(it->x)>fabs(it->z))
	      {
		//apply rotation to world
		glRotatef(it->x, xax[0], xax[1], xax[2]);
		//apply rotation to axes
		cang=cos(it->x*M_PI/180.);
		sang=sin(it->x*M_PI/180.);
		r[0][0]=xax[0];
		r[0][1]=xax[1];
		r[0][2]=xax[2];
		r[1][0]=yax[0]*cang-zax[0]*sang;
		r[1][1]=yax[1]*cang-zax[1]*sang;
		r[1][2]=yax[2]*cang-zax[2]*sang;
		r[2][0]=yax[0]*sang+zax[0]*cang;
		r[2][1]=yax[1]*sang+zax[1]*cang;
		r[2][2]=yax[2]*sang+zax[2]*cang;
		n[0]=sqrt(r[0][0]*r[0][0]+r[0][1]*r[0][1]+r[0][2]*r[0][2]);
		n[1]=sqrt(r[1][0]*r[1][0]+r[1][1]*r[1][1]+r[1][2]*r[1][2]);
		n[2]=sqrt(r[2][0]*r[2][0]+r[2][1]*r[2][1]+r[2][2]*r[2][2]);
		xax[0]=r[0][0]/n[0];
		xax[1]=r[0][1]/n[0];
		xax[2]=r[0][2]/n[0];
		yax[0]=r[1][0]/n[1];
		yax[1]=r[1][1]/n[1];
		yax[2]=r[1][2]/n[1];
		zax[0]=r[2][0]/n[2];
		zax[1]=r[2][1]/n[2];
		zax[2]=r[2][2]/n[2];
	      }
	    //rotation about y
	    else if (fabs(it->y)>fabs(it->x) && fabs(it->y)>fabs(it->z))
	      {
		//to world
		glRotatef(it->y, yax[0], yax[1], yax[2]);
		//to axes
		cang=cos(it->y*M_PI/180.);
		sang=sin(it->y*M_PI/180.);
		r[0][0]=xax[0]*cang+zax[0]*sang;
		r[0][1]=xax[1]*cang+zax[1]*sang;
		r[0][2]=xax[2]*cang+zax[2]*sang;
		r[1][0]=yax[0];
		r[1][1]=yax[1];
		r[1][2]=yax[2];
		r[2][0]=-xax[0]*sang+zax[0]*cang;
		r[2][1]=-xax[1]*sang+zax[1]*cang;
		r[2][2]=-xax[2]*sang+zax[2]*cang;
		n[0]=sqrt(r[0][0]*r[0][0]+r[0][1]*r[0][1]+r[0][2]*r[0][2]);
		n[1]=sqrt(r[1][0]*r[1][0]+r[1][1]*r[1][1]+r[1][2]*r[1][2]);
		n[2]=sqrt(r[2][0]*r[2][0]+r[2][1]*r[2][1]+r[2][2]*r[2][2]);
		xax[0]=r[0][0]/n[0];
		xax[1]=r[0][1]/n[0];
		xax[2]=r[0][2]/n[0];
		yax[0]=r[1][0]/n[1];
		yax[1]=r[1][1]/n[1];
		yax[2]=r[1][2]/n[1];
		zax[0]=r[2][0]/n[2];
		zax[1]=r[2][1]/n[2];
		zax[2]=r[2][2]/n[2];
	      }
	    //about z
	    else if (fabs(it->z)>fabs(it->x) && fabs(it->z)>fabs(it->y))
	      {
		//to worldf
		glRotatef(it->z, zax[0], zax[1], zax[2]);
		//to axes
		cang=cos(it->z*M_PI/180.);
		sang=sin(it->z*M_PI/180.);
		r[0][0]=xax[0]*cang-yax[0]*sang;
		r[0][1]=xax[1]*cang-yax[1]*sang;
		r[0][2]=xax[2]*cang-yax[2]*sang;
		r[1][0]=xax[0]*sang+yax[0]*cang;
		r[1][1]=xax[1]*sang+yax[1]*cang;
		r[1][2]=xax[2]*sang+yax[2]*cang;
		r[2][0]=zax[0];
		r[2][1]=zax[1];
		r[2][2]=zax[2];
		n[0]=sqrt(r[0][0]*r[0][0]+r[0][1]*r[0][1]+r[0][2]*r[0][2]);
		n[1]=sqrt(r[1][0]*r[1][0]+r[1][1]*r[1][1]+r[1][2]*r[1][2]);
		n[2]=sqrt(r[2][0]*r[2][0]+r[2][1]*r[2][1]+r[2][2]*r[2][2]);
		xax[0]=r[0][0]/n[0];
		xax[1]=r[0][1]/n[0];
		xax[2]=r[0][2]/n[0];
		yax[0]=r[1][0]/n[1];
		yax[1]=r[1][1]/n[1];
		yax[2]=r[1][2]/n[1];
		zax[0]=r[2][0]/n[2];
		zax[1]=r[2][1]/n[2];
		zax[2]=r[2][2]/n[2];
	      }
	  }
	//translation
	else
	  {
	    //calculate rotation applied to current axes
	    double tx, ty, tz;
	    tx=it->x*xax[0]+it->y*yax[0]+it->z*zax[0];
	    ty=it->x*xax[1]+it->y*yax[1]+it->z*zax[1];
	    tz=it->x*xax[2]+it->y*yax[2]+it->z*zax[2];
	    //add to total translation
	    tot.x+=tx;
	    tot.y+=ty;
	    tot.z+=tz;
	  }
      }

    //apply net transform
    glTranslatef(tot.x, tot.y, tot.z);

    //draw tracks
    if (m_showtracks)
      {
	for (int i=0; i<tracks.size(); i++)
	  {
	    Shape& s=tracks[i];
	    if (s.show)
	      glCallList(s.gl_wire);
	  }
      }

    //then detector
    //first wire frame
    if (m_showwireframe)
      {
	for (int i=detector.size()-1; i>=0; i--)
	  {
	    Shape& s=detector[i];
	    glPushName(i+1);
	    if (m_showwireframe && s.show)
	      {
		glCallList(s.gl_wire);
	      }
	    glPopName();
	  }
      }
    //then solids
    if (m_showfulldet)
      {
	if (detorder.size())
	  {
	    for (int i=0; i<detorder.size(); i++)
	      {
		Shape& s=detector[detorder[i].id];
		glPushName(detorder[i].id+1);
		if (s.show)
		  {
		    glCallList(s.gl_full);
		  }
		glPopName();
	      }
	  }
	else
	  {
	    //draw detector
	    for (int i=detector.size()-1; i>=0; i--)
	      {
		Shape& s=detector[i];
		glPushName(i+1);
		if (s.show)
		  {
		    glCallList(s.gl_full);
		  }
		glPopName();
	      }
	  }
      }

    //write our current position etc to screen
    //lights must be turned off for writing
    glDisable(GL_LIGHTING);
//     int fx=tot.x*xax[0]+tot.y*yax[0]+(tot.z-m_origposn)*zax[0];
//     int fy=tot.x*xax[1]+tot.y*yax[1]+(tot.z-m_origposn)*zax[1];
//     int fz=tot.x*xax[2]+tot.y*yax[2]+(tot.z-m_origposn)*zax[2];
    int fx=tot.x-m_origposn*zax[0];
    int fy=tot.y-m_origposn*zax[1];
    int fz=tot.z-m_origposn*zax[2];
	    //    cout << "position: " << fx << ", " << fy << ", " << fz << endl;
    //QString posn=QString("position: ")+QString::number(-(int)tot.x)+QString(", ")+QString::number(-(int)tot.y)+QString(", ")+QString::number(-(int)tot.z-m_origposn);
	    QString posn=QString("position: ")+QString::number(fx)+QString(", ")+QString::number(fy)+QString(", ")+QString::number(fz);
    glColor3f(0.,0.,0.);
    renderText(10, 20, posn);
    posn=QString("origin: ")+QString::number(m_origposn);
    renderText(width()-fontMetrics().width(posn)-10, 20, posn);

    //direction of view
    float rx=xax[0];
    float ry=yax[0];
    float rz=zax[0];
    float rr=sqrt(rx*rx+ry*ry+rz*rz);
    //QString theta=QString::number(acos(rx/sqrt(rx*rx+ry*ry))*180./M_PI, 'f', 1);
    //QString phi=QString::number(asin(sqrt(rx*rx+ry*ry)/rr)*180./M_PI, 'f', 1);
    QString theta=QString::number(atan2(ry, rx)*180./M_PI, 'f', 1);
    QString phi=QString::number(atan2(rz, rr)*180./M_PI, 'f', 1);
    QString thetastr("T=");
    QString degstr("d");
    QString phistr("P=");
#ifdef QT4
    phistr[0]=QChar(0x03c6);
    thetastr[0]=QChar(0x03d1);
    degstr[0]=QChar(0x00b0);
#endif
    QString dirn=thetastr+theta+degstr+QString(", ")+phistr+phi+degstr;
    renderText(10, height()-10, dirn);

    //what is the mouse pointing at
    renderText(width()-fontMetrics().width(mouseinfo)-10, height()-10, mouseinfo);

    //lights back on
    glEnable(GL_LIGHTING);
    //done
    glFlush();

}

//construct sphere out of triangles
void World::
blob(void)
{
  //number of traingles to use
   const int steps=20;
   //pre-calculate angles to save time later
   static float cosines[steps+1][steps/2][3];
   static float sines[steps+1][steps/2][3];
   static bool trig=false;
   if (!trig)
     {
       trig=true;
       for (unsigned short j=0; j<steps/2; j++)
	 {
	   float theta1=j*2*M_PI/steps-M_PI/2;
	   float theta2=(j+1)*2*M_PI/steps-M_PI/2;
	   for (unsigned short i=0; i<=steps; i++)
	     {
	       float theta3=i*2*M_PI/steps;
	       cosines[i][j][0]=cos(theta1);
	       cosines[i][j][1]=cos(theta2);
	       cosines[i][j][2]=cos(theta3);
	       sines[i][j][0]=sin(theta1);
	       sines[i][j][1]=sin(theta2);
	       sines[i][j][2]=sin(theta3);
	     }
	 }
     }
   //build from triangles
   glBegin( GL_TRIANGLE_STRIP );
   //honestly i copied this from the net.  best not to look at it too hard
   for (unsigned short j=0; j<steps/2; j++)
     {
       for (unsigned short i=0; i<=steps; i++)
	 {
	   float x=cosines[i][j][1]*cosines[i][j][2];
	   float y=sines[i][j][1];
	   float z=cosines[i][j][1]*sines[i][j][2];
	   glNormal3f(x, y, z);
	   glTexCoord2f(i*1./steps, 2*(j+1)*1./steps);
	   glVertex3f(BLOB_SIZE*x, BLOB_SIZE*y, BLOB_SIZE*z);
	   x=cosines[i][j][0]*cosines[i][j][2];
	   y=sines[i][j][0];
	   z=cosines[i][j][0]*sines[i][j][2];
	   glNormal3f(x, y, z);
	   glTexCoord2f(i*1./steps, 2*(j)*1./steps);
	   glVertex3f(BLOB_SIZE*x, BLOB_SIZE*y, BLOB_SIZE*z);
	 }
     }
   //done
   glEnd();
}


//a cross in the centre of the view
void World::
docrosshair(void)
{
  //just a couple of lines
  const float size=10;
  glBegin(GL_LINE_STRIP);
  glVertex2d(0, -size);
  glVertex2d(0, size);
  glVertex2d(0, 0);
  glVertex2d(-size, 0);
  glVertex2d(size, 0);
  glEnd();
}
//effect of mouse action by user
Action World::
get_action(void)
{
  return m_action;
}
void World::
set_action(Action action)
{
  m_action=action;
  //update
  paintGL();
  updateGL();
  
}
//add a cell hit to internal array
void World::
add(Shape s)
{
  //render shape in gl
  render_shape(s);
  //  cout << "end of list\n";
  //add to appropriate array
  if (s.faces)
    detector.push_back(s);
  else
    tracks.push_back(s);
}
//perform necessary gl magic to create shape
void World::
render_shape(Shape& s)
{  
  //cleanup first
  if (s.gl_full)
    glDeleteLists(s.gl_full, 1);
  if (s.gl_wire)
    glDeleteLists(s.gl_full, 1);
  //set colour
  //4'th number is opacity
  GLfloat col[]={s.r, s.g, s.b, 0.0};
  col[3]=m_opacity;
  //keep track of where we are in the coordinate index array
  int i=0;
  //if this shape has faces, not a squiggly line
  if (s.faces)
    {
      //filled
      //create new gl object
      s.gl_full=glGenLists(1);
      glNewList(s.gl_full, GL_COMPILE);
      //set colour
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col);
      //repeat until end of coordinate index array
      while (i<s.index.size())
	{
	  //create a polygon with arbitrary number of sides
	  glBegin(GL_POLYGON);
	  //repeat until we see a -1 in the coordinate index, indicating end of polygon, or until we reach the end of the coordinate index
	  while (i<s.index.size() && s.index[i]!=-1)
	    {
	      //add this point to the polygon
	      //if (s.points[s.index[i]].y<0.)
	      glVertex3d(s.points[s.index[i]].x, s.points[s.index[i]].y, s.points[s.index[i]].z);
	      //move to the next point
	      i++;
	    }
	  //we've hit a -1, move on to the next coordinate index point
	  i++;
	  //end this polygon
	  glEnd();
	}
      //end of coordinate index, therefore end of shape
      //there may be many polygons within, but it sows up as 1 opengl object
      glEndList();
      i=0;
    }
  //wireframe, whether the shape has faces or not, we shall prepare a wireframe version
  //create new opengl ovhect
  s.gl_wire=glGenLists(1);
  glNewList(s.gl_wire, GL_COMPILE);
  //same colour, but make fully opaque
  col[3]=1.0;
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col);
  //repeat process as above
  while (i<s.index.size())
    {
      //but use lines rather than polygons
      glBegin(GL_LINE_STRIP);
      //      cout << "new line\n";
      while (i<s.index.size() && s.index[i]!=-1)
	{
	  //geant often produces tracks with steps in them of zero length
	  if (!i || s.index[i-1]==-1 || s.points[s.index[i]].x!=s.points[s.index[i-1]].x || s.points[s.index[i]].y!=s.points[s.index[i-1]].y || s.points[s.index[i]].z!=s.points[s.index[i-1]].z)
	    {
	      glVertex3d(s.points[s.index[i]].x, s.points[s.index[i]].y, s.points[s.index[i]].z);
	      //	      cout << "vertex " << i << " at " << s.points[s.index[i]].x << ", " << s.points[s.index[i]].y << ", " <<  s.points[s.index[i]].z << endl;
	    }
	  i++;
	}
      i++;
      glEnd();
      //      cout << "end of line\n";
    }
  glEndList();

}

//go to default viewpoint
void World::
reset(bool doinit)
{
  //clear all user actions
  //and go back to initial viewpoint
  m_origsize=500;
  m_origposn=DEFORIG;
//   //destroy origin blob  
//   glDeleteLists(m_origin, 1);
   m_moves.clear();
   //if initial transforms are not disabled
   if (doinit)
     //perform them
     init();
   //no information on bottom right of display
   mouseinfo="";
  //redraw
   zorderGL();
  paintGL();
  updateGL();
}
//initial transforms based on constants in header
void World::
init(void)
{
  //default shape opacity
  m_opacity=0.3;
  //if the initial rotation about the x-axis is not zero 
#if INITRX
  //define a matching rotation about the axis
   Move rx;
   rx.is_rot=true;
   rx.x=INITRX;
   //add it to the list of transforms
   m_moves.push_back(rx);
#endif
   //similarly for an initial rotation about the y-axis
#if INITRY
   Move ry;
   ry.is_rot=true;
   ry.y=INITRY;
   m_moves.push_back(ry);
#endif
   //and the z-axis
#if INITRZ
   Move rz;
   rz=is_rot=true;
   rz.z=INITRZ;
   m_moves.push_back(rz);
#endif
   //if the initial x (left-right) transform is non-zero
#if INITX
   //define a matching translation move
   Move tx;
   tx.x=INITX;
   //add it to the list of moves
   m_moves.push_back(tx);
#endif
   //similarly for y (up-down)
#if INITY
   Move ty;
   ty.y=INITY;
   m_moves.push_back(ty);
#endif
   //and for z (forward-back)
#if INITZ
   Move tz;
   tz.z=INITZ;
   m_moves.push_back(tz);
#endif
}
//forget current set of hits and go to default viewpoint
void World::
clear(void)
{
  //forget shapes and tracks
  for (int i=0; i<detector.size(); i++)
    {
      if (detector[i].gl_full>=0)
	glDeleteLists(detector[i].gl_full, 1);
      if (detector[i].gl_wire>=0)
	glDeleteLists(detector[i].gl_wire, 1);
    }
  detector.clear();
  for (int i=0; i<tracks.size(); i++)
    if (tracks[i].gl_wire>=0)
      glDeleteLists(tracks[i].gl_wire, 1);
  tracks.clear();
  //go to default viewpoint
  reset();
}
//what to show
bool World::
show_tracks(void)
{
  return m_showtracks;
}
void World::
show_tracks(bool s)
{
  m_showtracks=s;
  //re-paint
  //zorderGL();
  paintGL();
  updateGL();
}
bool World::
show_fulldet(void)
{
  return m_showfulldet;
}
void World::
show_fulldet(bool s)
{
  m_showfulldet=s;
  //re-paint
  //zorderGL();
  paintGL();
  updateGL();
}
bool World::
show_wireframe(void)
{
  return m_showwireframe;
}
void World::
show_wireframe(bool s)
{
  m_showwireframe=s;
  //re-paint
  //zorderGL();
  paintGL();
  updateGL();
}
//move to focus on a specific point
void World::
seek(float x, float y, float z)
{
  //remove all previous moves
  reset(false);
  m_origposn=30;
  m_origsize=3;
  //move in x
  Move m;
  m.is_rot=false;
  m.x=-x;
  m.y=-y;
  m.z=-z;
  m_moves.push_back(m);
  //re-paint
  zorderGL();
  paintGL();
  updateGL();
}
//add a move to the list of moves of the geometry
void World::
addmove(int deltax, int deltay)
{
  //determine proper effect of mouse move
  Move move;
  memset(&move, 0, sizeof(Move));
  //x/y translation
  if (m_action==MOVE)
    {
      //horizontal
      float shift1=m_origposn*1./width()*deltax;
      move.x=shift1;
      //vertical
      float shift2=m_origposn*1./height()*(-deltay);
      move.y=shift2;

    }
  //z translation/rotation about z-axis
  else if (m_action==ZOOM)
    {
      //choose translation
      if (abs(deltay)>abs(deltax))
	{
	  //note size of translation
	  float shift=3000./height()*(-deltay);
	  move.z=shift;
	}
      //or rotation, not both
      else
	{
	  //size of rotation
	  move.is_rot=true;
	  move.z=180./height()*(-deltax);
	}
    }
  //rotation about x or y axis
  else if (m_action==PAN)
    {
      move.is_rot=true;
      //choose axis and scale
      if (abs(deltay)>abs(deltax))
	move.x=-180./height()*(-deltay)/3.;
      else
	move.y=180./width()*(deltax)/3.;
    }
  //move rotation vertex
  else if (m_action==ORIGIN)
    {
      //move vertex
      if (abs(deltay)>abs(deltax))
	{
	  //move origin and shift self back to compensate
	  float shift=m_origposn/height()*(-deltay);
	  //not having the vertex right on top of, or behind viewer
	  if (m_origposn<10)
	    {
	      shift-=m_origposn-10;
	      m_origposn=10;
	    }
	  //move self
	  move.z=shift;
	  //shift origin
	  m_origposn+=shift;
	}
      else
	{
	  m_origsize+=m_origsize*(deltax)/10.;
	  if (m_origsize<1)
	    m_origsize=1;
	}
    }
  //record this viewpoint change
  if (m_action!=ORIGIN || move.z!=0)
    m_moves.push_back(move);
  //update world
   zorderGL();
  paintGL();
  updateGL();
  
}

//access shape list
int World::
get_numshapes(void)
{
  return detector.size();
}
const Shape& World::
get_shape(int i)
{
  return detector.at(i);
}
//set shape visibility
void World::
set_shapevis(int i, bool v, bool ref)
{
  bool change=false;
  //apply visibility change
  if (i>=0 && i<detector.size() && detector[i].show!=v)
    {
      detector[i].show=v;
      change=true;
    }
  //update display
  if (ref && change)
    {
      zorderGL();
      paintGL();
      updateGL();
    }
}
//from outside, force refresh
void World::
refresh(void)
{
  zorderGL();
  paintGL();
  updateGL();
}
//change surface opacity
void World::
opacity(float o)
{
  if (o<0) o=0;
  if (o>1) o=1;
  //change opacity
  m_opacity=o;
  //re-render affected shapes in gl
  for (int i=0; i<detector.size(); i++)
    render_shape(detector[i]);
  //refresh
  paintGL();
  updateGL();
}
//return shape opacity
float World::
opacity(void)
{
  return m_opacity;
}
//set background colour
void World::
background(float r, float g, float b)
{
  if (r>1) r=1;
  if (r<0) r=0;
  if (g>1) g=1;
  if (g<0) g=0;
  if (b>1) b=1;
  if (b<0) b=0;
  m_bgr=r;
  m_bgg=g;
  m_bgb=b;
  //apply colour as background
  glClearColor(m_bgr, m_bgg, m_bgb, 0.0);
  //update display
  paintGL();
  updateGL();
}
float World::
background_red(void)
{
  return m_bgr;
}
float World::
background_green(void)
{
  return m_bgg;
}
float World::
background_blue(void)
{
  return m_bgb;
}
