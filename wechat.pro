#-------------------------------------------------
#
# Project created by QtCreator 2018-11-23T21:49:34
#
#-------------------------------------------------
#DEFINES  -= UNICODE

#DEFINES  += UMBCS
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wechat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    login.cpp \
    login.cpp \
    main.cpp \
    widget.cpp \
    thirdparty/tinyxml/tinyxml2.cpp \
    config.cpp \
    userpreference.cpp \
    md5.cpp \
    btn.cpp \
    bindphone.cpp

HEADERS += \
        widget.h \
    login.h \
    thirdparty/rapidjson/error/en.h \
    thirdparty/rapidjson/error/error.h \
    thirdparty/rapidjson/internal/biginteger.h \
    thirdparty/rapidjson/internal/diyfp.h \
    thirdparty/rapidjson/internal/dtoa.h \
    thirdparty/rapidjson/internal/ieee754.h \
    thirdparty/rapidjson/internal/itoa.h \
    thirdparty/rapidjson/internal/meta.h \
    thirdparty/rapidjson/internal/pow10.h \
    thirdparty/rapidjson/internal/regex.h \
    thirdparty/rapidjson/internal/stack.h \
    thirdparty/rapidjson/internal/strfunc.h \
    thirdparty/rapidjson/internal/strtod.h \
    thirdparty/rapidjson/internal/swap.h \
    thirdparty/rapidjson/msinttypes/inttypes.h \
    thirdparty/rapidjson/msinttypes/stdint.h \
    thirdparty/rapidjson/allocators.h \
    thirdparty/rapidjson/document.h \
    thirdparty/rapidjson/encodedstream.h \
    thirdparty/rapidjson/encodings.h \
    thirdparty/rapidjson/filereadstream.h \
    thirdparty/rapidjson/filewritestream.h \
    thirdparty/rapidjson/fwd.h \
    thirdparty/rapidjson/istreamwrapper.h \
    thirdparty/rapidjson/memorybuffer.h \
    thirdparty/rapidjson/memorystream.h \
    thirdparty/rapidjson/ostreamwrapper.h \
    thirdparty/rapidjson/pointer.h \
    thirdparty/rapidjson/prettywriter.h \
    thirdparty/rapidjson/rapidjson.h \
    thirdparty/rapidjson/reader.h \
    thirdparty/rapidjson/schema.h \
    thirdparty/rapidjson/stream.h \
    thirdparty/rapidjson/stringbuffer.h \
    thirdparty/rapidjson/writer.h \
    thirdparty/rapidxml/rapidxml.hpp \
    thirdparty/rapidxml/rapidxml_iterators.hpp \
    thirdparty/rapidxml/rapidxml_print.hpp \
    thirdparty/rapidxml/rapidxml_utils.hpp \
    login.h \
    widget.h \
    status.h \
    thirdparty/tinyxml/tinyxml2.h \
    config.h \
    userpreference.h \
    md5.h \
    btn.h \
    globalconf.h \
    bindphone.h \
    conf.h \
    widgetconf.h

FORMS += \
        widget.ui \
    login.ui \
    userpreference.ui \
    bindphone.ui

win32: LIBS += -lUser32

RESOURCES += \
    res/res.qrc

DISTFILES +=

TRANSLATIONS += ch.ts
