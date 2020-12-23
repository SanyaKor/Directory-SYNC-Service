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
        if (fs::exists(rem[i])) {
            if (fs::is_directory(rem[i])) {
                fs::remove_all(rem[i]);
            }
            fs::remove(rem[i]);
        }
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
    for (int i = 0; i < base.size(); i++)
    {
        if (!fs::exists(base[i]))
            res.push_back(base[i]);
    }



    for (int j = 0; j < base.size(); j++)
    {
        str = base[j].string().substr(pa[0].size());
        for (int i = 1; i < pa.size(); i++)
        {
            if (!fs::exists(pa[i] + str)) {
                res.push_back(base[j].string());
                break;
            }
            else if (fs::last_write_time(pa[i] + str) > lastsync)
            {
                if (fs::exists(base[j])) {
                    if (fs::last_write_time(base[j]) < fs::last_write_time(pa[i] + str))
                        fs::copy(pa[i] + str, base[j], fs::copy_options::overwrite_existing);
                    break;
                }
            }
        }

    }

    for (int i = 1; i < pa.size(); i++)
    {
        fs::copy(pa[i], pa[0], fs::copy_options::skip_existing | fs::copy_options::recursive);
    }
    return res;
}


void sync_cycle(vector<string> paths, vector<fs::path> &buf) {
        buf = we_are_conected_by_the_one_feeling(paths, buf);
        buf = there_can_be_only_one(paths, buf);
}


void update(int& counter, bool am, int cbuff, vector<string> paths, vector<fs::path> &buf) {
    char c;
    bool do_once = true;
    if (counter >= 0 && am)
    {
        if (am) {
            std::cout << "\r(A)Time remaining(type [esc or n] to open main menu): " << counter << std::flush;
        }
        Sleep(1000);
        counter--;
    }
    if (am && counter == 0) 
    {
        sync_cycle(paths,buf);
        Sleep(3000);
        counter = cbuff;
    }
    else if (!am && counter == cbuff) {
        sync_cycle(paths, buf);
        std::cout << "Current dir was synced successfully" << std::endl;
        std::cout << " (type[esc or n] to open main menu)" << std::endl;
        counter--;
    }
}




bool overlapping(bool menu, int& counter, bool& active, int cbuff, vector<string> paths, vector<fs::path> &buf) {
    char c;
    bool auto_mode = false;
    

    std::cout << "Welcome to the DIRSYNC service!" << std::endl;
    std::cout << "DIRYNC service is a Windows service that enables easier user management." << std::endl;
    std::cout << "This service can help u to sync ur local directory." << std::endl;
    std::cout << "Do u wanna proceed and start syncin? [y/n or esc]" << std::endl;
    std::cout << "(automode) " << auto_mode << std::flush;

    while (!menu) {
        if (_kbhit()) {
            c = _getch();
            //cout << (int)c;
            if ((int)c == 27 || (int)c == 110) {
                menu = false;
                system("cls");
                active = false;
                break;
            }
            else if ((int)c == 121) {
                counter = cbuff;
                menu = true;
                system("cls");
                break;
            }
            else if ((int)c == 97) {
                auto_mode = !auto_mode;
                std::cout << "\r(automode) " << auto_mode << std::flush;
            }
        }
    }

    while (menu) {
        if (_kbhit()) {
            c = _getch();
            //std::cout << (int)c;
            if ((int)c == 27 || (int)c == 110) {
                menu = false;
                break;
            }
        }
        update(counter, auto_mode, cbuff, paths,buf);
    }
    system("cls");
    return menu;

}


int main() {

    Sleep(1000);
    int counter = 30;
    int cbuff = counter;
    bool menu = false;
    bool active = true;
    vector<string> paths;
    ofstream file;
    string str;
    if (fs::exists("config")) {
        
        ifstream file("config");
        while (getline(file, str)) {
            if (fs::exists(str)) {
                paths.push_back(str);
            }
            else
                cout << "no directory :" << "    " << str << endl;
        }

    }
    else
        cout << "no config";

    vector<fs::path> buf;
    buf.clear();
    buf = there_can_be_only_one(paths, buf);

    while (active) {
        menu = overlapping(menu, counter, active, cbuff, paths,buf);
    }
}