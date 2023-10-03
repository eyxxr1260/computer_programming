#include <iostream>
using namespace::std;
#include <fstream>
#include <string>
#include <ctime>

// input all words from the file Dictionary.txt, and put them to the array "dictionary"
void readDictionary(string* dictionary, int& numDicWords);

// return true if and only if wordToCheck == dictionary[ i ] for some i
bool legal(string wordToCheck, string* dictionary, int numDicWords);

// Find every word w in the array "dictionary" which satisfies two conditions:
// 1. the length of w is equal to the length of wordToCheck, and
// 2. wordToCheck and w are different on exactly one character.
// Put these words into the array "result".
void spellCheck1(string wordToCheck, string* dictionary, int numDicWords, string* result, int& numResult);

// Find every word w in the array "dictionary" which satisfies two conditions:
// 1. the length of w is one greater than the length of wordToCheck, and
// 2. wordToCheck is equal to the substring obtained by deleting a character from w.
// Put these words into the array "result".
void spellCheck2(string wordToCheck, string* dictionary, int numDicWords, string* result, int& numResult);

// Find every word w in the array "dictionary" which satisfies two conditions:
// 1. the length of w is one less than the length of wordToCheck, and
// 2. w is equal to the substring obtained by deleting a character from wordToCheck.
// Put these words into the array "result".
void spellCheck3(string wordToCheck, string* dictionary, int numDicWords, string* result, int& numResult);

// find the words in the array "dictionary" whose length is equal to the last parameter "length",
// and puts these words into the array "words"
void findWords(string*& words, int& numWords, string* dictionary, int numDicWords, size_t length);

// output all words in the array "result" to the file Result.txt
void saveWords(string* result, int numResult);

int main()
{
	srand(1);

	int numDicWords = 0;
	string* dictionary = new string[40000];

	readDictionary(dictionary, numDicWords);

	string wordToCheck;
	int numResult = 0;
	string* result = new string[3000];
	for (int i = 0; i < 10; i++)
	{
		wordToCheck = dictionary[rand() + numDicWords - 32768]; // skip short words
		wordToCheck.erase(rand() % wordToCheck.size(), 1);

		if (legal(wordToCheck, dictionary, numDicWords))
			cout << wordToCheck << " is a legal english word.\n";
		else
		{
			cout << wordToCheck << endl;
			result[numResult++] = wordToCheck;

			spellCheck1(wordToCheck, dictionary, numDicWords, result, numResult);
			spellCheck2(wordToCheck, dictionary, numDicWords, result, numResult);
			spellCheck3(wordToCheck, dictionary, numDicWords, result, numResult);
			result[numResult++] = "";
		}
	}

	saveWords(result, numResult);

	delete[] dictionary;
	delete[] result;

	system("pause");
}

bool legal(string wordToCheck, string* dictionary, int numDicWords)
{
	for (int i = 0; i < numDicWords; i++)
		if (dictionary[i] == wordToCheck)
			return true;
	return false;
}

void readDictionary(string* dictionary, int& numDicWords)
{
	ifstream inFile;
	inFile.open("Dictionary.txt", ios::in | ios::binary);

	// exit program if ifstream could not open file
	if (!inFile)
	{
		cout << "File could not be opened" << endl;
		system("pause");
		exit(1);
	}
	numDicWords = 40000;
	for (int j = 0; j < 40000; j++) {
		inFile >> dictionary[j];
		if (dictionary[j] == "\0") {
			numDicWords--;
		}
	}
	inFile.close();
	//for (int j = 0; j < numDicWords; j++) {
	//	cout << dictionary[j] << endl;
	//}

}

void spellCheck1(string wordToCheck, string* dictionary,
	int numDicWords, string* result, int& numResult)
{
	size_t length = wordToCheck.size();
	int numWords = 0;
	string* words = 0;

	findWords(words, numWords, dictionary, numDicWords, length);

	string buffer1;
	string buffer2;

	for (int i = 0; i < numWords; i++) {
		int n = 0;
		for (size_t j = 0; j < length; j++)
		{
			buffer1.push_back(words[i][j]);
			buffer2.push_back(wordToCheck[j]);
			if (buffer1[j] != buffer2[j]) {
				n++;
			}
		}

		if (n == 1) {
			for (size_t k = 0; k < length; k++) {
				result[numResult].push_back(buffer1[k]);
				cout << result[numResult][k];
			}
			cout << endl;
			numResult++;
		}
		buffer1.erase();
		buffer2.erase();
	}
	delete[] words;
}

void spellCheck2(string wordToCheck, string* dictionary, int numDicWords, string* result, int& numResult)
{
	size_t length = wordToCheck.size() + 1;
	int numWords = 0;
	string* words = 0;

	findWords(words, numWords, dictionary, numDicWords, length);

	string buffer;
	for (int i = 0; i < numWords; i++) {
		int n = 0;
		for (size_t j = 0; j < length; j++)
		{
			buffer.push_back(words[i][j]);
		}
		for (size_t n = 0; n < length; n++) {
			int count = 0;

			buffer.erase(n, 1);//刪除單一字母
			if (buffer == wordToCheck) {
				count++;
			}
			buffer = words[i];
			if (count == 1) {
				for (size_t k = 0; k < length; k++) {
					result[numResult].push_back(buffer[k]);
					//cout << result[numResult][k];
				}
				//cout << endl;
				numResult++;
				break;//有一個result就輸出並跳出迴圈
			}
		}
		buffer.erase();
	}
	delete[] words;
}

void spellCheck3(string wordToCheck, string* dictionary, int numDicWords, string* result, int& numResult)
{
	size_t length = wordToCheck.size() - 1;
	int numWords = 0;
	string* words = 0;

	findWords(words, numWords, dictionary, numDicWords, length);

	string buffer;
	for (int i = 0; i < numWords; i++) {
		for (size_t j = 0; j < length; j++)
		{
			buffer.push_back(words[i][j]);
		}
		for (size_t n = 0; n < length; n++) {
			int count = 0;

			buffer.insert(n, " ");//依次個別放入空格
			for (size_t j = 0; j < wordToCheck.size(); j++) {
				if (buffer[j] == wordToCheck[j]) {
					count++;
				}
			}
			buffer = words[i];
			if (count == length) {
				for (size_t k = 0; k < length; k++) {
					result[numResult].push_back(buffer[k]);
					cout << result[numResult][k];
				}
				cout << endl;
				numResult++;
				break;
			}
		}
		buffer.erase();
	}
	delete[] words;
}

void findWords(string*& words, int& numWords, string* dictionary,
	int numDicWords, size_t length)
{
	for (int i = 0; i < numDicWords; i++)
		if (dictionary[i].size() == length)
			numWords++;

	words = new string[numWords];

	numWords = 0;
	for (int i = 0; i < numDicWords; i++)
		if (dictionary[i].size() == length)
			words[numWords++] = dictionary[i];
}

void saveWords(string* result, int numResult)
{
	ofstream outFile("Result.txt", ios::out);
	if (!outFile)
	{
		cout << "File could not be opened" << endl;
		exit(1);
	}
	for (int i = 0; i < numResult; i++) {
		outFile << result[i] << endl;
	}

	outFile.close();

}