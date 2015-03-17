#ifndef __GAME_ENGINE_HH__
#define __GAME_ENGINE_HH__

#include <unordered_map>
#include <vector>

#include <allegro5/allegro.h>

namespace alengi {
    

    
    int init();
    int close();

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

    class GameState {

        typedef void (GameState::*EventHandler)(void);
        typedef std::unordered_map<ALLEGRO_EVENT_TYPE, EventHandler> EventMap;

        public:

            GameState(ALLEGRO_DISPLAY*);
            ~GameState();

            inline void start() { game_loop(); }
            void signal_close() { is_close = true; }

        protected:

            ALLEGRO_DISPLAY*            display;
            std::vector<GameObject>*    objects;
            ALLEGRO_EVENT_QUEUE*        event_queue;
            ALLEGRO_TIMER*              timer;
            bool                        is_close;
            EventMap*                   event_map;

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
