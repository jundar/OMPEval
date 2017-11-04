#include "omp/EquityCalculator.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace omp;
using namespace std;

const int BAD_USAGE = 1;
const int BAD_FILE = 2;
const int BAD_BOARD = 3;

/*
 * I don't really know what I'm doing here, but my plan is to
 * use this library to write my own super-fast calculator of
 * equities on all streets.
 *
 * It will read all hand combos from a file and calculate their equities
 * against a random hand for a specific, possibly empty, board.
 * The results will then be saved to a file, or maybe, printed out.
 */

/*
 * arg0 = this
 * arg1 = file with hand combos
 * arg2 = board/empty
 */
int main(int argc, char **argv)
{
	bool debug = true;

	//Check valid usage
	if(argc < 2)
	{
		cout << "Compute range equity: handFile [board]\n";
		cout << "Get winner: hand1 hand2 board\n";
		exit(BAD_USAGE);
	}

	//Calculate winner for two hands
	if(argc >= 4)
	{
		string hand1 = argv[1];
		string hand2 = argv[2];
		uint64_t board = CardRange::getCardMask(argv[3]);

		EquityCalculator winEq;
		winEq.start({hand1, hand2}, board, {}, false, 0.01);
		winEq.wait();
		auto results = winEq.getResults();

		/*
		 * tie 					= 	-1
		 * first player wins	= 	0
		 * second player wins	= 	1
		 */
		int winner = -1;
		if(results.wins[0] > 0)
		{
			winner = 0;
		}

		else if(results.wins[1] > 0)
		{
			winner = 1;
		}

		cout << winner << "\n";
	}

	//Calculate equity for range
	else
	{
		//Verify the hand file is readable
		ifstream handFile;
		handFile.open(argv[1]);

		if(!handFile.good() || !handFile.is_open())
		{
			cout << "Could not open input file\n";
			exit(BAD_FILE);
		}

		//Read hands into list of strings
		vector<string> handCombos;
		while(!handFile.eof())
		{
			//Read line into string
			string line;
			getline(handFile, line);

			handCombos.push_back(line);
		}

		//Get board
		uint64_t board = 0;
		if(argc > 2)
		{
			board = CardRange::getCardMask(argv[2]);
		}

		//Create equity calculators for each hand combo
		omp::EquityCalculator **eqs = new omp::EquityCalculator*[handCombos.size()];
		for(int i=0; i<handCombos.size(); i++)
		{
			eqs[i] = new omp::EquityCalculator();
		}

		//Calculate equities for each hand combo against a random range
		for(int i=0; i<handCombos.size(); i++)
		{
			string hand = handCombos[i];
			eqs[i]->start({hand, "random"}, board, {}, false, 0.01);
		}

		//All threads should be started, now we just need to wait on each one in turn
		for(int i=0; i<handCombos.size(); i++)
		{
			eqs[i]->wait();
			auto results = eqs[i]->getResults();

			//And display
			cout << handCombos[i] << "," << results.wins[0] << "," << (int) (results.ties[0] * 2) << "," << results.hands << "\n";
		}
	}

	return 0;
}