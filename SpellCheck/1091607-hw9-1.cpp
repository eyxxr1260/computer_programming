#include <iostream>
using namespace::std;
#include <fstream>
#include <cstring>

// input all words from the file Dictionary.txt, and put them to the array "dictionary"
void readDictionary(char(*dictionary)[21], int& numDicWords);

// return true if and only if wordToCheck == dictionary[ i ] for some i
bool legal(char* wordToCheck, char(*dictionary)[21], int numDicWords);

// Find every word w in the array "dictionary" which satisfies two conditions:
// 1. the length of w is equal to the length of wordToCheck, and
// 2. wordToCheck and w are different on exactly one character.
// Put these words into the array "result".
void spellCheck1(char* wordToCheck, char(*dictionary)[21], int numDicWords, char(*&result)[21], int& numResult);

// Find every word w in the array "dictionary" which satisfies two conditions:
// 1. the length of w is one greater than the length of wordToCheck, and
// 2. wordToCheck is equal to the substring obtained by deleting a character from w.
// Put these words into the array "result".
void spellCheck2(char* wordToCheck, char(*dictionary)[21], int numDicWords, char(*&result)[21], int& numResult);

// Find every word w in the array "dictionary" which satisfies two conditions:
// 1. the length of w is one less than the length of wordToCheck, and
// 2. w is equal to the substring obtained by deleting a character from wordToCheck.
// Put these words into the array "result".
void spellCheck3(char* wordToCheck, char(*dictionary)[21], int numDicWords, char(*&result)[21], int& numResult);

// find the words in the array "dictionary" whose length is equal to the last parameter "length",
// and puts these words into the array "words"
void findWords(char(*&words)[21], int& numWords, char(*dictionary)[21], int numDicWords, int length);

// output all words in the array "result" to the file Result.txt
void saveWords(char(*result)[21], int numResult);

int main()
{
	srand(1);

	int numDicWords = 0;
	char(*dictionary)[21] = new char[40000][21];

	readDictionary(dictionary, numDicWords);

	char wordToCheck[21];
	int numResult = 0;
	char(*result)[21] = new char[3000][21];
	for (int i = 0; i < 10; i++)
	{
		strcpy_s(wordToCheck, 21, dictionary[rand() + numDicWords - 32768]);
		int length = strlen(wordToCheck);
		for (int j = rand() % length; j < length; j++)
			wordToCheck[j] = wordToCheck[j + 1];

		if (legal(wordToCheck, dictionary, numDicWords))
			cout << wordToCheck << " is a legal english word.\n";
		else
		{
			cout << wordToCheck << endl;//hllmarks

			strcpy_s(result[numResult++], 21, wordToCheck);

			spellCheck1(wordToCheck, dictionary, numDicWords, result, numResult);
			spellCheck2(wordToCheck, dictionary, numDicWords, result, numResult);
			spellCheck3(wordToCheck, dictionary, numDicWords, result, numResult);

			strcpy_s(result[numResult++], 21, "");
		}
	}

	saveWords(result, numResult);

	delete[] dictionary;
	delete[] result;

	system("pause");
}

bool legal(char* wordToCheck, char(*dictionary)[21], int numDicWords)
{
	for (int i = 0; i < numDicWords; i++)
		if (strcmp(dictionary[i], wordToCheck) == 0)
			return true;
	return false;
}

void readDictionary(char(*dictionary)[21], int& numDicWords)
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
		if (*dictionary[j] == '\0') {
			numDicWords--;
		}
	}
	inFile.close();
	/*for (int j = 0; j < numDicWords; j++) {
		cout << dictionary[j]<<endl;
	}*/


}

void spellCheck1(char* wordToCheck, char(*dictionary)[21],
	int numDicWords, char(*&result)[21], int& numResult)
{
	int length = strlen(wordToCheck);
	int numWords = 0;
	char(*words)[21] = 0;

	findWords(words, numWords, dictionary, numDicWords, length);

	char buffer1[21];
	char buffer2[21];
	for (int i = 0; i < numWords; i++) {
		int n = 0;
		for (int j = 0; j < length; j++)
		{
			buffer1[j] = words[i][j];
			buffer2[j] = wordToCheck[j];
			if (buffer1[j] != buffer2[j]) {
				n++;
			}
		}
		if (n == 1) {//兩個單字只有一處不同時把buffer1放入result
			for (int k = 0; k < length; k++) {
				result[numResult][k] = buffer1[k];
				//cout << result[numResult][k];
			}
			for (int i = length; i < 21; i++) {
				result[numResult][i] = '\0';
			}
			//cout << endl;
			numResult++;

		}
	}
	delete[] words;
}

void spellCheck2(char* wordToCheck, char(*dictionary)[21],
	int numDicWords, char(*&result)[21], int& numResult)
{
	int length = strlen(wordToCheck) + 1;
	int numWords = 0;
	char(*words)[21] = 0;

	findWords(words, numWords, dictionary, numDicWords, length);

	char buffer[21];
	for (int i = 0; i < numWords; i++) {
		int n = 0;
		for (int j = 0; j < length; j++)
		{
			buffer[j] = words[i][j];
		}
		for (int n = 0; n < length; n++) {
			int count = 0;
			for (int k = n; k < length; k++)
			{
				buffer[k] = buffer[k + 1];//依序把其中一個字母刪掉
			}
			for (int j = 0; j < length; j++) {
				if (buffer[j] == wordToCheck[j]) {
					count++;
				}
				buffer[j] = words[i][j];
			}

			if (count == length - 1) {
				for (int k = 0; k < length; k++) {
					result[numResult][k] = words[i][k];
					//cout << result[numResult][k];
				}
				for (int i = length; i < 21; i++) {
					result[numResult][i] = '\0';
				}
				//cout << endl;
				numResult++;
				break;
			}

		}





	}
	delete[] words;
}

void spellCheck3(char* wordToCheck, char(*dictionary)[21],
	int numDicWords, char(*&result)[21], int& numResult)
{
	int length = strlen(wordToCheck) - 1;
	int numWords = 0;
	char(*words)[21] = 0;

	findWords(words, numWords, dictionary, numDicWords, length);

	char buffer[21];
	for (int i = 0; i < numWords; i++) {
		for (unsigned int j = 0; j < length; j++)
		{
			buffer[j] = words[i][j];
		}

		for (int n = 0; n < length; n++) {
			int count = 0;
			for (int k = length - 1; k >= n; k--)
			{
				buffer[k + 1] = buffer[k];
			}
			buffer[n] = ' ';
			for (int j = 0; j < strlen(wordToCheck); j++) {
				if (buffer[j] == wordToCheck[j]) {
					count++;
				}
				buffer[j] = words[i][j];
			}

			if (count == length) {//buffer與wordtochek只有一個地方不同(空格)
				for (int k = 0; k < length; k++) {
					result[numResult][k] = words[i][k];
					//cout << result[numResult][k];
				}
				for (int i = length; i < 21; i++) {
					result[numResult][i] = '\0';
				}
				//cout << endl;
				numResult++;
				break;
			}

		}

	}
	delete[] words;
}

void findWords(char(*&words)[21], int& numWords,
	char(*dictionary)[21], int numDicWords, int length)
{
	for (int i = 0; i < numDicWords; i++)
		if (strlen(dictionary[i]) == length)
			numWords++;

	words = new char[numWords][21];

	numWords = 0;
	for (int i = 0; i < numDicWords; i++)
		if (strlen(dictionary[i]) == length)
		{
			strcpy_s(words[numWords], 21, dictionary[i]);
			numWords++;
		}
}

void saveWords(char(*result)[21], int numResult)
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