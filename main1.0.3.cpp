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
        cout << "�������ʧ�ܣ�" << endl;
        return;
    }
    
    EmptyClipboard();
    
    int len = text.size() + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len * sizeof(char));
    if (hMem == NULL) {
        CloseClipboard();
        cout << "�ڴ����ʧ�ܣ��޷����Ƶ������壡" << endl;
        return;
    }
    
    char* pMem = (char*)GlobalLock(hMem);
    strcpy(pMem, text.c_str());
    GlobalUnlock(hMem);
    
    SetClipboardData(CF_TEXT, hMem);
    
    CloseClipboard();
    cout << "�ѽ����������Ƶ�������" << endl;
}

int main() {
    system("chcp 936 > null");
    cin.imbue(locale(""));
    cout.imbue(locale(""));
    cout << "music v1.0.3\n";
    cout << "�����ӹ����� ������ηַ���\n";
    cout << "������1.�鿴���У�2.������һ�ף�3.��Ӹ�����4.ɾ��������5.���Ҹ�����6.�滻������7.�˳�\n"; 
    while (true) {
        cout << ">>>";
        string choice;
        cin >> choice;
        cin.ignore();
        if (choice == "1") {
            vector<string> songs = readSongs();
            if (songs.empty()) {
                cout << "�赥Ϊ��" << endl;
            } else {
                cout << "��ǰ�赥��" << endl;
                for (const string& song : songs) {
                    cout << song << endl;
                }
            }
        }
        else if (choice == "2") {
            vector<string> songs = readSongs();
            if (songs.empty()) {
                cout << "�赥Ϊ�գ�û�пɲ��ŵĸ���" << endl;
                continue;
            }
            
            string nextSong = songs[0];
            songs.erase(songs.begin());
            saveSongs(songs);
            
            cout << "��һ���ǣ�" << endl;
            cout << nextSong << endl;
            copyToClipboard(nextSong);
        }
        else if (choice == "3") {
            cout << "������Ҫ��ӵĸ�����";
            string songName;
            getline(cin, songName);
            
            size_t start = songName.find_first_not_of(" \t");
            size_t end = songName.find_last_not_of(" \t");
            if (start == string::npos) {
                cout << "��������Ϊ��" << endl;
                continue;
            }
            songName = songName.substr(start, end - start + 1);
            
            ofstream file("music.txt", ios::app);
            file << songName << endl;
            file.close();
            
            cout << "����Ӹ�����" << songName << endl;
        }
        else if (choice == "4") {
            cout << "������Ҫɾ���ĸ�����";
            string songName;
            getline(cin, songName);
            
            size_t start = songName.find_first_not_of(" \t");
            size_t end = songName.find_last_not_of(" \t");
            if (start == string::npos) {
                cout << "��������Ϊ��" << endl;
                continue;
            }
            songName = songName.substr(start, end - start + 1);
            
            vector<string> songs = readSongs();
            if (songs.empty()) {
                cout << "�赥Ϊ�գ�û�п�ɾ���ĸ���" << endl;
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
            cout << "��ɾ�� " << deleteCount << " ����Ϊ��" << songName << "���ĸ���" << endl;
        }
        else if (choice == "5") {
            cout << "������Ҫ���ҵĸ�����";
            string songName;
            getline(cin, songName);
            
            size_t start = songName.find_first_not_of(" \t");
            size_t end = songName.find_last_not_of(" \t");
            if (start == string::npos) {
                cout << "�������ݲ���Ϊ��" << endl;
                continue;
            }
            songName = songName.substr(start, end - start + 1);
            
            vector<string> songs = readSongs();
            if (songs.empty()) {
                cout << "�赥Ϊ�գ�û�пɲ��ҵĸ���" << endl;
                continue;
            }
            
            vector<int> foundIndices;
            for (int i = 0; i < songs.size(); ++i) {
                if (songs[i] == songName) {
                    foundIndices.push_back(i + 1);
                }
            }
            
            if (foundIndices.empty()) {
                cout << "δ�ҵ���Ϊ��" << songName << "���ĸ���" << endl;
            } else {
                cout << "�ҵ� " << foundIndices.size() << " ����Ϊ��" << songName << "���ĸ�����λ�ã�";
                for (int idx : foundIndices) {
                    cout << idx << " ";
                }
                cout << endl;
            }
        }
        else if (choice == "6") {
            cout << "������Ҫ�滻�ĸ�����";
            string oldName;
            getline(cin, oldName);
            
            size_t start = oldName.find_first_not_of(" \t");
            size_t end = oldName.find_last_not_of(" \t");
            if (start == string::npos) {
                cout << "ԭ��������Ϊ��" << endl;
                continue;
            }
            oldName = oldName.substr(start, end - start + 1);
            
            cout << "�������滻��ĸ�����";
            string newName;
            getline(cin, newName);
            
            start = newName.find_first_not_of(" \t");
            end = newName.find_last_not_of(" \t");
            if (start == string::npos) {
                cout << "�¸�������Ϊ��" << endl;
                continue;
            }
            newName = newName.substr(start, end - start + 1);
            
            vector<string> songs = readSongs();
            if (songs.empty()) {
                cout << "�赥Ϊ�գ�û�п��滻�ĸ���" << endl;
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
                cout << "�ѽ� " << replaceCount << " �ס�" << oldName << "���滻Ϊ��" << newName << "��" << endl;
            } else {
                cout << "δ�ҵ���Ϊ��" << oldName << "���ĸ����������滻" << endl;
            }
        }
        else if (choice == "7") {
            break;
        }
        else {
            cout << "��Ч���룬������1-7֮�������" << endl;
        }
    }
    
    return 0;
}
