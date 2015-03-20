#ifndef __GAME_FRAME_H_
#define __GAME_FRAME_H_

/**
 * ALLFRAME game framework
 *
 * written by Dustin Walde
 * 
 */


#include <iostream>
#include <unordered_map>
#include <vector>

#include <allegro5/allegro.h>

namespace allframe {
    
    // startup and close the framework
    int init();
    int close();

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

            ObjectBehavior(GameObject* parent) : parent(parent) { setup(); }
            ~ObjectBehavior() { destroy(); }

            // called every iteration of a GameState iteration loop
            virtual void update() {}
            virtual std::string get_name() const { return behavior_name; }

        protected:

            // to allow for groups of objects and hierarchies
            GameObject* parent;
            // called on object creation
            virtual void setup() {}
            // called on object destruction
            virtual void destroy() {}

        private:
            std::string behavior_name = "ob_plain";

    };

    class Pen {

        public:
            virtual ~Pen() {}
            virtual void draw() const {}
            void set_parent(GameObject* parent) { this->parent = parent; }

        protected:
            GameObject* parent;

    };

    /* GameObject
     * To represent an object in the game.
     * Each Game object has a list of behaviors it employs
     */
    class GameObject {

        public:

            GameObject(GameObject* parent, GameState* parent_state, std::string name) : 
                name(name), behaviors(new std::unordered_map<std::string, ObjectBehavior>), 
                parent(parent), parent_state(parent_state), visible(true), pencil(NULL) {}
            GameObject(GameState* parent_state, std::string name) : 
                GameObject(NULL, parent_state, name) {}
            ~GameObject() { 
                //if (pencil != NULL) delete pencil;
                //delete behaviors;
            }
            
            // update function. relays the call to bahaviors
            inline void update() {
                for (auto it = behaviors->begin(); it != behaviors->end(); it++)
                    (it->second).update();
                update();
            }
            
            inline void draw() const { if (pencil != NULL && visible) pencil->draw(); }
            inline void rename(std::string name) { (*this).name = name; }
            inline std::string get_unique_name() const { return name; }

            inline void set_pen(Pen* pen) { 
                if (pencil != NULL) 
                    delete pencil; 
                pencil = pen;
                pencil->set_parent(this);
            }

            inline void add_behavior(ObjectBehavior& behave) { 
                //(*behaviors)[behave.get_name()] = behave;
            }
            inline void remove_behavior(std::string) { behaviors->erase(name); }
            inline bool has_behavior (std::string behavior) const {
                return behaviors->find(behavior) != behaviors->end();
            }

            inline ObjectBehavior* get_behavior(std::string behavior) const {
                return &(behaviors->find(behavior)->second);
            }

            inline double   get_rotation() const { return rotation; }
            inline void     set_rotation(double rotation) { this->rotation = rotation; }
            inline Point    get_position() const { return position; }
            inline void     set_position(const Point& point) { this->position = point; }

        protected:

            std::string                                         name;
            std::unordered_map<std::string, ObjectBehavior>*    behaviors;
            GameObject*                                         parent;
            GameState*                                          parent_state;
            Point                                               position;
            double                                              rotation;
            bool                                                visible;
            Pen*                                                pencil;

        private:
            std::string object_type = "go_plain";
    };


    /* GameState
     * core class that runs the update loop and 
     * holds reference to all objects and 
     * controllers in the game.
     *
     * only override needed is the setup for setting
     * the basevoid add_behavior(); game objects
     */
    class GameState {


        public:

            static constexpr float FRAME_RATE = 35.0f;

            GameState(ALLEGRO_DISPLAY*);
            ~GameState();

            inline GameState* run() { return game_loop(); }
            void signal_close() { is_close = true; }

            std::vector<GameObject*>* get_objects_of_behavior(std::string name) const;
            std::vector<ObjectBehavior*>* get_behaviors_of_type(std::string name) const;
            std::string add_object(GameObject& object);
            void remove_object(std::string name);

            virtual void setup() { std::cout << "setup GameState" << std::endl; }
            GameObject* get_object(std::string name) const;

        protected:

            // map event types to class member functions
            typedef void (GameState::*EventHandler)(void);
            typedef std::unordered_map<ALLEGRO_EVENT_TYPE, EventHandler> EventMap;

            ALLEGRO_DISPLAY*                                    display;
            std::unordered_map<std::string, GameObject>*        objects;
            ALLEGRO_EVENT_QUEUE*                                event_queue;
            ALLEGRO_TIMER*                                      timer;
            bool                                                is_close;
            GameState*                                          next_state;
            EventMap*                                           event_map;
            ALLEGRO_COLOR                                       scene_color;

            // override functions
            virtual void destroy() {}

            // event handling
            GameState* game_loop();     // loops through game events
            void action_tick();         // called at each timer event

    };

};

// template <GameState> class Game 
#include "game.inl"

#endif
