// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <chrono>
#include <string>
#include <fstream>
#include <sstream>
//#include <unistd.h>
#include <conio.h>
#include<windows.h>
namespace fs = std::filesystem;
using namespace  std;

fs::file_time_type  lastsync;

vector<fs::path> there_can_be_only_one(vector<string>pa, vector<fs::path>rem)
{
    vector<fs::path>res;
    for (int i = 0; i < rem.size(); i++)
    {
        if (fs::exists(rem[i]))
            fs::remove(rem[i]);
    }

    for (auto& p : fs::recursive_directory_iterator(pa[0]))
    {
        res.push_back(p);
    }

    for (int i = 1; i < pa.size(); i++)
    {
        fs::remove_all(pa[i]);
    }

    fs::remove(pa[1]);
    fs::create_directory(pa[1]);
    lastsync = fs::last_write_time(pa[1]);

    for (int i = 1; i < pa.size(); i++)
    {
        fs::copy(pa[0], pa[i], fs::copy_options::overwrite_existing | fs::copy_options::recursive);
    }
    return res;
}

vector<fs::path> we_are_conected_by_the_one_feeling(vector<string>pa, vector<fs::path>base)
{
    vector<fs::path>res;
    string str;
    for (auto& p : fs::recursive_directory_iterator(pa[0]))
    {
        str = p.path().string().substr(pa[0].size());
        for (int i = 1; i < pa.size(); i++)
        {
            if (!fs::exists(pa[i] + str)) {
                res.push_back(p.path());
                break;
            }
            else if (fs::last_write_time(pa[i] + str) > lastsync)
            {
                if (fs::last_write_time(p) < fs::last_write_time(pa[i] + str))
                    fs::copy(pa[i] + str, p, fs::copy_options::overwrite_existing);
                break;
            }
        }

    }

    for (int i = 0; i < base.size(); i++)
    {
        if (!fs::exists(base[i]))
            res.push_back(base[i]);
    }

    for (int i = 1; i < pa.size(); i++)
    {
        fs::copy(pa[i], pa[0], fs::copy_options::skip_existing | fs::copy_options::recursive);
    }
    return res;
}



bool overlapping(bool menu) {
    char c;    

    while (!menu) {
        if (_kbhit()) {
            c = _getch(); 
            if ((int)c == 27 || (int)c == 110) {
                menu = false;
                break;
            }
            else if ((int)c == 121) {
                menu = true;
                break;
            }

        }
    }

    return menu;
}


void update(int t) {
    while (t >= 0)
    {
        std::cout << "\rTime remaining: " << t << std::flush;
        Sleep(1000);
        t--;
    }
}

int main() {
    ofstream file;
    string str;
    Sleep(1000);
    bool menu = false;
    
    if (fs::exists("config")) {
        vector<string> paths;
        ifstream file("config");
        //bool sync = false;
        cout << "Directory SYNC service" << endl;
        cout << "If you want to begin syncing type [y/n]" << endl;
        
        while (menu = overlapping(menu)){
            while (getline(file, str)) {
                if (fs::exists(str)) {
                    paths.push_back(str);
                }
                else
                    cout << "no directory :" << "    " << str << endl;
            }
            //cout << "config read successfully" << endl;
            vector<fs::path> buf;
            buf.clear();
            buf = there_can_be_only_one(paths, buf);
        
            update(30);

            //Sleep(100);
            //std::cout << "keep going" << endl;
            buf = we_are_conected_by_the_one_feeling(paths, buf);
            there_can_be_only_one(paths, buf);
            //Timer_c(30);
            //cout << "im here" << endl;
        }
    }
    else
        cout << "no config";
}