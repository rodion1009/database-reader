#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include "windows.h"
#include "rapidjson-master/include/rapidjson/document.h"
#include "rapidjson-master/include/rapidjson/filereadstream.h"
#include "rapidjson-master/include/rapidjson/filewritestream.h"
#include "rapidjson-master/include/rapidjson/prettywriter.h"
using namespace std;
using namespace rapidjson;

vector<string> cities; //������ �������
map<unsigned int, string> streets; //����� �����, �����+�����
struct trashst {
	unsigned int station, //����� �������
		trashtype; //��� ������
};
vector<trashst> trashstation;
struct sch {
	unsigned int station; //����� �������
	string day_open, //day_open - day_close - ��� ������
		day_close,
		time_open, //time_open - time_close - ���� ������
		time_close;
};
vector<sch> schedule;
struct trashtyp {
	unsigned int pk; //����� ���� ������
	string name, //�������� ������
		description; //�������� ������
};
vector<trashtyp> trashtype;
map<unsigned int, string> address;
Document doc; // Document is GenericDocument<UTF8<> >


void getDocument() { //�������� ����� � ��������
	FILE* fp = fopen("inp.json", "r"); //���� ��� ������ (����� ���������� ��� ��)
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer)); //������ �����
	doc.ParseStream(is);
	fclose(fp); //������� ����
}

void writeDocument() { //����� � ����
	FILE* fout = fopen("out.json", "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	FileWriteStream os(fout, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> wr(os);
	doc.Accept(wr); //����� doc � ����
	fclose(fout); //������� ����
}

void cityParse(unsigned int i) {
	if (!strcmp(doc[i]["model"].GetString(), "re_app.city")) { //������ �� �����������-��������
		doc[i].HasMember("fields"); //�������, ��� � ������ ���� ���� fields
		doc[i]["fields"].IsObject(); //� ��� �������� ��������

		doc[i]["fields"].HasMember("name"); //�������, ��� � ������� fields ���� ���� name
		doc[i]["fields"]["name"].IsString(); //� ��� �������� �������

		cities.push_back(doc[i]["fields"]["name"].GetString());  //��������� � ������ ������� ���
																 //cout << doc[i]["fields"]["name"].GetString(); //����� ���� ������� � ��������� (����)*
	}
}

void streetParse(unsigned int i) { 
	if (!strcmp(doc[i]["model"].GetString(), "re_app.street")) {  //������ �� �����������-�������
		doc[i].HasMember("pk"); //� ����� ���� ���� pk
		doc[i]["pk"].IsInt(); //� ��� ���� int
		unsigned int pk = doc[i]["pk"].GetInt(); //��������� �������� ���� pk � ��������� ����������

		doc[i].HasMember("fields"); //� ����� ���� ���� fields
		doc[i]["fields"].IsObject(); //� ��� �������� ��������

		doc[i]["fields"].HasMember("name"); //�������, ��� � ������� fields ���� ���� name
		doc[i]["fields"]["name"].IsString(); //� ��� �������� �������

		doc[i]["fields"].HasMember("city"); //�������, ��� � ������� fields ���� ���� city
		doc[i]["fields"]["city"].IsInt(); //� ��� ���� int
		unsigned int t = doc[i]["fields"]["city"].GetInt(); //��������� ��� �������� � ��������� ����������

		string str = cities[t - 1] + ", " + doc[i]["fields"]["name"].GetString(); //str = �����, �����

		streets.insert(pair<unsigned int, string>(pk, str));  //��������� � streets ���� �� ������ ����� � ������ ������� <�����, �����>
	}
}

void trashatationParse(unsigned int i) {
	if (!strcmp(doc[i]["model"].GetString(), "re_app.trashstation")) {  //������ �� �����������-trashstation
		doc[i].HasMember("pk"); //� ������� ���� ���� pk
		doc[i]["pk"].IsInt(); //� ��� ���� int

		doc[i].HasMember("fields"); //���� ���� fields
		doc[i]["fields"].IsObject(); //� ��� �������� ��������

		doc[i]["fields"].HasMember("trash_type"); //� fields ���� ���� trash_type
		doc[i]["fields"]["trash_type"].IsInt(); //� ��� ���� int
		unsigned int trash_type = doc[i]["fields"]["trash_type"].GetInt(); //��������� trash_type � ��������� ����������

		doc[i]["fields"].HasMember("station"); //� fields ���� ���� station
		doc[i]["fields"]["station"].IsInt(); //� ��� ���� int
		unsigned int station = doc[i]["fields"]["station"].GetInt(); //��������� ��� � ��������� ����������

		trashst trashst; //������� ���������
		trashst.station = station; //��������� ���������
		trashst.trashtype = trash_type;
		trashstation.push_back(trashst); //��������� ��������� � ������
	}
}

void scheduleParse(unsigned int i) {
	if (!strcmp(doc[i]["model"].GetString(), "re_app.schedule")) { //������ �� �����������-schedule
		doc[i].HasMember("pk"); //� ���������� ���� ���� pk
		doc[i]["pk"].IsInt(); //� ��� ���� int

		doc[i].HasMember("fields"); //� ���������� ���� ���� fields
		doc[i]["fields"].IsObject(); //� ��� �������� ��������

		doc[i]["fields"].HasMember("station"); //� fields ���� ���� station
		doc[i]["fields"]["station"].IsInt(); //� ��� ���� int
		unsigned int station = doc[i]["fields"]["station"].GetInt(); //��������� ��� �������� � ��������� ����������

		doc[i]["fields"].HasMember("day_open"); //� fields ���� ���� day_open
		doc[i]["fields"]["day_open"].IsString(); //� ��� �������� �������
		string day_open = doc[i]["fields"]["day_open"].GetString(); //��������� ��� �������� � ��������� ����������

		doc[i]["fields"].HasMember("day_close"); //� fields ���� ���� day_close
		doc[i]["fields"]["day_close"].IsString(); //� ��� �������� �������
		string day_close = doc[i]["fields"]["day_close"].GetString(); //��������� ��� �������� � ��������� ����������

		doc[i]["fields"].HasMember("time_open"); //� fields ���� ���� time_open
		doc[i]["fields"]["time_open"].IsString(); //� ��� �������� �������
		string time_open = doc[i]["fields"]["time_open"].GetString(); //��������� ��� �������� � ��������� ����������

		doc[i]["fields"].HasMember("time_close"); //� fields ���� ���� time_close
		doc[i]["fields"]["time_close"].IsString(); //� ��� �������� �������
		string time_close = doc[i]["fields"]["time_close"].GetString(); //��������� ��� �������� � ��������� ����������

		sch sch; //������� ��������� ��� �������� ����������
		sch.day_close = day_close; //��������� ��������� ������������ ����������
		sch.day_open = day_open;
		sch.station = station;
		sch.time_close = time_close;
		sch.time_open = time_open;
		schedule.push_back(sch); //��������� ����������� � ������
	}
}

void trashtypeParse(unsigned int i) {
	if (!strcmp(doc[i]["model"].GetString(), "re_app.trashtype")) { //������ �� �����������-trashtype
		doc[i].HasMember("pk"); //� trashtype ���� ���� pk
		doc[i]["pk"].IsInt(); //� ��� ���� int
		unsigned int pk = doc[i]["pk"].GetInt(); //��������� ��� �������� � ��������� ���������

		doc[i].HasMember("fields"); //� trashtype ���� ���� fields
		doc[i]["fields"].IsObject(); //� ��� �������� ��������

		doc[i]["fields"].HasMember("name"); //� fields ���� ���� name
		doc[i]["fields"]["name"].IsString(); //� ��� ���� string
		string name = doc[i]["fields"]["name"].GetString(); //��������� ��� �������� � ��������� ����������


		doc[i]["fields"].HasMember("description"); //� fields ���� ���� description
		doc[i]["fields"]["description"].IsString(); //� ��� ���� string
		string description = doc[i]["fields"]["description"].GetString(); //��������� ��� �������� � ��������� ����������

		trashtyp trash; //������� ���������
		trash.description = description; //��������� �� ������������ ����������
		trash.name = name;
		trash.pk = pk;
		trashtype.push_back(trash); //��������� �� � ������
	}
}

void stationParse(unsigned int i) {
	if (!strcmp(doc[i]["model"].GetString(), "re_app.station")) { //������ �� �����������-���������
		doc[i].HasMember("pk");
		doc[i]["pk"].IsInt();
		unsigned int pks = doc[i]["pk"].GetInt(); //id �������

		doc[i].HasMember("fields");
		doc[i]["fields"].IsObject();

		doc[i]["fields"].HasMember("street");
		doc[i]["fields"]["street"].IsInt();
		unsigned int st = doc[i]["fields"]["street"].GetInt(); //����� �����

		doc[i]["fields"].HasMember("house");
		doc[i]["fields"]["house"].IsInt();
		unsigned int h = doc[i]["fields"]["house"].GetInt(); //����� ���� � int
		string hs = to_string(h); //����� ���� � string


		doc[i]["fields"].HasMember("building");
		doc[i]["fields"]["building"].IsInt();
		unsigned int bd = doc[i]["fields"]["building"].GetInt(); //������ � int
		string b = to_string(bd);  //������ � string

		if (bd != 0) {
			address.insert(pair<unsigned int, string>(pks, streets.find(st)->second + ", " + hs + ", " + b));  //�����, �����, ����� ����, ������  (�����)
		}
		else {
			address.insert(pair<unsigned int, string>(pks, streets.find(st)->second + ", " + hs)); //�����, �����, ����� ����  (�����)
		}
		string s1 = address.find(pks)->second;  //���������� ������
		Value v1(s1.c_str(), doc.GetAllocator());
		doc[i]["fields"].AddMember("address", v1, doc.GetAllocator());
	}
}

void correctDoc() { //������������� doc
	for (unsigned int i = 0; i < doc.Size() - 1; i++) {
		Value vv1(kObjectType);
		doc[i]["fields"].AddMember("schedule", vv1, doc.GetAllocator()); //���������� ���� "schedule"
		Value vv2(kObjectType);
		doc[i]["fields"].AddMember("trash", vv2, doc.GetAllocator());//���������� ���� "trash"
		doc[i]["fields"].EraseMember("street"); //�������� ���� �������� �����
		doc[i]["fields"].EraseMember("house");
		doc[i]["fields"].EraseMember("building");
		doc[i]["fields"].EraseMember("raiting");
		doc[i]["fields"].EraseMember("nof_use");
		doc[i]["fields"].EraseMember("add_date");
		doc[i]["fields"].EraseMember("update_date");
		doc[i]["fields"].EraseMember("show");
	}
}

void addFields() { //���������� �����
	for (unsigned int i = 0; i < doc.Size() - 1; i++) {
		if (!strcmp(doc[i]["model"].GetString(), "re_app.station")) {
			doc[i].HasMember("pk");
			doc[i]["pk"].IsInt();
			unsigned int pks = doc[i]["pk"].GetInt(); //id �������
			for (unsigned int j = 0; j < trashstation.size(); j++) { //���������� ������ � �������
				if (trashstation[j].station == pks) {
					unsigned int q = trashstation[j].trashtype;
					string s2 = trashtype[q - 1].name;
					string s3 = trashtype[q - 1].description;
					Value v2(s2.c_str(), doc.GetAllocator());
					Value v3(s3.c_str(), doc.GetAllocator());
					doc[i]["fields"]["trash"].AddMember(v2, v3, doc.GetAllocator());
				}
			}
			for (unsigned int j = 0; j < schedule.size(); j++) { //���������� ���������� � �������
				if (schedule[j].station == pks) {
					string s2 = schedule[j].day_open + " - " + schedule[j].day_close;
					string s3 = schedule[j].time_open + " - " + schedule[j].time_close;
					Value v2(s2.c_str(), doc.GetAllocator());
					Value v3(s3.c_str(), doc.GetAllocator());
					doc[i]["fields"]["schedule"].AddMember(v2, v3, doc.GetAllocator());
				}
			}
		}
	}
}

void corDoc() {
	correctDoc();
	addFields();
	doc.Erase(doc.End() - 1); //�������� ���������� ��������
}

void parse() {
	for (unsigned int i = 0; i < doc.Size() - 1; i++) {
		//�������� ����������� �� ��������� ������� (���������� �������� ��������� �������)
		assert(doc[i].IsObject()); //���������, ��� ������� ������� �������� ��������
		assert(doc[i].HasMember("model")); //�������, ��� � ���� ������� ���� ���� model
		assert(doc[i]["model"].IsString()); //� ��� ���� string

		cityParse(i);
		streetParse(i);
		trashatationParse(i);
		scheduleParse(i);
		trashtypeParse(i);
		stationParse(i);

		if (strcmp(doc[i]["model"].GetString(), "re_app.station")) //������� ��� �������� �������, ������� �� �������� �������� ������ ������
		{
			doc.Erase(&doc[i]);
			i--;
		}

	}
}

int main(void) {
	setlocale(LC_ALL, "rus"); //�����������	
	getDocument(); //���� ��������� �� �����
	assert(doc.IsArray()); //�������, ��� ������ DOM-������ �������� ������
	parse(); //�������� ��� ������ �������� �� ������ ��������
	corDoc(); //���������� doc � ������ ���
	writeDocument(); //����� ��������� � ����
	system("pause");
	return 0;
}