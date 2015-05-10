Summary: See A Geant Event
Name: sage
Version: %{version}
Release: 1
License: GPL
Group: VR/Viewer
URL: hep0.shef.ac.uk:/scratch/robinson/software/sage
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot

%description
VRML viewer designed for geant4

%prep
%setup -q

%build
make

%install
rm -rf $RPM_BUILD_ROOT
make install prefix=$RPM_BUILD_ROOT%{_prefix}

%clean
rm -rf $RPM_BUILD_ROOT

%changelog
* Mon Aug  1 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.12-1
- list items must not be visible to be considered selected.  This makes multiple selection whilst using the filter work properly

* Thu Jul 21 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.11-1
- fixed crashing problem on some linux variants.  it was due to the requirement to pass argc to the qapplication object by reference.

* Thu Jun 16 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.10-1
- fixed handling of names of oddly named volumes

* Thu Apr 28 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.9-1
- added option to change background colour

* Wed Apr  6 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.8-1
- I was still using the README from EDAM.  Fixed now.

* Tue Mar 29 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.7-1
- minor bug fixes

* Tue Mar 29 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.6-1
- added filter to detector list to make it easier to find things

* Thu Mar 25 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.5-1
- released to EDWII

* Mon Mar 21 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.4-1
- draw detector wireframe first so that it cannot be hidden behind full detector

* Fri Mar 18 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.4-1
- changed initial viewpoint to better match typical detector geometry

* Fri Mar 18 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.3-1
- fixed bug in drawing long tracks (longer than 10 points) due to mis-reading the index from indexedlineset objects in vrml

* Fri Mar 18 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.2-1
- volumes can now be turned off one by one, or all at once

* Thu Mar 17 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.2-1
- removed glass mode since depth sorting makes it redundant

* Thu Mar 17 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.1-1
- implemented depth sorting so that one can see further into a detector

* Wed Mar 16 2011 Matt Robinson <m.robinson@sheffield.ac.uk> - 0.0.0-1
- forked from EDAM.  First working build.



%files 
%{_bindir}/sage
%{_datadir}/sage.pdf
%defattr(-,root,root,-)

%doc
