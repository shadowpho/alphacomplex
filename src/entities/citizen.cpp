#include "entities/citizen.hpp"
#include "joblist.hpp"
#include "city.hpp"
#include "clock.hpp"
#include "components/ai/ai.hpp"
#include "components/ai/job.hpp"
#include "components/ai/needsai.hpp"
#include "windows.hpp"
#include "components/renderable.hpp"
#include "components/citizenname.hpp"
#include "components/movable.hpp"

#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cassert>

string get_full_name(Ent* e) {
  CitizenName* cn = e->assert_get<CitizenName>();
  ClearanceComp* cc = e->assert_get<ClearanceComp>();

  stringstream ss;
  ss << cn->name;

  if (cc->security() != Security::INFRARED)
    ss << '-' << Security::mask_to_dcode(cc->security())[0];

  ss << '-' << cn->sector;

  return ss.str();
}

struct IdleAI : AIScript {
  IdleAI() : job(nullptr) { }

  virtual int start(AI* c) {
    return request_job(c);
  }
  virtual int update(AI* c) {
    return request_job(c);
  }

  int request_job(AI* ai) {
    if (job) {
      job->complete();
      job = nullptr;
    }

    ClearanceComp* c = ai->parent->assert_get<ClearanceComp>();

    job = jobs.find_job(c->clearance());
    if (job) {
      job->reserve();
      return ai->push_script( job->script() );
    } else {
      return 30;
    }
  }

  Job* job;
};

Ent* new_citizen(Position pos, Security::Mask sec) {
  Ent* e = new Ent;
  e->add(new PositionComp(pos));
  e->add(new Movable(pos));
  e->add(new AI(make_shared<IdleAI>()));
  e->add(new ClearanceComp(Clearance{ sec, Department::random_dept() }));
  e->add(new Renderable(Security::mask_to_dcode(sec)[0]));
  e->add(random_citizenname());
  e->add(new Needs());

  e->add(&aisystem);
  e->add(&smsystem);
  e->add(&needssystem);
  return e;
}

// int Citizen::description(char* buf, size_t n) const {
//   if (security() == Security::INFRARED)
//     return snprintf(buf, n, "%s-%s [%1d]", name.c_str(), sect.c_str(), ssn);
//   const char* dcode = Security::mask_to_dcode(security());
//   return snprintf(buf, n, "%s-%c-%s [%1d]", name.c_str(), *dcode, sect.c_str(), ssn);
// }

// Security::Mask Citizen::security() const {
//   return sec;
// }

// Citizen::Citizen(int x_, int y_, Security::Mask s, City& c)
//   : AIEntity(x_, y_, c),
//     sec(s),
//     fact(Faction::PLAYER),
//     dept(Department::random_dept()),
//     aistate(new IdleAI()),
//     //intsec_review_job(nullptr),
//     ssn(rand() % 10000)
// {
//   name = names[rand() % names.size()];
//   sect = sectors[rand() % sectors.size()];
  
//   skills = Skill::random_skills(sec);
// }

