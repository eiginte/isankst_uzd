#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <regex>
#include <cctype>
#include <algorithm>
#include <iomanip>

using namespace std;

// URL regex
regex urlRegex(R"((https?://|http://|www\.)[A-Za-z0-9./?=&%_-]+\.[A-Za-z]{2,})", regex_constants::icase);

// Funkcija mažosioms raidėms
string toLower(const string& s) {
    string res;
    for (char c : s)
        res += tolower(static_cast<unsigned char>(c));
    return res;
}

// Funkcija apdoroja eilute zodziu analizei
void processLineWords(const string& line, map<string,int>& wordCount, map<string,set<int>>& wordLines, int& totalWords, int lineNumber) {
    string cleanedLine = regex_replace(line, urlRegex, " "); // pasalinam URL

    string word;
    for (char c : cleanedLine) {
        if (isalpha(static_cast<unsigned char>(c))) {
            word += c;
        } else {
            if (!word.empty()) {
                string w = toLower(word);
                wordCount[w]++;
                wordLines[w].insert(lineNumber);
                totalWords++;
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        string w = toLower(word);
        wordCount[w]++;
        wordLines[w].insert(lineNumber);
        totalWords++;
    }
}

// Funkcija nuskaityti TLD faila
set<string> loadTLDs(const string& tldFile) {
    set<string> tlds;
    ifstream file(tldFile);
    if (!file) {
        cout << "Nepavyko atidaryti TLD failo!" << endl;
        return tlds;
    }
    string domain;
    while (getline(file, domain)) {
        if (!domain.empty() && domain[0] == '#') continue;
        transform(domain.begin(), domain.end(), domain.begin(), ::tolower);
        tlds.insert(domain);
    }
    return tlds;
}

// Funkcija URL suradimui su TLD patikra
void processLineURLs(const string& line, const set<string>& tlds, set<string>& urls) {
    auto begin = sregex_iterator(line.begin(), line.end(), urlRegex);
    auto end = sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        string url = it->str();
        size_t pos = url.find_last_of('.');
        if (pos != string::npos) {
            string tld = url.substr(pos + 1);
            transform(tld.begin(), tld.end(), tld.begin(), ::tolower);
            if (tlds.count(tld) == 0) continue;
        }
        urls.insert(url);
    }
}

int main() {
    cout << "Pasirinkite norima funkcija:\n";
    cout << "1 - Zodziu analize su cross-reference\n";
    cout << "2 - URL adresu suradimas tekste\n";
    cout << "Jusu pasirinkimas (1 arba 2): ";
    int choice;
    cin >> choice;
    cin.ignore(); // nuvalom newline

    string filename;
    cout << "Iveskite failo pavadinima su tekstu: ";
    getline(cin, filename);

    ifstream file(filename);
    if (!file) {
        cout << "Nepavyko atidaryti failo!" << endl;
        return 1;
    }

    if (choice == 1) {
        map<string,int> wordCount;
        map<string,set<int>> wordLines;
        int totalWords = 0;
        string line;
        int lineNumber = 0;

        while (getline(file, line)) {
            lineNumber++;
            processLineWords(line, wordCount, wordLines, totalWords, lineNumber);
        }
        file.close();

        ofstream out("lentele.txt");
        if (!out) {
            cout << "Nepavyko sukurti failo lentele.txt!" << endl;
            return 1;
        }

        const int widthWord = 20;
        const int widthCount = 12;

        out << left << setw(widthWord) << "Zodis"
            << setw(widthCount) << "Kiek kartu"
            << "Eilutes" << endl;
        out << string(widthWord + widthCount + 50, '-') << endl;

        for (const auto& pair : wordCount) {
            if (pair.second > 1) {
                out << left << setw(widthWord) << pair.first
                    << setw(widthCount) << pair.second;
                for (auto ln : wordLines[pair.first]) {
                    out << ln << " ";
                }
                out << endl;
            }
        }

        out.close();
        cout << "Cross-reference tipo lentele sukurta faile 'lentele.txt'.\n";
        cout << "Is viso zodziu tekste: " << totalWords << endl;
        cout << "Skirtingu zodziu: " << wordCount.size() << endl;

    } else if (choice == 2) {
        string tldFile;
        cout << "Iveskite TLD failo pavadinima (pvz., tlds-alpha-by-domain.txt): ";
        getline(cin, tldFile);
        set<string> tlds = loadTLDs(tldFile);
        if (tlds.empty()) return 1;

        set<string> urls;
        string line;
        while (getline(file, line)) {
            processLineURLs(line, tlds, urls);
        }
        file.close();

        cout << "\nRasti URL:\n";
        for (const auto& u : urls) {
            cout << u << endl;
        }

    } else {
        cout << "Neteisingas pasirinkimas!" << endl;
        return 1;
    }

    return 0;
}
