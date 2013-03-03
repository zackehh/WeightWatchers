#include "BodyStatsCalculator.h"
#include "Member.h"
#include "Trainer.h"
#include "WeightWatchers.h"

WeightWatchers::WeightWatchers(){
	exitStatus = "";
	memberName = "";
	height = 0;
	weight = 0;
	gender = "";
	runTime = 0;
}

WeightWatchers::~WeightWatchers(){
	// TODO Auto-generated destructor stub
}

/**
 * Calculates the client's BMI, BSA and LBW and returns the results to the users after
 * validating the inputs. Asks the user if they want to return to the main menu after usage
 * or if they wish to exit the program.
 */
void WeightWatchers::calculateStatistics(){
	BodyStatsCalculator * pStats = new BodyStatsCalculator();
	// Sets up a new member and asks for user input
	cout << "Please enter the required information:\n" << endl;
	Member member;

	// Validates and sets the client's name
	cout << "Please enter your first name: ";
	memberName = validateString(memberName, "name");
	member.setMemberName(memberName);

	// Validates and sets the client's height
	cout << "Please enter your height(cm): ";
	height = validateDouble(height);
	member.setHeight(height);

	// Validates and sets the client's weight
	cout << "Please enter your weight(kg): ";
	weight = validateDouble(weight);
	member.setWeight(weight);

	// Validates and sets the client's gender
	cout << "Please enter your gender: ";
	gender = validateString(gender, "gender");
	gender = toLowerCase(gender);

	// Verify the user has entered a correct gender before allowing them to continue
	enterGender:
	if (gender == "male" || gender == "female" || gender == "f" || gender == "m"){
		member.setGender(gender);
	} else {
		cout << "Please provide a valid gender: ";
		goto enterGender;
	}

	// Pass all the information to the Member class
	double bmi = pStats->bodyMassIndex(member.getHeight(), member.getWeight());
	double bsa = pStats->bodySurfaceArea(member.getHeight(), member.getWeight());
	double lbw = pStats->leanBodyWeight(member.getHeight(), member.getWeight(), member.getGender());
	string clientGender = pStats->getGender(member.getGender());
	string bmiResult = pStats->bodyMassResult(bmi, member.getGender());

	// Clear the screen and display the resulting information
	clearScreen();
	cout << "These are your calculated statistics:\n" << endl;
	cout << "Your Body Mass Index is " << bmi << " which indicates that you are " << bmiResult << endl;
	cout << "Your Body Surface Area is " << bsa << " square metres." << endl;
	cout << "Your Lean Body Weight is " << lbw << " kilograms." << endl;
	cout << clientGender << "\n" << endl;

	// Ask the user if they wish to exit or return to the main menu
	toReturnOrExit();
	delete pStats;
}

/**
 * Runs the client server simulation after retrieving the desired inputs from the users. Also
 * displays how long the server is going to run from and what the current time is. Asks the
 * user if they want to return to the main menu after running.
 */
void WeightWatchers::runSimulation(){
	int trainerNo; double transactionTime = 0;
	BodyStatsCalculator * pStats = new BodyStatsCalculator();
	Trainer * pTrainer = new Trainer();

	// Display the opening information in the cleared screen
	cout << "Please enter the required information:\n" << endl;
	cout << "Please enter the amount of time you want to run the simulation(min): "; cin >> runTime;
	cout << "Please enter the amount of trainers you wish to utilize: "; cin >> trainerNo;
	cout << "Please enter the amount of transaction time you wish to have(hours): ";
	// Input and validate transaction time
	transactionTime = validateDouble(transactionTime);
	// Convert to minutes
	pTrainer->setTransactionTime(60 * pStats->twoDecimalPlaces(transactionTime));

	// Because the following thread decrement runTime, we need a variable to remember the original
	int runningTime = runTime;
	// Create new thread to track the run time of the simulation
	thread simRunTime(&WeightWatchers::simulationRunTime, this);
	// Detach thread so it can run independantly
	simRunTime.detach();

	// Clear screen and display current time and how long simulation will run
	clearScreen();
	cout << "The time is now " << pTrainer->currentTime() << "." << endl;
	cout << "The server will run for " << (runningTime / 60) % 24 << " hour";
	// If the server runs for over an hour, append "s" to hour
	if(((runningTime / 60) % 24) != 1){
		cout << "s";
	}
	// If the server runs for X hours and minutes > 0 append " and X minute(s)"
	if((runningTime % 60) != 0){
		cout << " and ";
		if(runningTime % 60 != 1){
			// If it's not a minute, append " minutes."
			cout << runningTime % 60 << " minutes.\n" << endl;
		} else {
			// If it's a single minute, append " minute."
			cout << runningTime % 60 << " minute.\n" << endl;
		}
	} else {
		// Else just print a new line
		cout << ".\n" << endl;
	}

	// Create array of trainer objects based on user input
	Trainer trainers[trainerNo];
	for(int i = 0; i <= trainerNo; i++){
		// Give all trainers an ID number
		trainers[i].setTrainerID(i + 1);
	}
	toReturnOrExit();
}

/**
 * Tracks the allocated run time of the simulation and exits when the time is up.
 */
void WeightWatchers::simulationRunTime(){
	// Converts the run time to seconds, easier to handle
	runTime = runTime * 60;
	while(runTime > 0){
		// Decrement the runTime remaining
		runTime--;
		// Sleep for a second
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
	// Exit the program when the session time is over
	std::exit(0);
}

/**
 * Validates the users input to check that it's a double.
 *
 * @param input the users input
 * @return input the validated input
 */
double WeightWatchers::validateDouble(double input){
	// Currently has a bug if the user enters a double with an integer as the first character.
	// Can be solved by changing the input to string and converting to double.
	int i = 0;
	while (1) {
		cin >> input;
		if (cin.good()){
			break;
		} else {
			if (i == 0) cout << endl, i++;
			cout << "Please provide a valid number: ";
			cin.clear();
			while (cin.get() != '\n');
		}
	}
	return input;
}

/**
 * Validates the input string to force the user to enter a string without any integers.
 *
 * @param input the users input string
 * @param attribute the attribute the user is setting
 * @return input the validated input
 */
string WeightWatchers::validateString(string input, string attribute){
	int i = 0;
	while(1){
		cin >> input;
		if (find_if(input.begin(), input.end(), ::isdigit) != input.end())
		{
			if (i == 0) cout << endl, i++;
			cout << "Please provide a valid " << attribute << ": ";
			cin.clear();
			while (cin.get() != '\n');
		} else {
			break;
		}
	}
	return input;
}

/**
 * Asks the user if they want to exit the application or return to the main menu.
 *
 */
void WeightWatchers::toReturnOrExit(){
	string selection;
	cout << "To return to the main menu type \"menu\" or type \"exit\" to exit: ";
	cin >> WeightWatchers::exitStatus;
}

/**
 * Converts the input gender to lower case to make for easier handling.
 *
 * @param gender the client's input gender
 * @return gender the client's gender in lowercase
 */
string WeightWatchers::toLowerCase(string &gender){
	transform(gender.begin(), gender.end(), gender.begin(), ::tolower);
	return gender;
}

/**
 * Returns the current exit status of the program.
 */
string WeightWatchers::getExitStatus(){
	return exitStatus;
}

/**
 * Allows setting of the exit status to allow the closing of the program.
 * Used as a way for the user to terminate as opposed to forced closure.
 *
 * @param string the exit status
 */
void WeightWatchers::setExitStatus(string string){
	exitStatus = string;
}

/**
 * Clears the current command windows on either Windows or Linux using an if statement
 * to avoid any bad practices. Sometimes unsafe, but for this assignment it's appropriate.
 */
void WeightWatchers::clearScreen(){
#ifdef _WIN32 // Detect if user is running windows
	system("CLS");
#else // If user isn't running Windows (most likely running UNIX)
	system("clear");
#endif
}
