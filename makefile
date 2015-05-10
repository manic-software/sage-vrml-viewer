project:=sage
appname=SAGE
version:=0.0.13

ifdef SystemRoot
system:=Windows
endif

ifeq "$(system)" "Windows"
cc:=g++
clink:=$(cc)
qtver=4
else
$(shell [ `id -u` = 0 ] || chown `id -u`:`id -g` *)
system:=$(shell uname -s | cut -d_ -f1)
release:=$(shell echo `grep Release $(project).spec | cut -d ':' -f 2`)
arch:=$(shell if ! uname -i 2>/dev/null; then uname -p; fi)
url:=$(shell echo `grep URL $(project).spec | cut -d ':' -f 2-`)
host:=$(shell echo $(url) | cut -d ':' -f 1)
postdir:=$(shell echo $(url) | cut -d ':' -f 2)
wd=$(shell pwd)
qtver:=$(shell moc -v 2>&1 | grep "Qt 4" >/dev/null 2>&1 && echo 4 || echo 3)
prefix?=$(HOME)

ifeq "$(system)" "Darwin"
sdkpath=/Developer/SDKs/MacOSX$(sdk).sdk
clink:=g++ $(opt) 
cc:=g++
else
clink=g++ 
cc=g++
endif
ifeq "$(system)" "Linux"
clink+= -lGL -lGLU
endif

endif

ifeq "$(qtver)" "4"
cc+=-DQT4 -DQT3_SUPPORT
moc+=-DQT4 -DQT3_SUPPORT
else
$(shell sh forqt3.sh)
endif


dcc:=$(cc)

moc:=moc
flags:= -pipe -D_REENTRANT -DVERSION=\"$(version)\" -Wall -Wno-parentheses -Wno-sign-compare -o
cc+= $(flags) 
dcc+= $(flags) 


.PHONY: gui all clean install send dist rpm dmg

target:=$(project)
ifeq "$(system)" "Windows"
target:=$(project).exe
endif

gui: $(target)

all: gui

ifneq "$(system)" "Windows"
clean:
	rm -f *.o *.d *~ \#* *-moc.cc makefile.qt *.tar.gz *.3.cc *.3.hh\
	 $(project) src *.rpm *.dmg `find . -type l` \
	.DS_Store $(project).exe link.sh \
	$(project).dvi $(project).ps *.aux *.log *.out ChangeLog \
	rm -rf rpmbuild 
	rm -rf $(project)
	rm -rf $(project).app
	rm -rf $(project)-$(version)

install: gui_install
ifeq "$(system)" "Darwin"
	 osascript -e "tell Application \"Finder\" to move POSIX file \"`pwd`/SAGE.app\" to the Desktop replacing true"
else
ifeq "$(system)" "Windows"
	@echo "no install method for Windows"
else
	[ -d $(prefix)/bin ] || mkdir -p $(prefix)/bin
	install -m 755 $(project) $(prefix)/bin/$(project)
	[ -d $(prefix)/share ] || mkdir $(prefix)/share
	install -m 664 $(project).pdf $(prefix)/share/$(project).pdf
endif
endif

gui_install: gui
ifeq "$(system)" "Darwin"
	rm -rf $(project).app
	mkdir -p $(appname).app/Contents/MacOS
	cp $(project) $(appname).app/Contents/MacOS/$(appname)
	mkdir $(appname).app/Contents/Resources/
	cp $(project).icns $(appname).app/Contents/Resources/
	cp vrml.icns $(appname).app/Contents/Resources/
	cp $(project).pdf $(appname).app/Contents/Resources/
	cp Info.plist $(appname).app/Contents/Info.plist
	@. maclibs.sh $(appname).app/Contents/MacOS/$(project) /opt/local/ @executable_path/../Resources/ $(appname).app/Contents/Resources/
endif

ChangeLog: $(project).spec
	perl changelog.pl

dist: $(project)-$(version).tar.gz

$(project)-$(version).tar.gz: 
	make clean
	[ ! -d /tmp/$(project)-$(version) ] || rm -rf /tmp/$(project)-$(version)
	mkdir /tmp/$(project)-$(version)
	cp -r * /tmp/$(project)-$(version)/
	gtar -C /tmp --exclude $(project)-$(version)/builds --exclude $(project)-$(version)/.svn --exclude $(project)-$(version)/figs/.svn -czf $(project)-$(version).tar.gz $(project)-$(version) 
	rm -rf /tmp/$(project)-$(version)


send: $(project)-$(version).tar.gz ChangeLog
	ln -s . src
	tar chf - ChangeLog README src/$(project)-$(version).tar.gz | ssh $(host) "cd $(postdir) && tar xf - && cd src && rm -f $(project)-current.tar.gz && ln -s $(project)-$(version).tar.gz $(project)-current.tar.gz && ls -lrt"
	rm -f src

rpm: $(project)-$(version).tar.gz
	mkdir -p rpmbuild/{SOURCES,BUILD,SRPMS,RPMS}
	mv $(project)-$(version).tar.gz rpmbuild/SOURCES/
	rpmbuild -ba --define "_topdir $(wd)/rpmbuild" \
	--define "_tmppath $(wd)/tmp" \
	--define "version $(version)" $(project).spec
	mv rpmbuild/SOURCES/$(project)-$(version).tar.gz .
	mv rpmbuild/SRPMS/$(project)-$(version)-$(release).src.rpm .
	mv rpmbuild/RPMS/$(arch)/$(project)-$(version)-$(release).$(arch).rpm .
	rm -rf rpmbuild tmp


dmg: gui_install
	rm -rf $(project)-$(version)
	mkdir $(project)-$(version)
	cp -R $(project).app $(project)-$(version)/
	hdiutil create -ov -srcdir $(project)-$(version) ~/Desktop/$(project)-$(version).dmg
endif

ifneq "$(MAKECMDGOALS)" "clean"
ifneq "$(MAKECMDGOALS)" "dist"
ifneq "$(MAKECMDGOALS)" "send"
ifneq "$(MAKECMDGOALS)" "ChangeLog"
ifneq "$(MAKECMDGOALS)" "rpm"


ifeq "$(system)" "Windows"
-include makefile.qt.win
else
$(shell perl findqt.pl)
-include makefile.qt
endif
ifeq "$(system)" "Darwin"
QT_LIBS+=-framework Carbon
endif

mobjs:=gui.$(qtver).hh world.$(qtver).hh $(project).$(qtver).hh
$(mobjs:.hh=-moc.cc): %-moc.cc: %.hh
	$(moc) -o $@ $< 

#objects used by the gui
objs= $(mobjs:.hh=.o) $(mobjs:.hh=-moc.o)
ifeq "$(system)" "Windows"
$(objs): %.o : %.cc
	$(cc) $@ $(QT_INCPATH) -c $<
else
$(objs:.o=.d): %.d: %.cc
	$(dcc) $@ $(QT_INCDEP) -MM -MT '$@ $(@:.d=.o)' $<
$(objs): %.o : %.cc
	$(cc) $@ $(QT_INCPATH) -c $<
-include $(objs:.o=.d)
endif

ifeq "$(system)" "Windows"
objs+=resource.o
resource.o: $(project).rc $(project).ico json.ico
	windres -Ocoff -o $@ $<
endif

#linking
$(target): $(objs)
	$(clink) $(QT_LFLAGS)  -Wall -o $@ $(objs) $(QT_LIBS)
	strip $@
endif
endif
endif
endif
endif
