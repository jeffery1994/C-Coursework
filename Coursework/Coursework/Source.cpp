#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <thread>
#include <stdlib.h>
#include <Windows.h>
#include <set>
#include <sstream>
#include <mutex> 
#include "PRILIB.h"
/*
Why not Mutex?
Because the Execute Function just simply read the variables in the strtegy class, so it doesn't overwrite anything.
If I use Mutex, then Because of the existence of lock, the main thread have to wait for the sub-thread to unlock the data.
So that it will actually slow down the speed.
*/

using namespace std;

//To store the best combination in coursework2
int BestCombination[5] = {2,3,4,5,8};
//Rival Combinations plays against the best in spy tournament
int NormalCombination[5] = { 0,1,6,7,9 };

//Global Getting random number function
static int seed = (int)time(NULL);

int GetRandNum(int min, int max) {
	srand(seed);
	seed = rand();
	return (min + rand() % (max - min + 1));
}

const int ITERATION = 200;

const int Num_of_prisoners = 10;

const int member_in_each_gang = 5;

//Max sentences in a random strategy
const int MaxSentences = 9;

const string edge = "***********************************************************************************************************************";

const string text_edge = "              ";

//container reset used in coursework2
void InitializeSet(set<int> &a)
{
	for (int i = 0; i < 10; i++) {
		a.insert(i);
	}
}

//ViewSet
void ShowSet(set<int> a)
{
	cout << " ";
	set<int>::iterator b;
	for (b = a.begin(); b != a.end(); b++) {
		cout << *b << " ";
	}
}

string int_to_string(int num)
{
	string temp_s;
	stringstream s;
	s << num;
	s >> temp_s;
	return temp_s;
}

void Output_A_Strategy(string filename, vector< vector<string> > command)
{
	ofstream in;
	in.open(filename, ios::trunc); 
	if (in) {
		for (int unsigned i = 0; i < command.size(); i++) {
			for (int unsigned j = 0; j < command[i].size(); j++) {
				in << command[i][j] << " ";
			}
			in << endl;
		}
		in.close();
	}
	else {
		throw 0.1;
	}
}


//Prisoner or strategy class
class strategy
{
protected:
	char LASTOUTCOME;
	int ALLOUTCOMES_W;
	int ALLOUTCOMES_X;
	int ALLOUTCOMES_Y;
	int ALLOUTCOMES_Z;
	double MYSCORE;
private:
	int win_times;
public:
	vector< vector<string> > command;
	//Use STL container Vector to shape a double-dimisional array, So that it will be easier to search the given sentence number
	friend void Output_A_Strategy(string filename, vector< vector<string> > command);
	int current_ITERATIONS;
	bool strategy_loaded;

	//Default constructor
	strategy()
	{
		ALLOUTCOMES_W = 0;
		ALLOUTCOMES_X = 0;
		ALLOUTCOMES_Y = 0;
		ALLOUTCOMES_Z = 0;
		MYSCORE = 0;
		strategy_loaded = false;
		current_ITERATIONS = 0;
		win_times = 0;
	}
	~strategy(){}

	//Reset all variables after a 200 iterations game
	void Reset()
	{
		ALLOUTCOMES_W = 0;
		ALLOUTCOMES_X = 0;
		ALLOUTCOMES_Y = 0;
		ALLOUTCOMES_Z = 0;
		MYSCORE = 0;
		current_ITERATIONS = 0;
	}

	//Show all variables after a 200 iteration game
	void ShowStat()
	{
		cout << "ALLOUTCOMES_W: " << ALLOUTCOMES_W << endl;
		cout << "ALLOUTCOMES_X: " << ALLOUTCOMES_X << endl;
		cout << "ALLOUTCOMES_Y: " << ALLOUTCOMES_Y << endl;
		cout << "ALLOUTCOMES_Z: " << ALLOUTCOMES_Z << endl;
		cout << "Total Sentence received: " << MYSCORE << endl;
	}

	double& GetScore()
	{
		return MYSCORE;
	}

	int& GetWinTimes()
	{
		return win_times;
	}
	//Load a strategy provided
	void Load_A_Strategy_File(string file_name)
	{
		fstream read;
		//int length = 0;
		read.open(file_name, ios::in);
		if (read)
		{
			strategy_loaded = true;
			while (!read.eof())
			{
				char line[1001];
				read.getline(line, 1001);
				vector<string> temp_vec;
				string temp_s;
				char temp[20];
				int j = 0;
				for (int i = 0; line[i]<1001; ++i)
				{
					if (line[i] == ' ') {
						temp_s = temp;
						temp_s.assign(temp_s, 0, j);
						temp_vec.push_back(temp_s);
						j = 0;
					}
					else if (line[i] == '\0') {
						temp_s = temp;
						temp_s.assign(temp_s, 0, j);
						temp_vec.push_back(temp_s);
						break;
					}
					else {
						temp[j] = line[i];
						j++;
					}
				}
				command.push_back(temp_vec);
			}
			read.close();
		}
		else {
			throw - 1;
			return;
		}
	}

	bool S_loaded()
	{
		return strategy_loaded;
	}

	//This is for debugging purpose
	/*void CheckC()
	{
		for (unsigned int i = 0; i < command.size(); i++) {
			for (unsigned int j = 0; j < command[i].size(); j++)
			{
				cout << command[i][j] << ' ';
			}
			cout << endl;
		}
	}*/

	//Get a decision(either 0 or 1) made by a prisoner based on his strategy, 0 represent stay silence, 1 represent betray, using a recursion function
	virtual int Execute(int current_sentence = 0)
	{
		//lock_guard<mutex> locker(_mutex);
		if (command[current_sentence][1] == "IF")
		{
			//Get the if statement result
			bool temp = Compare(command[current_sentence]);
			if (temp == true) {
				//assign a impossible number for exception handling
				int next_sentence = 333;
				//find the GOTO sentence
				const vector<string>::iterator next_id = command[current_sentence].end() - 1;
				for (unsigned int i = 0; i < command.size(); i++) {
					if (command[i][0] == *next_id) {
						next_sentence = i;
						break;
					}
				}
				//if this next_sentence != 333, meaning that the function found the next sentence number
				if (next_sentence != 333) {
					return Execute(next_sentence);
				}
				else {
					cout << "Cannot find next sentence, strategy is invalid" << endl;
					return -1;
				}
			}
			else {
				//if it returns a false, then just execute the next sentence
				int next_sentence = current_sentence + 1;
				return Execute(next_sentence);
			}
		}
		else if (command[current_sentence][1] == "BETRAY" || command[current_sentence][1] == "SILENCE" || command[current_sentence][1] == "RANDOM")
		{
			if (command[current_sentence][1] == "BETRAY") {
				return 1;
			}
			else if (command[current_sentence][1] == "SILENCE") {
				return 0;
			}
			else if (command[current_sentence][1] == "RANDOM") {
				return GetRandNum(0, 1);
			}
		}
		else {
			cout << "cannot find key words, strategy invaild" << endl;
			return -1;
		}
		return -1;
	}

	//Judge the condition in the 'IF' sentence whether it's true or false
	bool Compare(vector<string> vec)
	{
		double sum = MatchWithVariable(vec[2]);
		for (unsigned int i = 3; i < vec.size(); i += 2)
		{
			//Making calculation on the left side until a '>' , '<', '=' is found
			const char* temp = vec[i].data();
			switch (*temp)
			{
			case '+':
				sum += MatchWithVariable(vec[i + 1]);
				break;
			case '-':
				sum -= MatchWithVariable(vec[i + 1]);
				break;
			//If '>' '<' '=' is found, then deal with the right side, making the same calculation,
			//finally, return a bool value, whether the left side is match with the right side in terms of different suitations
			case '<':
				return sum < DealWithRightHand(vec, i + 1);
				break;
			case '>':
				return sum = DealWithRightHand(vec, i + 1);
				break;
			case '=':
				return sum > DealWithRightHand(vec, i + 1);
				break;
			}
		}
		return false;
	}
	double DealWithRightHand(vector<string> s, int a1)
	{
		double sum = MatchWithVariable(s[a1]);
		for (unsigned int i = a1 + 1; i < s.size(); i += 2) {
			if (s[i] == "GOTO") {
				break;
			}
			const char* temp = s[i].data();
			switch (*temp)
			{
			case '+':
				sum += MatchWithVariable(s[i + 1]);
				break;
			case '-':
				sum += MatchWithVariable(s[i + 1]);
				break;
			default:
				break;
			}
		}
		return sum;
	}

	//return a int variables with a string given
	virtual double MatchWithVariable(string s)
	{
		if (s == "LASTOUTCOME") {
			return LASTOUTCOME;
		}
		else if (s == "ALLOUTCOMES_W") {
			return ALLOUTCOMES_W;
		}
		else if (s == "ALLOUTCOMES_X") {
			return ALLOUTCOMES_X;
		}
		else if (s == "ALLOUTCOMES_Y") {
			return ALLOUTCOMES_Y;
		}
		else if (s == "ALLOUTCOMES_Z") {
			return ALLOUTCOMES_Z;
		}
		else if (s == "ITERATIONS") {
			return current_ITERATIONS;
		}
		else if (s == "MYSCORE") {
			return MYSCORE;
		}
		else if (s == "W") {
			return 'W';
		}
		else if (s == "X") {
			return 'X';
		}
		else if (s == "Y") {
			return 'Y';
		}
		else if (s == "Z") {
			return 'Z';
		}
		else if (s == "A") {
			return 'A';
		}
		else if (s == "B") {
			return 'B';
		}
		else if (s == "C") {
			return 'C';
		}
		else {
			const char* temp = s.data();
			int value = atoi(temp);
			//cout << value << endl;
			return value;
			//cout << "Invaild strategy" << endl;
			//return -1;
		}
	}

	//Update all variables after a single iteration
	virtual void Update(int decision_made_by_A, int decision_made_by_B)
	{
		if (decision_made_by_A == 0 && decision_made_by_B == 0) {
			LASTOUTCOME = 'W';
			MYSCORE += 2;
			ALLOUTCOMES_W += 1;
			current_ITERATIONS++;
		}
		else if (decision_made_by_A == 0 && decision_made_by_B == 1) {
			LASTOUTCOME = 'X';
			MYSCORE += 5;
			ALLOUTCOMES_X++;
			current_ITERATIONS++;
		}
		else if (decision_made_by_A == 1 && decision_made_by_B == 0) {
			LASTOUTCOME = 'Y';
			ALLOUTCOMES_Y++;
			current_ITERATIONS++;
		}
		else if (decision_made_by_A == 1 && decision_made_by_B == 1) {
			LASTOUTCOME = 'Z';
			MYSCORE += 4;
			ALLOUTCOMES_Z++;
			current_ITERATIONS++;
		}
	}


	void Generate_A_Random_Strategy()
	{
		for (int i = 10; i < MaxSentences*10+10; i+=10) {
			int choice = GetRandNum(0, 1);
			if (i == 90) {
				Generate_A_Decision_Sentence(i);
				break;
			}

			if (choice == 0) {
				Generate_A_If_Sentence(i);
				i += 10;
			}
			else {
				Generate_A_Decision_Sentence(i);
				break;
			}
		}
	}


	void Generate_A_If_Sentence(int num)
	{
		vector<string> if_sentence;
		if_sentence.push_back(int_to_string(num));
		if_sentence.push_back("IF");
		int temp = GetRandNum(0, 1);
		if (temp == 0) {
			if_sentence.push_back("LASTOUTCOME");
			if_sentence.push_back("=");
			if_sentence.push_back(MatchLASTOUTCOME(GetRandNum(0, 3)));
			if_sentence.push_back("GOTO");
			if_sentence.push_back(int_to_string(num+20));
		}
		else {
			if_sentence.push_back(MatchDifferentVariables(GetRandNum(0, 5)));
			if_sentence.push_back(MatchWithOperator(GetRandNum(0, 2)));
			if_sentence.push_back(MatchDifferentVariables(GetRandNum(0, 5)));
			if_sentence.push_back("GOTO");
			if_sentence.push_back(int_to_string(num + 20));
		}
		command.push_back(if_sentence);
		Generate_A_Decision_Sentence(num + 10);
	}

	void Generate_A_Decision_Sentence(int num)
	{
		vector<string> decision_sentence;
		decision_sentence.push_back(int_to_string(num));
		decision_sentence.push_back(Match_Decision_with_string(GetRandNum(0, 2)));
		command.push_back(decision_sentence);
	}

	string Match_Decision_with_string(int num)
	{
		if (num == 0) {
			return "BETRAY";
		}
		else if(num == 1){
			return "SILENCE";
		}
		else {
			return "RANDOM";
		}
	}

	string MatchDifferentVariables(int num)
	{
		switch (num) {
		case 0:
			return "ALLOUTCOMES_W";
		case 1:
			return "ALLOUTCOMES_X";
		case 2:
			return "ALLOUTCOMES_Y";
		case 3:
			return "ALLOUTCOMES_Z";
		case 4:
			return "ITERATIONS";
		case 5:
			return "MYSOCRE";
		}
		throw - 2;
	}

	string MatchWithOperator(int num) {
		switch(num) {
		case 0:
			return ">";
		case 1:
			return "=";
		case 2:
			return "<";
		}
		throw - 2;
	}

	string MatchLASTOUTCOME(int num) {
		switch (num)
		{
		case 0:
			return "W";
			break;
		case 1:
			return "X";
			break;
		case 2:
			return "Y";
			break;
		case 3:
			return "Z";
			break;
		default:
			break;
		}
		throw - 2;
	}

	//gangleader virtual function

	//Spy strategy based on always choose minority choice
};

void Threading_Execution(strategy&s, int&D_a)
{
	D_a = s.Execute();
}

class GangPrisoner :public strategy
{
private:
protected:
	double ALLOUTCOMES_A;
	double ALLOUTCOMES_B;
	double ALLOUTCOMES_C;
	bool is_a_spy;
	bool is_a_gangleader;
public:
	friend class gang;
	GangPrisoner()
	{
		bool is_a_spy = false;
		bool is_a_gangleader = false;
		ALLOUTCOMES_A = 0;
		ALLOUTCOMES_B = 0;
		ALLOUTCOMES_C = 0;
	}

	virtual int Pick_A_SPY(int chosed_id = 10, int revealed_id = 10)
	{
		int temp;
		for (int i = 0;; i++) {
			temp = GetRandNum(0, 9);
			if (temp != chosed_id && temp != revealed_id) {
				return temp;
			}
			else {
				continue;
			}
		}
		return temp;
	}
	  
	int spy_Execute(int others_decision)
	{
		if (others_decision >= 3) {
			return 0;
		}
		else if (others_decision <= 1) {
			return 1;
		}
		else {
			return GetRandNum(0, 1);
		}
	}

	virtual void Update(int Mygoup_LASTOUTCOME, int othergroup_LASTOUTCOME)
	{
		if (Mygoup_LASTOUTCOME + othergroup_LASTOUTCOME == 0) {
			ALLOUTCOMES_W++;
			LASTOUTCOME = 'W';
			MYSCORE += 2;
		}
		else if (Mygoup_LASTOUTCOME + othergroup_LASTOUTCOME == 10) {
			ALLOUTCOMES_Z++;
			LASTOUTCOME = 'Z';
			MYSCORE += 4;
		}
		else if (Mygoup_LASTOUTCOME == 5 && othergroup_LASTOUTCOME == 0) {
			ALLOUTCOMES_Y++;
			LASTOUTCOME = 'Y';
		}
		else if (Mygoup_LASTOUTCOME == 0 && othergroup_LASTOUTCOME == 5) {
			ALLOUTCOMES_X++;
			LASTOUTCOME = 'Y';
			MYSCORE += 5;
		}
		else if (Mygoup_LASTOUTCOME < othergroup_LASTOUTCOME) {
			ALLOUTCOMES_B++;
			LASTOUTCOME = 'B';
			MYSCORE += 3;
		}
		else if (Mygoup_LASTOUTCOME == othergroup_LASTOUTCOME) {
			ALLOUTCOMES_C++;
			LASTOUTCOME = 'C';
			MYSCORE += 2;
		}
		else {
			ALLOUTCOMES_A++;
			LASTOUTCOME = 'A';
			MYSCORE += 2.5;
		}
	}

	virtual double MatchWithVariable(string s)
	{
		if (s == "LASTOUTCOME") {
			return LASTOUTCOME;
		}
		else if (s == "ALLOUTCOMES_W") {
			return ALLOUTCOMES_W;
		}
		else if (s == "ALLOUTCOMES_X") {
			return ALLOUTCOMES_X;
		}
		else if (s == "ALLOUTCOMES_Y") {
			return ALLOUTCOMES_Y;
		}
		else if (s == "ALLOUTCOMES_Z") {
			return ALLOUTCOMES_Z;
		}
		else if (s == "ITERATIONS") {
			return current_ITERATIONS;
		}
		else if (s == "MYSCORE") {
			return MYSCORE;
		}
		else if (s == "W") {
			return 'W';
		}
		else if (s == "X") {
			return 'X';
		}
		else if (s == "Y") {
			return 'Y';
		}
		else if (s == "Z") {
			return 'Z';
		}
		else if (s == "ALLOUTCOMES_A") {
			return ALLOUTCOMES_A;
		}
		else if (s == "ALLOUTCOMES_B") {
			return ALLOUTCOMES_B;
		}
		else if (s == "ALLOUTCOMES_C") {
			return ALLOUTCOMES_C;
		}
		else if (s == "A") {
			return 'A';
		}
		else if (s == "B") {
			return 'B';
		}
		else if (s == "C") {
			return 'C';
		}
		else {
			const char* temp = s.data();
			int value = atoi(temp);
			//cout << value << endl;
			return value;
			//cout << "Invaild strategy" << endl;
			//return -1;
		}
	}
};

//GangLeader class
class GangLeader : public GangPrisoner
{
private:

public:
	GangLeader() {}
	virtual int Pick_A_SPY(int chosed_id = 10, int revealed_id = 10)
	{
		int temp;
		for (int i = 0;; i++) {
			temp = GetRandNum(0, 9);
			if (temp != chosed_id && temp != revealed_id) {
				return temp;
			}
			else {
				continue;
			}
		}
		return temp;
	}
};

//SPY class
class SPY :public GangPrisoner
{
private:
public:
	//Spy use this function to choose
	int spy_Execute(int others_decision)
	{
		if (others_decision >= 3) {
			return 0;
		}
		else if (others_decision <= 1) {
			return 1;
		}
		else {
			return GetRandNum(0, 1);
		}
	}
};

class gang
{
private:
	int group_LASTOUTCOME;
	double Total_groupscore;
public:
	GangPrisoner* member[member_in_each_gang];
	string name;
	~gang(){}
	//Default constructor. At the very begining, these pointer point at basic strategy type(which is actually prisoner). It might change if it becomes a gang leader or a spy
	gang(string name)
	{
		group_LASTOUTCOME = 0;
		Total_groupscore = 0;
		for (int i = 0; i < member_in_each_gang; ++i) {
			member[i] = new GangPrisoner;
		}
		int Times_of_making_same_decision = 0;
		this->name = name;
	}


	int& operator=(int a1) {
		return group_LASTOUTCOME;
	}
	//GetTotalScore
	double GetTotalScore()
	{
		return Total_groupscore;
	}

	//Get each member's choice
	void Group_Execution()
	{
		int sum = 0;
		for (int i = 0; i < member_in_each_gang; ++i)
		{
			sum += member[i]->Execute();
		}
		group_LASTOUTCOME = sum;
	}

	//Get each member's choice if a spy is in
	void Spy_Group_Execution()
	{
		int spy_id;
		int sum = 0;
		bool spy = false;
		for (int i = 0; i < member_in_each_gang; ++i)
		{
			if (member[i]->is_a_spy == false) {
				sum += member[i]->Execute();
			}
			else {
				spy_id = i;
				spy = true;
				continue;
			}
		}
		if (spy == true) {
			sum += member[spy_id]->spy_Execute(sum);
		}
		group_LASTOUTCOME = sum;
	}

	//Update GroupScore
	void UpdateGroupScore()
	{
		double sum = 0;
		for (int i = 0; i < member_in_each_gang; ++i) {
			sum += member[i]->GetScore();
		}
		Total_groupscore = sum;
	}

	int& GetLASTOUTCOME()
	{
		return group_LASTOUTCOME;
	}

	//Update all relavent data in gang tournament
	void Group_Update(int LASTOUTCOME_B)
	{
		for(int i = 0; i < member_in_each_gang; i++)
		{
			member[i]->Update(group_LASTOUTCOME, LASTOUTCOME_B);
		}
		UpdateGroupScore();
	}

	void Generate_A_SPY()
	{
		int spy = GetRandNum(0, 4);
		member[spy]->is_a_spy = true;
	}

	void Generate_A_GangLeader(string filename[])
	{
		int gang_leader = GetRandNum(0, 4);
		member[gang_leader]->is_a_gangleader = true;
		//member[gang_leader]->Load_A_Strategy_File(filename[BestCombination[gang_leader]]);
	}

	void AskGangLeaderToPicKASPY(bool change, gang& the_other_gang)
	{
		int id, revealed_id, final_id, gangleader_id;
		for (int i = 0; i < member_in_each_gang; ++i) {
			if (member[i]->is_a_gangleader == true) {
				gangleader_id = i;
				id = member[gangleader_id]->Pick_A_SPY();
				break;
			}
		}
		if (id <= 4) {
			cout << "Gang Leader choose the " << id << " member in his gang, which is " << this->name << endl;
			revealed_id = Reveal_A_Member(id);
			if (change == false) {
				cout << "Gang Leader choose to stay with his choice!" << endl;
				if (member[id]->is_a_spy == true) {
					cout << "SPY is discovered" << endl << endl;
					for (int i = 0; i < 5; i++) {
						the_other_gang.member[i]->GetScore() += 5;
					}
					return;
				}
				else {
					cout << "SPY not discovered" << endl;
					return;
				}
			}
			else {
				cout << "Gang Leader decide to change his choice! " << endl;
				final_id = member[gangleader_id]->Pick_A_SPY(id, revealed_id);
				if (final_id <= 4) {
					cout << "Gang Leader  now choose the " << final_id << " member in his gang, which is " << this->name << endl;
					if (member[final_id]->is_a_spy == true) {
						cout << "Spy is discovered! " << endl << endl;
						for (int i = 0; i < 5; i++) {
							the_other_gang.member[i]->GetScore() += 5;
							member[i]->GetScore() += 2;
						}
						return;
					}
					else {
						cout << "SPY not discovered" << endl;
						return;
					}
				}
				else {
					cout << "Gang Leader now choose the " << final_id - 5 << " in oppsite gang, which is " << the_other_gang.name << endl;
					if (the_other_gang.member[final_id - 5]->is_a_spy == true) {
						cout << "Spy is discovered! " << endl << endl;
						for (int i = 0; i < 5; i++) {
							the_other_gang.member[i]->GetScore() += 6;
							member[i]->GetScore() += 2;
						}
						return;
					}
					else {
						cout << "SPY not discovered" << endl;
						return;
					}
				}
			}
		}
		else {
			cout << "Gang Leader choose the " << id - 5 << " in oppsite gang, which is " << the_other_gang.name << endl;
			revealed_id = the_other_gang.Reveal_A_Member(id-5);
			if (change == false) {
				cout << "Gang Leader choose to stay with his choice!" << endl;
				if (member[id-5]->is_a_spy == true) {
					cout << "Spy is discovered! " << endl << endl;
					for (int i = 0; i < 5; i++) {
						the_other_gang.member[i]->GetScore() += 6;
					}
					return;
				}
				else {
					cout << "SPY not discovered" << endl;
					return;
				}
			}
			else {
				cout << "Gang Leader decide to change his choice! " << endl;
				final_id = member[gangleader_id]->Pick_A_SPY(id, revealed_id);
				if (final_id <= 4) {
					cout << "Gang Leader  now choose the " << final_id << " member in his gang, which is " << this->name << endl;
					if (member[final_id]->is_a_spy == true) {
						cout << "Spy is discovered! " << endl << endl;
						for (int i = 0; i < 5; i++) {
							the_other_gang.member[i]->GetScore() += 5;
							member[i]->GetScore() += 2;
						}
						return;
					}
					else {
						cout << "SPY not discovered" << endl;
						return;
					}
				}
				else {
					cout << "Gang Leader choose the " << final_id - 5 << " in oppsite gang, which is " << the_other_gang.name << endl;
					if (the_other_gang.member[final_id - 5]->is_a_spy == true) {
						cout << "Spy is discovered! " << endl << endl;
						for (int i = 0; i < 5; i++) {
							the_other_gang.member[i]->GetScore() += 6;
							member[i]->GetScore() += 2;
						}
						return;
					}
					else {
						cout << "SPY not discovered" << endl;
						return;
					}
				}
			}
		}
	}

	int Reveal_A_Member(int id) {
		int temp;
		temp = GetRandNum(0, 4);
		for (int i = 0;; i++) {
			if (member[temp]->is_a_spy == false && member[temp]->is_a_gangleader == false && temp != id) {
				cout << "member " << temp << " in " << this->name << "is revealed as not a spy!" << endl;
				break;
			}
			else {
				temp++;
				if (temp >= member_in_each_gang) {
					temp %= 5;
				}
			}
		}
		return temp;
	}

	void ResetAllMeberIdentity()
	{
		for (int i = 0; i < member_in_each_gang; ++i) {
			member[i]->is_a_gangleader = false;
			member[i]->is_a_spy = false;
		}
	}
};

void Threading_Gang_Decision_Making(gang& g)
{
	g.Group_Execution();
}

void ShowMainMenu()
{
	system("cls");
	cout << edge << endl;
	cout << text_edge<<"\t \t \t \t \tPrisoner Dilemma" << endl;
	cout << text_edge << "\t \t \t \t \t       Menu" << endl;
	cout << text_edge << "1. File Tournament(10 prisoners compete with each other by using pre-written strategy)" << endl;
	cout << text_edge << "2. Generate 10 random strategies locally" << endl;
	cout << text_edge << "3. Complain about the author(not recommended)" << endl;
	cout << text_edge << "4. Gang Tournament" << endl;
	cout << text_edge << "5. Exit" << endl;
	cout << edge << endl;
	cout << "Please choose an option" << endl;
}

void ShowGangTournamentMenu()
{
	system("cls");
	cout << edge << endl;
	cout << text_edge << "\t \t \t \t \t GANG TOURNAMENT" << endl;
	cout << text_edge << "1. Play without a spy" << endl;
	cout << text_edge << "2. Play with a spy" << endl;
	cout << edge << endl;
}

//UI
int main()
{
	My_SetTitle("PRISONER DILEMMA");
	My_SetColor(1, 7);
	for (int run = 0; run < 10; ++run)
	{
		ShowMainMenu();
		//User choice detection system
		string user_choice;
		cin >> user_choice;

		//Exit Program
		if (user_choice == "5") {
			system("cls");
			cout << "Thanks for playing my very first bug! Have a good day!" << endl;
			break;
		}

		//FileTournament
		else if (user_choice == "1") {
			system("cls");
			cout << "************************************************************************************************" << endl;
			cout << "\t \t \t \t \t File Tournament" << endl;
			cout << "1. Input file name" << endl;
			cout << "2. Using default File name" << endl;
			cout << "************************************************************************************************" << endl;
			string user_choice1;
			cin >> user_choice1;
			if (user_choice1 == "1") {
				//For inputting the filname
				cout << "Please input the name of 10 files! Including txt ending" << endl;
				string filename[Num_of_prisoners];
				for (int i = 0; i < Num_of_prisoners; i++) {
					cin >> filename[i];
				}

				try {
					//Read File into the strategy in turns
					strategy group[Num_of_prisoners];
					for (int i = 0; i < Num_of_prisoners; i++) {
						group[i].Load_A_Strategy_File(filename[i]);
						if (group[i].S_loaded() == false) {
							cout << "Stop loading file!" << endl;
							throw - 1;
							break;
						}
					}

					for (int i = 0; i < Num_of_prisoners; ++i) {
						for (int j = i; j < Num_of_prisoners; ++j) {
							for (int k = 0; k < ITERATION; ++k) {
								int D_a, D_b;
								thread t1(Threading_Execution, std::ref(group[i]), std::ref(D_a));
								D_b = group[j].Execute();
								t1.join();
								group[i].Update(D_a, D_b);
								group[j].Update(D_b, D_a);
							}
							//Print each prisoners' stat and decide who win
							cout << "\t Prisoner A stat " << endl;
							group[i].ShowStat();
							cout << "\t Prisoner B stat " << endl;
							group[j].ShowStat();
							if (group[i].GetScore() < group[j].GetScore()) {
								group[i].GetWinTimes()++;
								cout << "Prisoner A win!" << endl;
							}
							else if (group[i].GetScore() > group[j].GetScore()) {
								group[j].GetWinTimes()++;
								cout << "Prisoner B win!" << endl;
							}
							else {
								cout << "Even match!" << endl;
							}
							group[i].Reset();
							group[j].Reset();
						}
					}

					double max_win_times = 0;
					int winner_id = 11;
					for (int i = 0; i < Num_of_prisoners; i++) {
						if (group[i].GetWinTimes()>max_win_times) {
							max_win_times = group[i].GetWinTimes();
							winner_id = i;
						}
					}
					system("cls");
					cout << "Strategy in file " << filename[winner_id] << " have most wins!" << endl;
					Output_A_Strategy("Best_Strategy_in_filetournament1.txt", group[winner_id].command);
					continue;
				}
				catch (int e) {
					cout <<e<<"File Loading Error" << endl;
					cout << "Return to top menu" << endl;
					system("pause");
					continue;
				}
				
			}
			else if (user_choice1 == "2") {
				system("cls");
				cout << "Good choice, program will now load 10 files with default filename" << endl;
				string filename[Num_of_prisoners] = {
					"strategy1.txt",
					"strategy2.txt",
					"strategy3.txt",
					"strategy4.txt",
					"strategy5.txt",
					"strategy6.txt",
					"strategy7.txt",
					"strategy8.txt",
					"strategy9.txt",
					"strategy10.txt"
				};

				//Read File into the strategy in turns
				strategy group[Num_of_prisoners];
				bool Fileloaded = true;
				for (int i = 0; i < Num_of_prisoners; i++) {
					group[i].Load_A_Strategy_File(filename[i]);
					if (group[i].S_loaded() == false) {
						cout << "Stop loading file!" << endl;
						Fileloaded = false;
						break;
					}
				}

				//Tournament start
				if (Fileloaded == false) {
					cout << "Return to the top menu" << endl;
					continue;
				}
				else {
					for (int i = 0; i < Num_of_prisoners; ++i) {
						for (int j = i + 1; j < Num_of_prisoners; ++j) {
							for (int k = 0; k < ITERATION; ++k) {
								int D_a, D_b;
								thread t1(Threading_Execution, std::ref(group[i]), std::ref(D_a));
								D_b = group[j].Execute();
								t1.join();
								group[i].Update(D_a, D_b);
								group[j].Update(D_b, D_a);
							}
							//Print each prisoners' stat and decide who win
							cout << "Prisoner" << i << " " << "VS" << " " << "Prisoner" << j << endl;
							cout << "\t Prisoner A stat " << endl;
							group[i].ShowStat();
							cout << "\t Prisoner B stat " << endl;
							group[j].ShowStat();
							if (group[i].GetScore() < group[j].GetScore()) {
								group[i].GetWinTimes()++;
								cout << "Prisoner A win!" << endl;
							}
							else if (group[i].GetScore() > group[j].GetScore()) {
								group[j].GetWinTimes()++;
								cout << "Prisoner B win!" << endl;
							}
							else {
								cout << "Even match!" << endl;
							}
							group[i].Reset();
							group[j].Reset();
							system("pause");
						}
					}

					double max_win_times = 0;
					int winner_id = 11;
					for (int i = 0; i < Num_of_prisoners; i++) {
						if (group[i].GetWinTimes()>max_win_times) {
							max_win_times = group[i].GetWinTimes();
							winner_id = i;
						}
					}
					cout << "Strategy in file " << filename[winner_id] << " have most victories!" << endl;
					Output_A_Strategy("Best_Strategy_in_filetournament2.txt", group[winner_id].command);
					continue;
				}
			}


			else {
				cout << "Only integer 1-2 is acceptable, return to top menu" << endl;
				continue;
			}
		}
		else if (user_choice == "2") {
			system("cls");
			string filename[Num_of_prisoners] = {
				"Random_strategy1.txt",
				"Random_strategy2.txt",
				"Random_strategy3.txt",
				"Random_strategy4.txt",
				"Random_strategy5.txt",
				"Random_strategy6.txt",
				"Random_strategy7.txt",
				"Random_strategy8.txt",
				"Random_strategy9.txt",
				"Random_strategy10.txt"
			};
			strategy randomgroup[Num_of_prisoners];
			for (int i = 0; i < Num_of_prisoners; ++i) {
				randomgroup[i].Generate_A_Random_Strategy();
				Output_A_Strategy(filename[i], randomgroup[i].command);
			}
			cout << "Program has already generated 10 random srategies called Random_strategy_n, please check your file!" << endl;
			system("pause");
			continue;
		}
		else if (user_choice == "3") {
			system("cls");
			cout << "Forgiveness is the nearest way to happinese." << endl;
			cout << "Program will now return to top menu" << endl;
			system("pause");
			continue;
		}
		else if (user_choice == "4")
		{
			ShowGangTournamentMenu();
			string user_choice2;
			cin >> user_choice2;
			if (user_choice2 == "1") {
				system("cls");
				cout << "Good choice, program will now load 10 files with default filename to each gang member" << endl;
				system("pause");
				string filename[Num_of_prisoners] = {
					"gangstrategy1.txt",
					"gangstrategy2.txt",
					"gangstrategy3.txt",
					"gangstrategy4.txt",
					"gangstrategy5.txt",
					"gangstrategy6.txt",
					"gangstrategy7.txt",
					"gangstrategy8.txt",
					"gangstrategy9.txt",
					"gangstrategy10.txt"
				};

				double minScore = 100000;
				for (int a1 = 0; a1 < 6; a1++) {
					for (int a2 = a1 + 1; a2 < 7; a2++) {
						for (int a3 = a2 + 1; a3 < 8; a3++) {
							for (int a4 = a3 + 1; a4 < 9; a4++) {
								for (int a5 = a4 + 1; a5 < 10; a5++) {
									//Load All files accordingly using STL set container
									gang Purple_Hand_Gang("Purple Hand Gang");
									gang Margenta_hand_Gang("Margenta Hand Gang");

									Purple_Hand_Gang.member[0]->Load_A_Strategy_File(filename[a1]);
									Purple_Hand_Gang.member[1]->Load_A_Strategy_File(filename[a2]);
									Purple_Hand_Gang.member[2]->Load_A_Strategy_File(filename[a3]);
									Purple_Hand_Gang.member[3]->Load_A_Strategy_File(filename[a4]);
									Purple_Hand_Gang.member[4]->Load_A_Strategy_File(filename[a5]);

									set<int> possible_value;
									InitializeSet(possible_value);
									possible_value.erase(a1);
									possible_value.erase(a2);
									possible_value.erase(a3);
									possible_value.erase(a4);
									possible_value.erase(a5);
									set<int>::iterator ite;
									
									//i1 is for counting purpose
									int i1 = 0; 
									for (ite = possible_value.begin(); ite != possible_value.end(); ite++) {
										Margenta_hand_Gang.member[i1++]->Load_A_Strategy_File(filename[*ite]);
									}

									for (i1 = 0; i1 < ITERATION; i1++)
									{
										//Group_Decision_Making using threading
										thread t1(Threading_Gang_Decision_Making, std::ref(Purple_Hand_Gang));
										//Purple_Hand_Gang.Group_Execution();
										Margenta_hand_Gang.Group_Execution();
										t1.join();
										//Update all data required
										Purple_Hand_Gang.Group_Update(Margenta_hand_Gang.GetLASTOUTCOME());
										Margenta_hand_Gang.Group_Update(Purple_Hand_Gang.GetLASTOUTCOME());

										//operator overload
										Purple_Hand_Gang = 0;
										Margenta_hand_Gang = 0;
									}

									if (Purple_Hand_Gang.GetTotalScore() < Margenta_hand_Gang.GetTotalScore()) {
										cout << "Purple_Hand_Gang win!" << endl;
										cout << "Combnination " << a1 << " " << a2 << " " << a3 << " " << a4 << " " << a5 << " is better with a score " << Purple_Hand_Gang.GetTotalScore() <<endl << endl;
										if (Purple_Hand_Gang.GetTotalScore() < minScore) {
											BestCombination[0] = a1;
											BestCombination[1] = a2;
											BestCombination[2] = a3;
											BestCombination[3] = a4;
											BestCombination[4] = a5;
											minScore = Purple_Hand_Gang.GetTotalScore();
										}
									}
									else if (Purple_Hand_Gang.GetTotalScore() > Margenta_hand_Gang.GetTotalScore()) {
										cout << "Margenta_Hand_Gang win!" << endl;
										cout << "Combnination";
										ShowSet(possible_value);
										cout << "is better with a score " <<Margenta_hand_Gang.GetTotalScore() <<endl << endl;
										if (Margenta_hand_Gang.GetTotalScore() < minScore) {
											ite = possible_value.begin();
											for (int i = 0; i < member_in_each_gang; i++) {
												BestCombination[i] = *ite;
												ite++;
											}
											minScore = Margenta_hand_Gang.GetTotalScore();
										}
									}
									else {
										cout << "Even Match" << endl;
									}
									//system("pause");
								}
							}
						}
					}
				}

				cout << "Best combination is ";
				for (int i = 0; i < member_in_each_gang; i++) {
					cout << BestCombination[i] << " ";
				}
				cout << "With a score of " << minScore << endl;
				system("pause");
				continue;
			}
			else if (user_choice2 == "2") {
				system("cls");
				cout << "Good choice, program will now use the best strategy combinations play against the other when a Spy is exist" << endl;

				string filename[Num_of_prisoners] = {
					"gangstrategy1.txt",
					"gangstrategy2.txt",
					"gangstrategy3.txt",
					"gangstrategy4.txt",
					"gangstrategy5.txt",
					"gangstrategy6.txt",
					"gangstrategy7.txt",
					"gangstrategy8.txt",
					"gangstrategy9.txt",
					"gangstrategy10.txt"
				};

				bool change;
				cout << "************************************************************************************************" << endl;
				cout << "1. Gang Leader won't change after reveal a normal gang member" << endl;
				cout << "2. Gang Leader will change after reveal a normal gang member" << endl;
				cout << "************************************************************************************************" << endl;
				cout << "Please choose an option!" << endl;
				int user_choice4;
				cin >> user_choice4;
				if (user_choice4 == 1) {
					change = false;
				}
				else if (user_choice4 == 2) {
					change = true;
				}
				else {
					cout << "Invaild input, return to top menu!" << endl;
					system("pause");
					continue;
				}
				gang Purple_Hand_Gang("Purple Hand Gang");
				gang Margenta_hand_Gang("Margenta Hand Gang");
				for (int j = 0; j < member_in_each_gang; j++) {
					Purple_Hand_Gang.member[j]->Load_A_Strategy_File(filename[BestCombination[j]]);
					Margenta_hand_Gang.member[j]->Load_A_Strategy_File(filename[NormalCombination[j]]);
				}
				for (int j = 0; j < ITERATION; j++) {
					Purple_Hand_Gang.Spy_Group_Execution();
					Margenta_hand_Gang.Spy_Group_Execution();
					Purple_Hand_Gang.Group_Update(Margenta_hand_Gang.GetLASTOUTCOME());
					Margenta_hand_Gang.Group_Update(Purple_Hand_Gang.GetLASTOUTCOME());
					Purple_Hand_Gang.GetLASTOUTCOME() = 0;
					Margenta_hand_Gang.GetLASTOUTCOME() = 0;
				}
				cout << "************************************************************************************************" << endl;
				cout << "Stat when spy is not exist: " << endl;
				cout << "Purple Hand Gang Score: " << Purple_Hand_Gang.GetTotalScore() << endl;
				cout << "Margenta Hnad Gang Score: " << Margenta_hand_Gang.GetTotalScore() << endl;
				cout << "************************************************************************************************" << endl;


				for (int i = 20; i >= 5; i -= 5) {
					gang Purple_Hand_Gang("Purple Hand Gang");
					gang Margenta_hand_Gang("Margenta Hand Gang");
					for (int j = 0; j < member_in_each_gang; j++) {
						Purple_Hand_Gang.member[j]->Load_A_Strategy_File(filename[BestCombination[j]]);
						Margenta_hand_Gang.member[j]->Load_A_Strategy_File(filename[NormalCombination[j]]);
					}
					for (int j = 0; j < ITERATION; j++) {
						//Spy execution
						if (j%i == 0) {
							int r = GetRandNum(0, 1);
							if (r == 1) {
								Purple_Hand_Gang.Generate_A_SPY();
							}
							else {
								Margenta_hand_Gang.Generate_A_SPY();
							}
							Purple_Hand_Gang.Generate_A_GangLeader(filename);
							Margenta_hand_Gang.Generate_A_GangLeader(filename);
							Purple_Hand_Gang.AskGangLeaderToPicKASPY(change,Margenta_hand_Gang);
							Margenta_hand_Gang.AskGangLeaderToPicKASPY(change, Purple_Hand_Gang);
						}
						//normal execution
						Purple_Hand_Gang.Spy_Group_Execution();
						Margenta_hand_Gang.Spy_Group_Execution();
						Purple_Hand_Gang.Group_Update(Margenta_hand_Gang.GetLASTOUTCOME());
						Margenta_hand_Gang.Group_Update(Purple_Hand_Gang.GetLASTOUTCOME());
						Purple_Hand_Gang.GetLASTOUTCOME() = 0;
						Margenta_hand_Gang.GetLASTOUTCOME() = 0;
						Purple_Hand_Gang.ResetAllMeberIdentity();
						Margenta_hand_Gang.ResetAllMeberIdentity();
					}
					cout << "************************************************************************************************" << endl;
					cout << "Stat when spy exist for " << double(ITERATION / i) << " % of the total time: " << endl;
					if (change == true) {
						cout << "Gang Leader will change after revealing a normal gang member!" << endl;
					}
					else {
						cout << "Gang Leader won't change after revealing a normal gang member!" << endl;
					}
					cout << "Purple Hand Gang Score: " << Purple_Hand_Gang.GetTotalScore() << endl;
					cout << "Margenta Hnad Gang Score: " << Margenta_hand_Gang.GetTotalScore() << endl;
					cout << "************************************************************************************************" << endl;
					system("pause");
				}

				system("pause");
				continue;
			}
			else {
				cout << "only integer 1 -2 is acceptable !" << endl;
				system("pause");
				continue;
			}
		}
		else {
			cout << "only integer 1-4 is acceptable! Please be kind to this little program! " << endl;
			continue;
		}
	}

	system("pause");
	return 0;
}
