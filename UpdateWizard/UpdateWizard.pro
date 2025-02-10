TEMPLATE = app
QT += core gui
TARGET = "UpdateWizard"

QTITANDIR = $$quote($$(QTITANDIR))

include($$QTITANDIR/src/shared/qtitanfastinfoset.pri)
include($$QTITANDIR/src/shared/qtitanribbon.pri)
include($$QTITANDIR/src/shared/qtitandocking.pri)
include($$QTITANDIR/src/shared/qtitanstyle.pri)
include($$QTITANDIR/src/shared/qtitangrid.pri)

DESTDIR = $$QTITANDIR/bin
DESTDIR = $$member(DESTDIR, 0)$$QTITAN_LIB_PREFIX
DESTDIR = "$$DESTDIR"


!debug_and_release|build_pass {
    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)d
    }
}
HEADERS        = mainwindow.h \
                tabdialog.h \
                treemodel.h \
                treeitem.h

SOURCES        = main.cpp\
                 mainwindow.cpp \
                 tabdialog.cpp \
                 treemodel.cpp \
                 treeitem.cpp
RESOURCES     = UpdateWizard.qrc

!lessThan(QT_VER_MAJ, 5) {
  QT += printsupport
}
