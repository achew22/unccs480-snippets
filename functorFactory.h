#ifndef FUNCTORFACTORY_H_INCLUDED
#define FUNCTORFACTORY_H_INCLUDED

#include "DeltaFunctor.h"
#include <string>
#include <map>

class FunctorFactory {
public:
    static FunctorFactory* getInstance();
    static Delta_Functor* init(std::string mapPosition);
    static double get(std::string mapPosition);
    static Delta_Functor* create(std::string mapPosition);
    static Delta_Functor* create(std::string mapPosition, int start_time, int end_time, double start_value, double end_value, double exp);
protected:
    FunctorFactory();
    FunctorFactory(const FunctorFactory&);
    FunctorFactory& operator= (const FunctorFactory&);
private:
    std::map<std::string, Delta_Functor *> myFunctors;
    std::map<std::string, double> completeFunctors;
    static FunctorFactory* pinstance;

};
/*
    class Log {
      void Write(char const *logline);
      bool SaveTo(char const *filename);
    private:
      Log();          // ctor is hidden
      Log(Log const&);      // copy ctor is hidden
      Log& operator=(Log const&);  // assign op is hidden

      static std::list<std::string> m_data;
    };
*/
#endif // FUNCTORFACTORY_H_INCLUDED
