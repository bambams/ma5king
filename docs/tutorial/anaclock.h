#ifndef		ANACLOCK_H
#define		ANACLOCK_H

#include <MASkinG.h>

class AnaClock : public MAS::Widget {
	protected:
		int timerID;
	
		void MsgStart();
		void MsgEnd();
		void MsgTimer(int ID);
		void Draw(MAS::Bitmap &canvas);
		bool MsgWantfocus();
		bool MsgWantmouse();
		void MsgLPress();
		void MsgLRelease();
		void MsgMousemove(const MAS::Point &d);
		bool MsgChar(int c);
		void MsgLostfocus();
	
		void Move(const MAS::Point &d);
	
	public:
		AnaClock();
		~AnaClock();
};

#endif		//ANACLOCK_H
