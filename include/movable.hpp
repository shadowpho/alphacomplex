#pragma once

#include "entity.hpp"
#include "position.hpp"
#include "subsystem.hpp"

struct Movable : AspectStatic<Aspect::Movable, Movable> {
  Movable(struct Position p) : pos(p) { }

  inline City& city() { return *pos.city; }
  inline int x() const { return pos.x; }
  inline int y() const { return pos.y; }
  inline point as_point() const { return pos.as_point(); }

  inline void set(int x, int y) {
    pos.x = x;
    pos.y = y;
  }
  inline void set(point p) {
    pos.x = p.first;
    pos.y = p.second;
  }

  struct Position pos;
};

struct SimpleMovableSystem : SubSystem<SimpleMovableSystem, Movable> {
  inline void update(value_type& p) {
    PositionComp* pc = p.first->assert_get<PositionComp>();
    pc->move(p.second->pos);
  }
};

extern SimpleMovableSystem smsystem;