//----------------------file: MainDlg.h-----------------------------

#include <MASkinG.h>
#include "anaclock.h"
using namespace MAS;

#ifndef			MAINDLG_H
#define			MAINDLG_H


class MainDlg : public Dialog {
	protected:
		ClearScreen desktop;
		Panel pBox;
		Button bQuit;
		Slider slider;
		EditBox textSlider;
		Label textInfo;
		AnaClock anaclock;

		Menu menuFile, menuEdit, menuHelp, menuMain;
		enum { FILE_NEW = MSG_SUSER, FILE_OPEN, FILE_SAVE, FILE_QUIT, EDIT_COPY, EDIT_PASTE, EDIT_CUT, HELP_CONTENTS, HELP_ABOUT };
		void MakeMenu();

	public:
		MainDlg();
		virtual ~MainDlg();

		void HandleEvent(Widget &obj, int msg, int arg1=0, int arg2=0);
		void MsgInitSkin();
};

#endif			//MAINDLG_H

//---------------------------eof------------------------------------
