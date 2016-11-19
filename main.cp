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

vector<string> cities; //вектор городов
map<unsigned int, string> streets; //номер улицы, город+улица
struct trashst {
	unsigned int station, //номер станции
		trashtype; //тип мусора
};
vector<trashst> trashstation;
struct sch {
	unsigned int station; //номер станции
	string day_open, //day_open - day_close - дни работы
		day_close,
		time_open, //time_open - time_close - часы работы
		time_close;
};
vector<sch> schedule;
struct trashtyp {
	unsigned int pk; //номер типа мусора
	string name, //название мусора
		description; //описание мусора
};
vector<trashtyp> trashtype;
map<unsigned int, string> address;
Document doc; // Document is GenericDocument<UTF8<> >


void getDocument() { //загрузка файла в документ
	FILE* fp = fopen("inp.json", "r"); //файл для чтения (здесь подключают всю БД)
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer)); //чтение файла
	doc.ParseStream(is);
	fclose(fp); //закрыли файл
}

void writeDocument() { //вывод в файл
	FILE* fout = fopen("out.json", "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	FileWriteStream os(fout, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> wr(os);
	doc.Accept(wr); //вывод doc в файл
	fclose(fout); //закрыли файл
}

void cityParse(unsigned int i) {
	if (!strcmp(doc[i]["model"].GetString(), "re_app.city")) { //работа со структурами-городами
		doc[i].HasMember("fields"); //говорим, что у города есть поле fields
		doc[i]["fields"].IsObject(); //и оно является объектом

		doc[i]["fields"].HasMember("name"); //говорим, что в объекте fields есть поле name
		doc[i]["fields"]["name"].IsString(); //и оно является строкой

		cities.push_back(doc[i]["fields"]["name"].GetString());  //добавляем в вектор городов имя
																 //cout << doc[i]["fields"]["name"].GetString(); //вывод этих городов в консольку (тест)*
	}
}

void streetParse(unsigned int i) { 
	if (!strcmp(doc[i]["model"].GetString(), "re_app.street")) {  //работа со структурами-улицами
		doc[i].HasMember("pk"); //у улицы есть поле pk
		doc[i]["pk"].IsInt(); //и оно типа int
		unsigned int pk = doc[i]["pk"].GetInt(); //сохраняем значение поля pk в локальной переменной

		doc[i].HasMember("fields"); //у улицы есть поле fields
		doc[i]["fields"].IsObject(); //и оно является объектом

		doc[i]["fields"].HasMember("name"); //говорим, что в объекте fields есть поле name
		doc[i]["fields"]["name"].IsString(); //и оно является строкой

		doc[i]["fields"].HasMember("city"); //говорим, что в объекте fields есть поле city
		doc[i]["fields"]["city"].IsInt(); //и оно типа int
		unsigned int t = doc[i]["fields"]["city"].GetInt(); //сохраняем его значение в локальной переменной

		string str = cities[t - 1] + ", " + doc[i]["fields"]["name"].GetString(); //str = Город, улица

		streets.insert(pair<unsigned int, string>(pk, str));  //добавляем в streets пару из номера улицы и строки формата <Город, улица>
	}
}

void trashatationParse(unsigned int i) {
	if (!strcmp(doc[i]["model"].GetString(), "re_app.trashstation")) {  //работа со структурами-trashstation
		doc[i].HasMember("pk"); //у станции есть поле pk
		doc[i]["pk"].IsInt(); //и оно типа int

		doc[i].HasMember("fields"); //есть поле fields
		doc[i]["fields"].IsObject(); //и оно является объектом

		doc[i]["fields"].HasMember("trash_type"); //у fields есть поле trash_type
		doc[i]["fields"]["trash_type"].IsInt(); //и оно типа int
		unsigned int trash_type = doc[i]["fields"]["trash_type"].GetInt(); //сохраняем trash_type в локальную переменную

		doc[i]["fields"].HasMember("station"); //у fields есть поле station
		doc[i]["fields"]["station"].IsInt(); //и оно типа int
		unsigned int station = doc[i]["fields"]["station"].GetInt(); //сохраняем его в локальную переменную

		trashst trashst; //создаем структуру
		trashst.station = station; //заполняем структуру
		trashst.trashtype = trash_type;
		trashstation.push_back(trashst); //добавляем структуру в вектор
	}
}

void scheduleParse(unsigned int i) {
	if (!strcmp(doc[i]["model"].GetString(), "re_app.schedule")) { //работа со структурами-schedule
		doc[i].HasMember("pk"); //у расписания есть поле pk
		doc[i]["pk"].IsInt(); //и оно типа int

		doc[i].HasMember("fields"); //у расписания есть поле fields
		doc[i]["fields"].IsObject(); //и оно является объектом

		doc[i]["fields"].HasMember("station"); //у fields есть поле station
		doc[i]["fields"]["station"].IsInt(); //и оно типа int
		unsigned int station = doc[i]["fields"]["station"].GetInt(); //сохраняем его значение в локальной переменной

		doc[i]["fields"].HasMember("day_open"); //у fields есть поле day_open
		doc[i]["fields"]["day_open"].IsString(); //и оно является строкой
		string day_open = doc[i]["fields"]["day_open"].GetString(); //сохраняем его значение в локальной переменной

		doc[i]["fields"].HasMember("day_close"); //у fields есть поле day_close
		doc[i]["fields"]["day_close"].IsString(); //и оно является строкой
		string day_close = doc[i]["fields"]["day_close"].GetString(); //сохраняем его значение в локальной переменной

		doc[i]["fields"].HasMember("time_open"); //у fields есть поле time_open
		doc[i]["fields"]["time_open"].IsString(); //и оно является строкой
		string time_open = doc[i]["fields"]["time_open"].GetString(); //сохраняем его значение в локальной переменной

		doc[i]["fields"].HasMember("time_close"); //у fields есть поле time_close
		doc[i]["fields"]["time_close"].IsString(); //и оно является строкой
		string time_close = doc[i]["fields"]["time_close"].GetString(); //сохраняем его значение в локальной переменной

		sch sch; //создаем структуру для хранения расписания
		sch.day_close = day_close; //заполняем структуру сохраненными значениями
		sch.day_open = day_open;
		sch.station = station;
		sch.time_close = time_close;
		sch.time_open = time_open;
		schedule.push_back(sch); //добавляем расаписание в вектор
	}
}

void trashtypeParse(unsigned int i) {
	if (!strcmp(doc[i]["model"].GetString(), "re_app.trashtype")) { //работа со структурами-trashtype
		doc[i].HasMember("pk"); //у trashtype есть поле pk
		doc[i]["pk"].IsInt(); //и оно типа int
		unsigned int pk = doc[i]["pk"].GetInt(); //сохраняем его значение в локальной перменной

		doc[i].HasMember("fields"); //у trashtype есть поле fields
		doc[i]["fields"].IsObject(); //и оно является объектом

		doc[i]["fields"].HasMember("name"); //у fields есть поле name
		doc[i]["fields"]["name"].IsString(); //и оно типа string
		string name = doc[i]["fields"]["name"].GetString(); //сохраняем его значение в локальной переменной


		doc[i]["fields"].HasMember("description"); //у fields есть поле description
		doc[i]["fields"]["description"].IsString(); //и оно типа string
		string description = doc[i]["fields"]["description"].GetString(); //сохраняем его значение в локальной переменной

		trashtyp trash; //создаем структуру
		trash.description = description; //заполняем ее сохраненными значениями
		trash.name = name;
		trash.pk = pk;
		trashtype.push_back(trash); //добавляем ее в вектор
	}
}

void stationParse(unsigned int i) {
	if (!strcmp(doc[i]["model"].GetString(), "re_app.station")) { //работа со структурами-станциями
		doc[i].HasMember("pk");
		doc[i]["pk"].IsInt();
		unsigned int pks = doc[i]["pk"].GetInt(); //id станции

		doc[i].HasMember("fields");
		doc[i]["fields"].IsObject();

		doc[i]["fields"].HasMember("street");
		doc[i]["fields"]["street"].IsInt();
		unsigned int st = doc[i]["fields"]["street"].GetInt(); //номер улицы

		doc[i]["fields"].HasMember("house");
		doc[i]["fields"]["house"].IsInt();
		unsigned int h = doc[i]["fields"]["house"].GetInt(); //номер дома в int
		string hs = to_string(h); //номер дома в string


		doc[i]["fields"].HasMember("building");
		doc[i]["fields"]["building"].IsInt();
		unsigned int bd = doc[i]["fields"]["building"].GetInt(); //корпус в int
		string b = to_string(bd);  //корпус в string

		if (bd != 0) {
			address.insert(pair<unsigned int, string>(pks, streets.find(st)->second + ", " + hs + ", " + b));  //город, улица, номер дома, корпус  (адрес)
		}
		else {
			address.insert(pair<unsigned int, string>(pks, streets.find(st)->second + ", " + hs)); //город, улица, номер дома  (адрес)
		}
		string s1 = address.find(pks)->second;  //добавление адреса
		Value v1(s1.c_str(), doc.GetAllocator());
		doc[i]["fields"].AddMember("address", v1, doc.GetAllocator());
	}
}

void correctDoc() { //корректировка doc
	for (unsigned int i = 0; i < doc.Size() - 1; i++) {
		Value vv1(kObjectType);
		doc[i]["fields"].AddMember("schedule", vv1, doc.GetAllocator()); //добавление поля "schedule"
		Value vv2(kObjectType);
		doc[i]["fields"].AddMember("trash", vv2, doc.GetAllocator());//добавление поля "trash"
		doc[i]["fields"].EraseMember("street"); //удаление всех ненужных полей
		doc[i]["fields"].EraseMember("house");
		doc[i]["fields"].EraseMember("building");
		doc[i]["fields"].EraseMember("raiting");
		doc[i]["fields"].EraseMember("nof_use");
		doc[i]["fields"].EraseMember("add_date");
		doc[i]["fields"].EraseMember("update_date");
		doc[i]["fields"].EraseMember("show");
	}
}

void addFields() { //добавление полей
	for (unsigned int i = 0; i < doc.Size() - 1; i++) {
		if (!strcmp(doc[i]["model"].GetString(), "re_app.station")) {
			doc[i].HasMember("pk");
			doc[i]["pk"].IsInt();
			unsigned int pks = doc[i]["pk"].GetInt(); //id станции
			for (unsigned int j = 0; j < trashstation.size(); j++) { //добавление мусора в станции
				if (trashstation[j].station == pks) {
					unsigned int q = trashstation[j].trashtype;
					string s2 = trashtype[q - 1].name;
					string s3 = trashtype[q - 1].description;
					Value v2(s2.c_str(), doc.GetAllocator());
					Value v3(s3.c_str(), doc.GetAllocator());
					doc[i]["fields"]["trash"].AddMember(v2, v3, doc.GetAllocator());
				}
			}
			for (unsigned int j = 0; j < schedule.size(); j++) { //добавление расписания в станции
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
	doc.Erase(doc.End() - 1); //удаление последнего элемента
}

void parse() {
	for (unsigned int i = 0; i < doc.Size() - 1; i++) {
		//начинаем проходиться по элементам массива (элементами являются различные объекты)
		assert(doc[i].IsObject()); //объявляем, что элемент массива является объектом
		assert(doc[i].HasMember("model")); //говорим, что в этом объекте есть поле model
		assert(doc[i]["model"].IsString()); //и оно типа string

		cityParse(i);
		streetParse(i);
		trashatationParse(i);
		scheduleParse(i);
		trashtypeParse(i);
		stationParse(i);

		if (strcmp(doc[i]["model"].GetString(), "re_app.station")) //удаляем все элементы массива, которые не являются пунктами приема мусора
		{
			doc.Erase(&doc[i]);
			i--;
		}

	}
}

int main(void) {
	setlocale(LC_ALL, "rus"); //русификатор	
	getDocument(); //ввод документа из файла
	assert(doc.IsArray()); //сказали, что корнем DOM-дерева является массив
	parse(); //записали все нужные значения из прочих объектов
	corDoc(); //приведение doc в нужный вид
	writeDocument(); //вывод документа в файл
	system("pause");
	return 0;
}