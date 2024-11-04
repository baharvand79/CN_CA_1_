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
    "peerIsConnected",
    "peerIsOffererChanged",
    "onSignalingServerConnected",
    "onSignalingServerDisconnected",
    "onSignalingMessageReceived",
    "sendOffer",
    "sendAnswer",
    "onAudioCaptured",
    "audioData",
    "sendTrack",
    "peerId",
    "buffer",
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
    "createAnswer",
    "peerIsOfferer"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSWebRTCENDCLASS_t {
    uint offsetsAndSizes[76];
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
    char stringdata13[16];
    char stringdata14[21];
    char stringdata15[27];
    char stringdata16[30];
    char stringdata17[27];
    char stringdata18[10];
    char stringdata19[11];
    char stringdata20[16];
    char stringdata21[10];
    char stringdata22[10];
    char stringdata23[7];
    char stringdata24[7];
    char stringdata25[6];
    char stringdata26[3];
    char stringdata27[5];
    char stringdata28[15];
    char stringdata29[25];
    char stringdata30[12];
    char stringdata31[10];
    char stringdata32[21];
    char stringdata33[19];
    char stringdata34[12];
    char stringdata35[20];
    char stringdata36[13];
    char stringdata37[14];
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
        QT_MOC_LITERAL(164, 15),  // "peerIsConnected"
        QT_MOC_LITERAL(180, 20),  // "peerIsOffererChanged"
        QT_MOC_LITERAL(201, 26),  // "onSignalingServerConnected"
        QT_MOC_LITERAL(228, 29),  // "onSignalingServerDisconnected"
        QT_MOC_LITERAL(258, 26),  // "onSignalingMessageReceived"
        QT_MOC_LITERAL(285, 9),  // "sendOffer"
        QT_MOC_LITERAL(295, 10),  // "sendAnswer"
        QT_MOC_LITERAL(306, 15),  // "onAudioCaptured"
        QT_MOC_LITERAL(322, 9),  // "audioData"
        QT_MOC_LITERAL(332, 9),  // "sendTrack"
        QT_MOC_LITERAL(342, 6),  // "peerId"
        QT_MOC_LITERAL(349, 6),  // "buffer"
        QT_MOC_LITERAL(356, 5),  // "setId"
        QT_MOC_LITERAL(362, 2),  // "id"
        QT_MOC_LITERAL(365, 4),  // "init"
        QT_MOC_LITERAL(370, 14),  // "registerClient"
        QT_MOC_LITERAL(385, 24),  // "connectToSignalingServer"
        QT_MOC_LITERAL(410, 11),  // "createOffer"
        QT_MOC_LITERAL(422, 9),  // "callOnRun"
        QT_MOC_LITERAL(432, 20),  // "setRemoteDescription"
        QT_MOC_LITERAL(453, 18),  // "addRemoteCandidate"
        QT_MOC_LITERAL(472, 11),  // "setTargetId"
        QT_MOC_LITERAL(484, 19),  // "checkWebSocketState"
        QT_MOC_LITERAL(504, 12),  // "createAnswer"
        QT_MOC_LITERAL(517, 13)   // "peerIsOfferer"
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
    "peerIsConnected",
    "peerIsOffererChanged",
    "onSignalingServerConnected",
    "onSignalingServerDisconnected",
    "onSignalingMessageReceived",
    "sendOffer",
    "sendAnswer",
    "onAudioCaptured",
    "audioData",
    "sendTrack",
    "peerId",
    "buffer",
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
    "createAnswer",
    "peerIsOfferer"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSWebRTCENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       1,  227, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  176,    2, 0x06,    2 /* Public */,
       4,    1,  179,    2, 0x06,    4 /* Public */,
       6,    1,  182,    2, 0x06,    6 /* Public */,
       8,    0,  185,    2, 0x06,    8 /* Public */,
       9,    1,  186,    2, 0x06,    9 /* Public */,
      11,    0,  189,    2, 0x06,   11 /* Public */,
      12,    0,  190,    2, 0x06,   12 /* Public */,
      13,    0,  191,    2, 0x06,   13 /* Public */,
      14,    0,  192,    2, 0x06,   14 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      15,    0,  193,    2, 0x0a,   15 /* Public */,
      16,    0,  194,    2, 0x0a,   16 /* Public */,
      17,    1,  195,    2, 0x0a,   17 /* Public */,
      18,    0,  198,    2, 0x0a,   19 /* Public */,
      19,    0,  199,    2, 0x0a,   20 /* Public */,
      20,    1,  200,    2, 0x0a,   21 /* Public */,
      22,    2,  203,    2, 0x0a,   23 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      25,    1,  208,    2, 0x02,   26 /* Public */,
      27,    0,  211,    2, 0x02,   28 /* Public */,
      28,    0,  212,    2, 0x02,   29 /* Public */,
      29,    0,  213,    2, 0x02,   30 /* Public */,
      30,    0,  214,    2, 0x02,   31 /* Public */,
      31,    0,  215,    2, 0x02,   32 /* Public */,
      32,    1,  216,    2, 0x02,   33 /* Public */,
      33,    1,  219,    2, 0x02,   35 /* Public */,
      34,    1,  222,    2, 0x02,   37 /* Public */,
      35,    0,  225,    2, 0x02,   39 /* Public */,
      36,    0,  226,    2, 0x02,   40 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QByteArray,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,   23,   24,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      37, QMetaType::Bool, 0x00015107, uint(8), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject WebRTC::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSWebRTCENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSWebRTCENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSWebRTCENDCLASS_t,
        // property 'peerIsOfferer'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
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
        // method 'peerIsConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'peerIsOffererChanged'
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
        // method 'onAudioCaptured'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        // method 'sendTrack'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
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
        case 7: _t->peerIsConnected(); break;
        case 8: _t->peerIsOffererChanged(); break;
        case 9: _t->onSignalingServerConnected(); break;
        case 10: _t->onSignalingServerDisconnected(); break;
        case 11: _t->onSignalingMessageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->sendOffer(); break;
        case 13: _t->sendAnswer(); break;
        case 14: _t->onAudioCaptured((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 15: _t->sendTrack((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[2]))); break;
        case 16: _t->setId((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->init(); break;
        case 18: _t->registerClient(); break;
        case 19: _t->connectToSignalingServer(); break;
        case 20: _t->createOffer(); break;
        case 21: _t->callOnRun(); break;
        case 22: _t->setRemoteDescription((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: _t->addRemoteCandidate((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 24: _t->setTargetId((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: _t->checkWebSocketState(); break;
        case 26: _t->createAnswer(); break;
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
        {
            using _t = void (WebRTC::*)();
            if (_t _q_method = &WebRTC::peerIsConnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (WebRTC::*)();
            if (_t _q_method = &WebRTC::peerIsOffererChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<WebRTC *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->getPeerIsOfferer(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<WebRTC *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPeerIsOfferer(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
        auto *_t = static_cast<WebRTC *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->resetPeerIsOfferer(); break;
        default: break;
        }
    } else if (_c == QMetaObject::BindableProperty) {
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
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 27)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 27;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
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

// SIGNAL 7
void WebRTC::peerIsConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void WebRTC::peerIsOffererChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP
