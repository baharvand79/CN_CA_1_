/****************************************************************************
** Meta object code from reading C++ file 'webrtc.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../voice_call_app/webrtc.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'webrtc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSWebRTCENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSWebRTCENDCLASS = QtMocHelpers::stringData(
    "WebRTC",
    "peerIdGenerated",
    "",
    "peerId",
    "localDescriptionGenerated",
    "sdp",
    "localCandidateGenerated",
    "candidate",
    "audioReceived",
    "data",
    "gatheringCompleted",
    "onSignalingServerConnected",
    "onSignalingServerDisconnected",
    "onSignalingMessageReceived",
    "message"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSWebRTCENDCLASS_t {
    uint offsetsAndSizes[30];
    char stringdata0[7];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[26];
    char stringdata5[4];
    char stringdata6[24];
    char stringdata7[10];
    char stringdata8[14];
    char stringdata9[5];
    char stringdata10[19];
    char stringdata11[27];
    char stringdata12[30];
    char stringdata13[27];
    char stringdata14[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSWebRTCENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSWebRTCENDCLASS_t qt_meta_stringdata_CLASSWebRTCENDCLASS = {
    {
        QT_MOC_LITERAL(0, 6),  // "WebRTC"
        QT_MOC_LITERAL(7, 15),  // "peerIdGenerated"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 6),  // "peerId"
        QT_MOC_LITERAL(31, 25),  // "localDescriptionGenerated"
        QT_MOC_LITERAL(57, 3),  // "sdp"
        QT_MOC_LITERAL(61, 23),  // "localCandidateGenerated"
        QT_MOC_LITERAL(85, 9),  // "candidate"
        QT_MOC_LITERAL(95, 13),  // "audioReceived"
        QT_MOC_LITERAL(109, 4),  // "data"
        QT_MOC_LITERAL(114, 18),  // "gatheringCompleted"
        QT_MOC_LITERAL(133, 26),  // "onSignalingServerConnected"
        QT_MOC_LITERAL(160, 29),  // "onSignalingServerDisconnected"
        QT_MOC_LITERAL(190, 26),  // "onSignalingMessageReceived"
        QT_MOC_LITERAL(217, 7)   // "message"
    },
    "WebRTC",
    "peerIdGenerated",
    "",
    "peerId",
    "localDescriptionGenerated",
    "sdp",
    "localCandidateGenerated",
    "candidate",
    "audioReceived",
    "data",
    "gatheringCompleted",
    "onSignalingServerConnected",
    "onSignalingServerDisconnected",
    "onSignalingMessageReceived",
    "message"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSWebRTCENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,
       4,    1,   65,    2, 0x06,    3 /* Public */,
       6,    1,   68,    2, 0x06,    5 /* Public */,
       8,    1,   71,    2, 0x06,    7 /* Public */,
      10,    0,   74,    2, 0x06,    9 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      11,    0,   75,    2, 0x08,   10 /* Private */,
      12,    0,   76,    2, 0x08,   11 /* Private */,
      13,    1,   77,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QByteArray,    9,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,

       0        // eod
};

Q_CONSTINIT const QMetaObject WebRTC::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSWebRTCENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSWebRTCENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSWebRTCENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<WebRTC, std::true_type>,
        // method 'peerIdGenerated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'localDescriptionGenerated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'localCandidateGenerated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'audioReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        // method 'gatheringCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSignalingServerConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSignalingServerDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSignalingMessageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void WebRTC::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WebRTC *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->peerIdGenerated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->localDescriptionGenerated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->localCandidateGenerated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->audioReceived((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 4: _t->gatheringCompleted(); break;
        case 5: _t->onSignalingServerConnected(); break;
        case 6: _t->onSignalingServerDisconnected(); break;
        case 7: _t->onSignalingMessageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WebRTC::*)(const QString & );
            if (_t _q_method = &WebRTC::peerIdGenerated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WebRTC::*)(const QString & );
            if (_t _q_method = &WebRTC::localDescriptionGenerated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WebRTC::*)(const QString & );
            if (_t _q_method = &WebRTC::localCandidateGenerated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WebRTC::*)(const QByteArray & );
            if (_t _q_method = &WebRTC::audioReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (WebRTC::*)();
            if (_t _q_method = &WebRTC::gatheringCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *WebRTC::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WebRTC::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSWebRTCENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WebRTC::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void WebRTC::peerIdGenerated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WebRTC::localDescriptionGenerated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WebRTC::localCandidateGenerated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WebRTC::audioReceived(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void WebRTC::gatheringCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
