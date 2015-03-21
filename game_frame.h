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
            virtual ~ObjectBehavior() { destroy(); }

            // called every iteration of a GameState iteration loop
            virtual void update() {}
            inline void set_parent(GameObject* parent) { this->parent = parent; }
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
            inline void set_parent(GameObject* parent) { this->parent = parent; } 

        protected:
            GameObject* parent;

    };

    class EventHandler {
        
        public:
            EventHandler(GameState* state) : parent(state) {}
            virtual ~EventHandler() {}
            virtual void event(ALLEGRO_EVENT&) = 0;

        protected:
            GameState* parent;

    };

    class TickEvent : public EventHandler {

        public:
            TickEvent(GameState* state) : EventHandler(state) {}
            void event(ALLEGRO_EVENT&);


    };

    class CloseEvent : public EventHandler {

        public:
            CloseEvent(GameState* state) : EventHandler(state) {}
            void event(ALLEGRO_EVENT&);
            
    };

    /* GameObject
     * To represent an object in the game.
     * Each Game object has a list of behaviors it employs
     */
    class GameObject {

        public:

            GameObject(GameState* parent_state, std::string name) : 
                name(name), behaviors(new std::unordered_map<std::string, ObjectBehavior*>), 
                parent(NULL), parent_state(parent_state), visible(true), pencil(NULL) {}
            
            GameObject(const GameObject& other) : 
                name(other.name), 
                behaviors(new std::unordered_map<std::string, ObjectBehavior*>),
                parent(other.parent), parent_state(other.parent_state),
                position(other.position), rotation(other.rotation), 
                visible(other.visible), pencil(other.pencil) {
                for (auto it = other.behaviors->begin(); it != other.behaviors->end(); it++)
                    behaviors->insert(*it);
            }
            ~GameObject() { 
                for(auto it = behaviors->begin(); it != behaviors->end(); it++)
                    delete it->second;
                delete behaviors;
            }
            
            // update function. relays the call to bahaviors
            inline void update() {
                for (auto it = behaviors->begin(); it != behaviors->end(); it++)
                    (it->second)->update();
            }
            
            inline void draw() const { if (pencil != NULL && visible) pencil->draw(); }
            inline void rename(std::string name) { (*this).name = name; }
            inline std::string get_unique_name() const { return name; }

            void set_pen(Pen*); 
    
            inline void set_parent_object(GameObject* parent) { this->parent = parent; }

            inline void add_behavior(ObjectBehavior* behave) { 
                behave->set_parent(this);
                (*behaviors)[behave->get_name()] = behave;
            }
            inline void remove_behavior(std::string) { behaviors->erase(name); }
            inline bool has_behavior (std::string behavior) const {
                return behaviors->find(behavior) != behaviors->end();
            }

            inline ObjectBehavior* get_behavior(std::string behavior) const {
                return (behaviors->find(behavior)->second);
            }

            inline double   get_rotation() const { return rotation; }
            inline void     set_rotation(double rotation) { this->rotation = rotation; }
            inline Point    get_position() const { return position; }
            inline void     set_position(const Point& point) { this->position = point; }

        protected:

            std::string                                         name;
            std::unordered_map<std::string, ObjectBehavior*>*   behaviors;
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
            std::unordered_map<std::string, GameObject>*            objects;

            GameState(ALLEGRO_DISPLAY*);
            ~GameState();

            inline GameState* run() { return game_loop(); }
            void signal_close() { is_close = true; }

            std::vector<GameObject*>* get_objects_of_behavior(std::string name) const;
            std::vector<ObjectBehavior*>* get_behaviors_of_type(std::string name) const;
            std::string add_object(std::string name);
            void remove_object(std::string name);

            virtual void setup() { std::cout << "setup GameState" << std::endl; }
            GameObject* get_object(std::string name) const;
            inline void add_pen(Pen* pen) { pens->push_back(pen); }
            inline ALLEGRO_COLOR get_color() { return scene_color; }

        protected:

            ALLEGRO_DISPLAY*                                        display;
            ALLEGRO_EVENT_QUEUE*                                    event_queue;
            ALLEGRO_TIMER*                                          timer;
            bool                                                    is_close;
            GameState*                                              next_state;
            std::unordered_map<ALLEGRO_EVENT_TYPE, EventHandler*>*  event_map;
            ALLEGRO_COLOR                                           scene_color;
            std::vector<Pen*>*                                      pens;
            
            // override functions
            virtual void destroy() {}

            // event handling
            GameState* game_loop();     // loops through game events
            //void action_tick();         // called at each timer event

    };

};

// template <GameState> class Game 
#include "game.inl"

#endif
