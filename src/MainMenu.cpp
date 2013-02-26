#include "WeightWatchers.h"

int main() {
	WeightWatchers * pMain = new WeightWatchers();
	int selection;
	while(pMain->exitStatus != "exit"){
		system("CLS");
		cout << "Welcome to the Healthy Club Command System." << endl;
		cout << "Please type the number of the feature you wish to use:\n" << endl;
		cout << "1. Calculate your body statistics\n" << endl;
		cout << "Your selection: "; cin >> selection;

		if(selection == 1){
			system("CLS");
			pMain->calculateStatistics();
		}
	}
	delete pMain;
}
