/*
    This file is part of Orange.

    Orange is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Orange is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Orange; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Authors: Janez Demsar, Blaz Zupan, 1996--2002
    Contact: janez.demsar@fri.uni-lj.si
*/


#ifndef __PREPROCESSORS_HPP
#define __PREPROCESSORS_HPP

#include "stladdon.hpp"

#include "vars.hpp"
#include "domain.hpp"
#include "examples.hpp"
#include "examplegen.hpp"
#include "ormap.hpp"

#include "filter.hpp"
#include "discretize.hpp"
#include "classify.hpp"

WRAPPER(Filter);
class TExampleTable;


class TPreprocessor : public TOrange {
public:
  __REGISTER_ABSTRACT_CLASS

  virtual PExampleGenerator operator()(PExampleGenerator generators, const int &weightID, int &newWeight)=0;

protected:
  PExampleGenerator filterExamples(PFilter filter, PExampleGenerator generator);
};

WRAPPER(Preprocessor);

#define TVariableFilterMap TOrangeMap_KV<PVariable, PValueFilter>
MWRAPPER(VariableFilterMap)


class TPreprocessor_ignore : public TPreprocessor {
public:
  __REGISTER_CLASS

  PVarList attributes; //P tells which attributes to remove

  TPreprocessor_ignore();
  TPreprocessor_ignore(PVarList);
  virtual PExampleGenerator operator()(PExampleGenerator, const int &weightID, int &newWeight);
};


class TPreprocessor_select : public TPreprocessor {
public:
  __REGISTER_CLASS

  PVarList attributes; //P tells which attributes to select

  TPreprocessor_select();
  TPreprocessor_select(PVarList);
  virtual PExampleGenerator operator()(PExampleGenerator, const int &weightID, int &newWeight);
};


class TPreprocessor_drop : public TPreprocessor {
public:
  __REGISTER_CLASS

  PVariableFilterMap values; //P variable-filter pairs
  bool conjunction; //P decides whether to take conjunction or disjunction of values

  TPreprocessor_drop();
  TPreprocessor_drop(PVariableFilterMap, bool = true);
  virtual PExampleGenerator operator()(PExampleGenerator, const int &weightID, int &newWeight);
};


class TPreprocessor_take : public TPreprocessor {
public:
  __REGISTER_CLASS

  PVariableFilterMap values; //P variable-filter pairs
  bool conjunction; //P decides whether to take conjunction or disjunction of values

  TPreprocessor_take();
  TPreprocessor_take(PVariableFilterMap, bool = true);
  virtual PExampleGenerator operator()(PExampleGenerator, const int &weightID, int &newWeight);

  static PFilter constructFilter(PVariableFilterMap values, PDomain domain, bool conj);
};


class TPreprocessor_removeDuplicates : public TPreprocessor {
public:
  __REGISTER_CLASS
  virtual PExampleGenerator operator()(PExampleGenerator, const int &weightID, int &newWeight);
};


class TPreprocessor_dropMissing : public TPreprocessor {
public:
  __REGISTER_CLASS
  virtual PExampleGenerator operator()(PExampleGenerator, const int &weightID, int &newWeight);
};

class TPreprocessor_takeMissing: public TPreprocessor {
public:
  __REGISTER_CLASS
  virtual PExampleGenerator operator()(PExampleGenerator, const int &weightID, int &newWeight);
};

class TPreprocessor_dropMissingClasses : public TPreprocessor {
public:
  __REGISTER_CLASS
  virtual PExampleGenerator operator()(PExampleGenerator, const int &weightID, int &newWeight);
};

class TPreprocessor_takeMissingClasses : public TPreprocessor {
public:
  __REGISTER_CLASS
  virtual PExampleGenerator operator()(PExampleGenerator, const int &weightID, int &newWeight);
};


#define TVariableFloatMap TOrangeMap_K<PVariable, float>
MWRAPPER(VariableFloatMap)

class TPreprocessor_addNoise : public TPreprocessor {
public:
  __REGISTER_CLASS

  PVariableFloatMap proportions; //P proportion of changed values for individual attributes
  float defaultProportion; //P default proportion of changed values (for attributes not specified above)
  PRandomGenerator randomGenerator; //P random number generator

  TPreprocessor_addNoise();
  TPreprocessor_addNoise(PVariableFloatMap, const float & = 0.0);
  virtual PExampleGenerator operator()(PExampleGenerator generators, const int &weightID, int &newWeight);
};


class TPreprocessor_addGaussianNoise : public TPreprocessor {
public:
  __REGISTER_CLASS

  PVariableFloatMap deviations; //P deviations individual attribute values
  float defaultDeviation; //P default deviation
  PRandomGenerator randomGenerator; //P random number generator

  TPreprocessor_addGaussianNoise();
  TPreprocessor_addGaussianNoise(PVariableFloatMap, const float & = 0.0);
  virtual PExampleGenerator operator()(PExampleGenerator generators, const int &weightID, int &newWeight);
};


class TPreprocessor_addMissing : public TPreprocessor {
public:
  __REGISTER_CLASS

  PVariableFloatMap proportions; //P proportion of removed values for individual values
  float defaultProportion; //P default proportion of removed values (for attributes not specified above)
  int specialType; //P special value type (1=DC, 2=DK)
  PRandomGenerator randomGenerator; //P random number generator

  TPreprocessor_addMissing();
  TPreprocessor_addMissing(PVariableFloatMap, const float & = 0.0, const int &specialType = valueDK);
  virtual PExampleGenerator operator()(PExampleGenerator generators, const int &weightID, int &newWeight);
};


class TPreprocessor_addMissingClasses : public TPreprocessor {
public:
  __REGISTER_CLASS

  float proportion; //P proportion of removed class values
  int specialType; //P special value type (1=DC, 2=DK)
  PRandomGenerator randomGenerator; //P random number generator

  TPreprocessor_addMissingClasses(const float & = 0.0, const int & = valueDK);
  virtual PExampleGenerator operator()(PExampleGenerator generators, const int &weightID, int &newWeight);

private:
  void addMissing(PExampleGenerator);
};


class TPreprocessor_addClassNoise : public TPreprocessor {
public:
  __REGISTER_CLASS

  float proportion; //P proportion of changed class values
  PRandomGenerator randomGenerator; //P random number generator

  TPreprocessor_addClassNoise(const float & = 0.0);
  virtual PExampleGenerator operator()(PExampleGenerator generators, const int &weightID, int &newWeight);
};


class TPreprocessor_addGaussianClassNoise : public TPreprocessor {
public:
  __REGISTER_CLASS

  float deviation; //P class deviation
  PRandomGenerator randomGenerator; //P random number generator

  TPreprocessor_addGaussianClassNoise(const float & = 0.0);
  virtual PExampleGenerator operator()(PExampleGenerator generators, const int &weightID, int &newWeight);
};



class TPreprocessor_addClassWeight : public TPreprocessor {
public:
  __REGISTER_CLASS

  PFloatList classWeights; //P weights of examples of particular classes
  bool equalize; //P reweight examples to equalize class proportions
  PRandomGenerator randomGenerator; //P random number generator

  TPreprocessor_addClassWeight();
  TPreprocessor_addClassWeight(PFloatList, const bool & = false);
  virtual PExampleGenerator operator()(PExampleGenerator generators, const int &weightID, int &newWeight);
};


class TPreprocessor_addCensorWeight : public TPreprocessor {
public:
  __REGISTER_CLASS

  enum {linear, km, bayes};

  PVariable outcomeVar; //P outcome variable
  PVariable timeVar; //P time variable
  TValue eventValue; //P event (fail) value
  int method; //P weighting method
  float maxTime; //P maximal time
  bool addComplementary; //P if true (default is false), complementary examples are added for censored examples

  TPreprocessor_addCensorWeight();
  TPreprocessor_addCensorWeight(PVariable, PVariable, const TValue & = TValue(), const int & = km, const float & = 0.0);
  virtual PExampleGenerator operator()(PExampleGenerator generators, const int &weightID, int &newWeight);

  void addExample(TExampleTable *table, const int &weightID, const TExample &example, const float &weight, const int &complementary, const float &compWeight=0.0);
};


WRAPPER(Discretization);

class TPreprocessor_discretize : public TPreprocessor {
public:
  __REGISTER_CLASS

  PVarList attributes; //P attributes to be discretized (all, if not defined or empty)
  bool discretizeClass; //P also discretize the class attribute (default: false)
  PDiscretization method; //P discretization method

  PDomain discretizedDomain(PExampleGenerator, int &);

  TPreprocessor_discretize();
  TPreprocessor_discretize(PVarList, const bool & = false, PDiscretization = PDiscretization());
  virtual PExampleGenerator operator()(PExampleGenerator, const int &weightID, int &newWeight);
};


WRAPPER(Learner);
WRAPPER(ClassifierFromVar);

class TImputeClassifier : public TClassifier {
public:
  __REGISTER_CLASS

  PClassifierFromVar classifierFromVar; //P ClassifierFromVar that is used to retrieve defined values
  PClassifier imputer; //P classifier that is used to determine the missing values 

  TImputeClassifier(PVariable newVar = PVariable(), PVariable oldVar = PVariable());
  TImputeClassifier(const TImputeClassifier &);

  virtual TValue operator ()(const TExample &ex);
};


class TPreprocessor_imputeByLearner : public TPreprocessor {
public:
  __REGISTER_CLASS

  PLearner learner; //P learner used for inducing a model for imputation

  virtual PExampleGenerator operator()(PExampleGenerator, const int &weightID, int &newWeight);
};


class TPreprocessor_filter : public TPreprocessor {
public:
  __REGISTER_CLASS

  PFilter filter; //P filter

  TPreprocessor_filter(PFilter = PFilter());
  virtual PExampleGenerator operator()(PExampleGenerator generators, const int &weightID, int &newWeight);
};

#endif
