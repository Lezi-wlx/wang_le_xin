#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

vector<string> readSongs() {
    vector<string> songs;
    ifstream file("music.txt");
    if (!file.is_open()) {
        return songs;
    }
    
    string song;
    while (getline(file, song)) {
        songs.push_back(song);
    }
    file.close();
    return songs;
}

void saveSongs(const vector<string>& songs) {
    ofstream file("music.txt");
    for (const string& song : songs) {
        file << song << endl;
    }
    file.close();
}

void copyToClipboard(const string& text) {
    if (!OpenClipboard(NULL)) {
        cout << "剪贴板打开失败！" << endl;
        return;
    }
    
    EmptyClipboard();
    
    int len = text.size() + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len * sizeof(char));
    if (hMem == NULL) {
        CloseClipboard();
        cout << "内存分配失败，无法复制到剪贴板！" << endl;
        return;
    }
    
	char* pMem = (char*)GlobalLock(hMem);
	strcpy(pMem, text.c_str());
	GlobalUnlock(hMem);
    
    SetClipboardData(CF_TEXT, hMem);
    
    CloseClipboard();
    cout << "已将歌曲名复制到剪贴板" << endl;
}

int main() {
    system("chcp 936 > null");
    cin.imbue(locale(""));
    cout.imbue(locale(""));
    cout <<"music v1.0.0\n";
    cout <<"找乐子工作室 请勿二次分发！";
    cout <<"帮助：1.查看所有；2.播放下一首；3.添加歌曲；4.删除歌曲；5.退出\n"; 
    while (true) {
        cout << ">>>";
        string choice;
        cin >> choice;
        cin.ignore();

        if (choice == "1") {
            vector<string> songs = readSongs();
            if (songs.empty()) {
                cout << "歌单为空" << endl;
            } else {
                cout << "当前歌单：" << endl;
                for (const string& song : songs) {
                    cout << song << endl;
                }
            }
        }
        else if (choice == "2") {
            vector<string> songs = readSongs();
            if (songs.empty()) {
                cout << "歌单为空，没有可播放的歌曲" << endl;
                continue;
            }
            
            string nextSong = songs[0];
            songs.erase(songs.begin());
            saveSongs(songs);
            
            cout << "下一首是：" << endl;
            cout << nextSong << endl;
            copyToClipboard(nextSong);
        }
        else if (choice == "3") {
            cout << "请输入要添加的歌名：";
            string songName;
            getline(cin, songName);
            
            size_t start = songName.find_first_not_of(" \t");
            size_t end = songName.find_last_not_of(" \t");
            if (start == string::npos) {
                cout << "歌名不能为空" << endl;
                continue;
            }
            songName = songName.substr(start, end - start + 1);
            
            ofstream file("music.txt", ios::app);
            file << songName << endl;
            file.close();
            
            cout << "已添加歌曲：" << songName << endl;
        }
        else if (choice == "4") {
            cout << "请输入要删除的歌名：";
            string songName;
            getline(cin, songName);
            
            size_t start = songName.find_first_not_of(" \t");
            size_t end = songName.find_last_not_of(" \t");
            if (start == string::npos) {
                cout << "歌名不能为空" << endl;
                continue;
            }
            songName = songName.substr(start, end - start + 1);
            
            vector<string> songs = readSongs();
            if (songs.empty()) {
                cout << "歌单为空，没有可删除的歌曲" << endl;
                continue;
            }
            
            int deleteCount = 0;
            vector<string> remaining;
            for (const string& song : songs) {
                if (song == songName) {
                    deleteCount++;
                } else {
                    remaining.push_back(song);
                }
            }
            
            saveSongs(remaining);
            cout << "已删除 " << deleteCount << " 首名为《" << songName << "》的歌曲" << endl;
        }
        else if (choice == "5") {
            break;
        }
        else {
            cout << "无效输入，请输入1-5之间的数字" << endl;
        }
    }
    
    return 0;
}
