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
//#include <ctime>

#include <vector>
using std::vector;

struct Date
{
    int year = 0;
    int month = 0;
    int day = 0;
};

struct AvailRoom
{
    Date date;
    int numAvailRooms[ 6 ] = {}; // number of available rooms
    // numAvailRooms[ 0 ]: not used
    // numAvailRooms[ 1 ]: the number of available superior rooms
    // numAvailRooms[ 2 ]: the number of available deluxe rooms
    // numAvailRooms[ 3 ]: the number of available deluxe twin rooms
    // numAvailRooms[ 4 ]: the number of available superior suites
    // numAvailRooms[ 5 ]: the number of available deluxe suites
};

struct Reservation
{
    char id[ 12 ] = "";
    char name[ 12 ] = "";
    char mobile[ 12 ] = "";
    int roomType = 0; // 1: superior rooms, 2: deluxe rooms, 3: deluxe twin rooms,
                      // 4: superior suites, 5: deluxe suites
    int numRooms = 0; // number of rooms reserved
    Date checkInDate;
    Date checkOutDate;
};

const int roomRate[ 6 ] = { 0, 5390, 6270, 6270, 6820, 8470 }; // room rate per night for each room type
const int totalNumRooms[ 6 ] = { 0, 9, 5, 3, 5, 4 }; // total number of rooms for each type
const char roomTypeName[ 6 ][ 20 ] = { "", "Superior Room", "Deluxe Room", "Deluxe Twin Room",
                                           "Superior Suite", "Deluxe Suite" };
// return the current date
Date computeCurrentDate();

// return true if and only if year is a leap year
inline bool leapYear( int year );

// return true if and only if date1 == date2
bool equal( Date date1, Date date2 );

// return true if and only if date1 < date2
bool less( const Date &date1, const Date &date2 );

// return true if and only if date1 <= date2
bool lessEqual( Date date1, Date date2 );

// read reservations information whose checkInDate are after or equal to currentDate,
// from the file Reservations.dat
void loadReservations( vector< Reservation > &reservations, const Date &currentDate );

// load all available rooms information after or equal to currentDate,
// from the file Available Rooms.dat
void loadAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate );

// create available rooms information for 6 months, starting from currentDate
// provided that availRooms.size() == 0
void initAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate );

// append new available rooms information, provided that availRooms.size() != 0
void completeAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate );

// input an integer in the range [ begin, end ]
int inputAnInteger( int begin, int end );

void makeReservation( vector< AvailRoom > &availRooms, vector< Reservation > &reservations, const Date &currentDate );

// input check in date and check out date by user, and
// put them into reservation.checkInDate and reservation.checkOutDate, respectively.
void inputDates( const Date &currentDate, Reservation &newReservation );

// return the minimum between availRooms[ first .. last ].numAvailRooms[ roomType ],
// where availRooms[ first ].date == reservation.checkInDate and
// availRooms[ last ].date == reservation.checkOutDate.
int compMinNumRooms( const Reservation &reservation, vector< AvailRoom > &availRooms, int roomType );

void displayAvailRooms( const AvailRoom &availableRoom );

void displayReservations( const Reservation &reservation );

// return the number of nights for reservation
int numNights( Reservation reservation );

void reservationInquiry( const vector< Reservation > &reservations, const Date &currentDate );

bool exist( const vector< Reservation > &reservations, const Date &currentDate, char id[] );

// save all available rooms information into the file Available Rooms.dat
void saveAvailRooms( const vector< AvailRoom > &availRooms );

// save all reservations information into the file Reservations.dat
void saveReservations( const vector< Reservation > &reservations );

int main()
{
    cout << "Evergreen Laurel Hotel Online Reservation System\n";

    srand( static_cast< unsigned int >( time( 0 ) ) );
    Date currentDate = computeCurrentDate();

    vector< Reservation > reservations;
    loadReservations( reservations, currentDate ); // load all reservation informations

    vector< AvailRoom > availRooms;
    loadAvailRooms( availRooms, currentDate ); // load all available rooms information

 //   for( size_t i = 0; i < availRooms.size(); i++ )
 //      displayAvailRooms( availRooms[ i ] );

    if( availRooms.size() == 0 )
        initAvailRooms( availRooms, currentDate ); // create available rooms information for 6 months
    else
        completeAvailRooms( availRooms, currentDate ); // append new available rooms information

    bool userExited = false; // user has not chosen to exit

    // loop while user has not chosen option to exit system
    while( !userExited )
    {
        // show main menu and get user selection
        cout << "\n1 - Making Reservation\n";
        cout << "2 - Reservation Inquiry\n";
        cout << "3 - End\n\n";

        int mainMenuSelection;
        do cout << "Enter your choice: (1~3): ";
        while( ( mainMenuSelection = inputAnInteger( 1, 3 ) ) == -1 );

        Date temp;
        // decide how to proceed based on user's menu selection
        switch( mainMenuSelection )
        {
        case 1:
            makeReservation( availRooms, reservations, currentDate ); // make reservation
            break;

        case 2:
            reservationInquiry( reservations, currentDate ); // reservation inquiry
            break;

        case 3: // user chose to terminate session
            saveAvailRooms( availRooms );
            saveReservations( reservations );
            userExited = true; // this session should end
            cout << endl;
            break;

        default: // user did not enter an integer from 1-3
            cout << "\nIncorrect choice!" << endl;
        }
    }

    system( "pause" );
}

Date computeCurrentDate()
{
    tm structuredTime;
    time_t rawTime = time( 0 );
    localtime_s( &structuredTime, &rawTime );

    Date currentDate;
    currentDate.year = structuredTime.tm_year + 1900;
    currentDate.month = structuredTime.tm_mon + 1;
    currentDate.day = structuredTime.tm_mday;

    return currentDate;
}

inline bool leapYear( int year )
{
    return ( year % 400 == 0 || ( year % 4 == 0 && year % 100 != 0 ) );
}

bool equal( Date date1, Date date2 )
{
    return ( date1.year == date2.year &&
        date1.month == date2.month &&
        date1.day == date2.day );
}

bool less( const Date &date1, const Date &date2 )
{
    if( date1.year < date2.year )
        return true;
    if( date1.year > date2.year )
        return false;

    if( date1.month < date2.month )
        return true;
    if( date1.month > date2.month )
        return false;

    if( date1.day < date2.day )
        return true;
    else
        return false;
}

bool lessEqual( Date date1, Date date2 )
{
    return less( date1, date2 ) || equal( date1, date2 );
}

void loadReservations( vector< Reservation > &reservations, const Date &currentDate )
{
    ifstream inFileR( "Reservations.dat", ios::in | ios::binary );
    if( !inFileR ) {
        cout << "inFileR unopened";
        system( "pause" );
        exit( 1 );
    }
    Reservation bufferR;
    inFileR.seekg( 0 );
    while( inFileR.read( reinterpret_cast< char* >( &bufferR ), sizeof( Reservation ) ) ) {
        if( less( currentDate, bufferR.checkInDate ) ) {
            reservations.push_back( bufferR );
        }
    }
    inFileR.close();
}

void loadAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate )
{
    ifstream inFileA( "Available Rooms blank.dat", ios::in | ios::binary );
    if( !inFileA ) {
        cout << "inFileA unopened";
        system( "pause" );
        exit( 1 );
    }
    AvailRoom bufferA;
    inFileA.seekg( 0 );
    //int i = 0;
    while( inFileA.read( reinterpret_cast< char* >( &bufferA ), sizeof( AvailRoom ) ) ) {
        if( lessEqual( currentDate, bufferA.date ) ) {
            availRooms.push_back( bufferA );
        }
    }
    inFileA.close();
}

void initAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate )
{
    // number of days in each month
    int days[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if( leapYear( currentDate.year ) ) {
        days[ 2 ] = 29;
    }
    AvailRoom Abuffer;
    for( int i = 0; i < 6; i++ ) {
        Abuffer.numAvailRooms[ i ] = totalNumRooms[ i ];
    }
    Abuffer.date.year = currentDate.year;

    for( int i = currentDate.month; i < currentDate.month + 6; i++ ) {
        Abuffer.date.month = i;
        if( i == currentDate.month ) {
            for( int k = currentDate.day; k <= days[ currentDate.month ]; k++ ) {
                Abuffer.date.day = k;
                availRooms.push_back( Abuffer );
            }
        }
        else if( i > 12 ) {
            if( i % 12 != 0 ) {
                Abuffer.date.month = i % 12;
                for( int j = 1; j <= days[ i % 12 ]; j++ ) {
                    Abuffer.date.day = j;
                    availRooms.push_back( Abuffer );
                }
            }
            else {
                Abuffer.date.month = 12;
                for( int j = 1; j <= days[ 12 ]; j++ ) {
                    Abuffer.date.day = j;
                    availRooms.push_back( Abuffer );
                }
            }
        }
        else {
            for( int j = 1; j <= days[ i ]; j++ ) {
                Abuffer.date.day = j;
                availRooms.push_back( Abuffer );
            }
        }

    }

}

void completeAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate )
{
    // number of days in each month
    int days[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if( leapYear( currentDate.year ) ) {
        days[ 2 ] = 29;
    }
    AvailRoom Abuffer2;
    for( int i = 0; i < 6; i++ ) {
        Abuffer2.numAvailRooms[ i ] = totalNumRooms[ i ];
    }
    Abuffer2.date.year = currentDate.year;
    int beginMonth = availRooms[ availRooms.size() - 1 ].date.month;

    for( size_t i = beginMonth + 1; i < currentDate.month + 6; i++ ) {
        Abuffer2.date.month = i;
        if( i > 12 ) {
            if( i % 12 != 0 ) {
                Abuffer2.date.month = i % 12;
                for( int j = 1; j <= days[ i % 12 ]; j++ ) {
                    Abuffer2.date.day = j;
                    availRooms.push_back( Abuffer2 );
                }
            }
            else {
                Abuffer2.date.month = 12;
                for( int j = 1; j <= days[ 12 ]; j++ ) {
                    Abuffer2.date.day = j;
                    availRooms.push_back( Abuffer2 );
                }
            }
        }
        else {
            for( int j = 1; j <= days[ i ]; j++ ) {
                Abuffer2.date.day = j;
                availRooms.push_back( Abuffer2 );
            }
        }
    }
}

int inputAnInteger( int begin, int end )
{
    char string[ 80 ];
    cin.getline( string, 80, '\n' );

    if( strlen( string ) == 0 )
        exit( 0 );

    for( unsigned int i = 0; i < strlen( string ); i++ )
        if( string[ i ] < '0' || string[ i ] > '9' )
            return -1;

    int number = atoi( string );

    if( number >= begin && number <= end )
        return number;
    else
        return -1;
}

void makeReservation( vector< AvailRoom > &availRooms, vector< Reservation > &reservations, const Date &currentDate )
{
    Reservation newReservation;

    // input check in date and check out date by user, and
    // put them into reservation.checkInDate and reservation.checkOutDate, respectively.
    inputDates( currentDate, newReservation );

    cout << "\nCheck in date: " << newReservation.checkInDate.year << '-'
        << setw( 2 ) << setfill( '0' ) << newReservation.checkInDate.month << '-'
        << setw( 2 ) << newReservation.checkInDate.day << endl;

    cout << "Check out date: " << newReservation.checkOutDate.year << '-'
        << setw( 2 ) << newReservation.checkOutDate.month << '-'
        << setw( 2 ) << newReservation.checkOutDate.day << endl;


    // input room type and put it into newReservation.roomType
    
    //modify-----------------------start---------------------------
    int from = 0;
    bool cannotuse[6] = { };
    for( int j = 0; j < availRooms.size(); j++ ) {
        if( equal( availRooms[ j ].date, newReservation.checkInDate ) ) {
            from = j;
            break;
        }
    }
    for (int n = from; n < from + numNights(newReservation); n++) {
        for (int k = 1; k <= 5; k++) {
            if (availRooms[n].numAvailRooms[k] == 0) {
                cannotuse[k] = true;
            }
        }
    }
    //modify-----------------------end-----------------------------
    //add--------------------------start---------------------------
    int count = 0;
    for (int i = 1; i < 6; i++) {
        if (cannotuse[i] == true) {
            count++;
        }
    }
    if (count == 5) {
        cout << "\nThere are no rooms available!\n";
        return;
    }
    //add--------------------------end-----------------------------
    cout << "\nThe number of available rooms of each room type:\n";
    cout << setw( 10 ) << setfill( ' ' ) << "Date" << setfill( ' ' ) << setw( 20 ) << "Superior Room"
        << setfill( ' ' ) << setw( 20 ) << "Deluxe Room" << setfill( ' ' ) << setw( 20 ) << "Deluxe Twin Room"
        << setfill( ' ' ) << setw( 20 ) << "Superior Suite" << setfill( ' ' ) << setw( 20 ) << "Deluxe Suite" << endl;
    //modify(僅改變順序,未更改內容)

    for( int i = from; i < from + numNights( newReservation ); i++ ) {
        cout << availRooms[ i ].date.year << "-" << setw( 2 ) << setfill( '0' )
            << availRooms[ i ].date.month << "-" << setw( 2 ) << setfill( '0' )
            << availRooms[ i ].date.day;
        for( int j = 1; j <= 5; j++ ) {
            cout << setw( 20 ) << setfill( ' ' ) << availRooms[ i ].numAvailRooms[ j ];
        }
        cout << endl;
    }
    int k = numNights( newReservation );
    
    do {
        cout << "\nSelect Room Type：\n";
        //modify(僅改變順序,未更改內容)
        for( int m = 1; m <= 5; m++ ) {
            if( cannotuse[ m ] == false ) {
                cout << m << ". " << roomTypeName[ m ] << endl;
            }
        }
        cout << "6. Give up\n" << "?";

    } while( ( newReservation.roomType = inputAnInteger( 1, 6 ) ) == -1 || cannotuse[ newReservation.roomType ] == true );
    if( newReservation.roomType == 6 ) {
        return;
    }


    // input number of rooms and put it into newReservation.numRooms
    int min = compMinNumRooms( newReservation, availRooms, newReservation.roomType );
    do {
        cout << "\nNumber of rooms (" << min << " rooms available ):";
    } while( ( newReservation.numRooms = inputAnInteger( 1, min ) ) == -1 );
    for( int p = from; p < from + numNights( newReservation ); p++ ) {
        availRooms[ p ].numAvailRooms[ newReservation.roomType ] -= newReservation.numRooms;
    }
    saveAvailRooms( availRooms );


    cout << "\nID Number：";
    cin >> newReservation.id;
    cout << "\nName：";
    cin >> newReservation.name;
    cout << "\nMobile Phone：";
    cin >> newReservation.mobile;
    cin.ignore();

    cout << "\nYour reservations:\n\n";
    cout << "    Name   Fare       Mobile          Room type"
        << "   Number of rooms   Check in date   Check out date\n";
    displayReservations( newReservation );

    reservations.push_back( newReservation );
}

void inputDates( const Date &currentDate, Reservation &newReservation )
{
    int chooseInMonth;
    bool nextYear = true;
    do {
        cout << "\nPlease Input Check In Date\n";
        cout << "\nMonth:\n";
        for( size_t i = 0; i < 6; i++ ) {
            if( currentDate.month + i <= 12 ) {
                cout << i + 1 << "." << "\t" << currentDate.year << "-" << setfill( '0' ) << setw( 2 ) << currentDate.month + i << endl;
                nextYear = false;
            }
            else {
                cout << i + 1 << "." << "\t" << currentDate.year + 1 << "-" << setfill( '0' ) << setw( 2 ) << ( currentDate.month + i ) - 12 << endl;
            }
        }
        cout << "?";
    } while( ( chooseInMonth = inputAnInteger( 1, 6 ) ) == -1 );
    if( nextYear == false ) {
        newReservation.checkInDate.year = currentDate.year;
        newReservation.checkInDate.month = currentDate.month + chooseInMonth - 1;
    }
    else {
        newReservation.checkInDate.year = currentDate.year + 1;
        newReservation.checkInDate.month = currentDate.month + chooseInMonth - 13;
    }

    //---------------------------------------------------------
    int chooseInDay;
    int begin = 1;
    int end;
    do {
        if( newReservation.checkInDate.month == currentDate.month ) {
            while( begin < currentDate.day ) {
                begin++;
            }
        }
        if( newReservation.checkInDate.month == 1 || newReservation.checkInDate.month == 3 || newReservation.checkInDate.month == 5 || newReservation.checkInDate.month == 7
            || newReservation.checkInDate.month == 8 || newReservation.checkInDate.month == 10 || newReservation.checkInDate.month == 12 ) {
            end = 31;
        }
        else if( newReservation.checkInDate.month == 2 ) {
            if( leapYear( currentDate.year ) ) {
                end = 29;
            }
            else {
                end = 28;
            }
        }
        else {
            end = 30;
        }

        if( ( currentDate.month + 5 ) % 12 != 0 ) {
            if( newReservation.checkInDate.month == ( currentDate.month + 5 ) % 12 ) {
                end -= 1;
            }
        }
        else {
            if( newReservation.checkInDate.month == ( currentDate.month + 5 ) % 12 + 12 ) {
                end -= 1;
            }
        }

        cout << "\nDay (" << begin << " ~ " << end << "): ";
    } while( ( chooseInDay = inputAnInteger( begin, end ) ) == -1 );
    newReservation.checkInDate.day = chooseInDay;

    //===============================================================================================
    int chooseOutMonth;
    bool nextYear2 = true;
    int days[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if( leapYear( currentDate.year ) ) {
        days[ 2 ] = 29;
    }
    do {
        cout << "\nPlease Input Check Out Date\n";
        cout << "\nMonth:\n";
        for( size_t i = 0; i < 6; i++ ) {
            if( chooseInMonth <= i + 1 ) {
                if( currentDate.month + i <= 12 ) {
                    nextYear2 = false;
                    if( chooseInDay == days[ currentDate.month + i ] && chooseInMonth == currentDate.month + i ) {
                        ;
                    }
                    else {
                        cout << i + 1 << "." << "\t" << currentDate.year << "-" << setfill( '0' ) << setw( 2 ) << currentDate.month + i << endl;
                    }
                }
                else {
                    if( chooseInDay == days[ currentDate.month + i - 12 ] && chooseInMonth == currentDate.month + i - 12 ) {
                        ;
                    }
                    else {
                        cout << i + 1 << "." << "\t" << currentDate.year + 1 << "-" << setfill( '0' ) << setw( 2 ) << currentDate.month + i - 12 << endl;
                    }
                }
            }
        }
        cout << "?";
    } while( ( chooseOutMonth = inputAnInteger( chooseInMonth, 6 ) ) == -1 );
    if( nextYear2 == false ) {
        newReservation.checkOutDate.year = currentDate.year;
        newReservation.checkOutDate.month = currentDate.month + chooseOutMonth - 1;
    }
    else {
        newReservation.checkOutDate.year = currentDate.year + 1;
        newReservation.checkOutDate.month = currentDate.month + chooseOutMonth - 13;
    }

    //----------------------------------------------------------
    int chooseOutDay;
    int beginO = 1;
    int endO;
    do {
        if( newReservation.checkOutDate.month == newReservation.checkInDate.month ) {
            beginO = chooseInDay + 1;
        }
        if( newReservation.checkOutDate.month == 1 || newReservation.checkOutDate.month == 3 || newReservation.checkOutDate.month == 5 || newReservation.checkOutDate.month == 7
            || newReservation.checkOutDate.month == 8 || newReservation.checkOutDate.month == 10 || newReservation.checkOutDate.month == 12 ) {
            endO = 31;
        }
        else if( newReservation.checkOutDate.month == 2 ) {
            if( leapYear( currentDate.year ) ) {
                endO = 29;
            }
            else {
                endO = 28;
            }
        }
        else {
            endO = 30;
        }

        cout << "\nDay (" << beginO << " ~ " << endO << "): ";
    } while( ( chooseOutDay = inputAnInteger( beginO, endO ) ) == -1 );
    newReservation.checkOutDate.day = chooseOutDay;
}

int compMinNumRooms( const Reservation &reservation, vector< AvailRoom > &availRooms, int roomType )
{
    int tmp;
    int begin = 0;
    for( int i = 0; i < availRooms.size(); i++ ) {
        if( equal( reservation.checkInDate, availRooms[ i ].date ) ) {
            begin = i;
            break;
        }
    }
    tmp = availRooms[ begin ].numAvailRooms[ roomType ];
    for( int j = begin; j < begin + numNights( reservation ) - 1; j++ ) {//modify
        if( tmp > availRooms[ j + 1 ].numAvailRooms[ roomType ] ) {
            tmp = availRooms[ j + 1 ].numAvailRooms[ roomType ];
        }
    }
    return tmp;

}

void displayAvailRooms( const AvailRoom &availableRoom )
{
    cout << availableRoom.date.year << "-"
        << setw( 2 ) << setfill( '0' ) << availableRoom.date.month << "-"
        << setw( 2 ) << availableRoom.date.day
        << setw( 16 ) << setfill( ' ' ) << availableRoom.numAvailRooms[ 1 ]
        << setw( 14 ) << availableRoom.numAvailRooms[ 2 ]
        << setw( 19 ) << availableRoom.numAvailRooms[ 3 ]
        << setw( 17 ) << availableRoom.numAvailRooms[ 4 ]
        << setw( 15 ) << availableRoom.numAvailRooms[ 5 ] << endl;
}

void displayReservations( const Reservation &reservation )
{
    cout << setw( 8 ) << setfill( ' ' ) << reservation.name
        << setw( 7 ) << reservation.numRooms * roomRate[ reservation.roomType ] * numNights( reservation )
        << setw( 13 ) << reservation.mobile
        << setw( 19 ) << roomTypeName[ reservation.roomType ]
        << setw( 18 ) << reservation.numRooms
        << setw( 10 ) << reservation.checkInDate.year << "-"
        << setw( 2 ) << setfill( '0' ) << reservation.checkInDate.month << "-"
        << setw( 2 ) << reservation.checkInDate.day
        << setw( 11 ) << setfill( ' ' ) << reservation.checkOutDate.year << "-"
        << setw( 2 ) << setfill( '0' ) << reservation.checkOutDate.month << "-"
        << setw( 2 ) << reservation.checkOutDate.day << endl;
}

int numNights( Reservation reservation )
{
    tm time1 = { 0, 0, 8, 0, 0, 0, 0, 0, 0 };
    tm time2 = { 0, 0, 8, 0, 0, 0, 0, 0, 0 };
    time1.tm_year = reservation.checkInDate.year - 1900;
    time1.tm_mon = reservation.checkInDate.month - 1;
    time1.tm_mday = reservation.checkInDate.day;
    time2.tm_year = reservation.checkOutDate.year - 1900;
    time2.tm_mon = reservation.checkOutDate.month - 1;
    time2.tm_mday = reservation.checkOutDate.day;
    int numSeconds = static_cast< int >( mktime( &time2 ) - mktime( &time1 ) );
    return numSeconds / ( 24 * 3600 );
}

void reservationInquiry( const vector< Reservation > &reservations, const Date &currentDate )
{
    cout << "\nEnter Your ID Number：";
    char id[ 12 ];
    cin >> id;
    cin.ignore();

    if( !exist( reservations, currentDate, id ) )
    {
        cout << "\nSorry! You do not have reservations!\n";
        return;
    }

    cout << "\n    Name   Fare       Mobile          Room type"
        << "   Number of rooms   Check in date   Check out date\n";
    for( size_t i = 0; i < reservations.size(); i++ )
        if( strcmp( reservations[ i ].id, id ) == 0 )
            displayReservations( reservations[ i ] );
}

bool exist( const vector< Reservation > &reservations, const Date &currentDate, char id[] )
{
    for( size_t i = 0; i < reservations.size(); i++ )
        if( strcmp( reservations[ i ].id, id ) == 0 &&
            lessEqual( currentDate, reservations[ i ].checkInDate ) )
            return true;

    return false;
}

void saveAvailRooms( const vector< AvailRoom > &availRooms )
{
    ofstream outFileA( "Available Rooms blank.dat", ios::out | ios::binary );
    if( !outFileA ) {
        cout << "outFileA unopened";
        system( "pause" );
        exit( 1 );
    }
    AvailRoom writeA;
    outFileA.seekp( 0 );
    for( size_t i = 0; i < availRooms.size(); i++ ) {
        writeA = availRooms[ i ];
        outFileA.write( reinterpret_cast< char* >( &writeA ), sizeof( AvailRoom ) );
    }
    outFileA.close();
}

void saveReservations( const vector< Reservation > &reservations )
{
    ofstream outFileR( "Reservations.dat", ios::out | ios::binary );
    if( !outFileR ) {
        cout << "outFileR unopened";
        system( "pause" );
        exit( 1 );
    }
    Reservation writeR;
    outFileR.seekp( 0 );
    for( size_t i = 0; i < reservations.size(); i++ ) {
        writeR = reservations[ i ];
        outFileR.write( reinterpret_cast< char* >( &writeR ), sizeof( Reservation ) );
    }
    outFileR.close();
}