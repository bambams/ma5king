// example 9: Demonstrates how you can use MASkinG side by side with an existing engine
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "MASkinG.h"

using namespace MAS;

#define	NSTARS	200


// a simple dialog
class MyDialog : public Dialog {
   private:
      PanelRaised panel;
      TextArea info;
      Button bExit;

   public:
      MyDialog() {
         info.AddText("Example 01:");
         info.AddText("Hello! Welcome to MA5kinG - a MASkinG for A5. This is the first");
         info.AddText("example program which resembles ex09, just to see how things work.");
         info.AddText("Contribute more examples, please.");

         // Put a panel somewhere in the middle and a couple of widgets
         // on top of it.
         // Note that a top level dialog must take the whole screen!
         panel.Shape(120, 90, 460, 220);
         info.Shape(130, 100, 440, 170);
         bExit.Setup(290, 278, 120, 24, ALLEGRO_KEY_X, D_EXIT, "E&xit");
         bExit.SetTooltipText("Clicking will exit!");

         Add(panel);
         Add(info);
         Add(bExit);
      }
};


// a simple star class
class Star {
   protected:
      float x, y;
      float vy;
      int w, h;
      ALLEGRO_COLOR col;

      void NewStar() {
         x = (float)(rand() % w);
         y = 0.0f;
         int l = rand() % 128 + 128;
         vy = (float)(l) / 128.0f;
         col = al_map_rgb(l, l, l);
   }

   public:
      Star() {
         w = al_get_display_width(al_get_current_display());
         h = al_get_display_height(al_get_current_display());
         NewStar();
         y = (float)(rand() % h);
      }

      void Draw() {
         al_draw_pixel((int)x, (int)y, col);
      }

      void Update() {
         y += vy;
         if ((int)y >= h) {
	         NewStar();
         }
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

   // make some stars;
   Star stars[NSTARS];

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
               // periodically update the stars
               for (int i = 0; i < NSTARS; i++) {
                  stars[i].Update();
               }

               /* Perform MASkinG() logic tick. */
               dlg->DoTick();

               /* Skip the drawing if we've got more events to process. */
               if (!al_event_queue_is_empty(queue))
                  continue;

               /* Redraw the screen */
               al_clear_to_color(al_map_rgb(0, 0, 0));
               for (int i = 0; i < NSTARS; i++) {
                  stars[i].Draw();
               }
               dlg->Draw();
               al_flip_display();
            }
         break;
         default:
            /* Let MASkinG process the event.
               Note: if you also send the tick timer event to it, the event
               will propage to all widgets as MsgTimer(). This is however not
               necessary here, but you can use this feture to send timed events
               to widgets. */
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
