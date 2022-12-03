#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;

int createsplitfile(string inputfile, int sizechuck);
void mergefile(string outputfilename, int sizechunk, int numberfile);

string dongdau;

struct Book {
	string ID;
	string data;
	friend bool operator <(Book a, Book b) {
		if (a.ID == "")return 0;
		if (b.ID == "")return 1;
		return a.ID < b.ID;
	}
	Book() {
		ID =data= "";
	}
	Book(string tmp) {
		data = tmp;
		stringstream ss(tmp);
		getline(ss, ID, ',');
	}
};

int createsplitfile(string inputfile, int sizechuck) {
	int numberfile = 0;
	const char* inputfilename =inputfile.c_str();
	fstream fin;
	fin.open(inputfilename, ios_base::in | ios_base::binary);
	vector<Book>str;
	getline(fin, dongdau);
	while (1) {
		str.clear();
		if (!fin.eof()) {
			int i = 0;
			for (; i < sizechuck; i++) {
				if (!fin.eof()) {
					string tmp;
					getline(fin, tmp);
					Book a(tmp);
					str.push_back(a);
				}
				else break;
			}
			sort(str.begin(), str.end());
			string s = "temp" + to_string(numberfile)+".csv";
			fstream fout;
			fout.open(s, ios_base::out | ios_base::binary);
			for(int j=0;j<i;j++){
				fout << str[j].data << endl;
			}
			fout.close();
			numberfile++;
		}
		else break;
	}
	fin.close();
	return numberfile;
}

int fineminbook(vector<Book>tmp) {
	Book min = tmp[0];
	int index = 0;
	for (int i = 0; i < tmp.size(); i++) {
		if (tmp[i] < min) {
			min = tmp[i];
			index = i;
		}
	}
	return index;
}

void mergefile(string outputfilename, int sizechunk, int numberfile) {
	fstream out;
	out.open(outputfilename, ios_base::out | ios_base::binary);
	out << dongdau << endl;
	fstream* readsplitfile = new fstream[numberfile];
	for (int i = 0; i < numberfile; i++) {
		string s = "temp" + to_string(i) + ".csv";
		readsplitfile[i].open(s, ios_base::in | ios_base::binary);
	}
	vector<Book>minbook;
	for (int i = 0; i < numberfile; i++) {
		string tmp;
		getline(readsplitfile[i], tmp);
		Book a(tmp);
		minbook.push_back(a);
	}
	int count = 0;
	while (count != numberfile) {
		int indexminbook = fineminbook(minbook);
		out << minbook[indexminbook].data << endl;
		if (!readsplitfile[indexminbook].eof()) {
			string tmp1;
			getline(readsplitfile[indexminbook], tmp1);
			Book b(tmp1);
			minbook[indexminbook] = b;
		}
		else {
			count++;
			Book c;
			minbook[indexminbook] = c;
		}
	}
	for (int i = 0; i < numberfile; i++) {
		readsplitfile[i].close();
	}
	out.close();
	for (int i = 0; i < numberfile; i++) {
		string namefilechunk = "temp" + to_string(i) + ".csv";
		remove(namefilechunk.c_str());
	}
}

void sortbigfile(string namefileinput, string namefileoutput, int sizechunk) {
	int numberfile = createsplitfile(namefileinput, sizechunk);
	mergefile(namefileoutput, sizechunk, numberfile);
}

int main() {
	string inputfilename = "Books_rating_sorted.csv.000.part";
	string outputfilename = "Books_rating_sorted.csv";
	int chunksize = 2000;
	sortbigfile(inputfilename, outputfilename, chunksize);
}