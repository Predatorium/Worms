#include <iostream>
#include "Gestion.h"
#include "Times.h"

using namespace std;
using namespace sf;

/// SERVER

int main()
{
	bool done = false;			// Booléen pour la boucle
	short int port = 9666;		// Définition du port à écouter

	Gestion Gestionnaire;
	Gestionnaire.Init(port);
	srand(time(NULL));

	Times time;

	while (!done)
	{
		time.RestartClock();
		// attend qu'un socket reçoit des données

		Gestionnaire.Update(time.Get_TimeDeltaF());
	}
	return 0;
}