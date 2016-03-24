#include "TypingRace.h"
#include "csv.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

vector<pair<int, int>> getThreadPartitions(int NumberOfRows, int NumberOfThreads)
{
	vector<pair<int, int>> spots;

	int spot=0;

	int increment=NumberOfRows/NumberOfThreads;

	int count=0;

	int first {};

	int second {};

	while(count < NumberOfThreads)
	{	
		if(count==0)
		{
			second+=increment;
		}
		else
		{
			first+=increment;

			second+=increment;
		}

		spots.emplace_back(make_pair(first, second));

		count++;
	}

	cout << "Spots:" << endl;

	for(const auto & spot : spots)
	{
		cout << spot.first << " " << spot.second << endl;
	}

	return spots;
}

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

	/*
	lambda function to remove the time part from the date string since
	when we read the date from the date column in the csv file,
	the column has both the date and time of the race
	*/
	auto dateParse=[&] (string S) -> void 
	{
		auto lastSpace=S.find_last_of(" ");

		string partToErase {S.substr(lastSpace+1)};

		auto found=S.find(partToErase);

		S.erase(found, partToErase.size());
	};

	/*
	lambda function to print out vector
	*/
	auto printTypingRaces=[&] () -> void
	{
		for_each(typingRaces.begin(), typingRaces.end(), [] (const auto & race) 
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

			cout << endl;
		});
	};

	int count=0;

	int numberOfRows=0;

	//read in the colum data in the variables
	while(in.read_row(race, wpm, accuracy, rank, text_id, date))
	//while(count < 2)
	{
		//cout << "Count: " << count << endl;

		//cout << endl;

		//in.read_row(race, wpm, accuracy, rank, text_id, date);

		TypingRace t;

		t.setRaceNumber(race);

		t.setWordsPerMinute(wpm);

		t.setAccuracy(accuracy);

		t.setFinishingPlace(rank);

		t.setTextID(text_id);

		dateParse(date);

		t.setDate(date);

		typingRaces.push_back(t);

		count++;

		numberOfRows++;
	}

	cout << "The number of rows read: " << numberOfRows << endl;

	auto determineThreadsToUse=[=] (const int & NumberOfRows) -> int
	{
		int count=1;

		while(count < numberOfRows)
		{
			if(count > 1 && numberOfRows%count==0)
			{
				//cout << "count: " << count << endl;

				break;
			}

			count++;
		}

		return count;	
	};

	int numberOfThreads=determineThreadsToUse(numberOfRows);

	cout << "The number of threads to use is: " << numberOfThreads << endl;

	vector<thread> threadsVector;

	threadsVector.resize(numberOfThreads);

	auto pairs=getThreadPartitions(numberOfRows, numberOfThreads);

	//join the rest of the non-main threads
	//for_each(threadsVector.begin(), threadsVector.end(), [] (thread & t) {t.join();});
}