#ifndef __CORE_BEHAVIOR_H_
#define __CORE_BEHAVIOR_H_

#include "game_frame.h"

namespace allframe {

    // Common colors and random generators
    namespace Colors {

        extern ALLEGRO_COLOR BLACK;
        extern ALLEGRO_COLOR WHITE;

        extern ALLEGRO_COLOR RED;
        extern ALLEGRO_COLOR GREEN;
        extern ALLEGRO_COLOR BLUE;

        ALLEGRO_COLOR random_color();
        ALLEGRO_COLOR random_color_alpha();

    };

    // rotates a Point double radians about the origin
    Point point_rotate(const Point&, double);

    // Defines a 2-dimensional area and checks if points are
    // within the bounds
    class Bounds {

        public:
            virtual ~Bounds() {}

            virtual bool is_within(const Point&) const = 0;
            virtual bool is_within(double x, double y) const = 0;
            virtual bool is_within(int x, int y) const = 0;
    };

    // box from origin to width,height
    class BoxBounds : public Bounds {

        public:
            BoxBounds(const Point& size) : size(size) {}
            bool is_within(const Point&) const;
            bool is_within(double x, double y) const;
            bool is_within(int x, int y) const;

        protected:
            Point size;
    };

    // box from -width/2,-height/2 to width/2,height/2
    class CenterBoxBounds : public BoxBounds {

        public: 
            CenterBoxBounds(const Point& size) : BoxBounds(size) {}
            bool is_within(const Point&) const;
            bool is_within(double x, double y) const;
            bool is_within(int x, int y) const;
    };

    // bounds within a radius of the origin
    class CircleBounds : public Bounds {

        public:
            CircleBounds(double radius) : radius(radius) {} 
            bool is_within(const Point&) const;
            bool is_within(double x, double y) const;
            bool is_within(int x, int y) const;

        private:
            double radius;
    };

    // elliptical bounds centered at the origin
    class OvalBounds : public Bounds {

        public:
            OvalBounds(const Point& radii) : radii(radii) {}
            bool is_within(const Point&) const;
            bool is_within(double x, double y) const;
            bool is_within(int x, int y) const;

        private:
            Point radii;
    };

    // object with click detected by left lick event handler
    class LeftClickable : public ObjectBehavior {

        public:
            LeftClickable(Bounds* bounds) : bounds(bounds) {}
            virtual ~LeftClickable() { delete bounds; }

            void mouse_down(ALLEGRO_EVENT& mouse);
            void mouse_up(ALLEGRO_EVENT& mouse);

            std::string get_name() const { return "af_left_clickable"; }

        protected:
            Bounds* bounds;
            bool    is_pressed;

            virtual void on_click() = 0;
    };

    // class that defines behavior for multilple click buttons
    class Clickable : public ObjectBehavior {

        public:
            Clickable() : bounds{NULL}, is_pressed(0) {}
            virtual ~Clickable() { if (bounds != NULL) delete bounds; }

            void set_bounds(Bounds* bounds);
            void mouse_down(ALLEGRO_EVENT&, unsigned);
            void mouse_up(ALLEGRO_EVENT&, unsigned);

            std::string get_name() const { return "af_clickable"; }

        protected:
            Bounds* bounds;
            uint64_t is_pressed; 

            virtual void on_click(unsigned) = 0;
    };

    // triggers mouse_up and mouse_down of LeftCLickables
    // only handles button #1
    class LeftClickEvent : public EventHandler {
        public: 
            void event(ALLEGRO_EVENT&);
    };

    // triggers mouse_up and mouse_down of Clickables
    // handles buttons numbered 0-15
    class ClickEvent : public EventHandler {
        public:
            void event(ALLEGRO_EVENT&);
    };

};
#endif
