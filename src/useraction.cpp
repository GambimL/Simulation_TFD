#include "useraction.h"
#include "source.h"
#include "RunAction.h"


void useraction::Build() const{

   SetUserAction(new source());
   SetUserAction(new RunAction);

};


