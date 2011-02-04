//---------------------file: MainDlg.cpp----------------------------

#include "MainDlg.h"


MainDlg::MainDlg() : Dialog() {
	pBox.Shape(10, 30, 620, 440);
	bQuit.Setup(20, 440, 150, 20, KEY_Q, D_EXIT, "&Quit");
	slider.Setup(30, 40, 20, 200, 0, 0, 0, 9, 0, 0);
	textSlider.Setup(25, 245, 30, 20, 0, D_DISABLED, "0", 2);
	textInfo.ClearFlag(D_AUTOSIZE);
	textInfo.Setup(320, 450, 300, 20, 0, 0, "This is a tutorial application v1.0", 1);
	anaclock.Shape(160, 80, 200, 200);
	MakeMenu();

	Add(desktop);
	Add(pBox);
	Add(bQuit);
	Add(slider);
	Add(textSlider);
	Add(textInfo);
	Add(anaclock);
	Add(menuMain);
}


MainDlg::~MainDlg() {
}


void MainDlg::MakeMenu() {
	menuFile.Add("&New\tCtrl-N", FILE_NEW);
	menuFile.Add("&Open\tCtrl-O", FILE_OPEN);
	menuFile.Add("&Save\tCtrl-S", FILE_SAVE);
	menuFile.Add();
	menuFile.Add("&Quit\tCtrl-Q", FILE_QUIT);
	
	menuEdit.Add("&Copy\tCtrl-C", EDIT_COPY);
	menuEdit.Add("&Paste\tCtrl-V", EDIT_PASTE);
	menuEdit.Add("Cu&t\tCtrl-X", EDIT_CUT);
	
	menuHelp.Add("&Contents\tF1", HELP_CONTENTS);
	menuHelp.Add("&About\tCtrl-A", HELP_ABOUT);
	
	menuMain.Add("&File", menuFile);
	menuMain.Add("&Edit", menuEdit);
	menuMain.Add("&Help", menuHelp);
}


void MainDlg::HandleEvent(Widget &obj, int msg, int arg1, int arg2) {
	Dialog::HandleEvent(obj, msg, arg1, arg2);

	switch (msg) {
		case MSG_SCROLL:
			if (obj == slider) {
				textSlider.SetNumber(arg1);
			}
			break;

		case FILE_QUIT:
			Close();
			break;
	};
}


void MainDlg::MsgInitSkin() {
	Dialog::MsgInitSkin();

	textInfo.SetFontColor(Color(128,128,196), Color::white, Skin::NORMAL);
}

//---------------------------eof------------------------------------
