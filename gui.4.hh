#ifndef EDAM_GUI
#define EDAM_GUI

#include <qwidget.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <q3progressbar.h>
#include <qmenubar.h>
#include <q3popupmenu.h>
#include <qlabel.h>
#include <qsplitter.h>
#include <qimage.h>
#include <q3listview.h>
#include <qevent.h>
#include <qlineedit.h>
#include <q3header.h>
#include <qtabwidget.h>
#include <qscrollbar.h>
#include <qdialog.h>

#include <iostream>

#ifdef QT4
#include <q3boxlayout.h>
#else
#include <qlayout.h>
#endif
#include <vector>
using std::vector;
#include <map>

#include "world.4.hh"

class ClearLineEdit : public QLineEdit
{
  Q_OBJECT
 
public:
  ClearLineEdit(QWidget *parent = 0);
 
protected:
  void resizeEvent(QResizeEvent *);
				  
private slots:
  void updateCloseButton(const QString &text);
  
private:
  QPushButton btn_clear;
};

//dialog for user to change surface opacity
class OpacityDialog : public QDialog
{
  Q_OBJECT
  public:
  OpacityDialog(QWidget* parent, float o):
    QDialog(parent),
    layout(this),
    bar(0, 100, 1, 10, (int)(o*100), Qt::Horizontal, this),
    box(),
    btn("Close", this)
  {
    setCaption("Surface Opacity");
    layout.addWidget(&bar);
    bar.setMinimumWidth(100);
    layout.addLayout(&box);
    box.addStretch();
    box.addWidget(&btn);
    box.addStretch();
    btn.setDefault(true);
    bar.setValue(o*100);
    connect(&btn, SIGNAL(clicked()), this, SLOT(hide()));
    connect(&bar, SIGNAL(valueChanged(int)), this, SLOT(cb_bar(int)));
  }

    private:
  QVBoxLayout layout;
  QScrollBar bar;
  QHBoxLayout box;
  QPushButton btn;

private slots:
  void cb_bar(int v)
  {
    emit changed(v/100.);
  }

signals:
  void changed(float);
};

//a simple dialog for the user to enter a colour
class ColourDialog : public QDialog
{
Q_OBJECT
public:
  ColourDialog(QWidget* parent, QString caption, float r, float g, float b):
    QDialog(parent),
    layout(this),
    lbl_red(this, "R"),
    lbl_green(this, "G"),
    lbl_blue(this, "B"),
    red(0, 100, 1, 10, (int)(r*100), Qt::Horizontal, this),
    green(0, 100, 1, 10, (int)(g*100), Qt::Horizontal, this),
    blue(0, 100, 1, 10, (int)(b*100), Qt::Horizontal, this),
    box(),
    btn("Close", this)
  {
    setCaption(caption);
    layout.addLayout(&box_red);
    box_red.addWidget(&lbl_red, 1);
    box_red.addWidget(&red, 100);
    layout.addLayout(&box_green);
    box_green.addWidget(&lbl_green, 1);
    box_green.addWidget(&green, 100);
    layout.addLayout(&box_blue);
    box_blue.addWidget(&lbl_blue, 1);
    box_blue.addWidget(&blue, 100);
    red.setMinimumWidth(100);
    green.setMinimumWidth(100);
    blue.setMinimumWidth(100);
    layout.addLayout(&box);
    box.addStretch();
    box.addWidget(&btn);
    box.addStretch();
    btn.setDefault(true);
    red.setValue(r*100);
    green.setValue(g*100);
    blue.setValue(b*100);
    connect(&btn, SIGNAL(clicked()), this, SLOT(hide()));
    connect(&red, SIGNAL(valueChanged(int)), this, SLOT(cb_bar(int)));
    connect(&green, SIGNAL(valueChanged(int)), this, SLOT(cb_bar(int)));
    connect(&blue, SIGNAL(valueChanged(int)), this, SLOT(cb_bar(int)));

  }

private:
  QVBoxLayout layout;
  QLabel lbl_red, lbl_green, lbl_blue;
  QHBoxLayout box_red, box_green, box_blue;
  QScrollBar red, green, blue;
  
  QHBoxLayout box;
  QPushButton btn;

private slots:
  void cb_bar(int v)
  {
    emit changed(red.value()/100., green.value()/100., blue.value()/100.);
  }

signals:
  void changed(float, float, float);

};

//helper class for detector shape list
class DetectorListViewItem : public QObject, public Q3CheckListItem
{
  Q_OBJECT

public:
  //just the name of the shape will do
  DetectorListViewItem(Q3ListView* parent, QString name, int i):
    Q3CheckListItem(parent, name, Q3CheckListItem::CheckBox)
  {
    //should be checked by default
    setOn(true);
    //note, position in detector shape list in World
    index=i;
  }
  //when checked or un-checled
  void activate(void)
  {
    //carry on as normal
    Q3CheckListItem::activate();
    //signal the change 
    emit check(this);
  }

  int index;

signals:
  void check(DetectorListViewItem*);

};

//main ui window
class Gui : public QWidget
{
Q_OBJECT

public:
  Gui(void);

private:
  //map from PDG particle id code to string describing particle
  std::map<int, QString> pdg;
  //list of all application main windows
  vector<Gui*> windows;
  //buffer vrml shapes here, but store them in World
  vector<Shape> tmpshapes;
  //main menu
  //behaviour slighly different on macs from others
  //watch out for this
  QMenuBar menu;
  //top level menu widgets
  Q3PopupMenu mnu_file;
  Q3PopupMenu mnu_nav;
  Q3PopupMenu mnu_rec;
  Q3PopupMenu mnu_view;
  Q3PopupMenu mnu_windows;
  Q3PopupMenu mnu_help;
  //initially widgets will be distributed vertically down the main window
  Q3VBoxLayout layout;
  //progress bar for things that take more than a few seconds
  Q3ProgressBar bar;
  //splitter, creates zones in main window with a slider in-between to 
  //  allow the user to decide how much space to give each zone
  QSplitter sides;
  //layout for world and buttons below
  //this contains everything on the left of the splitter
  QWidget left;
  //the world 3d display widget
  //allow buttons to be placed below the world
  Q3VBoxLayout box_world;
  //the display widget itself
  World world;
  //layout for first row of controls below plot
  QHBoxLayout box;
  //buttons for mouse action in 3d mode
  QButtonGroup grp_3d;
  QRadioButton btn_move;
  QRadioButton btn_zoom;
  QRadioButton btn_pan;
  QRadioButton btn_origin;
  //right hand side, lists of particles/tracks
  //this contains everything on the right side of the splitter
  QWidget right;
  //I may want to add something above or below the track list
  Q3VBoxLayout box_right;
  //track list filter
  ClearLineEdit txt_filter;
  //detector track list
  Q3ListView lst_detector;
  //space point list
  //Q3ListView lst_hits;
  //current filename
  QString m_filename;
  //flag indicating a file has been loaded
  bool havefile;
  //recording a movie
  //when recording for a movie, this will contain the movie filename
  QString m_movie;
  //current frame number
  int m_frame;
  //frame for movie.  uses a lot of memory.
  //movies should be reasonably short, or computer should kick arse
  vector<QImage> images;

private:
  //catch window close
  void closeEvent(QCloseEvent* event)
  {
    emit closed(this);
    event->accept();
  }
  void changeEvent ( QEvent * event )
  {
#ifdef QT4
    if (event->type()==QEvent::ActivationChange)
#else
    if (event->type()==QEvent::WindowActivate)
#endif
      if (isActiveWindow())
	{
	  int myid=0;
	  for (int i=1; i<windows.size(); i++)
	    if (windows[i]==this)
	      myid=i;
	  for (int i=0; i<windows.size(); i++)
	    windows[i]->updatewindows(windows, myid);
	}
  }

public:
  //load a json file into memory
  void loadfile(QString);
  //has a vrml file been loaded?
  bool hasfile(void);
  //update window list menu
  void updatewindows(vector<Gui*>, int);
  //something to identify this window on the windows menu
  QString description(void);
  //update detector component list
  void detlist(void);

private slots:
  //  it's an opengl thing
  void delayaddshapes(void);
  //snapshot, saves png image of current display
  void cb_snap(void);
  //movie recording
  //movie buffer
  void cb_movie(void);
  //timer for movie frame
  void cb_moviesnap(void);
  //record movie frame (not really a callback)
  void movie_shot(void);
  //reset viewpoint
  void cb_reset(void);
  //menus
  //file - open
  void cb_open(void);
  //file - new
  void cb_new(void);
  //switch mouse action in 3d mode from menu rather than buttons
  void cb_menumove(void) {
    btn_move.setChecked(true);
    cb_action(MOVE);
  }
  void cb_menupan(void) {
    btn_pan.setChecked(true);
    cb_action(PAN);
  }
  void cb_menuzoom(void) {
    btn_zoom.setChecked(true);
    cb_action(ZOOM);
  }
  void cb_menuorigin(void) {
    btn_origin.setChecked(true);
    cb_action(ORIGIN);
  }
  void cb_action(int);
  //view menu
  //full-screen
  void cb_full(void);
  //show tracks?
  void cb_tracks(void);
  //show detector wire-frame?
  void cb_wireframe(void);
  //show detector surfaces?
  void cb_fulldet(void);
  //set view menu ticks to match current settings of world
  void viewchecks(void);
  //window menu
  //selection from window menu
  void cb_window(int);
  //help
  //open manual
  void cb_manual(void);
  //change in detector filter
  void cb_detfilter(const QString&);
  //change in detector list
  void cb_detector(DetectorListViewItem*);
  //navigation mode changed from World
  void cb_modechange(Action);
  //right click on detector list
  void cb_detconf(Q3ListViewItem*, const QPoint&);
  //click on item from pop-up menu above
  void cb_detmenu(int);
  //user right clicks on a volume
  void cb_select(int);
  //user double clicks on a volume
  void cb_hide(int);
  //user wants to change opacity, from menu
  void cb_opacity(void);
  void cb_opacityval(float);
  //user wants to change background colour
  void cb_background(void);
  void cb_backgroundval(float, float, float);

  //signals to trigger functions elsewhere in the code
signals:
  //'New' from file menu
  void newgui(void);
  //close this window
  void closed(Gui*);

};

#endif //ifndef EDAM_GUI
