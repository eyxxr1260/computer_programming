#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;
using std::setfill;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ios;

#include <vector>
using std::vector;

struct Date
{
	int year = 0;
	int month = 0;
	int day = 0;
};

struct AvailTable
{
	Date date;
	int numAvailTables[4][6] = {}; // the number of available tables,
};  // numAvailTables[ 1 ][ i ] is the number of available type i tables on 11:30
	// numAvailTables[ 2 ][ i ] is the number of available type i tables on 14:30
	// numAvailTables[ 3 ][ i ] is the number of available type i tables on 17:30

struct Reservation
{
	int numCustomers = 0;              // number of customers
	Date date;                         // reservation date
	int time = 0;                      // reservation time code
	char name[8] = "";               // name
	char mobileNumber[12] = "";      // mobile phone number
	char email[40] = "";             // email address
	char password[12] = "";          // reservation password
	char reservationNumber[12] = ""; // reservation number
};

// tableTypes[ i ] is the table type for i customers
const int tableTypes[13] = { 0, 1, 1, 2, 2, 3, 4, 4, 0, 0, 5, 5, 5 };

const int TOTAL_NUM_TABLES = 2; // suppose there are totally 2 tables for all types

// return current date
Date computeCurrentDate();

// return true if and only if year is a leap year
inline bool leapYear(int year);

// input an integer in the range [ begin, end ]
int inputAnInteger(int begin, int end);

// return true if and only if date1 == date2
bool equal(Date date1, Date date2);

// return true if and only if date1 < date2
bool less(Date date1, Date date2);

// return true if and only if date1 <= date2
bool lessEqual(Date date1, Date date2);

// return date + numDays
Date add(Date date, int numDays);

// delete out of date data in availTables, and append new data from rear of availTables
void initAvailTables(vector< AvailTable >& availTables, Date currentDate);

// read available table informations after currentDate, from the file AvailTables.dat
void loadAvailTables(vector< AvailTable >& availTables, Date currentDate);

// read reservation informations after or equal to currentDate, from the file Reservations.dat
void loadReservations(vector< Reservation >& reservations, Date currentDate);

// make reservation
void makeReservation(vector< Reservation >& reservations, vector< AvailTable >& availTables, Date currentDate);

// choose a date and a time
void chooseDateTime(vector< AvailTable >& availTables, Reservation& reservation, Date currentDate, int tableType);

// display dates on which there are tables available
void displayAvailDates(vector< AvailTable >& availTables, bool available[], int tableType);

// return true if there are available tables on availTables[ i ].date for corresponding table type
bool availableTables(vector< AvailTable >& availTables, size_t i, int tableType);

// --availTables[ i ].numAvailTables[ timeCode ][ tableType ], where availTables[ i ].date == date
void decreaseAvailTables(vector< AvailTable >& availTables, Date date, int timeCode, int tableType);

// use reservationNumber to make a reservation inquiry, and let user choose whether to cancel it
void reservationInquiry(vector< Reservation >& reservations, vector< AvailTable >& availTables);

// display all fields of reservation which includes
// mobileNumber, name, date, time, email, numCustomers, password and reservationNumber
void displayReservationInfo(const Reservation& reservation);

// display reservations[ i ], where
// reservations[ i ].reservationNumber is equal to the specified reservationNumber
void displayReservationInfo(const vector< Reservation >& reservations, char reservationNumber[]);

// erase reservations[ position ]
void erase(vector< Reservation >& reservations, int position);

// write availTables into the file AvailTables.dat
void saveAvailTables(const vector< AvailTable >& availTables);

// write reservations into the file Reservations.dat
void saveReservations(const vector< Reservation >& reservations);

int main()
{
	cout << "Welcome to Hi-Lai Harbour Taoyuan Location!\n";

	srand(static_cast<unsigned int>(time(0)));
	Date currentDate = computeCurrentDate();

	vector< AvailTable > availTables; // vector of all available table informations
	initAvailTables(availTables, currentDate);

	vector< Reservation > reservations; // vector of all available table informations
	loadReservations(reservations, currentDate);

	bool userExited = false; // user has not chosen to exit
	//saveAvailTables(availTables);
	// loop while user has not chosen option to exit system
	while (!userExited)
	{
		// show main menu and get user selection
		cout << "\n1 - Book a Table\n";
		cout << "2 - My Bookings\n";
		cout << "3 - End\n\n";

		int mainMenuSelection;
		do cout << "Enter your choice: (1~3): ";
		while ((mainMenuSelection = inputAnInteger(1, 3)) == -1);

		// decide how to proceed based on user's menu selection
		switch (mainMenuSelection)
		{
		case 1:
			makeReservation(reservations, availTables, currentDate); // make reservation
			break;
		case 2:
			reservationInquiry(reservations, availTables); // reservation inquiry
			break;
		case 3: // user chose to terminate session
			saveAvailTables(availTables);
			saveReservations(reservations);
			userExited = true; // this session should end
			cout << endl;
			break;
		default: // user did not enter an integer from 1-3
			cout << "\nIncorrect choice!" << endl;
		}
	}

	system("pause");
}

Date computeCurrentDate()
{
	Date AvailDates;
	int second = static_cast<int>(time(0)) + 8 * 60 * 60;

	int year = 1970;

	int secondsPerYear = 365 * 24 * 60 * 60;
	if (leapYear(year))
		secondsPerYear += 24 * 60 * 60;//閏年+1天
	while (second >= secondsPerYear)
	{
		second -= secondsPerYear;
		year++;
		secondsPerYear = 365 * 24 * 60 * 60;
		if (leapYear(year))
			secondsPerYear += 24 * 60 * 60;
	}

	int days[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (leapYear(year))
		days[2] = 29;

	int month = 1;
	while (second >= days[month] * 24 * 60 * 60)
	{
		second -= days[month] * 24 * 60 * 60;
		month++;
	}

	int day = second / (24 * 60 * 60) + 1;
	AvailDates.day = day;
	AvailDates.month = month;
	AvailDates.year = year;

	return AvailDates;
}

inline bool leapYear(int year)
{
	return (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0));
}

int inputAnInteger(int begin, int end)
{
	char string[80];
	cin.getline(string, 80, '\n');

	if (strlen(string) == 0)
		//exit(0);

	for (unsigned int i = 0; i < strlen(string); i++)
		if (string[i] < '0' || string[i] > '9')
			return -1;

	int number = atoi(string);

	if (number >= begin && number <= end)
		return number;
	else
		return -1;
}

bool equal(Date date1, Date date2)
{
	return (date1.year == date2.year &&
		date1.month == date2.month &&
		date1.day == date2.day);
}

bool less(Date date1, Date date2)
{
	if (date1.year < date2.year)
		return true;
	if (date1.year > date2.year)
		return false;

	if (date1.month < date2.month)
		return true;
	if (date1.month > date2.month)
		return false;

	if (date1.day < date2.day)
		return true;

	return false;
}

bool lessEqual(Date date1, Date date2)
{
	return less(date1, date2) || equal(date1, date2);
}

Date add(Date date, int numDays)
{
	if (date.month == 1 || date.month == 3 || date.month == 5 || date.month == 7 || date.month == 8 || date.month == 10 || date.month == 12) {
		if (date.month == 12) {
			if (date.day + numDays <= 31) {
				date.day += numDays;
			}
			else {
				date.year += 1;
				date.month = 1;
				date.day += (numDays - 31);//隔年
			}
		}
		else if (date.day + numDays <= 31) {
			date.day += numDays;
		}
		else {
			date.month += 1;
			date.day += (numDays - 31);
		}
	}
	else if (date.month == 2) {
		if (leapYear(date.year)) {
			if (date.day + numDays <= 29) {
				date.day += numDays;
			}
			else {
				date.month += 1;
				date.day += (numDays - 29);
			}
		}
		else {
			if (date.day + numDays <= 28) {
				date.day += numDays;
			}
			else {
				date.month += 1;
				date.day += (numDays - 28);
			}
		}
	}
	else {
		if (date.day + numDays <= 30) {
			date.day += numDays;
		}
		else {
			date.month += 1;
			date.day += (numDays - 30);
		}
	}
	return date;

}

void initAvailTables(vector< AvailTable >& availTables, Date currentDate)
{

	loadAvailTables(availTables, currentDate);


}

void loadAvailTables(vector< AvailTable >& availTables, Date currentDate)
{
	ifstream inFile4("AvailTables1.dat", ios::in | ios::binary);
	if (!inFile4) {
		cout << "File could not be opened" << endl;
		system("pause");
		exit(1);
	}
	AvailTable kk;
	inFile4.seekg(0);
	while (inFile4.read(reinterpret_cast<char*>(&kk), sizeof(AvailTable)))
	{
		if (less(currentDate, kk.date)) {//今天的隔一天才能訂位
			availTables.push_back(kk);
		}
	}
	inFile4.close();
}

void loadReservations(vector< Reservation >& reservations, Date currentDate)
{
	ifstream inFile("Reservations.dat", ios::in | ios::binary);
	if (!inFile) {
		cout << "File could not be opened" << endl;
		system("pause");
		exit(1);
	}
	Reservation kkk;
	inFile.seekg(0);
	while (inFile.read(reinterpret_cast<char*>(&kkk), sizeof(Reservation)))
	{
		if (lessEqual(currentDate, kkk.date)) {//依然可以看到當天的訂位
			reservations.push_back(kkk);
		}
	}
	inFile.close();
}

void makeReservation(vector< Reservation >& reservations, vector< AvailTable >& availTables, Date currentDate)
{
	Reservation newReservation;

	do {
		cout << "\nEnter the number of customers (1 - 7, 10 - 12): ";
		newReservation.numCustomers = inputAnInteger(1, 12);
	} while (newReservation.numCustomers == 8 ||
		newReservation.numCustomers == 9 ||
		newReservation.numCustomers == -1);

	int tableType = tableTypes[newReservation.numCustomers];

	chooseDateTime(availTables, newReservation, currentDate, tableType);

	cout << "\nEnter name: ";
	cin >> newReservation.name;

	cout << "\nEnter mobile number: ";
	cin >> newReservation.mobileNumber;

	cout << "\nEnter email address: ";
	cin >> newReservation.email;

	cout << "\nEnter reservation password: ";
	cin >> newReservation.password;
	cin.ignore();

	newReservation.reservationNumber[0] = 1 + rand() % 9 + '0';
	for (int i = 1; i < 8; i++)
		newReservation.reservationNumber[i] = rand() % 10 + '0';

	cout << setfill(' ') << endl << setw(11) << "Mobile No." << setw(10) << "Name"
		<< setw(14) << "Date" << setw(9) << "Time" << setw(30) << "Email"
		<< setw(19) << "No of Customers" << setw(12) << "Password"
		<< setw(19) << "Reservation No." << endl;

	displayReservationInfo(newReservation);

	cout << "\nReservation Completed!\n";

	reservations.push_back(newReservation);

	decreaseAvailTables(availTables, newReservation.date, newReservation.time, tableType);
}

// choose a date and a time
void chooseDateTime(vector< AvailTable >& availTables, Reservation& reservation, Date currentDate, int tableType)
{
	int date;
	do {
		cout << "\nChoose a date" << endl;
		displayAvailDates(availTables, 0, tableType);
		cout << "\n?";
	} while ((date = inputAnInteger(1, 30)) == -1);
	reservation.date = add(currentDate, date);

	//--------------------------------------------------
	do {
		cout << "\nChoose a time:" << endl;
		for (int i = 1; i <= 3; i++) {
			if (availTables[date - 1].numAvailTables[i][tableType] != 0) {
				if (i == 1) { cout << i << ". 11:30" << endl; }
				else if (i == 2) { cout << i << ". 14:30" << endl; }
				else if (i == 3) { cout << i << ". 17:30" << endl; }
			}
		}cout << "?";
	} while ((reservation.time = inputAnInteger(1, 3)) == -1);

}

void displayAvailDates(vector< AvailTable >& availTables, bool availableDate[], int tableType)
{
	Date kk = add(computeCurrentDate(), 1);
	//Reservations
	availableDate = new bool[30]();
	
	for (int i = 0; i < availTables.size(); i++) {
		if (!availableTables(availTables, i, tableType)) {
			availableDate[i] = 1;
		}
	}
	for (int i = 0; i < 30; i++) {
		if (availableDate[i] == 0) {
			cout << setfill(' ') << setw(2) << i + 1 << "." << "\t";
			cout << kk.year << "/" << setfill('0') << setw(2) << kk.month << "/" << setw(2) << kk.day << "\t";
		}
		if (i % 4 == 3)
			cout << endl;
		kk = add(kk, 1);
	}

}

// returns true if there are available tables on availTables[ i ].date for corresponding table type
bool availableTables(vector< AvailTable >& availTables, size_t i, int tableType)
{
	for (int j = 1; j <= 3; j++) {
		if (availTables[i].numAvailTables[j][tableType] != 0) {//某天的一種座位的所有時段是否有位子
			return true;
		}
	}
	return false;

}

// --availTables[ i ].numAvailTables[ timeCode ][ tableType ], where availTables[ i ].date == date
void decreaseAvailTables(vector< AvailTable >& availTables, Date date, int timeCode, int tableType)
{
	for (int i = 0; i < availTables.size(); i++) {
		//if (availTables[i].date.year == date.year&& availTables[i].date.month == date.month&& availTables[i].date.day == date.day) {
		if (equal(availTables[i].date, date)) {
			availTables[i].numAvailTables[timeCode][tableType]--;
			break;
		}
	}
	saveAvailTables(availTables);
}

void reservationInquiry(vector< Reservation >& reservations, vector< AvailTable >& availTables)
{
	if (reservations.size() == 0)
	{
		cout << "\nNo reservations!\n";
		return;
	}

	cout << "\nEnter reservation number: ";
	char reservationNumber[12];
	cin >> reservationNumber;
	cin.ignore();

	cout << "\nEnter reservation password: ";
	char passwords[12];
	cin >> passwords;
	int position = 0;
	for (int i = 0; i < reservations.size(); i++) {
		if (strcmp(passwords, reservations[i].password) == 0 && strcmp(reservations[i].reservationNumber, reservationNumber) == 0) {
			cout << "\n";
			displayReservationInfo(reservations, reservationNumber);
			position = i;
			char cancel;
			cout << "\nCancel this reservation ( y/n )?" << endl;
			cin >> cancel;
			if (cancel == 'y') {
				for (size_t j = 0; j < availTables.size(); j++) {
					if (equal(availTables[j].date, reservations[i].date))//刪掉的預約要把位子加回去
						availTables[j].numAvailTables[reservations[i].time][tableTypes[reservations[i].numCustomers]]++;
				}
				erase(reservations, position);
				saveReservations(reservations);
				saveAvailTables(availTables);
				cout << "\nThis reservation has been cancelled." << endl;
				return;
			}
			else if (cancel == 'n') {
				return;
			}
		}
		else if (i == reservations.size() - 1) {
			cout << "\nNo reservations with this reservation number!" << endl;
		}
	}
}

// displays all fields of reservation which includes
// mobileNumber, name, date, time, email, numCustomers, password and reservationNumber
void displayReservationInfo(const Reservation& reservation)
{
	char times[4][8] = { "", "11:30", "14:30", "17:30" };
	cout << setfill(' ') << setw(11) << reservation.mobileNumber
		<< setw(10) << reservation.name
		<< "    " << reservation.date.year << "/" << setfill('0')
		<< setw(2) << reservation.date.month << '/'
		<< setw(2) << reservation.date.day
		<< setfill(' ') << setw(9) << times[reservation.time]
		<< setw(30) << reservation.email
		<< setw(19) << reservation.numCustomers
		<< setw(12) << reservation.password
		<< setw(19) << reservation.reservationNumber << endl;
}

// displays reservations[ i ], where
// reservations[ i ].reservationNumber is equal to the specified reservationNumber
void displayReservationInfo(const vector< Reservation >& reservations, char reservationNumber[])
{
	cout << setfill(' ') << setw(11) << "Mobile No." << setw(10) << "Name"
		<< setw(14) << "Date" << setw(9) << "Time" << setw(30) << "Email"
		<< setw(19) << "No of Customers" << setw(12) << "Password"
		<< setw(19) << "Reservation No." << endl;

	for (size_t i = 0; i < reservations.size(); ++i)
		if (strcmp(reservations[i].reservationNumber, reservationNumber) == 0)
			break;

	char times[4][8] = { "", "11:30", "14:30", "17:30" };
	int count = 0;
	for (size_t i = 0; i < reservations.size(); ++i)
		if (strcmp(reservations[i].reservationNumber, reservationNumber) == 0)
			displayReservationInfo(reservations[i]);
}

// erase reservations[ position ]
void erase(vector< Reservation >& reservations, int position)
{

	for (size_t i = position; i < reservations.size() - 1; i++) {
		reservations[i] = reservations[i + 1];
	}
	reservations.pop_back();

}

void saveAvailTables(const vector< AvailTable >& availTables)
{
	ofstream outFileA("AvailTables1.dat", ios::out | ios::binary);
	//outFileA.clear();
	AvailTable ppp;
	outFileA.seekp(0);
	for (size_t i = 0; i < availTables.size(); i++) {
		ppp = availTables[i];
		outFileA.write(reinterpret_cast<char*>(&ppp), sizeof(AvailTable));
	}
	outFileA.close();

}


void saveReservations(const vector< Reservation >& reservations)
{
	ofstream outFileR("Reservations.dat", ios::out | ios::binary);
	Reservation rrr;
	outFileR.seekp(0);
	for (size_t i = 0; i < reservations.size(); i++) {
		rrr = reservations[i];
		outFileR.write(reinterpret_cast<char*>(&rrr), sizeof(Reservation));
		//cout << rrr.reservationNumber;
	}
	outFileR.close();

}