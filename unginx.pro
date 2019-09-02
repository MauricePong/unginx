QT += core
QT -= gui
QT += network

TARGET = unginx
CONFIG += c++11 console
CONFIG -= app_bundle
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
#CONFIG += static
SOURCES += \
    src/Startup.cpp \
    src/main.cpp \
    src/requestmapper.cpp \
    src/controller/dumpcontroller.cpp \
    src/controller/templatecontroller.cpp \
    src/controller/formcontroller.cpp \
    src/controller/fileuploadcontroller.cpp \
    src/controller/sessioncontroller.cpp

HEADERS += \
    src/Startup.h \
    src/requestmapper.h \
    src/controller/dumpcontroller.h \
    src/controller/templatecontroller.h \
    src/controller/formcontroller.h \
    src/controller/fileuploadcontroller.h \
    src/controller/sessioncontroller.h


include(QtWebApp/httpserver/httpserver.pri)
include(QtWebApp/templateengine/templateengine.pri)
include(QtWebApp/logging/logging.pri)
include(QtWebApp/qtservice/qtservice.pri)


OTHER_FILES += docroot/*.html           \
               docroot/files/css/*      \
               docroot/files/*.html     \
               docroot/files/jquery/*   \
               docroot/templates/*      \
               etc/*.ini                \
               etc/ssl/*                \
               logs/*.log               \
               *.md

RC_FILE  += \
    resources/applogo.rc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


