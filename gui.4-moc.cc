/****************************************************************************
** Meta object code from reading C++ file 'gui.4.hh'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gui.4.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gui.4.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ClearLineEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   41,   46,   46, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ClearLineEdit[] = {
    "ClearLineEdit\0updateCloseButton(QString)\0"
    "text\0\0"
};

void ClearLineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ClearLineEdit *_t = static_cast<ClearLineEdit *>(_o);
        switch (_id) {
        case 0: _t->updateCloseButton((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ClearLineEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ClearLineEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_ClearLineEdit,
      qt_meta_data_ClearLineEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ClearLineEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ClearLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ClearLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ClearLineEdit))
        return static_cast<void*>(const_cast< ClearLineEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int ClearLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_OpacityDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   29,   29,   29, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,   42,   29,   29, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_OpacityDialog[] = {
    "OpacityDialog\0changed(float)\0\0cb_bar(int)\0"
    "v\0"
};

void OpacityDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OpacityDialog *_t = static_cast<OpacityDialog *>(_o);
        switch (_id) {
        case 0: _t->changed((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: _t->cb_bar((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OpacityDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OpacityDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_OpacityDialog,
      qt_meta_data_OpacityDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OpacityDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OpacityDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OpacityDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OpacityDialog))
        return static_cast<void*>(const_cast< OpacityDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int OpacityDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void OpacityDialog::changed(float _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_ColourDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   40,   43,   43, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   56,   43,   43, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ColourDialog[] = {
    "ColourDialog\0changed(float,float,float)\0"
    ",,\0\0cb_bar(int)\0v\0"
};

void ColourDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ColourDialog *_t = static_cast<ColourDialog *>(_o);
        switch (_id) {
        case 0: _t->changed((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 1: _t->cb_bar((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ColourDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ColourDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ColourDialog,
      qt_meta_data_ColourDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ColourDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ColourDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ColourDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ColourDialog))
        return static_cast<void*>(const_cast< ColourDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ColourDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ColourDialog::changed(float _t1, float _t2, float _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_DetectorListViewItem[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   50,   50,   50, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DetectorListViewItem[] = {
    "DetectorListViewItem\0check(DetectorListViewItem*)\0"
    "\0"
};

void DetectorListViewItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DetectorListViewItem *_t = static_cast<DetectorListViewItem *>(_o);
        switch (_id) {
        case 0: _t->check((*reinterpret_cast< DetectorListViewItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DetectorListViewItem::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DetectorListViewItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DetectorListViewItem,
      qt_meta_data_DetectorListViewItem, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DetectorListViewItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DetectorListViewItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DetectorListViewItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DetectorListViewItem))
        return static_cast<void*>(const_cast< DetectorListViewItem*>(this));
    if (!strcmp(_clname, "Q3CheckListItem"))
        return static_cast< Q3CheckListItem*>(const_cast< DetectorListViewItem*>(this));
    return QObject::qt_metacast(_clname);
}

int DetectorListViewItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DetectorListViewItem::check(DetectorListViewItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_Gui[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       4,   13,   13,   13, 0x05,
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      27,   13,   13,   13, 0x08,
      44,   13,   13,   13, 0x08,
      54,   13,   13,   13, 0x08,
      65,   13,   13,   13, 0x08,
      80,   13,   13,   13, 0x08,
      93,   13,   13,   13, 0x08,
     104,   13,   13,   13, 0x08,
     114,   13,   13,   13, 0x08,
     123,   13,   13,   13, 0x08,
     137,   13,   13,   13, 0x08,
     150,   13,   13,   13, 0x08,
     164,   13,   13,   13, 0x08,
     180,   13,   13,   13, 0x08,
     195,   13,   13,   13, 0x08,
     205,   13,   13,   13, 0x08,
     217,   13,   13,   13, 0x08,
     232,   13,   13,   13, 0x08,
     245,   13,   13,   13, 0x08,
     258,   13,   13,   13, 0x08,
     273,   13,   13,   13, 0x08,
     285,   13,   13,   13, 0x08,
     307,   13,   13,   13, 0x08,
     342,   13,   13,   13, 0x08,
     364,  399,   13,   13, 0x08,
     401,   13,   13,   13, 0x08,
     417,   13,   13,   13, 0x08,
     432,   13,   13,   13, 0x08,
     445,   13,   13,   13, 0x08,
     458,   13,   13,   13, 0x08,
     479,   13,   13,   13, 0x08,
     495,  531,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Gui[] = {
    "Gui\0newgui()\0\0closed(Gui*)\0delayaddshapes()\0"
    "cb_snap()\0cb_movie()\0cb_moviesnap()\0"
    "movie_shot()\0cb_reset()\0cb_open()\0"
    "cb_new()\0cb_menumove()\0cb_menupan()\0"
    "cb_menuzoom()\0cb_menuorigin()\0"
    "cb_action(int)\0cb_full()\0cb_tracks()\0"
    "cb_wireframe()\0cb_fulldet()\0viewchecks()\0"
    "cb_window(int)\0cb_manual()\0"
    "cb_detfilter(QString)\0"
    "cb_detector(DetectorListViewItem*)\0"
    "cb_modechange(Action)\0"
    "cb_detconf(Q3ListViewItem*,QPoint)\0,\0"
    "cb_detmenu(int)\0cb_select(int)\0"
    "cb_hide(int)\0cb_opacity()\0"
    "cb_opacityval(float)\0cb_background()\0"
    "cb_backgroundval(float,float,float)\0"
    ",,\0"
};

void Gui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Gui *_t = static_cast<Gui *>(_o);
        switch (_id) {
        case 0: _t->newgui(); break;
        case 1: _t->closed((*reinterpret_cast< Gui*(*)>(_a[1]))); break;
        case 2: _t->delayaddshapes(); break;
        case 3: _t->cb_snap(); break;
        case 4: _t->cb_movie(); break;
        case 5: _t->cb_moviesnap(); break;
        case 6: _t->movie_shot(); break;
        case 7: _t->cb_reset(); break;
        case 8: _t->cb_open(); break;
        case 9: _t->cb_new(); break;
        case 10: _t->cb_menumove(); break;
        case 11: _t->cb_menupan(); break;
        case 12: _t->cb_menuzoom(); break;
        case 13: _t->cb_menuorigin(); break;
        case 14: _t->cb_action((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->cb_full(); break;
        case 16: _t->cb_tracks(); break;
        case 17: _t->cb_wireframe(); break;
        case 18: _t->cb_fulldet(); break;
        case 19: _t->viewchecks(); break;
        case 20: _t->cb_window((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->cb_manual(); break;
        case 22: _t->cb_detfilter((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 23: _t->cb_detector((*reinterpret_cast< DetectorListViewItem*(*)>(_a[1]))); break;
        case 24: _t->cb_modechange((*reinterpret_cast< Action(*)>(_a[1]))); break;
        case 25: _t->cb_detconf((*reinterpret_cast< Q3ListViewItem*(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 26: _t->cb_detmenu((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->cb_select((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->cb_hide((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->cb_opacity(); break;
        case 30: _t->cb_opacityval((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 31: _t->cb_background(); break;
        case 32: _t->cb_backgroundval((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Gui::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Gui::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Gui,
      qt_meta_data_Gui, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Gui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Gui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Gui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Gui))
        return static_cast<void*>(const_cast< Gui*>(this));
    return QWidget::qt_metacast(_clname);
}

int Gui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }
    return _id;
}

// SIGNAL 0
void Gui::newgui()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Gui::closed(Gui * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
