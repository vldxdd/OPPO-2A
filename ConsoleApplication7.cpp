#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iomanip>
#include <clocale>
#include <vector>
#include <sstream>
#include <regex>

using namespace std;

struct Tovar {
    string date;
    string name;
    int count;
};

// Функция для извлечения внутреннего имени из строки в кавычках
string extractInnerName(const string& quotedName) {
    return quotedName.substr(1, quotedName.size() - 2);
}

// Функция для сопоставления шаблону регулярного выражения
bool matchRegex(const string& str, const regex& regex) {
    return regex_match(str, regex);
}

// Функция извлечения даты из строки
void extractDate(const string& str, Tovar& tovar) {
    regex dateRegex("\\d{4}\\.\\d{1,2}\\.\\d{1,2}"); // регулярное выражение даты (ГГГГ.ММ.ДД)
    if (matchRegex(str, dateRegex)) {
        tovar.date = str;
    }
}

// Функция для извлечения счетчика из строки
void extractCount(const string& str, Tovar& tovar) {
    regex countRegex("\\d+"); // счётчик
    if (matchRegex(str, countRegex)) {
        tovar.count = stoi(str);
    }
}

// Функция извлечения имени из строки
void extractName(const string& str, Tovar& tovar) {
    regex nameRegex("\"([^\"]|\\\")*\""); // регулярное выражение имени (допускает внутренние кавычки)
    if (matchRegex(str, nameRegex)) {
        string innerName = extractInnerName(str);
        tovar.name = innerName; // сохранять внутренние кавычки
    }
}

// Функция для анализа строки и извлечения даты, имени и количества
void parseLine(const string& line, int lineNumber, Tovar& tovar) {
    istringstream iss(line);
    vector<string> parts;
    string part;
    while (iss >> part) {
        parts.push_back(part);
    }

    bool nameFound = false;
    for (const auto& p : parts) {
        extractDate(p, tovar);
        extractCount(p, tovar);
        extractName(p, tovar);
        if (tovar.name != "") {
            nameFound = true;
        }
    }

    if (!nameFound) {
        cerr << "Ошибка: Неправильные данные в строке " << lineNumber << "!" << endl;
        return;
    }
}

//Функция считывания из текстового файла
void load_txt(const string& filename, vector<Tovar>& tovars) {
    ifstream inputFile(filename);

    if (!inputFile) {
        cerr << "Ошибка открытия текстового файла!" << endl;
        return;
    }

    int lineNumber = 0;
    string line;
    while (getline(inputFile, line)) {
        lineNumber++;
        Tovar tovar;
        parseLine(line, lineNumber, tovar);
        tovars.push_back(tovar);
    }

    inputFile.close();
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<Tovar> tovars;
    load_txt("input.txt", tovars);

    for (const auto& tovar : tovars) {
        cout << tovar.date << " " << tovar.name << " " << tovar.count << endl;
    }
}