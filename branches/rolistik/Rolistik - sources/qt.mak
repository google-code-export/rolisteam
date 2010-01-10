QT_MOC_UI=qt-moc-ui.exe

all-before: qt_moc_ui $(patsubst %.cpp,%.o,$(wildcard moc_*.cpp))  moclib.a

qt_moc_ui:
	$(QT_MOC_UI)

moc_%.o: moc_%.cpp
	$(CPP) -c $< -o $@ $(CXXFLAGS)		

moclib.a:
	ar cq moclib.a $(wildcard moc_*.o) 

clean-custom:
	${RM} moc_*.cpp
	${RM} ui_*.h

