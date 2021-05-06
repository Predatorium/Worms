#include <iostream>
#include "Gestion.h"
#include "Times.h"

using namespace std;
using namespace sf;

/// SERVER

int main()
{
	bool done = false;			// Bool�en pour la boucle
	short int port = 9666;		// D�finition du port � �couter

	Gestion Gestionnaire;
	Gestionnaire.Init(port);
	srand(time(NULL));

	Times time;

	while (!done)
	{
		time.RestartClock();
		// attend qu'un socket re�oit des donn�es

		Gestionnaire.Update(time.Get_TimeDeltaF());
	}
	return 0;
}