#include "functorFactory.h"

/************************************************************************
 *                           PLEASE READ
 * This code is not used anywhere, but is kept since it is a good sample
 * implementation of what a singleton should be, especially the suff at
 * the top of the file, the pinstance stuff took a while to figure out
 * remember, stay DRY (Don't repeat yourself)
 ***********************************************************************/

FunctorFactory* FunctorFactory::pinstance = 0;// initialize pointer

FunctorFactory* FunctorFactory::getInstance() {
    if (pinstance == 0) {
        pinstance = new FunctorFactory; // create sole instance
    }
    return pinstance; // address of sole instance
}

FunctorFactory::FunctorFactory() {
    //... perform necessary instance initializations
}

Delta_Functor* FunctorFactory::create(std::string mapPosition) {
    FunctorFactory * _this = FunctorFactory::getInstance();
    _this->myFunctors[mapPosition] = new Delta_Functor();
    _this->completeFunctors[mapPosition] = 0;
    return _this->myFunctors[mapPosition];
}

Delta_Functor* FunctorFactory::create(std::string mapPosition, int start_time, int end_time, double start_value, double end_value, double exp) {
    FunctorFactory * _this = FunctorFactory::getInstance();
    _this->myFunctors[mapPosition] = new Delta_Functor( start_time, end_time, start_value, end_value, exp);
    _this->completeFunctors[mapPosition] = 0;
    return _this->myFunctors[mapPosition];
}

Delta_Functor* FunctorFactory::init(std::string mapPosition) {
    FunctorFactory * _this = FunctorFactory::getInstance();
    Delta_Functor * thisFunctor = _this->myFunctors[mapPosition];
    if (!thisFunctor) {
        //Doesn't exist due to caching, make a new one and remove the cache value
        FunctorFactory::create(mapPosition);
    }
    return _this->myFunctors[mapPosition];

}

//Get the value of the functor, if it is complete cache the result so that next loop we don't do that again
double FunctorFactory::get(std::string mapPosition) {
    try {
        FunctorFactory * _this = FunctorFactory::getInstance();

        //Check to see if we have a cached value for this functor
        if (_this->completeFunctors[mapPosition]) {
            return _this->completeFunctors[mapPosition];
        }

        Delta_Functor * thisFunctor = _this->myFunctors[mapPosition];
        if (!thisFunctor) {
            throw "Object doesn't exist";
        }
        if (_this->myFunctors[mapPosition]->isComplete()) {

            _this->completeFunctors[mapPosition] = _this->myFunctors[mapPosition]->getValue();
            _this->myFunctors[mapPosition]->deleteChildren();
            delete _this->myFunctors[mapPosition];
            return _this->completeFunctors[mapPosition];

        }

        return _this->myFunctors[mapPosition]->getValue();
    }
    catch (char e[]) {
        printf("%s", e);
    }

}
