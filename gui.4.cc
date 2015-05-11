#include "gui.4.hh"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <q3textstream.h>
#include <qfile.h>
#include <qtimer.h>
#include <qfiledialog.h>
#include <q3process.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qstyle.h>
#include <qiconset.h>
#include <qcursor.h>

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <sstream>

#include <qapplication.h>

#include <sys/stat.h>

//couts etc which give blow by blow accounting of progress reading json file
#define VRML_DEBUG 0

//menu item id numbers which allow menu entries to be referred to later and modified
//record movie
#define REC_ID 10
//set navigation move to Move
#define MOVE_ID (20+MOVE)
//set to Pan
#define PAN_ID (20+PAN)
//set to zoom
#define ZOOM_ID (20+ZOOM)
//set to move origin
#define ORIGIN_ID (20+ORIGIN)
//set to display tracks or not
#define TRACKS_ID 30
//detector wireframe
#define WIREFRAME_ID 32
//detector surfaces
#define FULLDET_ID 33
//window list
#define WINLIST_OFFSET 100

//macro orders qt main loop to update display and respond to any other events
//needed to make progress bars display changes made in a loop and other such things
//annoyingly qt4 method is different enough from qt3 to require this wrapping
#ifdef QT4
#define RUNEVENTS(a) qApp->processEvents(QEventLoop::AllEvents, a)
#else
#define RUNEVENTS(a) qApp->processEvents(a);
#endif

const char * lineedit_xpm[] = {
"16 16 175 2",
"  	c None",
". 	c #FFFFFF",
"+ 	c #F1F1F3",
"@ 	c #CECED4",
"# 	c #8B8B9A",
"$ 	c #7B7B8C",
"% 	c #7A7A8C",
"& 	c #898998",
"* 	c #CDCDD3",
"= 	c #F0F0F2",
"- 	c #C2C2CA",
"; 	c #54546B",
"> 	c #3F3F5D",
", 	c #686880",
"' 	c #77778D",
") 	c #78788D",
"! 	c #6B6B82",
"~ 	c #444461",
"{ 	c #515167",
"] 	c #C0C0C7",
"^ 	c #A4A4B1",
"/ 	c #353553",
"( 	c #7F7F97",
"_ 	c #C6C6D6",
": 	c #F2F2F8",
"< 	c #FDFDFE",
"[ 	c #FEFEFE",
"} 	c #F8F8FC",
"| 	c #D0D0DE",
"1 	c #85859C",
"2 	c #373755",
"3 	c #A0A0AB",
"4 	c #C3C3CB",
"5 	c #2F2F4E",
"6 	c #8B8BA3",
"7 	c #D2D2E0",
"8 	c #D6D6E1",
"9 	c #CFCFDC",
"0 	c #CFCFDB",
"a 	c #CECEDB",
"b 	c #D3D3E0",
"c 	c #D7D7E3",
"d 	c #9C9CB2",
"e 	c #C0C0C8",
"f 	c #F3F3F5",
"g 	c #55556B",
"h 	c #676784",
"i 	c #B8B8CC",
"j 	c #C3C3D2",
"k 	c #F6F6F7",
"l 	c #CECEDA",
"m 	c #B1B1C4",
"n 	c #B3B3C6",
"o 	c #ACACC0",
"p 	c #E4E4EB",
"q 	c #E7E7ED",
"r 	c #C6C6D7",
"s 	c #7B7B95",
"t 	c #505066",
"u 	c #F3F3F4",
"v 	c #CECED5",
"w 	c #353554",
"x 	c #8B8BA6",
"y 	c #A4A4BC",
"z 	c #F3F3F7",
"A 	c #F6F6F9",
"B 	c #B8B8CB",
"C 	c #9A9AB2",
"D 	c #DCDCE4",
"E 	c #A2A2B8",
"F 	c #757591",
"G 	c #2E2E4B",
"H 	c #CCCCD2",
"I 	c #8D8D9C",
"J 	c #474765",
"K 	c #8787A3",
"L 	c #83839F",
"M 	c #A4A4BB",
"N 	c #F5F5F8",
"O 	c #F5F5F7",
"P 	c #E0E0E8",
"Q 	c #8C8CA3",
"R 	c #434364",
"S 	c #3D3D5E",
"T 	c #2D2D4A",
"U 	c #898997",
"V 	c #7F7F90",
"W 	c #484866",
"X 	c #72728F",
"Y 	c #6B6B89",
"Z 	c #656584",
"` 	c #9090A8",
" .	c #F1F1F6",
"..	c #88889F",
"+.	c #353557",
"@.	c #3A3A5A",
"#.	c #3E3E5F",
"$.	c #7A7A8A",
"%.	c #3E3E5E",
"&.	c #565676",
"*.	c #535373",
"=.	c #4C4C6C",
"-.	c #656585",
";.	c #DBDBE3",
">.	c #6F6F8B",
",.	c #333354",
"'.	c #3D3D5D",
").	c #3F3F5F",
"!.	c #333352",
"~.	c #797989",
"{.	c #333351",
"].	c #434363",
"^.	c #585877",
"/.	c #D5D5E0",
"(.	c #F0F0F6",
"_.	c #70708C",
":.	c #373757",
"<.	c #424262",
"[.	c #2C2C49",
"}.	c #888895",
"|.	c #DEDEE8",
"1.	c #8B8BA1",
"2.	c #72728D",
"3.	c #EEEEF5",
"4.	c #252540",
"5.	c #CCCCD1",
"6.	c #55556A",
"7.	c #3D3D5C",
"8.	c #B6B6C8",
"9.	c #FDFDFD",
"0.	c #9696AB",
"a.	c #454566",
"b.	c #424263",
"c.	c #7F7F99",
"d.	c #D3D3DE",
"e.	c #5E5E7C",
"f.	c #393959",
"g.	c #4E4E61",
"h.	c #F2F2F4",
"i.	c #C1C1C8",
"j.	c #515170",
"k.	c #686888",
"l.	c #80809B",
"m.	c #5B5B7B",
"n.	c #5B5B7A",
"o.	c #5C5C7B",
"p.	c #81819B",
"q.	c #6C6C8B",
"r.	c #4F4F6E",
"s.	c #272741",
"t.	c #BEBEC5",
"u.	c #A1A1AC",
"v.	c #343451",
"w.	c #50506E",
"x.	c #777793",
"y.	c #73738F",
"z.	c #777795",
"A.	c #4D4D6B",
"B.	c #2E2E49",
"C.	c #9D9DA7",
"D.	c #BFBFC7",
"E.	c #545468",
"F.	c #373753",
"G.	c #4B4B68",
"H.	c #5F5F7D",
"I.	c #5D5D7C",
"J.	c #494966",
"K.	c #33334F",
"L.	c #4F4F63",
"M.	c #BDBDC4",
"N.	c #7C7C8C",
"O.	c #7B7B8A",
"P.	c #878795",
"Q.	c #CBCBD1",
"R.	c #F0F0F1",
". . . . + @ # $ % & * = . . . . ",
". . . - ; > , ' ) ! ~ { ] . . . ",
". . ^ / ( _ : < [ } | 1 2 3 . . ",
". 4 5 6 7 8 9 0 0 a b c d 5 e . ",
"f g h i j k l m n o p q r s t u ",
"v w x y z [ A B C D [ [ E F G H ",
"I J K L M N [ O P [ [ Q R S T U ",
"V W X Y Z `  .[ [ < ..+.@.#./ $.",
"V %.&.*.=.-.;.[ [  .>.,.'.).!.~.",
"I {.].S ^./.[ <  .[ (._.:.<.[.}.",
"@ [.).=.|.[ [ 1.2.3.[ N h '.4.5.",
"u 6.7.*.8.9.0.a.b.c.N d.e.f.g.h.",
". i.G j.k.l.m.n.o.&.p.q.r.s.t.. ",
". . u.v.w.Y x.X y.z.Y A.B.C.. . ",
". . . D.E.F.G.H.I.J.K.L.M.. . . ",
". . . . = H U N.O.P.Q.R.. . . . "};

ClearLineEdit::
ClearLineEdit(QWidget *parent)
  : QLineEdit(parent),
    btn_clear(this)
{
    QPixmap pixmap(lineedit_xpm);
    btn_clear.setIconSet(QIconSet(pixmap));
    btn_clear.setFixedSize(QSize(16,16));
    btn_clear.setCursor(QCursor(Qt::ArrowCursor));
     btn_clear.hide();
    btn_clear.setFlat(true);
    connect(&btn_clear, SIGNAL(clicked()), this, SLOT(clear()));
    connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(updateCloseButton(const QString&)));

}

void ClearLineEdit::resizeEvent(QResizeEvent *)
{
    QSize sz = btn_clear.size();
    btn_clear.move(rect().right() - sz.width()-(height()-sz.height())/2,
                      (rect().bottom() + 1 - sz.height())/2);
}

void ClearLineEdit::updateCloseButton(const QString& text)
{
  if (text.isEmpty())
    btn_clear.hide();
  else
    btn_clear.show();
}

//constructor
Gui::Gui(void):
  QWidget(),
  //menu widgets
  menu(this),
  mnu_file(this),
  mnu_view(this),
  mnu_help(this),
  layout(this),
  bar(this),
  //list vs World slider
  sides(Qt::Horizontal, this),
  //parent for widgets on left of slider
  left(&sides),
  //layout widgets around World
  box_world(&left),
  //OpenGl world itself
  world(&left),
  //buttons for mouse action in 3d mode
  box(),
  grp_3d(&left),
  btn_move("Move", &left),
  btn_zoom("Zoom/Twist", &left),
  btn_pan("Pan", &left),
  btn_origin("Centre", &left),
  //right side
  right(&sides),
  box_right(&right),
  //filter for following list
  txt_filter(&right),
  //detector component list
  lst_detector(&right)
{
  //hard-coded pdg id's.  there must be a better way
  //may need more
  pdg[22]="gamma";
  pdg[11]="electron";
  pdg[-11]="positron";
  pdg[12]="electron neutrino";
  pdg[-12]="electron anti-neutrino";
  pdg[13]="muon";
  pdg[-13]="anti-muon";
  pdg[14]="muon neutrino";
  pdg[-14]="muon anti-neutrino";
  pdg[15]="tau";
  pdg[-15]="anti-tau";
  pdg[16]="tau neutrino";
  pdg[116]="tau anti-neutrino";
  pdg[111]="pi zero";
  pdg[211]="pion+";
  pdg[-211]="pion-";

  //menu
  //file menu
  menu.insertItem("File", &mnu_file);
  //open a file in this window
  mnu_file.insertItem("Open", this, SLOT(cb_open()), Qt::CTRL+Qt::Key_O);
  //create a new main window - passes back to Application class
  mnu_file.insertItem("New", this, SLOT(cb_new()), Qt::CTRL+Qt::Key_N);
  //mavigation menu
  menu.insertItem("Navigation", &mnu_nav);
  //go back to initial viewpoint
  mnu_nav.insertItem("Reset Viewpoint", this, SLOT(cb_reset()), Qt::CTRL+Qt::Key_0);
  mnu_nav.insertSeparator();
  //swicth navigation mode to 'Move'
  mnu_nav.insertItem("Move", this, SLOT(cb_menumove()), Qt::CTRL+Qt::Key_M, MOVE_ID);
  //Move is the default, so tick it
  mnu_nav.setItemChecked(MOVE_ID, true);
  //switch navigation mode to Pan
  mnu_nav.insertItem("Pan", this, SLOT(cb_menupan()), Qt::CTRL+Qt::Key_P, PAN_ID);
  //switch to zoom
  mnu_nav.insertItem("Zoom/Twist", this, SLOT(cb_menuzoom()), Qt::CTRL+Qt::Key_Z, ZOOM_ID);
  //switch to move origin
  mnu_nav.insertItem("Centre", this, SLOT(cb_menuorigin()), Qt::CTRL+Qt::Key_C, ORIGIN_ID);
  //recording menu
  menu.insertItem("Record", &mnu_rec);
  //single png snapshot
  mnu_rec.insertItem("SnapShot", this, SLOT(cb_snap()), Qt::CTRL+Qt::Key_S);
  //many snapshots at 40 ms intervals to go into movie
  mnu_rec.insertItem("Record Movie", this, SLOT(cb_movie()), Qt::CTRL+Qt::Key_R, REC_ID);
  //view menu
  menu.insertItem("View", &mnu_view);
  //switch to/from full screen
  mnu_view.insertItem("Full Screen", this, SLOT(cb_full()), Qt::CTRL+Qt::Key_F);
  mnu_view.insertSeparator();
  //show space-points or not
  mnu_view.insertItem("Tracks", this, SLOT(cb_tracks()), Qt::CTRL+Qt::SHIFT+Qt::Key_T, TRACKS_ID);
  //show detector wire-frame or not
  mnu_view.insertItem("Wire-Frame Detector", this, SLOT(cb_wireframe()), Qt::CTRL+Qt::SHIFT+Qt::Key_W, WIREFRAME_ID);
  //show detector surfaces or not
  mnu_view.insertItem("Full Detector", this, SLOT(cb_fulldet()), Qt::CTRL+Qt::SHIFT+Qt::Key_F, FULLDET_ID);
  //change opacity of surfaces
  mnu_view.insertSeparator();
  mnu_view.insertItem("Opacity", this, SLOT(cb_opacity()), Qt::CTRL+Qt::SHIFT+Qt::Key_O);
  mnu_view.insertItem("Background Colour", this, SLOT(cb_background()), Qt::CTRL+Qt::SHIFT+Qt::Key_B);
  //set ticks on various "show" items above to indicate what is currently selected
  viewchecks();
  mnu_view.insertSeparator();

  //window list menu, coordinated with all other instances of gui
  menu.insertItem("Window", &mnu_windows);
  //event for any item clicked in Window menu, saves setting up a different one for each item in the Window list
  connect(&mnu_windows, SIGNAL(activated(int)), this, SLOT(cb_window(int)));

  //help menu
  menu.insertItem("Help", &mnu_help);
  mnu_help.insertItem("Manual", this, SLOT(cb_manual()));

#ifndef __APPLE__
  //on windows, the menu has to be packed into the window.  not on mac or linux
  layout.setMenuBar(&menu);
#else
  //on windows, a border around the window edge looks bad.  One mac/linux I like it
  layout.setMargin(5);
#endif

  //add progress bar at top of window, for things which take more than a couple of seconds to do
  
  layout.addWidget(&bar, 1);
  layout.addSpacing(5);

  //create system to distribure main window between viewer and lists
  //a slider appears between them which can be adjusted with the mouse
  layout.addWidget(&sides, 100);

  //the 3d display area should not be tiny
  world.setMinimumSize(600,400);
  //but it should be there...
  world.show();
  //we would like to know if a user right-clicks to select a volums
  connect(&world, SIGNAL(select(int)), this, SLOT(cb_select(int)));
  //we would like to know if a user double-clicks to hide a volume
  connect(&world, SIGNAL(hide(int)), this, SLOT(cb_hide(int)));
  //and if the navigation mode changes
  connect(&world, SIGNAL(modechange(Action)), this, SLOT(cb_modechange(Action)));

  //window caption, will add filename later if we get one
  setCaption(QString("See A Geant Event - ")+QString(VERSION));

  //insert 3d display area into left side
  box_world.addWidget(&world);
  //below that, a box for the navigation buttons
  box_world.addLayout(&box);
  //join 3d navigation buttons into cooperative
  grp_3d.setExclusive(true);
#ifdef QT4
  grp_3d.addButton(&btn_move, MOVE);
  grp_3d.addButton(&btn_zoom, ZOOM);
  grp_3d.addButton(&btn_pan, PAN);
  grp_3d.addButton(&btn_origin, ORIGIN);
#else
  grp_3d.hide();
  grp_3d.insert(&btn_move, MOVE);
  grp_3d.insert(&btn_zoom, ZOOM);
  grp_3d.insert(&btn_pan, PAN);
  grp_3d.insert(&btn_origin, ORIGIN);
#endif
  //lay out 3d navigation buttons below 3d display
  box.addStretch();
  box.addWidget(&btn_move);
  box.addWidget(&btn_pan);
  box.addWidget(&btn_zoom);
  box.addWidget(&btn_origin);
  box.addStretch();

  //right hand side of window
  //detector component filter
  box_right.addWidget(&txt_filter);
  //detector component list
  box_right.addWidget(&lst_detector);
  //set minimum size of list
  lst_detector.setMinimumWidth(300);
  //set flexible selection mode
  lst_detector.setSelectionMode(Q3ListView::Extended);
  //detector filter change
  connect(&txt_filter, SIGNAL(textChanged(const QString&)), this, SLOT(cb_detfilter(const QString&)));
  //detector right click
  connect(&lst_detector, SIGNAL(contextMenuRequested(Q3ListViewItem*, const QPoint&, int)), this, SLOT(cb_detconf(Q3ListViewItem*, const QPoint&)));

  //detector list
  //set up columns
  lst_detector.addColumn("Component", 250);
  //empty out the list
  lst_detector.clear();
  //disable sorting
  lst_detector.setSortColumn(-1);

  //default 3d navigation mode
  btn_move.setChecked(true);

 //callback for 3d navigation mode
#ifdef QT4
  connect(&grp_3d, SIGNAL(buttonClicked(int)), this, SLOT(cb_action(int)));
#else
  connect(&grp_3d, SIGNAL(clicked(int)), this, SLOT(cb_action(int)));
#endif

  //we have not yet been give a file to load
  havefile=false;
  //we are not recording a movie
  m_frame=-1;

}

//open supplied vrml file
void Gui::
loadfile(QString filename)
{
#if VRML_DEBUG
  cout << "added file " <<  filename.ascii() << endl;
#endif
  //clear out existing shapes
  tmpshapes.clear();
  //stoe filename for later
  m_filename=filename;
  //check file exists
  QFile file(filename);
  if (!file.exists())
    {
      QMessageBox::critical(this, "SAGE", QString("File does not exist\n")+filename);
      return;
    }
  //check file is readable
  if (!file.open(IO_ReadOnly))
    {
      QMessageBox::critical(this, "SAGE", QString("Failed to open file\n")+filename+QString("\nPermissions problem?"));
      return ;
    }
  //okay so this is our file
  havefile=true;
  //change window title to include filename
  QFileInfo fi(filename);
  setCaption(QString("See A Geant Event - ")+QString(VERSION)+QString("  :  ")+fi.baseName()+QString(".")+fi.extension());

      //open file
      Q3TextStream ts(&file);
      //check first line
      QString header=ts.readLine();
      if (!header.contains("#VRML"))
	QMessageBox::critical(this, "SAGE", QString("Does not appear to be a VRML file\n")+filename);
      else
	{
#if VRML_DEBUG
      cout << "reading vrml file\n";
#endif
      //size of file in bytes
      int filesize=file.size();
      //keep track of our depth inside the object brackets in the file
      int depth=0;
      //the depth at which we found the beginning of a shape
      int shape=0;
      //the depth at which we found the beginning of an indexed face set
      int ifs=0;
      //similarly for an indexed line set
      int ils=0;
      //the depth at which we found the point array, the vertices in the shape
      int point=0;
      //the depth at which we found the coordinate index, indicating which of the points to join together
      int index=0;
      //the depth at which we found the material description from where we get the colour
      int mat=0;
      Shape* s;
      //the name and replica number of the shape
      QString name;
      int rep=-1;
      //progress through file
      int progress=0;
      int lastprogress=0;
      bar.setTotalSteps(filesize);
      //continue until the end of the file
      while (!ts.atEnd())
	{
	  //read in 1 line
	  QString vrml=ts.readLine();
	  progress+=vrml.length();
	  if (progress-lastprogress>filesize/20)
	    {
	      bar.setProgress(progress);
	      RUNEVENTS(10);
	      lastprogress=progress;
	    }
	  //the "SOLID" comment gives the name of the following volume
	  if (vrml.contains("SOLID:"))
	    {
	      QStringList parts=QStringList::split(QRegExp("[:.]"), vrml, false);
	      if (parts.size()>=3)
		{
		  rep=parts.back().stripWhiteSpace().toInt();
		  parts.pop_back();
		  parts.pop_front();
		  name=parts.join(".").stripWhiteSpace();
		}
	    }
	  //find { and [ brackets in the line and adjust our bracket depth accordingly
	  //this allow us to keep track of what object we're inside
#ifdef QT4
	  //qt4 implementation of "count number of times this character appears
	  depth+=vrml.count("{")+vrml.count("[");
	  depth-=vrml.count("}")+vrml.count("]");
#else
	  //is different from qt3
	  depth+=vrml.contains("{")+vrml.contains("[");
	  depth-=vrml.contains("}")+vrml.contains("]");
#endif
	  //if we thought we were inside a shape
	  //but closing of brackets indicates that we're not
	  if (shape && depth<shape)
	    {
	      //not that we're not in a shape
	      shape=0;
#if VRML_DEBUG
	      cout << "left shape\n\n";
#endif
	      //the shape we have left is complete, so add it to the collection
	      tmpshapes.push_back(*s);
	    }
	  //similarly if we though we were inside an indexed face set 
	  if (ifs && depth<ifs)
	    {
	      ifs=0;
#if VRML_DEBUG
	      cout << "left indexed face set\n";
	      cout << "shape has " << s->points.size() << " points\n";
#endif
	    }
	  //or an indexed line set
	  if (ils && depth<ils)
	    {
	      ils=0;
#if VRML_DEBUG
	      cout << "left indexed line set\n";
	      cout << "shape has " << s->points.size() << " points and " << s->index.size() << " connections\n";
#endif
	    }
	  //or a point array
	  if (point && depth<point)
	    {
	      point=0;
#if VRML_DEBUG
	      cout << "left point array\n";
#endif
	    }
	  //or a coordinate index array
	  if (index && depth<index)
	    {
	      index=0;
#if VRML_DEBUG
	      cout << "left coordinate index\n";
#endif
	    }
	  //or a material description
	  if (mat && depth<mat)
	    {
	      mat=0;
#if VRML_DEBUG
	      cout << "left material description\n";
#endif
	    }
	  //okay that's the matter of leaving objects handled
	  //now onto the object information
	  //if a new shape starts here
	  if (vrml.contains(QRegExp("Shape\\s+\\{")))
	    {
#if VRML_DEBUG
	      cout << "new Shape\n";
#endif
	      //create a shape class instance to hold it's description
	      s=new Shape(name, rep);
	      name="";
	      rep=-1;
	      //note the bracket depth at which we found the shape
	      shape=depth;
	    }
	  //if we're inside a shape, we should look for things describing it
	  if (shape)
	    {
	      //furthermore, if in material description
	      if (mat)
		{
		  //if this line gives the colour of this shape
		  if (vrml.contains("diffuseColor"))
		    {
#if VRML_DEBUG
		      cout << "colour: " << vrml.ascii() << endl;
#endif
		      //break it up into r, g, b
		      QStringList parts=QStringList::split(QRegExp("\\s"), vrml, false);
		      //and store those numbers
		      if (parts.size()>=4)
			{
			  s->r=parts[1].toDouble();
			  s->g=parts[2].toDouble();
			  s->b=parts[3].toDouble();
			}
		    }
		}
	      //if in point array within shape
	      else if (point)
		{
		  //this is layed out as 3 space-separated numbers (x y z) then a comma, then the next 3.  It will probably also be spread over many lines
		  //break this section of the point array on commas
		  QStringList parts=QStringList::split(QRegExp(","), vrml, false);
		  //take each comma separated number group
		  for (int i=0; i<parts.size(); i++)
		    //if it is not empty (a parsing error)
		    if (parts[i].length())
		      {
			//break it again on spaces (or commas for good measure)
			QStringList parts2=QStringList::split(QRegExp("[\\s,]"), parts[i], false);
			//that should have split it into 3 numbers
			//if it did
			if (parts2.size()>=3)
			  {
			    //read those numbers as x, y and z, and add them to the collection of coordinates for this shape
			    ThreeVector v;
			    v.x=parts2[0].toDouble();
			    v.y=parts2[1].toDouble();
			    v.z=parts2[2].toDouble();
			    s->points.push_back(v);
			  }
		      }
		}
	      //if in coordinate index array
	      else if (index)
		{
		  //this is just a list of comma separated numbers
		  //a -1 appears to indicate the end of a face/polygon and then another follows
		  //so just break it up on commas
		  QStringList parts=QStringList::split(QRegExp("[,\\n]"), vrml, false);
		  //and add each comma separated number we find to the collection in the shape object
		  for (int i=0; i<parts.size(); i++)
		    {
		      parts[i]=parts[i].stripWhiteSpace();
		      if (parts[i].length())
			{
			  s->index.push_back(parts[i].toInt());
			}
		    }
		}
	      //we are just in the shape, but not inside any of it's sub-components
	      //look for the start of such a sub-component
	      else
		{
		  //an indexed face set, describing a 3d object
		  if (vrml.contains(QRegExp("geometry\\s+IndexedFaceSet\\s+\\{")))
		    {
#if VRML_DEBUG
		      cout << "Face set in shape\n";
#endif
		      //note that this shape is a 3d object and not a squiggly line
		      s->faces=true;
		      //remember at what bracket depth we found the indexed face set 
		      ifs=depth;
		    }
		  //an indexed line set describes a squiggly line
		  else if (vrml.contains(QRegExp("geometry\\s+IndexedLineSet\\s+\\{")))
		    {
		      //note that this is a squiggly line and not a 3d object
		      s->faces=false;
#if VRML_DEBUG
		      cout << "Line set in shape\n";
#endif
		      //remember at what bracket depth we found the indexed line set
		      ils=depth;
		    }
		  //indexed face sets and indexed line sets both contain points arrays in the same format
		  //if we're in either of those things and we see the start of a points array
		  if ((ifs || ils) && vrml.contains(QRegExp("point\\s+\\[")))
		    {
#if VRML_DEBUG
		      cout << "Entered point array\n";
#endif
		      //remember the depth at which we found the point array
		      point=depth;
		    }
		  //similarly for a coordinate index list
		  if (vrml.contains(QRegExp("coordIndex\\s+\\[")))
		    {
#if VRML_DEBUG
		      cout << "Entered coordinate index array\n";
#endif
		      index=depth;
		    }
		  //we've found the material description for the shape, which includes the colour
		  if (vrml.contains(QRegExp("material.+\\{")))
		    {
#if VRML_DEBUG
		      cout << "Entered material description\n";
#endif
		      //note the bracket depth at which we found the material description
		      mat=depth;
		    }
		}
	}
	}
    }

  //done reading file
  file.close();
  //reset progress bar
  bar.reset();
  //tracks should not be added to the 3d display until it has been initialised
  //using a 1 ms timer, which cannot be activated while qt is busy, ensures that this will be done after the gui has finished rendering and therefore after the 3d display is ready
  QTimer::singleShot(1, this, SLOT(delayaddshapes()));
  
  //we want the name of the file to appear in the window list menu
  //find out which window we are so that we can be ticked as being at the front
 int myid=0;
 for (int i=1; i<windows.size(); i++)
   if (windows[i]==this)
     myid=i;
 //then order all window lists on all guis to update
  for (int i=0; i<windows.size(); i++)
    windows[i]->updatewindows(windows, myid);

}

//indicates whether we have a file loaded
bool Gui::
hasfile(void)
{
  return havefile;
}
//order from Application or another gui to update the window list on account of a change in the number or status of this window or the other main windows
//update window list menu
void Gui::
updatewindows(vector<Gui*> w, int front)
{
  //copy window list
  windows=w;
  //empty windows menu
  mnu_windows.clear();
  //on macs we want the top menu item to be Cmd-W close window
#ifdef __APPLE__
  mnu_windows.insertItem("Close", this, SLOT(close()), Qt::CTRL+Qt::Key_W, WINLIST_OFFSET-1);
  mnu_windows.insertSeparator();
#endif
  //for each of the main windows
  for (int i=0; i<windows.size(); i++)
    //add an item to the window list menu
      mnu_windows.insertItem(windows[i]->description(), WINLIST_OFFSET+i);
  //tick the front window in the window list menu
  mnu_windows.setItemChecked(WINLIST_OFFSET+front, true);
}
//get current short filename, used for window list menu
QString Gui::
description(void)
{
  //if we have loaded a file
  if (m_filename.length())
    {
      //get and return the filename without the directory name
      QFileInfo fi(m_filename);
      return fi.baseName()+QString(".")+fi.extension();
    }
  //no file, say so
  else
    return "No File Loaded";
}
void Gui::
detlist(void)
{
  //updates off, speeds things up, will turn them back on at the end
  lst_detector.setUpdatesEnabled(false);
  //clear
  lst_detector.clear();
  //add each detector "Shape" to the list
  for (int i=world.get_numshapes()-1; i>=0; i--)
    {
      //add this shape to the list
      DetectorListViewItem* item=new DetectorListViewItem(&lst_detector, world.get_shape(i).name, i);
      //notice when it changes
      connect(item, SIGNAL(check(DetectorListViewItem*)), this, SLOT(cb_detector(DetectorListViewItem*)));
    }
  //updates back on
  lst_detector.setUpdatesEnabled(true);
}

//add tracks after windows have finished rendering so as not to anny opengl
void Gui::
delayaddshapes(void)
{
  world.reset();
  for (int i=0; i<tmpshapes.size(); i++)
    {
      if (tmpshapes[i].faces)
	world.add(tmpshapes[i]);
      else
	{
	  float dx=0;
	  float dy=0;
	  float dz=0;
	  for (int j=1; j<tmpshapes[i].points.size(); j++)
	    {
	      if (fabs(tmpshapes[i].points[j].x-tmpshapes[i].points[0].x)>dx)
		dx=fabs(tmpshapes[i].points[j].x-tmpshapes[i].points[0].x);
	      if (fabs(tmpshapes[i].points[j].y-tmpshapes[i].points[0].y)>dy)
		dy=fabs(tmpshapes[i].points[j].y-tmpshapes[i].points[0].y);
	      if (fabs(tmpshapes[i].points[j].z-tmpshapes[i].points[0].z)>dz)
		dz=fabs(tmpshapes[i].points[j].z-tmpshapes[i].points[0].z);
	    }
	  if (dx>0.1 || dy>0.1 || dz>0.1)
	    {
	      world.add(tmpshapes[i]);
	    }
	}
    }
  tmpshapes.clear();
  //update detector components list
  detlist();
  world.reset();
}
//snapshot
void Gui::
cb_snap(void)
{
  //grab the current 3d display image
  world.refresh();
  QImage img=world.grabFrameBuffer();
  //ask what filename to use
  QString filename=QFileDialog::getSaveFileName("sage.png", "PNG Image Files (*.png)", this, "Save Snapshot");
  //if a filename was given, save the image as a png
  if (filename!=QString::null) img.save(filename, "PNG", 100);

}
//movie recording
//movie buffer
void Gui::
cb_movie(void)
{
  //if currently recording
  if (m_frame>=0)
    //set flag to stop
    m_frame=-m_frame;
  //if not currently recording
  else
    {
      //prompt for filename for movie
      m_movie=QFileDialog::getSaveFileName("sage", "Extension to follow", this, "Record Movie").ascii();
      //if such is entered
      if (m_movie.length())
	{
	  //reset frame count
	  m_frame=0;
	  //run secondary callback after 1/25 seconds
	  QTimer::singleShot(40, this, SLOT(cb_moviesnap()));
	  //indicate that hitting movie button again will end recording
	  mnu_rec.changeItem(REC_ID, "Stop Recording");
	}
    }

}
//timer for movie frame
void Gui::
cb_moviesnap(void)
{
  //if have been asked to stop recording
  if (m_frame<0)
    {
      mnu_rec.changeItem(REC_ID, "Record");
      RUNEVENTS(10);
      //create directory for frames
      QDir dir;
      dir.mkdir(m_movie, true);
      //this can take a while so set up the progress bar
      bar.setTotalSteps(images.size());
      //go through all the images collected
      for (int i=0; i<images.size(); i++)
	{
	  //save frame as png file
	  QString filename=m_movie+QString("/frame")+QString::number(i+1).rightJustify(10, '0')+QString(".png");
	  images[i].save(filename, "PNG");
	  //update progress bar
	  bar.setProgress(i+1);
	  RUNEVENTS(10);
	  
	}
      //all done
      //free what may be considerable memory
      images.clear();
      //reset the progress bar
      bar.reset();

      //write out bash script to convert frames to avi file
      QString filename=m_movie+"-mkavi.sh";
      std::ofstream avi(filename.ascii());
      //msmpeg4v2 is compatible with basic windows media player.  
      //not a very good codec but good enough and highly portable
      avi << "#!/bin/sh\n\n";
      avi << "#msmpeg4v2 (media player)\n";
      avi << "mencoder -cache 102400 mf://\"" << m_movie.ascii() << "/frame*.png\" -mf fps=25:type=png -ovc lavc -lavcopts vcodec=msmpeg4v2:vqscale=5 -o \"" << m_movie.ascii() << ".avi\"\n";
      avi.close();
      //set executable flag in script (add it to any existing permissions)
#ifndef __MINGW32__
      struct stat s;
      stat(filename.ascii(), &s);
      chmod(filename.ascii(), s.st_mode | S_IXUSR);
#endif
      //second script to convert to h264 mp4
      //takes a while, but best codec around and compatible with basic quicktime
      filename=m_movie+"-mkmp4.sh";
      std::ofstream mp4(filename.ascii());
      mp4 << "#!/bin/sh\n\n";
      //2 pass encoding to h264 avi file
      mp4 << "#h264 (quicktime)\n";
      mp4 << "mencoder -cache 102400 mf://\"" << m_movie.ascii() << "/frame*.png\" -mf fps=25:type=png -ovc x264 -ofps 25 -x264encopts crf=25:nocabac:level_idc=30:frameref=2:subq=5:threads=0 -vf harddup -o \"" << m_movie.ascii() << "-avi2mp4.avi\"\n";
      //rip out of avi container and push into mp4
      mp4 << "#Extra h264\n";
      mp4 << "MP4Box -aviraw video \"" << m_movie.ascii() << "-avi2mp4.avi\"\n";
      mp4 << "rm \"" << m_movie.ascii() << "-avi2mp4.avi\"\n";
      mp4 << "#Create MP4\n";
      mp4 << "MP4Box -tight -ipod -itags name=\"SAGE\" -new -add \"" << m_movie.ascii() << "-avi2mp4_video.h264\":fps=25 \"" << m_movie.ascii() << ".mp4\"\n";
      mp4 << "rm \"" << m_movie.ascii() << "-avi2mp4_video.h264\"\n";
      mp4.close();
      //make script executable
#ifndef __MINGW32__
      stat(filename.ascii(), &s);
      chmod(filename.ascii(), s.st_mode | S_IXUSR);
#endif
    }
  //not ordered to stop
  else
    {
      //take another frame in 1/25 seconds
      QTimer::singleShot(40, this, SLOT(cb_moviesnap()));
      //take a frame
      movie_shot();
    }

}
//record movie frame (not really a callback)
void Gui::
movie_shot(void)
{
  //grab current 3d display images as we would for a snapshot
  //we're storing these in memory and the footprint may be large
  //need to keep an eye on this
  //we should get about 25 seconds.
  //scaling the image to 480 lines might help
  //images.push_back(world.grabFrameBuffer().scaleHeight(480));
  images.push_back(world.grabFrameBuffer());
  //if we're over a GB, or thereabouts
  if (4*images.back().width()*images.back().height()*images.size()>1024*1024*1024)
    //we're at risk of crashing something, so stop
    m_frame=-1;
  else
    //move to next frame
    m_frame++;

}
//reset viewpoint
void Gui::
cb_reset(void)
{
  //pass order on to 3d display
  world.reset();
}

//menus
//file - open
void Gui::
cb_open(void)
{
  //ask for filename
  QString filename=QFileDialog::getOpenFileName(QString::null, "Event Files (*.wrl)", this, "Open Event File");
  //if filename given, load file
  if (filename!=QString::null)  loadfile(filename);
}
//file - new
void Gui::
cb_new(void)
{
  //send newgui signal back to Application
  emit newgui();
}

//switch mouse action in 3d mode
void Gui::
cb_action(int iaction)
{
  //set selection 3d mouse action to 3d plot widget
  Action a=(Action)iaction;
  world.set_action(a);
  //synchronise ticks on navigation menu
  for (int i=MOVE; i<=ORIGIN; i++)
    mnu_nav.setItemChecked(20+i, false);
  mnu_nav.setItemChecked(20+iaction, true);
}

//view menu
//go to/from full screen
void Gui::
cb_full(void)
{
  //if we're fill screen now
  if (isFullScreen())
    {
      //bring back progress bar
      bar.show();
      //bring back navigation buttons
      btn_move.show();
      btn_pan.show();
      btn_zoom.show();
      btn_origin.show();
      //bring back track list etc
      right.show();
      //bring back menu (only affects windows)
      menu.show();
      //drop out of full screen
      showNormal();
    }
  //if we're no full screen now
  else
    {
      //loase progress bar
      bar.hide();
      //navigation buttons
      btn_move.hide();
      btn_pan.hide();
      btn_zoom.hide();
      btn_origin.hide();
      //track list etc from right side
      right.hide();
      //menu bar (only affects windows)
      menu.hide();
      //jump into full screen mode
      showFullScreen();
    }
}
//view menu again
//show tracks or not
void Gui::
cb_tracks(void)
{
  //reverse setting in 3d display
  world.show_tracks(!world.show_tracks());
  //update ticks on view menu
  viewchecks();
}

//show detector wire-frame
void Gui::
cb_wireframe(void)
{
  //reverse setting in 3d display
  world.show_wireframe(!world.show_wireframe());
  //update ticks on view menu
  viewchecks();
}
//show detector surfaces 
void Gui::
cb_fulldet(void)
{
  //reverse setting in 3d display
  world.show_fulldet(!world.show_fulldet());
  //update ticks on view menu
  viewchecks();
}
//update View menu ticks for current settings of what to show or not
void Gui::
viewchecks(void)
{
  //hits
  mnu_view.setItemChecked(TRACKS_ID, world.show_tracks());
  //detector
  mnu_view.setItemChecked(WIREFRAME_ID, world.show_wireframe());
  mnu_view.setItemChecked(FULLDET_ID, world.show_fulldet());
}

//selection from window menu
void Gui::
cb_window(int id)
{
  //check that this is a window, and not the "Close Window" item on mac
  if (id>=WINLIST_OFFSET)
    {
      //go through the other guis
      for (int i=0; i<windows.size(); i++)
	//update the window list on all main windows, including this one
	windows[i]->updatewindows(windows, id-WINLIST_OFFSET);
      //bring the chosen window to the front
      windows[id-WINLIST_OFFSET]->raise();
    }
}

//help
//open manual
void Gui::
cb_manual(void)
{
  QStringList cmd;
  //standard location for manual file on each OS
#if defined __APPLE__
  //inside mac application bundle
   cmd << "osascript";
   cmd << "-e";
   cmd << (QString("tell Application \"Preview\" to open the POSIX file \"")+qApp->applicationDirPath()+QString("/../Resources/sage.pdf\""));
#elif defined __MINGW32__ 
  //in c:\Program Files along with the exe
  cmd << "cmd";
  cmd << "/c";
  cmd << (QString(getenv("ProgramFiles"))+QString("\\SAGE\\sage.pdf"));
#else
  //in /usr/share or $HOME/share or /usr/local/share, next the exe on Linux
  cmd << QString("evince");
  cmd << (qApp->applicationDirPath()+QString("/../share/sage.pdf"));
#endif
  //launch os specific command
  Q3Process p(cmd);
  QString buf;
  if (!p.launch(buf))
    cerr << "failed to open manual with command " << cmd.join(" ").ascii() << endl;
}
//change in detector filter
void Gui::
cb_detfilter(const QString& text)
{
  //split text on white space
  QStringList parts=QStringList::split(QRegExp("\\s"), text, false);
  //go through list
  Q3ListViewItemIterator it(&lst_detector);
  for ( ; it.current(); ++it )
    {
      //set visible if matches filter, or no filter; invisible otherwise
      DetectorListViewItem* item=dynamic_cast<DetectorListViewItem*>(it.current());
      bool vis=true;
      for (int i=0; i<parts.size(); i++)
	if (parts[i].length())
	  vis = vis && item->text().contains(parts[i], false);
      item->setVisible(vis);
    }
}
//change in detector list
void Gui::
cb_detector(DetectorListViewItem* item)
{
  world.set_shapevis(item->index, item->isOn());
}
//navigation mode changed from World
void Gui::
cb_modechange(Action a)
{
  if (a==MOVE)
    btn_move.setChecked(true);
  else if (a==PAN)
    btn_pan.setChecked(true);
  else if (a==ZOOM)
    btn_zoom.setChecked(true);
  else if (a==ORIGIN)
    btn_origin.setChecked(true);
}
//right click on detector list
void Gui::
cb_detconf(Q3ListViewItem*, const QPoint& p)
{
  //create a pop-up menu at the click point
  Q3PopupMenu* mnu=new Q3PopupMenu(this);
  //assign an event handler to user clicking an item from the pop-up menu
  connect(mnu, SIGNAL(activated(int)), this, SLOT(cb_detmenu(int)));
  //2 options, check selected, uncheck selected
  mnu->insertItem("Check Selection", 0);
  mnu->insertItem("Uncheck Selection", 1);
  mnu->insertItem("Check one by one", 2);
  mnu->insertItem("Uncheck one by one", 3);
  //display the pop-up menu at the right-click position
  mnu->exec(p);

}
//click on item from pop-up menu above
void Gui::
cb_detmenu(int check)
{
  //check-uncheck selected items
  Q3ListViewItemIterator it(&lst_detector);
  for ( ; it.current(); ++it )
    if (it.current()->isSelected() && it.current()->isVisible())
      {
	DetectorListViewItem* item=dynamic_cast<DetectorListViewItem*>(it.current());
	//set on or off and to update or not depending on choice
        item->setOn(check==0 || check==2);
	world.set_shapevis(item->index, item->isOn(), check>1);
	if (check>1)
	  RUNEVENTS(1);
      }
  //if we have done the changes without re-painting the screen, we need to repaint it once at the end
  if (check<2)
    world.refresh();
}
//user has right clicked on a volume
void Gui::
cb_select(int id)
{
  Q3ListViewItemIterator it(&lst_detector);
  int i=0;
  for ( ; it.current(); ++it )
    {
      if (it.current()->isSelected() && id!=i)
	lst_detector.setSelected(it.current(), false);
      else if (!it.current()->isSelected() && id==i)
	{
	  lst_detector.setSelected(it.current(), true);
	  lst_detector.ensureItemVisible(it.current());
	}
      i++;
    }

}
//user has double right clicked on a volume
void Gui::
cb_hide(int id)
{
  Q3ListViewItemIterator it(&lst_detector);
  int i=0;
  for ( ; it.current(); ++it )
    {
      if (it.current()->isSelected())
	{
	  DetectorListViewItem* item=dynamic_cast<DetectorListViewItem*>(it.current());
	  item->setOn(false);
	  world.set_shapevis(item->index, false);
	}
      i++;
    }
  
}

//user wants to change opacity, from menu
void Gui::
cb_opacity(void)
{
  OpacityDialog* dlg=new OpacityDialog(this, world.opacity());
  connect(dlg, SIGNAL(changed(float)), this, SLOT(cb_opacityval(float)));
  dlg->show();
}
void Gui::
cb_opacityval(float v)
{
  world.opacity(v);
}
//user wants to change background colour
void Gui::
cb_background(void)
{
  ColourDialog* dlg=new ColourDialog(this, "Background Colour", world.background_red(), world.background_green(), world.background_blue());
  connect(dlg, SIGNAL(changed(float, float, float)), this, SLOT(cb_backgroundval(float, float, float)));
  dlg->show();
}
void Gui::
cb_backgroundval(float r, float g, float b)
{
  world.background(r, g, b);
}

