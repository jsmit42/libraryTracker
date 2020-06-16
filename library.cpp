#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;

struct Book {
    string authorSurname;
    string authorGiven;
    string title;
    bool inSeries;
    string series;
    int placeInSeries;
};

bool operator == (const Book & lhs, const Book & rhs) 
{
    if(lhs.authorGiven == rhs.authorGiven &&
       lhs.authorSurname == rhs.authorSurname &&
       lhs.title == rhs.title)
       return true;
    return false;
}

bool operator != (const Book & lhs, const Book & rhs) 
{
    return !(lhs == rhs);
}

bool operator < (const Book & lhs, const Book & rhs)
{
    if (lhs.authorSurname != rhs.authorSurname)
        return lhs.authorSurname < rhs.authorSurname;
    if (lhs.authorGiven != rhs.authorGiven)
        return lhs.authorGiven < rhs.authorGiven;
    
    if (lhs.inSeries && rhs.inSeries)
    {
        if (lhs.series != rhs.series)
            return lhs.series < rhs.series;
        return lhs.placeInSeries < rhs.placeInSeries;
    }
    else if (lhs.inSeries || rhs.inSeries)
        return lhs.inSeries < rhs.inSeries;
    else
        return lhs.title < rhs.title;
}

Book noBook;

vector<Book> library;

/*
std::string split implementation by using delimiter as a character.
*/
std::vector<std::string> split(std::string strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings;
}

string getBook(Book entry) {
    string fullEntry = entry.authorSurname;
    fullEntry += ", ";
    fullEntry += entry.authorGiven;
    fullEntry += ", ";
    fullEntry += entry.title;
    if (entry.inSeries) {
        fullEntry += ", ";
        fullEntry += entry.series;
        fullEntry += " ";
        fullEntry += to_string(entry.placeInSeries);
    }
    return fullEntry;
}

void displayBook(Book entry) {
    cout << getBook(entry) << endl;
}

Book enterBook() {
    Book newBook;
    char confirm;
    
    do {
    //Get Author
    cout << "Author Surname: ";
    cin >> newBook.authorSurname;

    cout << "Author Given Names: ";

    cin.ignore();
    getline(cin, newBook.authorGiven);

    //Get title
    cout << "Title: ";
    
    getline(cin, newBook.title);

    //Get series
    cout << "Series Placement: ";
    cin >> newBook.placeInSeries;
    newBook.inSeries = newBook.placeInSeries;

    if (newBook.inSeries) {
        cout << "Series Name: ";
        cin.ignore();
        getline(cin, newBook.series);
    }

    vector<Book>::iterator it;
    it = find(library.begin(), library.end(), newBook);
    if (it != library.end()){
        displayBook(newBook);
        cout << "is already saved in the library\n";
        return noBook;
    }
    cout << "About to save:\n";
    displayBook(newBook);
    cout << "Are you sure (y/n): ";
    cin >> confirm;
    if (confirm == 'n')
    {
        cout << "try again? (y/n): ";
        cin >> confirm;
        if (confirm == 'n')
            return noBook;
        confirm++;    
    }

    } while (confirm != 'y');

    return newBook;
}

void writeToHTML() {
    ofstream htmlOut("./library/book_List_HTML_Format.txt");

    if (htmlOut.fail())
        cout << "ERROR: Could not find ./library/book_List_HTML_Format.txt\n";
    assert(!htmlOut.fail());

    htmlOut << endl;

    //SORT VECTOR HERE
    sort(library.begin(), library.end());

    for (auto it = library.begin(); it < library.end(); it++) {
        htmlOut << "<li> " << getBook(*it) << " </li>" << endl;
    }
}

void writeToSave() {
    //clear file
    std::ofstream ofs;
    ofs.open("./library/library.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    ofstream saveOut ("./library/library.txt");
    if (saveOut.fail())
        cout << "ERROR: Could not find ./library/library.txt\n";
    assert(!saveOut.fail());

    //SORT VECTOR HERE
    sort(library.begin(), library.end());

    for(auto it = library.begin(); it < library.end(); it++) {
        saveOut << it->authorSurname << "|" << it->authorGiven << "|"
               << it->title;
       if(it->inSeries) 
           saveOut << "|" << it->series << "|" << it->placeInSeries;
        saveOut << endl;
   }
}

void readSaved() {
    cout << "Reading Saved Data\n";
    ifstream savedIn("./library/library.txt");
    if(savedIn.fail()) {
        cout << "No saved data found.\n";
        return;
    }

    cout << "Found ";
    bool stop = 0;
    while(!savedIn.eof()) {
        string savedEntry;
        Book entry;
        vector<string> entryVector;
        getline(savedIn, savedEntry);

        entryVector = split(savedEntry, '|');
        
        stop = entryVector.empty();
        if (!stop){
        entry.authorSurname = entryVector[0];
        entry.authorGiven   = entryVector[1];
        entry.title         = entryVector[2];

        if (entryVector.size() > 3) {
            entry.inSeries = 1;
            entry.series = entryVector[3];
            entry.placeInSeries = stoi(entryVector[4]);
        }
        else 
            entry.inSeries = 0;

        library.push_back(entry);
        }
    }
    cout << library.size() << " books.\n";
}

void displayPrompt() {
    cout << "Enter N to enter new book.\n"
         << "Enter W to write data to HTML format.\n"
         << "Enter Q to save and quit.\n"
         << "Enter H to display instructions again.\n";
}

int main()
{
    noBook.authorSurname = "NULL";
    noBook.authorGiven   = "NULL";
    noBook.title         = "NULL";
    readSaved();
    char prompt;
    displayPrompt(); 
    do
    {
        cout << ">" ;
        cin >> prompt;
        prompt = toupper(prompt);
        Book newBook;
        switch (prompt)
        {
        case 'N':
            cout << endl;
            newBook = enterBook();
            if (newBook != noBook)
                library.push_back(newBook);
            cout << endl;
            displayPrompt();
            break;
        case 'W':
            writeToHTML();
            break;
        case 'H':
            displayPrompt();
            break;
        case 'Q':
            writeToSave();
            writeToHTML();
            break;
        default:
            cout << "Invalid Input\n";
            break;
        }
    }while(prompt != 'Q');

    return 0;
}
