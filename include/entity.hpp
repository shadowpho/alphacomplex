#pragma once

// #include "city.hpp"
#include "pathfind.hpp"
#include "graphics.hpp"

#include <list>

using std::list;

struct City;

struct Entity {
  Entity(City& c) : 
    prev(nullptr),
    next(nullptr),
    city(c)
    { }

  virtual ~Entity() {
    remove();
  }

  virtual const char* rawname() const = 0;
  virtual int description(char*, size_t) const;
  virtual char render() const = 0;

  void insert_after(Entity* e);
  void remove();

  template<class T>
  T& as() { return (T&)(*this); }
  template<class T>
  const T& as() const { return (T&)(*this); }

  Entity* prev;
  Entity* next;
  City& city;
};

struct LocEntity : Entity {
  LocEntity(int x_, int y_, City& c) : Entity(c), x(x_), y(y_) { }

  int x, y;

  void set_loc(int x_, int y_) { x = x_; y = y_; }
};

struct AIEntity : LocEntity {
  AIEntity(int x_, int y_, City& c)
    : LocEntity(x_, y_, c)
    {
      ai_list.push_back(this);
      ai_it = --ai_list.end();
    }
  virtual ~AIEntity() { ai_list.erase(ai_it); }

  virtual void update() = 0;

  typedef vector<AIEntity*>::iterator iterator;

  static vector<AIEntity*> ai_list;
  iterator ai_it;
};

struct Dwarf : AIEntity {
  Dwarf(int x_, int y_, City& c, char pic_ = 'S') : AIEntity(x_, y_, c), pic(pic_) { }

  virtual const char* rawname() const { return RAWNAME; }
  virtual int description(char* buf, size_t n) const;
  virtual char render() const;
  virtual void update();

  char pic;
  int energy = 0;
  Direction facing = EAST;
  static const char* RAWNAME;
};
