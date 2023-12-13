#include <iostream>
#include <algorithm> // для replace
#include <string> // для стринга и его плюх
#include <fstream> // для работы с файлами
#include <sstream> // для трансляции строк
#include <windows.h> // русский язык
#include <vector> // для векторов и их плюх

using namespace std;

#pragma region <Структуры>
struct Data { // Дата
	int Day = 0; // День
	int Mounth = 0; // Месяц
	int Year = 0; // Год
};
struct Clients { // Клиенты
	string Name; // Имя
	string Surname; // Фамилия
	string Otchestvo; // Отчество
	string Phone; // Номер телефона
	string Nacahlo; // Начало контракта
	string Konez; // Конец контракта
	int Dolg = 0; // Долг
	int Vdolg = 0; // Возможный долг
};
struct Service { // Услуги
	string Nservice; // Имя услуги
	int Kod = 0; // Код услуги
	float Cost = 0; // Стоимость 
	string EdIzm; // Единица измерения
};
struct PodklService { // Подключённые услуги
	string Phone; // Номер телефона
	int UseKod = 0; // Код услуги
	struct Data DataUse; // Дата использования
	string VremUse; // Время искользования
	int HowVremUSe; // Сколько времени пользовался
};
struct Report // Отчёт
{
	string usluga; // Название услуги
};
struct Parametr // Параметры
{
	int minZnach = 0; // Минимальное значение
	int maxZnach = 0; // Максимальное значение
};
#pragma endregion

// Функция загрузки данных о клиентах. В ней переданы параметры ввиде векторов, которые в свою очередь являются динамичкеским массивом
void loading(vector<Parametr>& parametri,
	vector<Clients>& consumer,
	vector <Service>& servicee,
	vector <PodklService>& provideservice)
{
	ifstream client;
	int n = 0;
	client.open("client.txt");
	ifstream service;
	service.open("service.txt");
	ifstream podklservice;
	podklservice.open("podklserv.txt");
	if (!client.is_open() || !service.is_open() || !podklservice.is_open()) // Проверяем существует ли файл
	{
		cout << "Ошибка открытия одного из файлов" << endl;
	}
	else
	{
		Clients clien;
		Service serv;
		PodklService provserv;
		string line;
		while (getline(client, line)) // Получаем строку на вход
		{
			replace(line.begin(), line.end(), ',', ' '); // Меняем запятые на пробелы
			istringstream temp(line); // Транслируем строку 
			temp >> clien.Surname // Записываем строку
				>> clien.Name
				>> clien.Otchestvo
				>> clien.Phone
				>> clien.Nacahlo
				>> clien.Konez
				>> clien.Dolg
				>> clien.Vdolg;
			consumer.push_back(clien); // Добавляем в структуру
		}
		line.clear(); // Очищаем переменную

		while (getline(service, line))
		{
			stringstream temp1(line);
			string tmp;
			int i = 0;
			while (getline(temp1, tmp, ',')) // Разбиваем строку по запятым 
			{
				if (i == 0)
				{
					serv.Nservice = tmp;
					i++;
					continue;
				}
				if (i == 1)
				{
					serv.Kod = stoi(tmp);
					i++;
					continue;
				}
				if (i == 2)
				{
					stringstream st(tmp);
					st >> serv.Cost;
					i++;
					continue;
				}
				if (i == 3)
				{
					serv.EdIzm = tmp;
					i++;
					continue;
				}
			}
			servicee.push_back(serv);
		}
		line.clear();

		while (getline(podklservice, line))
		{
			replace(line.begin(), line.end(), ',', ' ');
			replace(line.begin(), line.end(), '.', ' ');
			istringstream temp2(line);
			temp2 >> provserv.Phone
				>> provserv.UseKod
				>> provserv.DataUse.Day
				>> provserv.DataUse.Mounth
				>> provserv.DataUse.Year
				>> provserv.VremUse
				>> provserv.HowVremUSe;
			provideservice.push_back(provserv);
		}
		line.clear();

		client.close();
		service.close();
		podklservice.close();
	}

	Parametr parametrs;
	ifstream params;
	string buffer;
	params.open("parametr.txt");

	while (getline(params, buffer))
	{
		replace(buffer.begin(), buffer.end(), ',', ' ');
		istringstream temp3(buffer);
		temp3 >> parametrs.minZnach >> parametrs.maxZnach;
		parametri.push_back(parametrs);
	}
	params.close();
}

// Функция создания отчёта.
void report(vector<Parametr>& parametri,
	vector<Clients>& consumer,
	vector <Service>& servicee,
	vector <PodklService>& provideservice)
{
	vector <Report> report;
	Report temp4;

	int month = 0;
	cout << "Введите месяц для составления отчёта" << endl;
	cout << "Ваш месяц: ";
	cin >> month;
	cout << endl;

	Parametr fuka;
	ofstream otch;
	otch.open("report.txt");

	int sum = 0;

	for (int q = 0; q < parametri.size(); q++)
		for (int i = 0; i < servicee.size(); i++)
		{

			if (sum <= parametri[q].maxZnach && sum >= parametri[q].minZnach)
			{
				temp4.usluga = servicee[i].Nservice;
				report.push_back(temp4);
			}
			else
			{
				sum = 0;
				for (int k = 0; k < provideservice.size(); k++)
					if (servicee[i].Kod == provideservice[k].UseKod && provideservice[i].DataUse.Mounth == month)
						sum += provideservice[k].HowVremUSe;
			}
		}

	if (report.empty())
	{
		otch << "Нет данных." << endl;
	}
	else
	{
		otch << "Список услуг, предоставленных в " << month << " месяце, которые попадают в заданный диапозон." << endl;
		for (int i = 0; i < report.size(); i++)
		{
			otch << report[i].usluga << endl;
		}
	}
	cout << "Отчёт создан" << endl;
	otch.close();
}

int main() {
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	vector <Clients> consumer;
	vector <Service> servicee;
	vector <PodklService> provideservice;
	vector <Parametr> parametri;
	loading(parametri, consumer, servicee, provideservice);
	report(parametri, consumer, servicee, provideservice);
	return 0;
}