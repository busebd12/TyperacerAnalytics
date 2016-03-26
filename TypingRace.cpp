#include "TypingRace.h"

TypingRace::TypingRace() {}

void TypingRace::setRaceNumber(const int RaceNumber)
{
	raceNumber=RaceNumber;
}

void TypingRace::setWordsPerMinute(const int WordsPerMinute)
{
	wordsPerMinute=WordsPerMinute;
}

void TypingRace::setAccuracy(const double Accuracy)
{
	accuracy=Accuracy*100;
}

void TypingRace::setFinishingPlace(const int FinishingPlace)
{
	finishingPlace=FinishingPlace;
}

void TypingRace::setTextID(const int TextID)
{
	text_id=TextID;
}

void TypingRace::setDate(std::string Date)
{
	boost::gregorian::date ds(boost::gregorian::from_string(Date));

	raceDate=ds;
}

int TypingRace::getRaceNumber() const
{
	return raceNumber;
}

int TypingRace::getWordsPerMinute() const
{
	return wordsPerMinute;
}

int TypingRace::getAccuracy() const
{
	return accuracy;
}

int TypingRace::getFinishingPlace() const
{
	return finishingPlace;
}

int TypingRace::getTextID() const
{
	return text_id;
}

boost::gregorian::date TypingRace::getDate() const
{
	return raceDate;
}

boost::gregorian::greg_year TypingRace::getYear() const
{
	return raceDate.year();
}

boost::gregorian::greg_month TypingRace::getMonth() const
{
	return raceDate.month();
}

boost::gregorian::greg_day TypingRace::getDay() const
{
	return raceDate.day();
}
