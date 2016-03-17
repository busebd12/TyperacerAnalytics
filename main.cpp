#include "TypingRace.h"
#include "csv.h"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char* argv [])
{
	vector<TypingRace> typingRaces;

	//open csv file and specify the number of columns that should be read with "<6>"
	io::CSVReader<6> in("./race_data.csv");

	//let the library know what columns it should look for
	in.read_header(io::ignore_extra_column, "Race #", "WPM", "Accuracy", "Rank", "Text ID", "Date/Time (UTC)");

	int race;

	int wpm;

	double accuracy;

	int rank;

	int text_id;

	string date;

	//function to remove the time part from the date string since
	//when we read the date from the date column in the csv file,
	//the column has both the date and time of the race
	auto dateParse=[&] (string S) -> void 
	{
		auto lastSpace=S.find_last_of(" ");

		string partToErase {S.substr(lastSpace+1)};

		auto found=S.find(partToErase);

		S.erase(found, partToErase.size());
	};

	//read in the colum data in the variables
	while(in.read_row(race, wpm, accuracy, rank, text_id, date))
	{
		TypingRace t;

		t.setRaceNumber(race);

		t.setWordsPerMinute(wpm);

		t.setAccuracy(accuracy);

		t.setFinishingPlace(rank);

		t.setTextID(text_id);

		dateParse(date);

		t.setDate(date);

		typingRaces.push_back(t);
	}

	for(const auto & race : typingRaces)
	{
		cout << "Race #: " << race.getRaceNumber() << endl;

		cout << endl;

		cout << "WPM: " << race.getWordsPerMinute() << endl;

		cout << endl;

		cout << "Accuracy: " << race.getAccuracy() << "%" << endl;

		cout << endl;

		cout << "Text id: " << race.getTextID() << endl;

		cout << endl;

		cout << "Race date: " << race.getDate() << endl;
	}
}