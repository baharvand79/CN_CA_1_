QT += quick multimedia network websockets widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17
CONFIG += no_keyword
CXXFLAGS += -fstack-protector

#CONFIG += moc_depends

# Sources
SOURCES += \
    Audio/audioinput.cpp \
    Audio/audiooutput.cpp \
    WebRTC/webrtc.cpp \
    main.cpp

#RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD/UI

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = $$PWD/UI

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Headers
HEADERS += \
    Audio/audioinput.h \
    Audio/audiooutput.h \
    WebRTC/webrtc.h

DISTFILES +=
# Additional libraries
# ### libdatachannel
INCLUDEPATH += $$PWD/libdatachannel/include
LIBS += -L$$PWD/libdatachannel/Windows/Mingw64 -ldatachannel.dll
LIBS += -LC:/Qt/Tools/OpenSSLv3/Win_x64/bin -lcrypto-3-x64 -lssl-3-x64
LIBS += -lws2_32
LIBS += -lssp

# ### opus
INCLUDEPATH += $$PWD/opus/include
LIBS += -L$$PWD/opus/Windows/Mingw64 -lopus
CXXFLAGS += -fstack-protector

## ### socket io
#HEADERS += \
# $$PWD/SocketIO/sio_client.h \
# $$PWD/SocketIO/sio_message.h \
# $$PWD/SocketIO/sio_socket.h \
# $$PWD/SocketIO/internal/sio_client_impl.h \
# $$PWD/SocketIO/internal/sio_packet.h
#SOURCES += \
# $$PWD/SocketIO/sio_client.cpp \
# $$PWD/SocketIO/sio_socket.cpp \
# $$PWD/SocketIO/internal/sio_client_impl.cpp \
# $$PWD/SocketIO/internal/sio_packet.cpp
#INCLUDEPATH += $$PWD/socket.io-client-cpp/lib/websocketpp
#INCLUDEPATH += $$PWD/socket.io-client-cpp/lib/asio/asio/include
#INCLUDEPATH += $$PWD/socket.io-client-cpp/lib/rapidjson/include
#DEFINES += _WEBSOCKETPP_CPP11_STL_
#DEFINES += _WEBSOCKETPP_CPP11_FUNCTIONAL_
#DEFINES += SIO_TLS

# to fix boost
DEFINES += BOOST_DATE_TIME_NO_LIB
DEFINES += BOOST_REGEX_NO_LIB
DEFINES +=ASIO_STANDLONE
DEFINES += _WEBSOCKETPP_CPP11_STL_
DEFINES += _WEBSOCKET_CPP11_FUNCTIONAL_
DEFINES += SIO_TLS

DEFINES += ASIO_STANDALONE


# to fix openssl
INCLUDEPATH += C:/Qt/Tools/OpenSSLv3/Win_x64/include
#INCLUDEPATH += C:/Qt/Tools/OpenSSLv3/src/include

RESOURCES += \
    qml.qrc

