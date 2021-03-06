#include "boolcnt.hpp"

#include "vars.hpp"
#include "examplegen.hpp"

#include "cartesian.ppp"


TValue TCartesianClassifier::operator ()(const TExample &ex)
{ TExample example(domain, ex);
  TValue res(int(0));
  vector<int>::iterator mi(mults.begin());
  ITERATE(TExample, ei, example)
    if ((*ei).isSpecial())
      return classVar->DK();
    else
      res.intV+=(*(mi++))*(*ei).intV;
  return res;
}

void TCartesianClassifier::afterSet(const char *name)
{ if (!strcmp(name, "domain"))
    domainHasChanged(); 

  TClassifierFD::afterSet(name);
}

void TCartesianClassifier::domainHasChanged()
{ TEnumVariable *classV = mlnew TEnumVariable("new");
  classVar = classV;

  mults = vector<int>(domain->attributes->size(), 0);
  TLimitsCounter counter(vector<int>(domain->attributes->size(), 0));
  
  int mult = 1;
  vector<int>::reverse_iterator li(counter.limits.rbegin());
  vector<int>::reverse_iterator mi(mults.rbegin());
  for(TVarList::reverse_iterator vi(domain->attributes->rbegin()), ve(domain->attributes->rend()); vi!=ve; vi++) {
    if ((*vi)->varType!=TValue::INTVAR)
      raiseError("invalid attribute '%s' (discrete attributes expected)", (*vi)->get_name().c_str());

    *li = (*vi)->noOfValues();
    if (!*li)
      raiseError("invalid attribute '%s' (no values)", (*vi)->get_name().c_str());

    (*(mi++)) = mult;
    mult *= *(li++);
  }

  counter.reset();
  do {
    string val;
    TVarList::iterator vi(domain->attributes->begin());
    ITERATE(TLimitsCounter, ci, counter) {
      if (val.length())
        val+="_";
      val += (*(vi++)).AS(TEnumVariable)->values->at(*ci);
    }
    classV->addValue(val);
  } while (counter.next());
}
