// example 2: Quite a complex example showing how to use most of the
// widgets MASkinG provides.

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "MASkinG.h"

using namespace MAS;

class MyDialog : public Dialog {
   protected:
      ClearScreen desktop;
      Label hello, hello2;
      Button b[5];
      Button wb[5];
      HyperText link1, link2;
      CheckBox ck[5];
      RadioButton rb1[3], rb2[3];
      Panel pan[5];
      Separator sep[2];
      Progress prg[2];
      Slider sld[2];
      Scroller scr[2];
      ListBox lst;
      ComboBox cmb;
      GroupBox gb;
      Button gbButton;
      EditBox edit;
      SpinBox spin;
      Shortcut sc[3];

   public:
      MyDialog() : Dialog() {
         int i;

         // setup the first panel: it's a sunken panel in the top left and
         // it has a label saying "Hello, world!" in it
         pan[0].Shape(40, 10, 110, 60);
         hello.Place(45, 30);
         hello.SetText("Hello, world!");

         // setup the second panel: it's the panel in the centre with
         // five toggle buttons on the left and five push buttons on
         // the right; two of the buttons are actually hidden
         pan[1].Shape(190, 190, 280, 210);
         b[0].Setup(200, 200, 120, 24, ALLEGRO_KEY_O, D_EXIT | D_TOGGLE, "&OK");
         b[1].Setup(200, 230, 120, 24, ALLEGRO_KEY_Q, D_EXIT | D_TOGGLE, "Cancel");
         b[2].Setup(200, 260, 120, 24, ALLEGRO_KEY_D, D_EXIT | D_TOGGLE | D_DISABLED, "&Disabled");
         b[3].Setup(200, 290, 120, 24, ALLEGRO_KEY_H, D_EXIT | D_TOGGLE | D_HIDDEN, "&Hidden");
         b[4].Setup(200, 320, 120, 24, ALLEGRO_KEY_T, D_TOGGLE, "&Toggle me");
         wb[0].Setup(340, 200, 120, 24, ALLEGRO_KEY_K, D_EXIT, "O&K");
         wb[1].Setup(340, 230, 120, 24, ALLEGRO_KEY_L, D_EXIT, "Cance&l");
         wb[2].Setup(340, 260, 120, 24, ALLEGRO_KEY_B, D_EXIT | D_DISABLED, "Disa&bled");
         wb[3].Setup(340, 290, 120, 24, ALLEGRO_KEY_N, D_EXIT | D_HIDDEN, "Hidde&n");
         wb[4].Setup(340, 320, 120, 24, ALLEGRO_KEY_P, 0, "&Push me");
         sep[0].Setup(329, 200, 0, 144, 0, 0, 0);

         // setup the third panel: the one with the checkboxes
         pan[2].Shape(10, 90, 140, 120);
         ck[0].Setup(20, 100, 120, 18, 0, 0, "Checkbox 1");
         ck[1].Setup(20, 120, 120, 18, 0, 0, "Checkbox 2");
         ck[2].Setup(20, 140, 120, 18, 0, D_DISABLED, "Disabled");
         ck[3].Setup(20, 160, 120, 18, 0, D_HIDDEN, "Hidden");
         ck[4].Setup(20, 180, 120, 18, 0, 0, "Check me");

         // fourth panel: has the first group of radio buttons
         pan[3].Shape(10, 220, 140, 80);
         rb1[0].Setup(20, 230, 120, 18, 0, D_SELECTED, "Group 1/Radio 1", 0);
         rb1[1].Setup(20, 250, 120, 18, 0, 0, "Group 1/Radio 2", 0);
         rb1[2].Setup(20, 270, 120, 18, 0, 0, "Group 1/Radio 3", 0);

         // fifth panel: the second group of radio buttons
         pan[4].Shape(10, 310, 140, 80);
         rb2[0].Setup(20, 320, 120, 18, 0, D_SELECTED, "Group 2/Radio 1", 1);
         rb2[1].Setup(20, 340, 120, 18, 0, 0, "Group 2/Radio 2", 1);
         rb2[2].Setup(20, 360, 120, 18, 0, D_DISABLED, "Group 2/Radio 3", 1);

         // place the links below the second panel
         link1.Place(208, 372);
         link1.SetText("This is a link");
         link2.Place(360, 372);
         link2.SetText("This one too");

         // put a couple of progress bars close to the edges of the screen
         prg[0].Setup(10, 450, 580, 30, 0, 0, 0, 1);
         prg[1].Setup(610, 10, 30, 460, 0, 0, 0, 0);
         prg[1].SetRange(0, 400);
         prg[0].SetIncrement(0.5);
         prg[1].SetIncrement(1.5);

         // add a couple of sliders and scrollers
         sld[0].Setup(500, 200, 100,  20, 0, 0, 100, 120,  0, 1);
         sld[1].Setup(500, 230, 100, 100, 0, 0,   0,   8,  2, 0);
         scr[0].Setup(500, 340, 100,  30, 0, 0, 100,  10, 30, 1);
         scr[1].Setup(530, 230,  20, 100, 0, 0,  20,   8,  2, 0);

         scr[0].SetHorizontalIncrement(4);
         scr[1].SetVerticalIncrement(16);

         // setup the list above the central panel
         lst.Shape(160, 10, 200, 130);
         lst.SetTooltipText("This is a list");
         lst.SetMultiSelect(true);
         for (i=0; i<15; i++) lst.InsertItem(new ListItemString("this actually works"));
         ((ListItemString *)lst.GetItem(12))->SetText("this is a very very looooong long line of text");

         // setup the combo box
         cmb.list.InsertItem(new ListItemString("option 1"), 0);
         cmb.list.InsertItem(new ListItemString("option 2"), 1);
         cmb.list.InsertItem(new ListItemString("option 3"), 2);
         cmb.list.InsertItem(new ListItemString("option 4"), 3);
         cmb.list.InsertItem(new ListItemString("option 5"), 4);
         cmb.list.InsertItem(new ListItemString("option 6"), 5);
         cmb.list.InsertItem(new ListItemString("option 7"), 6);
         cmb.Setup(160, 150, 200, 24, 0, 0, 2);
         cmb.list.h(100);
         cmb.editBox.SetFlag(D_READONLY);

         // make the groupbox on the top right
         gb.Shape(400, 10, 200, 165);
         gb.SetTitle("Group box");
         gbButton.Setup(10, 30, 120, 24, 0, 0, "Push me");
         hello2.Setup(20, 60, 100, 20, 0, 0, "Hello, world!", 0);
         edit.Setup(10, 100, 120, 24, 0, 0, "Edit me!");
         spin.Setup(10, 130, 120, 24, 0, 0, 0.0, 1.0, 0.0, 0.01);
         sep[1].Setup(10, 92, 180, 0, 0, 0, 1);
         gb.Add(gbButton);
         gb.Add(hello2);
         gb.Add(edit);
         gb.Add(spin);
         gb.Add(sep[1]);

         // setup some keyboard shortcuts
         sc[0].Setup(ALLEGRO_KEY_D, ALLEGRO_KEYMOD_ALT);
         sc[1].Setup(ALLEGRO_KEY_PAD_PLUS);
         sc[2].Setup(ALLEGRO_KEY_PAD_MINUS);

         // finally add all the widgets to the dialog
         Add(desktop);
         Add(cmb);
         for (i=0; i<5; i++) Add(pan[i]);
         for (i=0; i<5; i++) Add(b[i]);
         for (i=0; i<5; i++) Add(wb[i]);
         for (i=0; i<5; i++) Add(ck[i]);
         for (i=0; i<3; i++) Add(rb1[i]);
         for (i=0; i<3; i++) Add(rb2[i]);
         Add(hello);
         Add(link1);
         Add(link2);
         Add(sep[0]);
         Add(prg[0]);
         Add(prg[1]);
         Add(sld[0]);
         Add(sld[1]);
         Add(scr[0]);
         Add(scr[1]);
         Add(lst);
         Add(gb);
         for (i=0; i<3; i++) Add(sc[i]);
      }

      void MsgInitSkin() {
         Dialog::MsgInitSkin();

         // we can change the default bitmaps, fonts, colors, etc. only
         // in the MsgInitSkin() function; here we'll change some of
         // the panel's bitmaps
         pan[0].SetBitmap(Skin::PANEL_SUNKEN);
         pan[2].SetBitmap(Skin::BOX_SHADOW);
         pan[3].SetBitmap(Skin::PANEL_GROOVE);
         pan[4].SetBitmap(Skin::PANEL_RIDGE);
      }

      void MsgTick() {
         Dialog::MsgTick();

         // we have to manually update the progress bars
         for (int i=0; i<2; i++) {
            prg[i].Increment();
            if (prg[i].GetPosition() >= prg[i].GetMax()) {
               prg[i].SetPosition((int)prg[i].GetMin());
            }
         }
      }

      // handle key presses mouse moves, etc. in this function
   void HandleEvent(Widget &w, int msg, int arg1=0, int arg2=0) {
      switch (msg) {
         case MSG_ACTIVATE: {
            if (w == sc[0]) {
               if (lst.Disabled()) {
                  lst.Enable();
               }
               else {
                  lst.Disable();
               }
               lst.Draw();
            }
            else if (w == sc[1]) {
               lst.InsertItem(new ListItemString("new item"));
               lst.Draw();
            }
            else if (w == sc[2]) {
               lst.DeleteItem(0);
               lst.Draw();
            }
            break;
         }
      };

      Dialog::HandleEvent(w, msg, arg1, arg2);
      }
   };

int main() {
   ALLEGRO_DISPLAY     *display;
   ALLEGRO_EVENT_QUEUE *queue;
   ALLEGRO_TIMER       *timer;

   if (!al_init()) {
      TRACE("Allegro init failed!\n");
      return 1;
   }

   al_init_font_addon();
   al_init_primitives_addon();

   if (!al_init_image_addon()) {
      TRACE("Image addon init failed!\n");
      return 1;
   }

   if (!al_install_keyboard()) {
      TRACE("Keyboard init failed!\n");
      return 1;
   }

   if (!al_install_mouse()) {
      TRACE("Mouse init failed!\n");
      return 1;
   }

   if (!al_init_image_addon()) {
      TRACE("Image addon init failed!\n");
      return 1;
   }

   display = al_create_display(640, 480);
   if (!display) {
      TRACE("Display init failed!\n");
      return 1;
   }

   al_hide_mouse_cursor(display);

   /* Must set this before calling InstallMASkinG() ! */
   MAS::SetLogicRate(1.0 / 0.02);

   /* MASkinG must be initialized after the display. */
   if (InstallMASkinG("allegro.cfg") != MAS::Error::NONE ) {
      TRACE("MA5kinG init failed!\n");
      return 1;
   }

   timer = al_create_timer(0.02);

   queue = al_create_event_queue();
   al_register_event_source(queue, (ALLEGRO_EVENT_SOURCE *) al_get_keyboard_event_source());
   al_register_event_source(queue, (ALLEGRO_EVENT_SOURCE *) al_get_mouse_event_source());
   al_register_event_source(queue, (ALLEGRO_EVENT_SOURCE *) display);
   al_register_event_source(queue, (ALLEGRO_EVENT_SOURCE *) timer);

   // create a new dialog on the current display
   MyDialog *dlg = new MyDialog();

   al_start_timer(timer);

   /* You have to call this manually. */
   dlg->Start();

   while (!dlg->ShouldClose()) {
      ALLEGRO_EVENT event;
      al_wait_for_event(queue, &event);

      switch (event.type) {
         case ALLEGRO_EVENT_TIMER:
            /* see if this is our logic tick timer */
            if (event.timer.source == timer) {
               /* Perform MASkinG() logic tick. */
               dlg->DoTick();

               /* Skip the drawing if we've got more events to process. */
               if (!al_event_queue_is_empty(queue))
                  continue;

               /* Redraw the screen */
               al_clear_to_color(al_map_rgb(0, 0, 0));
               dlg->Draw();
               al_flip_display();
            }
         break;
         default:
            dlg->ProcessEvent(&event);
      }
   }

   /* You have to call this manually. */
   dlg->End();

   // delete the dialog
   delete dlg;

   ExitMASkinG();

   al_destroy_event_queue(queue);
   al_destroy_timer(timer);
   al_destroy_display(display);

   return 0;
}
