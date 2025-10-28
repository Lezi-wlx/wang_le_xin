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
    cout << "music v1.0.3\n";
    cout << "找乐子工作室 请勿二次分发！\n";
    cout << "帮助：1.查看所有；2.播放下一首；3.添加歌曲；4.删除歌曲；5.查找歌曲；6.替换歌曲；7.退出\n"; 
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
            cout << "请输入要查找的歌名：";
            string songName;
            getline(cin, songName);
            
            size_t start = songName.find_first_not_of(" \t");
            size_t end = songName.find_last_not_of(" \t");
            if (start == string::npos) {
                cout << "查找内容不能为空" << endl;
                continue;
            }
            songName = songName.substr(start, end - start + 1);
            
            vector<string> songs = readSongs();
            if (songs.empty()) {
                cout << "歌单为空，没有可查找的歌曲" << endl;
                continue;
            }
            
            vector<int> foundIndices;
            for (int i = 0; i < songs.size(); ++i) {
                if (songs[i] == songName) {
                    foundIndices.push_back(i + 1);
                }
            }
            
            if (foundIndices.empty()) {
                cout << "未找到名为《" << songName << "》的歌曲" << endl;
            } else {
                cout << "找到 " << foundIndices.size() << " 首名为《" << songName << "》的歌曲，位置：";
                for (int idx : foundIndices) {
                    cout << idx << " ";
                }
                cout << endl;
            }
        }
        else if (choice == "6") {
            cout << "请输入要替换的歌名：";
            string oldName;
            getline(cin, oldName);
            
            size_t start = oldName.find_first_not_of(" \t");
            size_t end = oldName.find_last_not_of(" \t");
            if (start == string::npos) {
                cout << "原歌名不能为空" << endl;
                continue;
            }
            oldName = oldName.substr(start, end - start + 1);
            
            cout << "请输入替换后的歌名：";
            string newName;
            getline(cin, newName);
            
            start = newName.find_first_not_of(" \t");
            end = newName.find_last_not_of(" \t");
            if (start == string::npos) {
                cout << "新歌名不能为空" << endl;
                continue;
            }
            newName = newName.substr(start, end - start + 1);
            
            vector<string> songs = readSongs();
            if (songs.empty()) {
                cout << "歌单为空，没有可替换的歌曲" << endl;
                continue;
            }
            
            int replaceCount = 0;
            for (string& song : songs) {
                if (song == oldName) {
                    song = newName;
                    replaceCount++;
                }
            }
            
            if (replaceCount > 0) {
                saveSongs(songs);
                cout << "已将 " << replaceCount << " 首《" << oldName << "》替换为《" << newName << "》" << endl;
            } else {
                cout << "未找到名为《" << oldName << "》的歌曲，无需替换" << endl;
            }
        }
        else if (choice == "7") {
            break;
        }
        else {
            cout << "无效输入，请输入1-7之间的数字" << endl;
        }
    }
    
    return 0;
}
