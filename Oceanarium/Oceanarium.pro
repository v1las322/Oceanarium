QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addwindow.cpp \
    main.cpp \
    autorizationwindow.cpp \
    registrationwindow.cpp \
    globals.cpp \
    residentswindow.cpp

HEADERS += \
    addwindow.h \
    autorizationwindow.h \
    registrationwindow.h \
    globals.h \
    residentswindow.h

FORMS += \
    addwindow.ui \
    autorizationwindow.ui \
    registrationwindow.ui \
    residentswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += \target

# Копирование содержимого папки "images" в созданную папку
CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release
CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug

images.path = $$DESTDIR/images
images.files = images/*

# Копирование содержимого папки "resources" в созданную папку
CONFIG(release, debug|release): RESOURCESDIR = $$OUT_PWD/release/resources
CONFIG(debug, debug|release): RESOURCESDIR = $$OUT_PWD/debug/resources

resources.path = $$RESOURCESDIR
resources.files = resources/*

INSTALLS += \
    images \
    resources

win32: RC_ICONS += resources/icon.ico
unix: RC_ICONS += resources/icon.png
