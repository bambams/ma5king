////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/wallpaper.h"


MAS::Wallpaper::Wallpaper()
:Image(),
   style(UNDEFINED)
{
}


void MAS::Wallpaper::SetStyle(WallpaperStyle style) {
   this->style = style;
}


void MAS::Wallpaper::Setup(int x, int y, int w, int h, int key, int flags, int i, WallpaperStyle style) {
   Image::Setup(x, y, w, h, key, flags, i);
   SetStyle(style);
}


void MAS::Wallpaper::Setup(int x, int y, int w, int h, int key, int flags, const char *file, WallpaperStyle style) {
   Image::Setup(x, y, w, h, key, flags, file);
   SetStyle(style);
}


void MAS::Wallpaper::Setup(int x, int y, int w, int h, int key, int flags, const Bitmap& bmp, WallpaperStyle style) {
   Image::Setup(x, y, w, h, key, flags, bmp);
   SetStyle(style);
}


void MAS::Wallpaper::UpdateSize() {
   if (parent) {
      Shape(0, 0, 100, 100, true);
   }
   else {
      Image::UpdateSize();
   }
}


void MAS::Wallpaper::MsgStart() {
   UpdateSize();
   Image::MsgStart();
}


void MAS::Wallpaper::MsgInitSkin() {
   if (bitmap == -1 && !bmp) SetBitmap(Skin::CLEAR_BACK);
   if (style == UNDEFINED) SetStyle((WallpaperStyle)skin->wallpaperStyle);
   if (bg == Color::transparent) SetBackgroundColor(skin->c_back);
   Widget::MsgInitSkin();
}


void MAS::Wallpaper::Draw(Bitmap &canvas) {
   if (!this->bmp && bitmap < 0) {
      return;
   }

   Bitmap bmp = this->bmp ? this->bmp : skin->GetBitmap(bitmap);
   if (!bmp) {
      return;
   }

   int xx, yy;
   int i, j;

   switch (style) {
      case CENTERED:
         canvas.Clear(bg ? bg : skin->c_back);
         xx = (w() - bmp.w())>>1;
         yy = (h() - bmp.h())>>1;
         bmp.Blit(canvas, 0, 0, xx, yy, bmp.w(), bmp.h());
         break;

      case TOPLEFT:
         canvas.Clear(bg ? bg : skin->c_back);
         xx = 0;
         yy = 0;
         bmp.Blit(canvas, 0, 0, xx, yy, bmp.w(), bmp.h());
         break;

      case TILED: {
         canvas.Clear(bg ? bg : skin->c_back);
         int bw = bmp.w();
         int bh = bmp.h();
         for (i=0; i<w(); i += bw) {
            for (j=0; j<h(); j += bh) {
               bmp.Blit(canvas, 0, 0, i, j, bw, bh);
            }
         }
         break;
      }

      case TILED2:
         canvas.Clear(bg ? bg : skin->c_back);
         bmp.TiledBlit(canvas, 0, 0, 0, 0, w(), h(), 1, 1);
         break;

      case STRETCHED:
         canvas.Clear(bg ? bg : skin->c_back);
         bmp.StretchBlit(canvas, 0, 0, bmp.w(), bmp.h(), 0, 0, w(), h());
         break;

      default: assert(0);
   };
}


MAS::Wallpaper::WallpaperStyle MAS::Wallpaper::GetStyle() {
   return style;
}
