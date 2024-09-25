#pragma once

struct DateInfo
{
	int year;
	int moth;
	int day;

	DateInfo(int year, int moth, int day)
	{
		this->year = year;
		this->moth = moth;
		this->day = day;
	}
	DateInfo()
	{
		this->year = 2015;
		this->moth = 4;
		this->day = 2;
	}

	void SetDay(int day);
	
	int GetDay();

	void SetYear(int year);
	
	int Getyear();
	
	void SetMoth(int moth);
	
	int GetMoth();
	

};