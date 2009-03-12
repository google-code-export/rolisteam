######################################################################
# Automatically generated by qmake (2.01a) sam. janv. 24 18:06:11 2009
######################################################################

TEMPLATE = app
TARGET = bin/Rolisteam
DEPENDPATH += . debug release src
INCLUDEPATH += . src
LANGUAGE = C++
# Input
HEADERS += src/AfficheurDisque.h \
           src/BarreOutils.h \
           src/Carte.h \
           src/CarteFenetre.h \
           src/ClientServeur.h \
           src/constantesGlobales.h \
           src/DessinPerso.h \
           src/EditeurNotes.h \
           src/Image.h \
           src/LecteurAudio.h \
           src/Liaison.h \
           src/ListeUtilisateurs.h \
           src/MainWindow.h \
           src/NouveauPlanVide.h \
           src/Rolisteam_private.h \
           src/SelecteurCouleur.h \
           src/SelecteurDiametre.h \
           src/Tchat.h \
           src/TextEditAmeliore.h \
           src/types.h \
           src/typesApplication.h \
           src/variablesGlobales.h \
           src/wincompat.h \
           src/WorkspaceAmeliore.h
FORMS += src/LecteurAudio.ui
SOURCES += src/AfficheurDisque.cpp \
           src/BarreOutils.cpp \
           src/Carte.cpp \
           src/CarteFenetre.cpp \
           src/ClientServeur.cpp \
           src/DessinPerso.cpp \
           src/EditeurNotes.cpp \
           src/Image.cpp \
           src/Liaison.cpp \
           src/ListeUtilisateurs.cpp \
           src/main.cpp \
           src/MainWindow.cpp \
           src/NouveauPlanVide.cpp \
           src/SelecteurCouleur.cpp \
           src/SelecteurDiametre.cpp \
           src/Tchat.cpp \
           src/TextEditAmeliore.cpp \
           src/WorkspaceAmeliore.cpp


win32 {
 SOURCES +=  src/LecteurAudiowin.cpp
 HEADERS +=  src/fmod.h  src/fmod_errors.h
}
unix {
 SOURCES +=  src/LecteurAudiounix.cpp
}
macx {
 SOURCES +=  src/LecteurAudiounix.cpp
}
RESOURCES += Rolisteam.qrc
OBJECTS_DIR = bin
QT += xml \
 phonon \
 core \
 gui \
 network
CONFIG += debug
VERSION = 1.1
MOC_DIR = src
UI_DIR = src
QMAKE_CXXFLAGS = "-fstack-protector -W -Wall -Wextra -pedantic -Wstack-protector -Wno-long-long -Wno-overlength-strings"
