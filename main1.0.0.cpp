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
    cout <<"music v1.0.0\n";
    cout <<"�����ӹ����� ������ηַ���";
    cout <<"������1.�鿴���У�2.������һ�ף�3.��Ӹ�����4.ɾ��������5.�˳�\n"; 
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
            break;
        }
        else {
            cout << "��Ч���룬������1-5֮�������" << endl;
        }
    }
    
    return 0;
}
