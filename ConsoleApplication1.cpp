#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <regex>
using namespace std;

void load();
void save();
void write();
void read();
void inputdate(int&, int&, int&);
void getdate(int&, int&, int&);
bool checkdate(int, int, int);

struct Diary {
    int date = 0;
    int year = 0;
    int month = 0;
    int day = 0;
    string content = "";
    Diary() {}
    Diary(int newyear, int newmonth, int newday, string newcontent) {
        year = newyear;
        month = newmonth;
        day = newday;
        date = year * 10000 + month * 100 + day;
        content = newcontent;
    }
    Diary(int newdate, string newcontent) {
        date = newdate;
        year = date / 10000;
        month = (date / 100) % 100;
        day = date % 100;
        content = newcontent;
    }
    void setdiary(int newyear, int newmonth, int newday, string newcontent) {
        year = newyear;
        month = newmonth;
        day = newday;
        date = year * 10000 + month * 100 + day;
        content = newcontent;
    }
    bool operator== (const Diary& d) const {
        return  date == d.date;
    }
};
bool operator< (const Diary& lhs, const Diary& rhs) {
    return lhs.date < rhs.date;
}


set<Diary> diary;

int main()
{
    load();
    int year, month, day;
    int c = 0;
    Diary tmp;
    set<Diary>::iterator it;
    string input = "";
    while (1) {
    if (diary.size() != 0) {
        it = diary.end();
        c = 0;
        while (it != diary.begin() && c < 7) {
            it--;
            c++;
        }
        for (int i = 0; i < c; i++) {
            cout << it->year << "年" << it->month << "月" << it->day << "日：" << it->content << endl;
            it++;
        }
    }
    else{
        cout << "記録がありません。" << endl;
    }
    cout << "----------------------------------------------------------------------------" << endl;
        
        do {
            cout << "書く[w]/読む[r]\n";
            cin >> input;
        } while (input != "w" && input != "r");

        if (input == "w")write();
        if (input == "r")read();
    }

}

void load() {
    ifstream infile("diary.dat");
    if (!infile) {
        return;
    }

    int date = 0;
    string content = "";
    while (infile >> date >> content) {
        diary.emplace(date, content);
    }
    infile.close();
}

void save() {
    ofstream outfile("diary.dat");
    for (auto& d : diary) {
        outfile << d.date << " " << d.content << endl;
    }
    outfile.close();
}

void write() {
    int year, month, day;
    string command = "";
    string input = "";
    string content = "";
    cout << "書く\n";
    inputdate(year, month, day);
    Diary tmp(year, month, day, "");
    auto it = diary.find(tmp);
    if (it != diary.end()) {
        cout << "既に存在します。(" << it->content << ")\n";
        command = "";
        do {
            cout << "上書き[o]/追記[p]/やめる[c]\n";
            cin >> command;
        } while (!(command == "o" || command == "p" || command == "c"));
        if (command == "c") {
            cout << "\n\n";
            return;
        }
        content = it->content;
        diary.erase(it);
    }
    cout << "内容：\n";
    cin >> input;
    if (command == "p") {
        input = content + input;
    }
    diary.emplace(year, month, day, input);
    save();
    cout << "書き込み完了\n\n\n";

}

void read() {
    int year, month, day;
    inputdate(year, month, day);
    Diary tmp(year, month, day, "");
    auto it = diary.find(tmp);
    cout << endl;
    if (it != diary.end()) {
    cout << year << "年" << month << "月" << day << "日：" << it->content << endl;
    }
    else {
        cout << "記録がありません。\n";
    }
    string input;
    cout << "\n任意の入力で戻る\n";
    cin >> input;
    cout << "\n\n";
}

void inputdate(int& year, int& month, int& day) {
    string input = "";
    regex rx1(R"(\d{4}/\d{1,2}/\d{1,2})");
    int a[3];
    do {
        do {
            cout << "今日[t]、または日付を入力[年(西暦)/月/日]\n";
            cin >> input;
        } while (!(regex_match(input, rx1)||input=="t"));
        if (input != "t") {
            regex rx2("/");
            sregex_token_iterator rit(input.begin(), input.end(), rx2, -1);
            wsregex_token_iterator end;
            year = stoi(*rit);
            month = stoi(*(++rit));
            day = stoi(*(++rit));
        }
        else{
            getdate(year, month, day);
        }
    } while (!checkdate(year, month, day));

}

void getdate(int& year, int& month, int& day) {
    time_t t;
    time(&t);
    tm newtime;
    localtime_s(&newtime, &t);
    year = newtime.tm_year + 1900;
    month = newtime.tm_mon + 1;
    day = newtime.tm_mday;
}

bool checkdate(int year, int month, int day) {
    if (day < 1 || month < 1 || month > 12) return false;
    if (month == 2) {
        if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
            if (day > 29)return false;
        }
        else {
            if (day > 28)return false;
        }
    }else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30)return false;
    }
    else {
        if (day > 31)return false;
    }
    return true;
}
