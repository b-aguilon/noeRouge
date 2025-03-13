#include <raylib.h>
#include <raymath.h>
#include "object.h"
#include "globals.h"

// ----- gameObject -----

/*------------------------------------------------------------------------------------------------------------------------------------------
    * Ben Aguilon. Edited by : Ethan Sheffield
    * @brief : Creates an object of an input size at an input position with an input speed.
    *
    * @param int _id : The object's ID number.
    * @param Vector2 _position : object's starting position.
    * @param Vector2 _size : object's starting size.
    * @param int _speed : object's starting speed value.
    *
    * @return : none
    */
GameObject::GameObject(int _id, Vector2 _position, Vector2 _size, int _speed)
{
    id = _id;
    position = _position;
    size = _size;
    speed = _speed;
    velocity = { 0 };
    direction = { 0 };
}

int GameObject::getId() 
{
    return this->id;
}

/*------------------------------------------------------------------------------------------------------------------------------------------
   * Ben Aguilon, Ethan Sheffield
   * @brief : Returns the x and y position, width, and height of a Rectangle object.
   *
   * @param : none
   *
   * @return float position.x : x position of a Rectangle object.
   * @return float position.y : y position of a Rectangle object.
   * @return float size.x : width of a Rectangle object.
   * @return float size.y : height of a Rectangle object.
   */
Rectangle GameObject::getBounds() 
{
    return { position.x, position.y, size.x, size.y };
}

/*------------------------------------------------------------------------------------------------------------------------------------------
          * Ethan Sheffield
          * @brief Getters for the GameObject class.
          */
Vector2 GameObject::getVelocity()
{
    return velocity;
}

Vector2 GameObject::getDirection()
{
    return direction;
}

void GameObject::setPosition(Vector2 _position)
{
    position = _position;
}

/*------------------------------------------------------------------------------------------------------------------------------------------
   * Ben Aguilon.
   * @brief : updates velocity, direction, and position and reacts to collisions.
   *
   * @param : Floor floor - used to parse map data from the current floor so to react to collisions.
   *
   * @return : none.
   */
void GameObject::onTick(Floor floor)
{
    direction = { 0, 0 };

    updateDirection();

        //direction is multiplied by speed, which makes velocity
        //speed is multiplied by the time between frames, which forces speed to be the same regardless of framerate.
    velocity = Vector2Scale(direction, speed * GetFrameTime());

        //collisions must be done before velocity is added to position so that the character does not go past 
        //a wall before collisions are checked, but that is
        //only necessary for very high movement speed
    updateCollisions(floor.getNearCollidables());

        //position is updated by adding velocity
    position = Vector2Add(position, velocity);
}

/*------------------------------------------------------------------------------------------------------------------------------------------
    * Ben Aguilon.
    * @brief : (For all 4 colliding functions)
    *          rect.x + rect.width + velocity.x > other.x :: 
    *          rect.x + rect.width is the right side of this rectangle. Velocity is added, 
    *          and the result is checked for if it is greater than the left of the other rectangle, 
    *          ie will the right side of this rectangle touch the left side of the other rectangle if velocity is added.
    *  
    *          rect.y + rect.height > other.y && rect.y < other.y + other.height ::
    *          checks to see if the bottom of this rectangle is below the top of the other rectangle, 
    *          and the top of this rectangle is above the bottom of the other rectangle,
    *          ie are the rectangles on the same horizontal plane
    *
    *          rect.x < other.x ::
    *          checks to see if the left side of this rectangle is to the left of the left side of the other recangle
    *
    * @param Rectangle other : The other rectangle, which acts as a collider which collides with this rectangle.
    * @return : a boolean which dictates whether or not this class's rectangle will collide with another rectangle.
    */
bool GameObject::collidingLeft(Rectangle other)
{
    Rectangle rect = getBounds();
    return rect.x + rect.width + velocity.x > other.x && rect.y + rect.height > other.y && rect.y < other.y + other.height && rect.x < other.x;
}

bool GameObject::collidingRight(Rectangle other)
{
    Rectangle rect = getBounds();
    return rect.x + velocity.x < other.x + other.width && rect.y + rect.height > other.y && rect.y < other.y + other.height && rect.x + rect.width > other.x + other.width;
}

bool GameObject::collidingTop(Rectangle other)
{
    Rectangle rect = getBounds();
    return rect.y + rect.height + velocity.y > other.y && rect.x < other.x + other.width && rect.x + rect.width > other.x && rect.y < other.y;
}

bool GameObject::collidingBottom(Rectangle other)
{
    Rectangle rect = getBounds();
    return rect.y + velocity.y < other.y + other.height && rect.x < other.x + other.width && rect.x + rect.width > other.x && rect.y + rect.height > other.y + other.height;
}

/*------------------------------------------------------------------------------------------------------------------------------------------
   * Ben Aguilon, comment by Ethan Sheffield
   * @brief : Checks collisions, for NPC object and all objects in colliders, in all directions.
   * @brief : Adjusts object's position and stops movement if collision is determined.
   *
   * @param vector<Rectangle> colliders : A collection of Rectangles that are potential colliders.
   *
   * @return : none
   */
void GameObject::updateCollisions(const std::vector<Rectangle> colliders)
{
    Rectangle rect = getBounds();

    for (Rectangle otherRect : colliders)
    {
        if (collidingLeft(otherRect) && velocity.x > 0)
        {
            velocity.x = 0;
            position.x = otherRect.x - rect.width;
        }
        else if (collidingRight(otherRect) && velocity.x < 0)
        {
            velocity.x = 0;
            position.x = otherRect.x + otherRect.width;
        }
        if (collidingTop(otherRect) && velocity.y > 0)
        {
            velocity.y = 0;
            position.y = otherRect.y - rect.height;
        }
        else if (collidingBottom(otherRect) && velocity.y < 0)
        {
            velocity.y = 0;
            position.y = otherRect.y + otherRect.height;
        }
    }
}

// ----------

// ----- objectHandler -----

/*------------------------------------------------------------------------------------------------------------------------------------------
   * John Crawford, edited by Ben Aguilon.
   * @brief : returns new game object pointer, adds it to objects list, and iterates the id this new object.
   *
   * @param position : The position of the new object in game space.
   * @param size : The size of the new object, width and height.
   * @param speed : The speed at which the new object can move.
   *
   * @return : game object pointer.
   */
class GameObject* ObjectHandler::createObject(Vector2 position, Vector2 size, int speed) 
{
    class GameObject *newObject = new GameObject(this->nextId++, position, size, speed);
    this->allObjects.push_back(newObject);
    this->numberOfObjects++;
    return newObject;
}

/*------------------------------------------------------------------------------------------------------------------------------------------
   * John Crawford.
   * @brief : returns game object pointer at id.
   *
   * @param id : the id of the game object, which is the same as the object's list index.
   *
   * @return : game object pointer.
   */
class GameObject* ObjectHandler::getObject(int id) 
{
    return this->allObjects[id];
}

/*------------------------------------------------------------------------------------------------------------------------------------------
   * John Crawford.
   * @brief : constructor which sets the nextId & numberOfObjects to zero.
   *
   * @param : none.
   *
   * @return : none.
   */
ObjectHandler::ObjectHandler()
{
    this->numberOfObjects = 0;
    this->nextId = 0;
}

/*------------------------------------------------------------------------------------------------------------------------------------------
   * John Crawford, edited by Ben Aguilon
   * @brief : calls onTick method for all gameobjects in allObjects list.
   *
   * @param : floor - the game map floor, used for objects colliding with walls / obstacles.
   *
   * @return : none.
   */
void ObjectHandler::tickAll(Floor floor) 
{
    for (int x = 0; x < this->numberOfObjects; x++) 
    {
        this->allObjects[x]->onTick(floor);
    }
}

/*------------------------------------------------------------------------------------------------------------------------------------------
   * Ben Aguilon, John Crawford.
   * @brief : calls onRender method for all gameobjects in allObjects list.
   *
   * @param : none.
   *
   * @return : none.
   */
void ObjectHandler::renderAll()
{
    for (int x = 0; x < this->numberOfObjects; x++) 
    {
        this->allObjects[x]->onRender();
    }
}
