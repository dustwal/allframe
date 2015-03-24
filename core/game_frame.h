#ifndef __GAME_FRAME_H_
#define __GAME_FRAME_H_

/**
 * ALLFRAME game framework
 *
 * written by Dustin Walde
 * 
 */
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include <allegro5/allegro.h>

namespace allframe {
    
    // startup and close the framework
    // returns true on succss
    bool init();
    bool close();

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
            ObjectBehavior() : parent(NULL) {}
            virtual ~ObjectBehavior() { destroy(); }

            // called every iteration of a GameState iteration loop
            virtual void update() {}
            // called on object creation
            virtual void setup() {}
            // set the object that has this behavior
            inline void set_parent(GameObject* parent) { this->parent = parent; }
            // reutrn the name of this behavior
            virtual std::string get_name() const { return behavior_name; }

        protected:
            // to allow for groups of objects and hierarchies
            GameObject* parent;
            // called on object destruction
            virtual void destroy() {}
            // get the current game state
            inline GameState* parent_state() { return parent->parent_state(); }

        private:
            std::string behavior_name = "ob_plain";

    };

    /* Pen
     * Object that defines how the parent object is drawn
     * on the game screen.
     */
    class Pen {

        public:
            virtual ~Pen() {}
            // draws object on the screen
            virtual void draw() const {}
            // sets the object to draw
            inline void set_parent(GameObject* parent) { this->parent = parent; } 

        protected:
            GameObject* parent;

    };

    /* EventHandler
     * Object to be called to on event call.
     */
    class EventHandler {
        
        public:
            EventHandler(GameState* state) : parent(state) {}
            virtual ~EventHandler() {}
            // handles the event
            virtual void event(ALLEGRO_EVENT&) = 0;

        protected:
            GameState* parent;

    };

    /* TickEvent
     * Handles timer event. Updates all objects and behaviors
     * and then draws all pens in z order.
     */
    class TickEvent : public EventHandler {

        public:
            TickEvent(GameState* state) : EventHandler(state) {}
            // updates and draws
            void event(ALLEGRO_EVENT&);

    };

    /* CloseEvent
     * Signals the game to exit.
     */
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
            
            // if it is a drawable object, calls the draw function of its Pen
            inline void draw() const { if (pencil != NULL && visible) pencil->draw(); }
            // renames this object to the given string
            inline void rename(std::string name) { (*this).name = name; }
            // returns the name of the instance of the object
            inline std::string get_unique_name() const { return name; }
            // sets how to draw this object
            void set_pen(Pen*); 
            // sets the parent object in the game hierarchy 
            inline void set_parent_object(GameObject* parent) { this->parent = parent; }
            // adds new behavior to this object
            inline void add_behavior(ObjectBehavior* behave) { 
                behave->set_parent(this);
                behave->setup();
                (*behaviors)[behave->get_name()] = behave;
            }
            // removes the behavior of this object with the given name
            inline void remove_behavior(std::string) { behaviors->erase(name); }
            // returns true if this object has the given bahavior
            inline bool has_behavior (std::string behavior) const {
                return behaviors->find(behavior) != behaviors->end();
            }
            // returns a pointer to the behavior of this object of the given name
            inline ObjectBehavior* get_behavior(std::string behavior) const {
                return (behaviors->find(behavior)->second);
            }
            // returns the rotation of this object relative to its parent
            inline double   get_rotation() const { return rotation; }
            // returns the global rotation of this object
            inline double   get_global_rotation() const {
                if (parent == NULL)
                    return rotation;
                return rotation + parent->get_global_rotation();
            }
            // sets the local rotation
            inline void     set_rotation(double rotation) { this->rotation = rotation; }
            // sets the rotation to the given global rotation
            void            set_global_rotation(double rotation);
            // returns a point representing the local position
            inline Point    get_position() const { return position; }
            // returns a point representing the global postion
            inline Point    get_global_position() const { 
                if (parent == NULL)
                    return position;
                return {position.x+parent->position.x, position.y+parent->position.y}; 
            }
            // sets the local position
            inline void     set_position(const Point& point) { this->position = point; }
            // sets local position to place the object to the given global position
            void            set_global_position(const Point&);

            inline GameObject* parent_object() { return parent; }
            inline GameState* parent_state() { return parent_state; }

        protected:

            std::string                                         name;           // object name
            std::unordered_map<std::string, ObjectBehavior*>*   behaviors;      // all behaviors
            GameObject*                                         parent;         // parent object
            GameState*                                          parent_state;   // parent state
            Point                                               position;       // local positon
            double                                              rotation;       // local rotation
            bool                                                visible;        // true if visible
            Pen*                                                pencil;         // pencil object

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

            static constexpr float FRAME_RATE = 30.0f;

            GameState(ALLEGRO_DISPLAY*);
            ~GameState();

            // starts the GameState loop
            inline GameState* run() { return game_loop(); }
            // tells the state to close
            void signal_close() { is_close = true; }
            // returs all objects that have the given behavior
            std::vector<GameObject*>* get_objects_of_behavior(std::string name) const;
            // returns the behaviors of the given type
            std::vector<ObjectBehavior*>* get_behaviors_of_type(std::string name) const;
            // creates a new object with the given name
            std::string add_object(std::string name);
            // creates a new object that will be drawn on top of the object with the given name
            std::string add_object_topof(std::string, std::string);
            // deletes the object with the given name
            void remove_object(std::string name);
            // function to override that initialises beginning objects and other state
            virtual void setup()=0;
            // retuns a pointer to the object of the given name
            GameObject* get_object(std::string name) const;
            // adds a pen to the list of pens used
            inline void add_pen(Pen* pen) { pens->push_back(pen); }
            // return the background color of the GameState
            inline ALLEGRO_COLOR get_color() { return scene_color; }
            // returns an iterator to the bottom most object
            inline std::map<double, GameObject*>::iterator get_begin() { 
                return sorted_objects->begin(); 
            }
            // returns an iterator to the end of the list of objects
            inline std::map<double, GameObject*>::iterator get_end() { 
                return sorted_objects->end();
            }
            // returns an iterator to the front most object
            inline std::map<double, GameObject*>::reverse_iterator get_rbegin() {
                return sorted_objects->rbegin();
            } 
            // returns an iterator to the end of the reverse iterator
            inline std::map<double, GameObject*>::reverse_iterator get_rend() { 
                return sorted_objects->rend();
            }

        protected:

            std::unordered_map<std::string, GameObject>*    objects;        // name object map
            std::unordered_map<std::string, double>*        names_to_z;     // name to z-index
            std::map<double, GameObject*>*                  sorted_objects; // z-index to object
            ALLEGRO_DISPLAY*                                display;
            ALLEGRO_EVENT_QUEUE*                            event_queue;
            ALLEGRO_TIMER*                                  timer;
            bool                                            is_close;       // true on close
            GameState*                                      next_state;     // return on exit
            std::unordered_map<ALLEGRO_EVENT_TYPE,EventHandler*>* 
                event_map;      // map from event type to handler object
            ALLEGRO_COLOR                                   scene_color;    // default scene color
            std::vector<Pen*>*                              pens;           // list of Pens used
            
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
