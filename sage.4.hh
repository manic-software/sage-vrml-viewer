#include "gui.4.hh"

#include <qapplication.h>
#include <qfiledialog.h>
#include <qwidget.h>
#ifndef QT4
#include <qwidgetlist.h>
#endif
#include <qtimer.h>

#include <iostream>
#include <vector>
using std::vector;

//macs have a funny way of handling orders to an app to open a file
//this here is the code necessary to pick up the open file order and act on it
//there are also extensions to qapplication here used under all oses to maintain the window list menu

#ifdef __APPLE__

#include <Carbon/Carbon.h>

const int OpenEventID = QEvent::User + 100;

//catch order from macos to open file   
class OpenEvent : public QCustomEvent
{
public:
  OpenEvent(const QString &fileName)
    : QCustomEvent(OpenEventID), file(fileName) {}

  QString fileName() const { return file; }
  
private:
  QString file;

};
  
#endif

//using inherotance to slightly extent standard qt application  
class Application : public QApplication
{
Q_OBJECT

public:
  Application(int& argc, char* argv[]):
    QApplication(argc, argv)
  {
    //flag so e know app hs launched
    running=false;
    //static 'this' pointer, useful later
    self=this;
    //focus window index, for the benefit of the window list menu
    frontwindow=0;

#ifdef __APPLE__
     AEInstallEventHandler(kCoreEventClass, kAEOpenDocuments,
                          Application::appleEventHandler, 0, false);
#endif
  }
  ~Application(void)
  {
#ifdef __APPLE__
    AERemoveEventHandler(kCoreEventClass, kAEOpenDocuments,
                         Application::appleEventHandler, false);
#endif
  }

  //files to open at startup
  static vector<QString> initfiles;
  //has application started?
  static bool running;
  //static version of 'this'
  static Application* self;
  //list of active guis, for window list menu
  static vector<Gui*> windows;
  //which window from window list menu is in foreground
  static int frontwindow;

#ifdef __APPLE__
  //catch macos openfile signal
  static OSErr appleEventHandler(const AppleEvent *ae, AppleEvent *, SRefCon)
  {
    AEDescList docs;
    if (AEGetParamDesc(ae, keyDirectObject, typeAEList, &docs) == noErr) {
        long cnt = 0;
        AECountItems(&docs, &cnt);
        UInt8 strBuffer[256];
        for (int i = 0; i < cnt; i++) {
            FSRef ref;
            if (AEGetNthPtr(&docs, i+1, typeFSRef, 0, 0, &ref, sizeof(ref), 0) != noErr)
                continue;
            if (FSRefMakePath(&ref, strBuffer, 256) == noErr) {
                OpenEvent event(QString::fromUtf8(reinterpret_cast<char *>(strBuffer)));
		//if application has started
		if (running)
		  {
		    Gui* g=NULL;
		    //look for a window with no file loaded
		    for (int i=0; i<windows.size() && !g; i++)
		      {
			if (!windows[i]->hasfile())
			  //use this currently idle window to open the file
			  g=windows[i];
		      }
		    //no unused main window is open
		    if (!g)
		      {
			//create new gui main window and open file
			g=new Gui;
			//respond to 'New' from gui file menu
			connect(g, SIGNAL(newgui()), self, SLOT(newgui()));
			//and to window close
			connect(g, SIGNAL(closed(Gui*)), self, SLOT(closegui(Gui*)));
			g->show();
		      }
		    //bring selected/new window to foreground
		    g->raise();
		    //order window to load chosen file
		    g->loadfile(event.fileName().ascii());
		    //update window list
		    windowlist();
		  }
		else
		  //first main gui is not up, so generate a list of files to load when it is
		  initfiles.push_back(event.fileName().ascii());
		    
            }
        }
    }
    return noErr;
  }
  #endif

  //regenerate the window list for the menu
  static void windowlist(Gui* ignoregui=NULL)
  {
    //start by emptying the list
    windows.clear();
    //this code is significantly different in QT4 and QT3
    //we're just cyling through the windows and building up a list
#ifdef QT4
    QWidgetList list=topLevelWidgets();
    QWidgetList::iterator it=list.begin();  // iterate over the widgets
    QWidget* w;
    while (it!=list.end()) {
      w=*it;
      ++it;
      if (dynamic_cast<Gui*>(w) && dynamic_cast<Gui*>(w)!=ignoregui)
	windows.push_back(dynamic_cast<Gui*>(w));
    }
#else
    QWidgetList  *list = topLevelWidgets();
    QWidgetListIt it( *list );  // iterate over the widgets
    QWidget * w;
    while ( (w=it.current()) != 0 ) {   // for each top level widget...
      ++it;
      if (dynamic_cast<Gui*>(w) && dynamic_cast<Gui*>(w)!=ignoregui)
	windows.push_back(dynamic_cast<Gui*>(w));
    }
    delete list;                // delete the list, not the widgets
#endif
    //order guis to update their windows menu
    for (int i=0; i<windows.size(); i++)
      windows[i]->updatewindows(windows, frontwindow);
  }

public slots:
  //triggered when a window closes
  void closegui(Gui* g)
  {
    //update the window list, removing the window which is in the process of closing
    windowlist(g);
  }
  void newgui(void)
  {
    //create new main window
    Gui* g=new Gui;
    //respond to 'New' from gui file menu
    connect(g, SIGNAL(newgui()), this, SLOT(newgui()));
    //and to window close
    connect(g, SIGNAL(closed(Gui*)), this, SLOT(closegui(Gui*)));
    //display said
    g->show();
    //prompt for filename
    QString filename=QFileDialog::getOpenFileName(QString::null, "Event Files (*.json)", g, "Open Event File");
    //if file dialog as given a file to open
    if (filename!=QString::null)  g->loadfile(filename);
    //regenerate window list and apply to all guis
    windowlist();
  }

};
