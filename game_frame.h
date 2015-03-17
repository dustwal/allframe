#ifndef __GAME_FRAME_H_
#define __GAME_FRAME_H_

#include <unordered_map>
#include <vector>

#include <allegro5/allegro.h>

namespace allframe {
    
    int init();
    int close();

    namespace Colors {
        
        const ALLEGRO_COLOR BLACK = al_map_rgb(0,0,0);
        const ALLEGRO_COLOR WHITE = al_map_rgb(255,255,255);

    };

    struct Point {
        double x;
        double y;
    };

    class GameObject;

    class ObjectBehavior {
        
        public:

            ObjectBehavior(GameObject* parent) : parent(parent) {} 

            virtual void update() {}

        protected:

            GameObject* parent;

            virtual void startup() {}
            virtual void destroy() {}

    };

    class GameObject {

        public:
            
            virtual void update() {}
            virtual void draw() {}

        protected:

            std::vector<ObjectBehavior> bevahiors;
            GameObject*                 parent;
            Point                       position;
            double                      rotation;

            virtual void destroy() {}
            virtual void startup() {}

    };

    class GameController {

        public:

            virtual void update() {}

        protected:

            virtual void destroy() {}
            virtual void setup() {}

        private:

    };

    class GameState {

        typedef void (GameState::*EventHandler)(void);
        typedef std::unordered_map<ALLEGRO_EVENT_TYPE, EventHandler> EventMap;

        public:

            GameState(ALLEGRO_DISPLAY*);
            ~GameState();

            inline void start() { game_loop(); }
            void signal_close() { is_close = true; }

        protected:

            ALLEGRO_DISPLAY*                display;
            std::vector<GameObject>*        objects;
            std::vector<GameController>*    controllers;
            ALLEGRO_EVENT_QUEUE*            event_queue;
            ALLEGRO_TIMER*                  timer;
            bool                            is_close;
            GameState*                      next_state;
            EventMap*                       event_map;

            virtual void setup() {}
            virtual void destroy() {}

            GameState* game_loop();
            void action_tick();

            inline GameState* shut_down(GameState* state) { 
                destroy();
                return state;
            }

    };

};

// template <GameState> class Game 
#include "game.inl"

#endif
