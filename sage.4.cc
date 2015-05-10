#include "sage.4.hh"

#include <qevent.h>
#include <qurl.h>
#include <qdir.h>
#include <qfileinfo.h>

#include <vector>
#include <qstring.h>
using std::vector;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

//statics from customised qapplication
vector<QString> Application::initfiles;
bool Application::running;
Application* Application::self;
vector<Gui*> Application::windows;
int Application::frontwindow;

//start here
int
main(int argc, char* argv[])
{
  //create an application based on the qapplication object
  Application app(argc, argv);

  //default font size on qt4 is a little small
#ifndef QT4
  app.setFont(QFont("SansSerif", 11));
#endif

  //list of files to load from command line
  for (unsigned short i=1; i<argc; i++)
    {
      //on macs some of the command line arguments may be null
      if (argv[i])
	{
	  //grab filename
	  QString filename=argv[i];
	  //convert to absolute if relative name given
	  QFileInfo fi(filename);
	  //add to list of files to open at start
	  app.initfiles.push_back(fi.absFilePath());
	}
    }

  //create at least one main window, even if the file list is empty
  if (!app.initfiles.size())
    {
      //create main window
      Gui* g=new Gui;
      //make said appear
      g->show();
      //update the window list for the new window
      app.windowlist();
      //respond to 'New' from gui file menu
      QObject::connect(g, SIGNAL(newgui()), &app, SLOT(newgui()));
      //and to window close
      QObject::connect(g, SIGNAL(closed(Gui*)), &app, SLOT(closegui(Gui*)));
    }
  else
    {
      //go through list of files from command line
      for (std::vector<QString>::iterator it=app.initfiles.begin(); it!=app.initfiles.end(); it++)
	{
	      //create an extra main window and load it with that
	      Gui* g=new Gui;
	      g->loadfile(*it);
	      g->show();
	      //respond to 'New' from gui file menu
	      QObject::connect(g, SIGNAL(newgui()), &app, SLOT(newgui()));
	      //and to window close
	      QObject::connect(g, SIGNAL(closed(Gui*)), &app, SLOT(closegui(Gui*)));
	}
    }

  //when last main window is closed, quit
  QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );
  //go
  app.running=true;
  app.exec();
}
