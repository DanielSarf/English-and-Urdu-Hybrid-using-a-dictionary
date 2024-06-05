#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

void checkAndHandleFileError(ifstream& file, const string& fileName)
{
    if (!file.is_open())
    {
        cerr << "Error: Cannot read file " << fileName << endl;

        exit(-1);
    }
}

void checkAndHandleFileError(ofstream& file, const string& fileName)
{
    if (!file.is_open())
    {
        cerr << "Error: Cannot write to file " << fileName << endl;
        
        exit(-1);
    }
}

void readDictionary(unordered_map<string, string> &wordPairs)
{
    ifstream dictionary("dictionary.txt");
    checkAndHandleFileError(dictionary, "dictionary.txt");
    
    string currentEnglishWord, currentUrduWord;

    // Dictionary has this format:
    // English_word    Urdu_word
    // English_word    Urdu_word
    // ...
    while(dictionary >> currentEnglishWord >> currentUrduWord)
    {
        // Since the input will have uppercase and lowercase words, we can turn uppercase to lowercase
        // before trying to find it in the unordered map 
        currentEnglishWord[0] = tolower(currentEnglishWord[0]);
        currentUrduWord[0] = tolower(currentUrduWord[0]);

        wordPairs.emplace(pair<string, string>(currentEnglishWord, currentUrduWord));
    }

    dictionary.close();
}

// Split string by word, whitespace and punctuation
vector<string> splitContent(const string &content)
{
    vector<string> result;
    string current;

    for (char ch : content)
    {
        if (isspace(ch) || ispunct(ch))
        {
            if (!current.empty())
            {
                result.push_back(current);
                current.clear();
            }
            result.push_back(string(1, ch));
        }
        else
        {
            current += ch;
        }
    }
    
    if (!current.empty())
    {
        result.push_back(current);
    }
    
    return result;
}

// Take each element of content vector and combine it into a string
string combineContent(const vector<string> &content)
{
    string result;

    for (const string &word : content)
    {
        result += word;
    }

    return result;
}

int main()
{
    // This unordered map lets us check for words in O(1) on average.
    unordered_map<string, string> wordPairs;
    
    // Load English words and their Urdu counterparts into the unordered map
    readDictionary(wordPairs);
    
    ifstream input("input.txt");
    checkAndHandleFileError(input, "input.txt");

    // Load file contents into string
    stringstream buffer;
    buffer << input.rdbuf();

    cout << "Input:\n" << buffer.str();

    // Split string by words, whitespace and punctuation 
    vector<string> splitInputContent = splitContent(buffer.str());

    input.close();

    // Variables are initialized here instead to avoid making them each time inside for loop
    string currentEnglishWord;
    string currentUrduWord;
    bool isFirstLetterUppercase;

    for (int i = 0; i < splitInputContent.size(); i++)
    {
        currentEnglishWord = splitInputContent[i];

        isFirstLetterUppercase = isupper(currentEnglishWord[0]);

        // We must convert the word to lowercase before trying to find it in the unordered map
        if (isFirstLetterUppercase)
        {
            currentEnglishWord[0] = tolower(currentEnglishWord[0]);
        }

        // Try finding English word in 
        auto check = wordPairs.find(currentEnglishWord);

        // Skip iteration if not found
        if (check == wordPairs.end())
        {
            continue;
        }

        currentUrduWord = check->second;

        // The idea is that if the English word that is being replaced had it's first letter uppercase
        // then make the Urdu word's first letter uppercase too (a challenge by our teacher)
        if (isFirstLetterUppercase)
        {
            currentUrduWord[0] = toupper(currentUrduWord[0]);
        }

        splitInputContent[i] = currentUrduWord;
    }
    
    ofstream output("output.txt");
    checkAndHandleFileError(output, "output.txt");

    // Recombine into a string
    string combinedOutputString = combineContent(splitInputContent);

    cout << "\n\nOutput:\n" << combinedOutputString;

    // Write to file
    output.write(combinedOutputString.c_str(), combinedOutputString.length());

    output.close();

    // Wait for user input before exiting program
    cin.get();
    return 0;
}