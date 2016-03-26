#ifndef TYPINGRACE_H
#define TYPINGRACE_H
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
class TypingRace
{
	private:
		int raceNumber;
		int wordsPerMinute;
		int accuracy;
		int finishingPlace;
		int text_id;
		boost::gregorian::date raceDate;

	public:
		TypingRace();
		void setRaceNumber(const int RaceNumber);
		void setWordsPerMinute(const int WordsPerMinute);
		void setAccuracy(const double Accuracy);
		void setFinishingPlace(const int FinishingPlace);
		void setTextID(const int TextID);
		void setDate(const std::string Date);
		int getRaceNumber() const;
		int getWordsPerMinute() const;
		int getAccuracy() const;
		int getFinishingPlace() const;
		int getTextID() const;
		boost::gregorian::date getDate() const;
		boost::gregorian::greg_year getYear() const;
		boost::gregorian::greg_month getMonth() const;
		boost::gregorian::greg_day getDay() const;
};
#endif