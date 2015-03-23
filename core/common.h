#ifndef __CORE_BEHAVIOR_H_
#define __CORE_BEHAVIOR_H_

#include "game_frame.h"

namespace allframe {

    namespace Colors {

        const ALLEGRO_COLOR BLACK   = al_map_rgb(0,0,0);
        const ALLEGRO_COLOR WHITE   = al_map_rgb(255,255,255);

        const ALLEGRO_COLOR RED     = al_map_rgb(255,0,0);
        const ALLEGRO_COLOR GREEN   = al_map_rgb(0,255,0);
        const ALLEGRO_COLOR BLUE    = al_map_rgb(0,0,255);

        ALLEGRO_COLOR random_color();
        ALLEGRO_COLOR random_color_alpha();

    };

    Point point_rotate(const Point&, double);

    class Bounds {

        public:
            virtual ~Bounds() {}

            virtual bool is_within(const Point&) const = 0;
            virtual bool is_within(double x, double y) const = 0;
            virtual bool is_within(int x, int y) const = 0;

    };

    class BoxBounds : public Bounds {

        public:
            BoxBounds(const Point& size) : size(size) {}
            bool is_within(const Point&) const;
            bool is_within(double x, double y) const;
            bool is_within(int x, int y) const;

        protected:
            Point size;
    };

    class CenterBoxBounds : public BoxBounds {

        public: 
            CenterBoxBounds(const Point& size) : BoxBounds(size) {}
            bool is_within(const Point&) const;
            bool is_within(double x, double y) const;
            bool is_within(int x, int y) const;

    };

    class CircleBounds : public Bounds {

        public:
            CircleBounds(double radius) : radius(radius) {} 
            bool is_within(const Point&) const;
            bool is_within(double x, double y) const;
            bool is_within(int x, int y) const;

        private:
            double radius;

    };

    class OvalBounds : public Bounds {

        public:
            OvalBounds(const Point& radii) : radii(radii) {}
            bool is_within(const Point&) const;
            bool is_within(double x, double y) const;
            bool is_within(int x, int y) const;

        private:
            Point radii;

    };

    class Clickable : public ObjectBehavior {

        public:
            virtual ~Clickable() { delete bounds; }

            void mouse_down(ALLEGRO_EVENT mouse);
            void mouse_up(ALLEGRO_EVENT mouse);

        protected:
            Bounds* bounds;
            bool    is_pressed;

            virtual void on_click() = 0;

    };

    class MenuButton : public GameObject {
        
    };

    class GameMenu : public GameState {

    };

};

#endif
