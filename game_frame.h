#ifndef __GAME_FRAME_H_
#define __GAME_FRAME_H_

/**
 * ALLFRAME game framework
 *
 * written by Dustin Walde
 * 
 */


#include <unordered_map>
#include <vector>

#include <allegro5/allegro.h>

namespace allframe {
    
    // startup and close the framework
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
    class GameState;

    /* ObjectBehavior
     * To be a child of a GameObject class or subcless to define
     * behavior for that object. 
     */
    class ObjectBehavior {
        
        public:

            ObjectBehavior(GameObject* parent) : parent(parent) {} 

            // called every iteration of a GameState iteration loop
            virtual void update() {}

        protected:

            // to allow for groups of objects and hierarchies
            GameObject* parent;

            // called on object creation
            virtual void startup() {}

            // called on object destruction
            virtual void destroy() {}

    };

    /* GameObject
     * To represent an object in the game.
     * Each Game object has a list of behaviors it employs
     */
    class GameObject {

        public:
            
            // update function. relays the call to bahaviors
            inline void object_update() {
                for (auto it = behaviors->begin(); it != behaviors->end(); it++)
                    (*it).update();
                update();
            }
            
            // called during the GameState draw phase
            virtual void draw() {}

        protected:

            std::vector<ObjectBehavior>*    behaviors;
            GameObject*                     parent;
            Point                           position;
            double                          rotation;

            // other functions of override
            virtual void update() {}
            virtual void destroy() {}
            virtual void startup() {}

    };

    /* GameController
     * defines stately behavior that is outside 
     * of the scope of a single object
     */
    class GameController {

        public:

            GameController(GameState* parent) : parent(parent) {}

            virtual void update() {}

        protected:

            GameState* parent;

            virtual void destroy() {}
            virtual void setup() {}

    };

    /* GameState
     * core class that runs the update loop and 
     * holds reference to all objects and 
     * controllers in the game.
     *
     * only override needed is the setup for setting
     * the base game objects
     */
    class GameState {

        // map event types to class member functions
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

            // override functions
            virtual void setup() {}
            virtual void destroy() {}

            // event handling
            GameState* game_loop();     // loops through game events
            void action_tick();         // called at each timer event

            inline GameState* shut_down(GameState* state) { 
                destroy();
                return state;
            }

    };

};

// template <GameState> class Game 
#include "game.inl"

#endif
