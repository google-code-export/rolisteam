INCLUDEPATH = /home/renaud/applications/rolisteam/src \
               /home/renaud/applications/rolisteam/bin
DEPENDPATH = /home/renaud/applications/rolisteam/src \
             /home/renaud/applications/rolisteam/bin
DEFINES += VERSION_MAJOR=2 VERSION_MIDDLE=0 VERSION_MINOR=0



ROLISTEAMSRC = ../../rolisteam #/home/renaud/applications/rolisteam


DEFINES+= PHONON
TEMPLATE=app
QT += core \
 gui \
 network \
 phonon \
 testlib
CONFIG += phonon
