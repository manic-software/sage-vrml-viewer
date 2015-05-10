#!/bin/bash

for file in *.4.cc *.4.hh
do
  ofile=`echo $file | sed 's/.4./.3./g'`
  if [ ! -e $ofile ] || [ $file -nt $ofile ]; then
    sed 's/4.hh/3.hh/g;s/Q3ListView/QListView/g;s/q3listview/qlistview/g;s/Q3VBox/QVBox/g;s/q3vbox/qvbox/g;s/q3valuelist/qvaluelist/g;s/Q3ValueList/QValueList/g;s/q3header/qheader/g;s/Q3Header/QHeader/g;s/q3boxlayout/qboxlayout/g;s/Q3VBoxLayout/QVBoxLayout/g;s/Q3HBoxLayout/QHBoxLayout/g;s/q3popupmenu/qpopupmenu/g;s/Q3PopupMenu/QPopupMenu/g;s/q3textstream/qtextstream/g;s/Q3TextStream/QTextStream/g;s/q3progressbar/qprogressbar/g;s/Q3ProgressBar/QProgressBar/g;s/q3process/qprocess/g;s/Q3Process/QProcess/g;s/q3checklistitem/qchecklistitem/g;s/Q3CheckListItem/QCheckListItem/g;' < $file > $ofile
  fi
done

