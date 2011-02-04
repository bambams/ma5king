//--------------------file: tutorial.cpp----------------------------

#include "MainDlg.h"
   
int main() {
	Error e = InstallMASkinG("allegro.cfg");
	if (e) {
		e.Report();
	}

	MainDlg *dlg = new MainDlg;
	dlg->Execute();
	delete dlg;

	ExitMASkinG();
	return 0;
}
END_OF_MAIN();

//---------------------------eof------------------------------------
