#include "TypingRace.h"
#include "csv.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include <unordered_map>
#include <set>
using namespace std;

void findAverageWPMByYear(const vector<TypingRace> & TypingRaces, const int & Start, const int & End, const boost::gregorian::greg_year & Year, vector<pair<int, boost::gregorian::greg_year>> & FinalAverages)
{
	mutex mtx;

	int totalWPM {};

	int count {};

	for_each(TypingRaces.begin()+Start, TypingRaces.begin()+End, [&] (const auto & element) 
	{
		totalWPM+=element.getWordsPerMinute();

		count++;
	});

	//cout << "Total WPM: " << totalWPM << endl;

	//cout << "Total races: " << count << endl;

	int averageWPM=totalWPM/count;

	lock_guard<mutex> lockGuard(mtx);

	FinalAverages.push_back({averageWPM, Year});
}

void findFastestWPM(const vector<TypingRace> & Races, const int Start, const int End, map<int, boost::gregorian::date> & Map)
{
	mutex mtx;

	auto threadID=this_thread::get_id();

	auto fastestWPM=max_element(Races.begin()+Start, Races.begin()+End, [] (const auto & elementOne, const auto & elementTwo) {return elementOne.getWordsPerMinute() < elementTwo.getWordsPerMinute();});

	lock_guard<mutex> lck(mtx);

	Map.insert({fastestWPM->getWordsPerMinute(), fastestWPM->getDate()});
}

void findSlowestWPM(const vector<TypingRace> & Races, const int Start, const int End, map<int, boost::gregorian::date> & Map)
{
	mutex mtx;

	auto threadID=this_thread::get_id();

	auto slowestWPM=min_element(Races.begin()+Start, Races.begin()+End, [] (const auto & elementOne, const auto & elementTwo) {return elementOne.getWordsPerMinute() < elementTwo.getWordsPerMinute();});

	lock_guard<mutex> lck(mtx);

	Map.insert({slowestWPM->getWordsPerMinute(), slowestWPM->getDate()});
}

void findAverageWPM(const vector<TypingRace> & Races, const int Start, const int End)
{
	auto threadID=this_thread::get_id();

	//cout << "Thread " << threadID << " is working on finding the average wpm from its share of races" << endl;

	int totalWPM {};

	int totalRaces {};

	for(int index=Start;index<End;++index)
	{
		totalWPM+=Races[index].getWordsPerMinute();

		totalRaces++;
	}
}

vector<pair<int, int>> getThreadPartitions(int NumberOfRows, int NumberOfThreads)
{
	vector<pair<int, int>> spots;

	int spot {};

	int increment=NumberOfRows/NumberOfThreads;

	cout << "Increment:" << increment << endl;

	int count {};

	int first {};

	int second=increment;

	int offset {};

	while(count < NumberOfThreads)
	{	
		if(count==0)
		{
			spots.emplace_back(make_pair(first, second));
		}
		else if(count==NumberOfThreads-1)
		{
			first=second+1;

			second+=(increment-(offset-1));

			spots.emplace_back(make_pair(first, second));
		}
		else
		{
			first=second+1;

			second+=(increment+1);

			spots.emplace_back(make_pair(first, second));
		}

		count++;

		offset++;
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

	set<boost::gregorian::greg_year> years;

	map<int, boost::gregorian::date> racesMap;

	vector<pair<int, boost::gregorian::greg_year>> finalAverages;

	//open csv file and specify the number of columns that should be read with "<6>"
	io::CSVReader<3> in("./race_data.csv");

	//let the library know what columns it should look for
	in.read_header(io::ignore_extra_column, "WPM", "Accuracy", "Date/Time (UTC)");

	int wpm;

	double accuracy;

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

	//lambda function to insert just the year part of the date into our set of years
	auto insertYear=[&] (string S, set<boost::gregorian::greg_year> & Years) -> void
	{
		boost::gregorian::date ds(boost::gregorian::from_string(S));

		Years.insert(ds.year());
	};

	//lambda function to print the typing races
	auto printRaces=[&] (const map<int, boost::gregorian::date> & Map) -> void
	{
		cout << "Races:" << endl;

		for_each(Map.begin(), Map.end(), [] (const auto & element) {cout << element.first << endl;});
	};

	auto printFastestWPM=[&] (const map<int, boost::gregorian::date> & Map) -> void
	{
		auto fastestWPM=max_element(Map.begin(), Map.end());

		cout << "The fastest WPM was: " << fastestWPM->first << endl;
	};

	auto printSlowestWPM=[&] (const map<int, boost::gregorian::date> & Map) -> void
	{
		auto slowestWPM=min_element(Map.begin(), Map.end());

		cout << "The slowest WPM was: " << slowestWPM->first << endl;
	};

	//lambda function to print the years of the typing races
	auto printYears=[&] (const set<boost::gregorian::greg_year> & Years) -> void
	{
		for_each(Years.begin(), Years.end(), [] (const auto & element) {cout << element << endl;});	
	};

	//lambda function that prints the average words per minutes by year
	auto printAverageWPMByYear=[&] (const vector<pair<int, boost::gregorian::greg_year>> & FinalAverages)
	{
		cout << "Average WPM by year:" << endl;

		for_each(FinalAverages.begin(), FinalAverages.end(), [] (const auto & element) 
		{
			cout << "WPM: " << element.first << endl;

			cout << "Year: " << element.second << endl;
		});
	};

	int count=0;

	int numberOfRows=0;

	//read in the colum data in the variables
	//while(in.read_row(wpm, accuracy, date))
	while(count < 100)
	{
		//cout << "Count: " << count << endl;

		//cout << endl;

		in.read_row(wpm, accuracy, date);

		TypingRace t;

		t.setWordsPerMinute(wpm);

		t.setAccuracy(accuracy);

		dateParse(date);

		insertYear(date, years);

		t.setDate(date);

		typingRaces.push_back(t);

		count++;

		numberOfRows++;
	}

	cout << "The number of rows read: " << numberOfRows << endl;

	cout << "The different years:" << endl;

	printYears(years);

	auto determineThreadsToUse=[=] (const int & NumberOfRows) -> int
	{
		int count=0;

		while(count < numberOfRows)
		{
			if(count > 1 && numberOfRows%count==0)
			{
				cout << "count: " << count << endl;

				break;
			}

			count++;
		}

		return count;	
	};


	int numberOfThreads=determineThreadsToUse(numberOfRows);

	cout << "The number of threads to use is: " << numberOfThreads << endl;

	vector<thread> threadsVector;

	auto pairs=getThreadPartitions(numberOfRows, numberOfThreads);

	cout << "Right before creating the threads" << endl;

	//create threads to find the fastest words per minute typed in their assigned part of the vector
	for(int i=0;i<numberOfThreads;++i)
	{
		for(const auto & element : pairs)
		{
			threadsVector.push_back(thread(findFastestWPM, cref(typingRaces), cref(element.first), cref(element.second), ref(racesMap)));
		}
	}
	

	/*
	//create threads to find the slowest words per minute typed in their assigned part of the vector
	for(int i=0;i<numberOfThreads;++i)
	{
		for(const auto & element : pairs)
		{
			threadsVector.push_back(thread(findSlowestWPM, cref(typingRaces), cref(element.first), cref(element.second), ref(racesMap)));
		}
	}
	*/
	
	/*
	//create threads to find the average words per minute typed by year in their assigned part of the vector
	for(int x=0;x<numberOfThreads;++x)
	{
		for(const auto & year : years)
		{
			for(const auto & pair : pairs)
			{
				threadsVector.push_back(thread(findAverageWPMByYear, cref(typingRaces), cref(pair.first), cref(pair.second), cref(year), ref(finalAverages)));			
			}
		}
	}
	*/

	printRaces(racesMap);
	
	printFastestWPM(racesMap);

	//printSlowestWPM(racesMap);

	//printAverageWPMByYear(finalAverages);

	//join the rest of the non-main threads
	for_each(threadsVector.begin(), threadsVector.end(), [] (thread & t) {t.join();});
	
}