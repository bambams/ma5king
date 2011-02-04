////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/colorsel.h"


MAS::ColorSelect::ColorSelect(const char *title, Color color)
:Window()
{
   this->title.SetText(title);
   ClearFlag(D_RESIZABLE);
   
   clientArea->Resize(320, 220);
   Resize(320, 220);

   panel.Setup(4, 4, 312, 212, 0, 0);
   lR.Setup(  12, 16,   8, 8, 0, 0, "R", 0);
   lG.Setup(  12, 44,   8, 8, 0, 0, "G", 0);
   lB.Setup(  12, 72,   8, 8, 0, 0, "B", 0);
   sR.Setup(  24, 12, 256, 8, 0, 0, 0, 255, 0, 1);
   sG.Setup(  24, 40, 256, 8, 0, 0, 0, 255, 0, 1);
   sB.Setup(  24, 68, 256, 8, 0, 0, 0, 255, 0, 1);
   tR.Setup( 284, 16,  24,18, 0, 0, "-", 2);   tR.ClearFlag(D_AUTOSIZE);
   tG.Setup( 284, 44,  24,18, 0, 0, "-", 2);   tG.ClearFlag(D_AUTOSIZE);
   tB.Setup( 284, 72,  24,18, 0, 0, "-", 2);   tB.ClearFlag(D_AUTOSIZE);
   sep.Setup(10, 94, 300, 2, 0, 0, 1);
   lH.Setup(  12,108,   8, 8, 0, 0, "H", 0);
   lS.Setup(  12,136,   8, 8, 0, 0, "S", 0);
   lV.Setup(  12,164,   8, 8, 0, 0, "V", 0);
   sH.Setup(  24,104, 256, 8, 0, 0, 0, 359, 0, 1);
   sS.Setup(  24,132, 256, 8, 0, 0, 0, 255, 0, 1);
   sV.Setup(  24,160, 256, 8, 0, 0, 0, 255, 0, 1);
   tH.Setup( 284,108,  24,18, 0, 0, "-", 2);   tH.ClearFlag(D_AUTOSIZE);
   tS.Setup( 284,136,  24,18, 0, 0, "-", 2);   tS.ClearFlag(D_AUTOSIZE);
   tV.Setup( 284,160,  24,18, 0, 0, "-", 2);   tV.ClearFlag(D_AUTOSIZE);
   bOK.Setup(112,188,  96,22, ALLEGRO_KEY_O, D_EXIT, "&OK");
   bCancel.Setup(216,188, 96, 22, ALLEGRO_KEY_C, D_EXIT, "&Cancel");
   colRect.Setup(8,188,96,22,0,0);

   int r =color.r();
   int g = color.g();
   int b = color.b();

   sR.SetPosition(r);
   sG.SetPosition(g);
   sB.SetPosition(b);

   tR.SetNumber((int)r);
   tG.SetNumber((int)g);
   tB.SetNumber((int)b);

   float h, s, v;
   Color::rgb_to_hsv(r, g, b, &h, &s, &v);
   sH.SetPosition((int)h);
   sS.SetPosition((int)(s*256.0f));
   sV.SetPosition((int)(v*256.0f));

   tH.SetNumber((int)h);
   tS.SetNumber((int)(s*256.0f));
   tV.SetNumber((int)(v*256.0f));

   Add(panel);
   Add(lR);   Add(lG);   Add(lB);
   Add(sR);   Add(sG);   Add(sB);
   Add(tR);   Add(tG);   Add(tB);
   Add(sep);
   Add(lH);   Add(lS);   Add(lV);
   Add(sH);   Add(sS);   Add(sV);
   Add(tH);   Add(tS);   Add(tV);
   Add(bOK);
   Add(bCancel);
   Add(colRect);
}


void MAS::ColorSelect::MsgInitSkin() {
   Window::MsgInitSkin();

   tR.SetTextMode(skin->c_face);
   tG.SetTextMode(skin->c_face);
   tB.SetTextMode(skin->c_face);
   tH.SetTextMode(skin->c_face);
   tS.SetTextMode(skin->c_face);
   tV.SetTextMode(skin->c_face);
   colRect.bg = MAS::Color((int)sR.GetPosition(), (int)sG.GetPosition(), (int)sB.GetPosition());
}


void MAS::ColorSelect::HandleEvent(Widget &obj, int msg, intptr_t arg1, intptr_t arg2) {
   Window::HandleEvent(obj, msg, arg1, arg2);

   int r,g,b;
   float h,s,v;

   if (msg == MSG_SCROLL) {
      if (obj == sR || obj == sG || obj == sB) {
         r = (int)sR.GetPosition();
         g = (int)sG.GetPosition();
         b = (int)sB.GetPosition();
         Color::rgb_to_hsv(r,g,b,&h,&s,&v);
      }
      else {
         h = (float)sH.GetPosition();
         s = (float)sS.GetPosition()/256.0f;
         v = (float)sV.GetPosition()/256.0f;
         Color::hsv_to_rgb(h,s,v,&r,&g,&b);
      }

      sH.SetPosition((int)h);            tH.SetNumber((int)sH.GetPosition());
      sS.SetPosition((int)(s*256.0f));   tS.SetNumber((int)sS.GetPosition());
      sV.SetPosition((int)(v*256.0f));   tV.SetNumber((int)sV.GetPosition());
      sR.SetPosition(r);               tR.SetNumber((int)sR.GetPosition());
      sG.SetPosition(g);               tG.SetNumber((int)sG.GetPosition());
      sB.SetPosition(b);               tB.SetNumber((int)sB.GetPosition());
      colRect.bg = MAS::Color(r,g,b);
   }
}


MAS::Color MAS::ColorSelect::Popup(MAS::Dialog *p, int xx, int yy) {
   SetParent(p);
   Centre();
   if (xx != INT_MAX) {
      x(xx);
      if (yy != INT_MAX) {
         y(yy);
      }
   }

   /* TODO: popups
   Color color = Color((int)sR.GetPosition(), (int)sG.GetPosition(), (int)sB.GetPosition()); 
   if (Window::Popup(p, xx, yy, &bOK) == &bOK) {
      return Color((int)sR.GetPosition(), (int)sG.GetPosition(), (int)sB.GetPosition());
   }
   else {
      return Color::transparent;
   }
   */
   return Color::transparent;
}


void MAS::ColorSelect::ColorPreview::Draw(Bitmap &canvas) {
   canvas.Clear(bg);
}
