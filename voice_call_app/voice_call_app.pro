QT += quick qml quickcontrols2 quicklayouts multimedia

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++20
CONFIG += no_keyword
CXXFLAGS += -fstack-protector

SOURCES += \
        audioinput.cpp \
        audiooutput.cpp \
        main.cpp \
        webrtc.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    audioinput.h \
    audiooutput.h \
    webrtc.h

DISTFILES += \
    main.qml
# Additional libraries
# ### libdatachannel
INCLUDEPATH += D:/UT/CN/CA1/git/CN_CA_1_/voice_call_app/libdatachannel/include
LIBS += -LD:/UT/CN/CA1/git/CN_CA_1_/voice_call_app/libdatachannel/Windows/Mingw64 -ldatachannel.dll
LIBS += -LC:/Qt/Tools/OpenSSLv3/Win_x64/bin -lcrypto-3-x64 -lssl-3-x64
LIBS += -lws2_32
LIBS += -lssp

# ### opus
INCLUDEPATH += D:/UT/CN/CA1/opus/include
LIBS += -LD:/UT/CN/CA1/git/CN_CA_1_/voice_call_app/opus/Windows/Mingw64 -lopus
