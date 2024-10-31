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
    "localDescriptionGenerated",
    "",
    "sdp",
    "localCandidateGenerated",
    "candidate",
    "audioReceived",
    "data",
    "gatheringCompleted",
    "debugMessage",
    "message",
    "clientIsRegistered",
    "answerIsReady",
    "onSignalingServerConnected",
    "onSignalingServerDisconnected",
    "onSignalingMessageReceived",
    "sendOffer",
    "sendAnswer",
    "setId",
    "id",
    "init",
    "registerClient",
    "connectToSignalingServer",
    "createOffer",
    "callOnRun",
    "setRemoteDescription",
    "addRemoteCandidate",
    "setTargetId",
    "checkWebSocketState",
    "createAnswer"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSWebRTCENDCLASS_t {
    uint offsetsAndSizes[60];
    char stringdata0[7];
    char stringdata1[26];
    char stringdata2[1];
    char stringdata3[4];
    char stringdata4[24];
    char stringdata5[10];
    char stringdata6[14];
    char stringdata7[5];
    char stringdata8[19];
    char stringdata9[13];
    char stringdata10[8];
    char stringdata11[19];
    char stringdata12[14];
    char stringdata13[27];
    char stringdata14[30];
    char stringdata15[27];
    char stringdata16[10];
    char stringdata17[11];
    char stringdata18[6];
    char stringdata19[3];
    char stringdata20[5];
    char stringdata21[15];
    char stringdata22[25];
    char stringdata23[12];
    char stringdata24[10];
    char stringdata25[21];
    char stringdata26[19];
    char stringdata27[12];
    char stringdata28[20];
    char stringdata29[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSWebRTCENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSWebRTCENDCLASS_t qt_meta_stringdata_CLASSWebRTCENDCLASS = {
    {
        QT_MOC_LITERAL(0, 6),  // "WebRTC"
        QT_MOC_LITERAL(7, 25),  // "localDescriptionGenerated"
        QT_MOC_LITERAL(33, 0),  // ""
        QT_MOC_LITERAL(34, 3),  // "sdp"
        QT_MOC_LITERAL(38, 23),  // "localCandidateGenerated"
        QT_MOC_LITERAL(62, 9),  // "candidate"
        QT_MOC_LITERAL(72, 13),  // "audioReceived"
        QT_MOC_LITERAL(86, 4),  // "data"
        QT_MOC_LITERAL(91, 18),  // "gatheringCompleted"
        QT_MOC_LITERAL(110, 12),  // "debugMessage"
        QT_MOC_LITERAL(123, 7),  // "message"
        QT_MOC_LITERAL(131, 18),  // "clientIsRegistered"
        QT_MOC_LITERAL(150, 13),  // "answerIsReady"
        QT_MOC_LITERAL(164, 26),  // "onSignalingServerConnected"
        QT_MOC_LITERAL(191, 29),  // "onSignalingServerDisconnected"
        QT_MOC_LITERAL(221, 26),  // "onSignalingMessageReceived"
        QT_MOC_LITERAL(248, 9),  // "sendOffer"
        QT_MOC_LITERAL(258, 10),  // "sendAnswer"
        QT_MOC_LITERAL(269, 5),  // "setId"
        QT_MOC_LITERAL(275, 2),  // "id"
        QT_MOC_LITERAL(278, 4),  // "init"
        QT_MOC_LITERAL(283, 14),  // "registerClient"
        QT_MOC_LITERAL(298, 24),  // "connectToSignalingServer"
        QT_MOC_LITERAL(323, 11),  // "createOffer"
        QT_MOC_LITERAL(335, 9),  // "callOnRun"
        QT_MOC_LITERAL(345, 20),  // "setRemoteDescription"
        QT_MOC_LITERAL(366, 18),  // "addRemoteCandidate"
        QT_MOC_LITERAL(385, 11),  // "setTargetId"
        QT_MOC_LITERAL(397, 19),  // "checkWebSocketState"
        QT_MOC_LITERAL(417, 12)   // "createAnswer"
    },
    "WebRTC",
    "localDescriptionGenerated",
    "",
    "sdp",
    "localCandidateGenerated",
    "candidate",
    "audioReceived",
    "data",
    "gatheringCompleted",
    "debugMessage",
    "message",
    "clientIsRegistered",
    "answerIsReady",
    "onSignalingServerConnected",
    "onSignalingServerDisconnected",
    "onSignalingMessageReceived",
    "sendOffer",
    "sendAnswer",
    "setId",
    "id",
    "init",
    "registerClient",
    "connectToSignalingServer",
    "createOffer",
    "callOnRun",
    "setRemoteDescription",
    "addRemoteCandidate",
    "setTargetId",
    "checkWebSocketState",
    "createAnswer"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSWebRTCENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  152,    2, 0x06,    1 /* Public */,
       4,    1,  155,    2, 0x06,    3 /* Public */,
       6,    1,  158,    2, 0x06,    5 /* Public */,
       8,    0,  161,    2, 0x06,    7 /* Public */,
       9,    1,  162,    2, 0x06,    8 /* Public */,
      11,    0,  165,    2, 0x06,   10 /* Public */,
      12,    0,  166,    2, 0x06,   11 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      13,    0,  167,    2, 0x0a,   12 /* Public */,
      14,    0,  168,    2, 0x0a,   13 /* Public */,
      15,    1,  169,    2, 0x0a,   14 /* Public */,
      16,    0,  172,    2, 0x0a,   16 /* Public */,
      17,    0,  173,    2, 0x0a,   17 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      18,    1,  174,    2, 0x02,   18 /* Public */,
      20,    0,  177,    2, 0x02,   20 /* Public */,
      21,    0,  178,    2, 0x02,   21 /* Public */,
      22,    0,  179,    2, 0x02,   22 /* Public */,
      23,    0,  180,    2, 0x02,   23 /* Public */,
      24,    0,  181,    2, 0x02,   24 /* Public */,
      25,    1,  182,    2, 0x02,   25 /* Public */,
      26,    1,  185,    2, 0x02,   27 /* Public */,
      27,    1,  188,    2, 0x02,   29 /* Public */,
      28,    0,  191,    2, 0x02,   31 /* Public */,
      29,    0,  192,    2, 0x02,   32 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QByteArray,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void,
    QMetaType::Void,

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
        // method 'debugMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'clientIsRegistered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'answerIsReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSignalingServerConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSignalingServerDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSignalingMessageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sendOffer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendAnswer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setId'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'init'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'registerClient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connectToSignalingServer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createOffer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'callOnRun'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setRemoteDescription'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'addRemoteCandidate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'setTargetId'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'checkWebSocketState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createAnswer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void WebRTC::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WebRTC *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->localDescriptionGenerated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->localCandidateGenerated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->audioReceived((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 3: _t->gatheringCompleted(); break;
        case 4: _t->debugMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->clientIsRegistered(); break;
        case 6: _t->answerIsReady(); break;
        case 7: _t->onSignalingServerConnected(); break;
        case 8: _t->onSignalingServerDisconnected(); break;
        case 9: _t->onSignalingMessageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->sendOffer(); break;
        case 11: _t->sendAnswer(); break;
        case 12: _t->setId((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->init(); break;
        case 14: _t->registerClient(); break;
        case 15: _t->connectToSignalingServer(); break;
        case 16: _t->createOffer(); break;
        case 17: _t->callOnRun(); break;
        case 18: _t->setRemoteDescription((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 19: _t->addRemoteCandidate((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->setTargetId((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: _t->checkWebSocketState(); break;
        case 22: _t->createAnswer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WebRTC::*)(const QString & );
            if (_t _q_method = &WebRTC::localDescriptionGenerated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WebRTC::*)(const QString & );
            if (_t _q_method = &WebRTC::localCandidateGenerated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WebRTC::*)(const QByteArray & );
            if (_t _q_method = &WebRTC::audioReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WebRTC::*)();
            if (_t _q_method = &WebRTC::gatheringCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (WebRTC::*)(QString );
            if (_t _q_method = &WebRTC::debugMessage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (WebRTC::*)();
            if (_t _q_method = &WebRTC::clientIsRegistered; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (WebRTC::*)();
            if (_t _q_method = &WebRTC::answerIsReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
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
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void WebRTC::localDescriptionGenerated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WebRTC::localCandidateGenerated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WebRTC::audioReceived(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WebRTC::gatheringCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void WebRTC::debugMessage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void WebRTC::clientIsRegistered()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void WebRTC::answerIsReady()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
