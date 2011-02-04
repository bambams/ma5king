////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/messagebox.h"


MAS::MessageBox::MessageBox(
   const char *title,
      const char *l1,
      const char *l2,
      const char *l3,
      const char *btxt1,
      const char *btxt2,
      const char *btxt3
) : Window()
{
   this->title.SetText(title);
   ClearFlag(D_RESIZABLE);
   
   line1.SetText(l1);
   line2.SetText(l2);
   line3.SetText(l3);
   
   line1.ClearFlag(D_AUTOSIZE);
   line2.ClearFlag(D_AUTOSIZE);
   line3.ClearFlag(D_AUTOSIZE);

   line1.AlignCentre();
   line2.AlignCentre();
   line3.AlignCentre();
   
   button1.SetText(btxt1);
   button2.SetText(btxt2);
   button3.SetText(btxt3);

   button1.MakeExit();
   button2.MakeExit();
   button3.MakeExit();
}


void MAS::MessageBox::CalculateGeometry() {
   // Get the default font and its height
   Font f = skin->GetFont(0);
   int th = f.TextHeight();
   int tmp;

   // Find the width and height of the dialog so that it will
   // hold all the text and buttons but won't be too big
   int ww = f.TextLength(line1.GetText());
   int hh = 2*th + 40;
   if (line2.GetText()) {
      tmp = 0;
      tmp = f.TextLength(line2.GetText());
      ww = ww > tmp ? ww : tmp;
      hh += th + 8;

      if (line3.GetText()) {
         tmp = f.TextLength(line3.GetText());
         ww = ww > tmp ? ww : tmp;
         hh += th + 8;
      }
   }

   ww += 20;

   // Setup the buttons
   int nbuttons = 1;

   if (button2.GetText()) {
      nbuttons++;

      if (button3.GetText()) {
         nbuttons++;
      }
   }
   
   int bw = f.TextLength(button1.GetText());
   if (button2.GetText()) {
      tmp = f.TextLength(button2.GetText());
      bw = tmp>bw ? tmp : bw;
      if (button3.GetText()) {
         tmp = f.TextLength(button3.GetText());
         bw = tmp>bw ? tmp : bw;
      }
   }
   bw += 20;
   bw = bw<60 ? 60 : bw;

   // Make sure the dialog isn't too small (depends on the number
   // of buttons)
   switch (nbuttons) {
      case 1:   ww = ww < bw+40 ? bw+40 : ww;      break;
      case 2:   ww = ww < 2*bw+50 ? 2*bw+50 : ww;   break;
      case 3:   ww = ww < 3*bw+60 ? 3*bw+60 : ww;   break;
   };

   // Setup the static text and add it to the dialog
   line1.Shape(0, 8, ww, th);
   Add(line1);

   if (line2.GetText()) {
      line2.Shape(0, th + 16, ww, th);
      Add(line2);

      if (line3.GetText()) {
         line3.Shape(0, 2*th + 24, ww, th);
         Add(line3);
      }
      else {
         Remove(line3);
      }
   }
   else {
      Remove(line2);
   }

   // Add the buttons to the dialog
   switch (nbuttons) {
      case 1:
         Add(button1);
         button1.Shape(ww/2 - bw/2, hh - th - 20, bw, th + 12);
         Remove(button2);
         Remove(button3);
         break;
      case 2:
         Add(button1);
         Add(button2);
         button1.Shape(ww/2 - bw - 5, hh - th - 20, bw, th + 12);
         button2.Shape(ww/2 +  5, hh - th - 20, bw, th + 12);
         Remove(button3);
         break;
      case 3:
         Add(button1);
         Add(button2);
         Add(button3);
         button1.Shape(ww/2 - 3*bw/2 - 10, hh - th - 20, bw, th + 12);
         button2.Shape(ww/2 -  bw/2, hh - th - 20, bw, th + 12);
         button3.Shape(ww/2 +  bw/2 + 10, hh - th - 20, bw, th + 12);
         break;
   };
   
   clientArea->Resize(ww, hh);
   Resize(clientArea->size());
}


int MAS::MessageBox::Popup(MAS::Dialog *p, const MAS::Point &pos) {
   return Popup(p, pos.x(), pos.y());
}


int MAS::MessageBox::Popup(MAS::Dialog *p, int xx, int yy) {
   SetParent(p);
   CalculateGeometry();
   Centre();
   if (xx != INT_MAX) {
      x(xx);
      if (yy != INT_MAX) {
         y(yy);
      }
   }

   //TODO
   //MAS::Widget *ret = Window::Popup(p, xx, yy, &button1);
   MAS::Widget *ret = NULL;
   if (ret == &button1)
      return 1;
   else if (ret == &button2)
      return 2;
   else if (ret == &button3)
      return 3;
   else
      return 0;
}
