#include"physics.h"

void physics_Update(SPhysicsSystem*Physics,unsigned Dt){
	if(Physics->Update){
		Physics->Update(NULL,Physics->PhyData,Dt);
	}
}

