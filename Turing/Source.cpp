#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

void input(vector<char>& alphabet, string& tape, vector<vector<string>>& commands_matrix, string fileName) {
	ifstream fin(fileName);

	int number_of_symbols;
	char symbol;
	fin >> number_of_symbols;
	for (int i = 0; i < number_of_symbols; i++) {
		fin >> symbol;
		alphabet.push_back(symbol);
	}

	getline(fin, tape);
	getline(fin, tape);

	string input, temp;
	vector<string> commands;
	while (!fin.eof()) {
		getline(fin, input);
		commands.push_back(input);
	}
	if (!commands.size()) {
		cout << "Ошибка, список команд не может быть пустым!";
		return;
	}

	int j, cond;
	for (int k = 0; k < alphabet.size(); k++) {
		commands_matrix.push_back(vector<string>());
		for (int i = 0; i < commands.size(); i++) {
			if (commands[i][0] == alphabet[k]) {
				j = 3, cond = 0;
				while (commands[i][j] != ' ') { cond = cond * 10 + (commands[i][j] - '0'); j++; }
				while (commands_matrix[k].size() <= cond) { commands_matrix[k].push_back("-"); }
				temp = commands[i].substr(j + 3, commands[i].size() - 1);
				commands_matrix[k][cond] = temp;
			}
		}
	}

	fin.close();
}

void printConfiguration(vector<char>& alphabet, string& tape, int condition, int& position) {
	int cut_to = -1, cut_from = tape.size();
	for (int i = 0; i < position; i++)
		if (tape[i] == alphabet[0]) cut_to = i;
		else break;
	for (int i = tape.size() - 1; i > position; i--)
		if (tape[i] == alphabet[0]) cut_from = i;
		else break;
	tape = tape.substr(cut_to + 1, cut_from);
	position = position - cut_to - 1;
	for (int i = 0; i < position; i++)
		cout << tape[i];
	cout << "q" << condition << "}";
	for (int i = position; i < tape.size(); i++)
		cout << tape[i];
	cout << endl;
}

void printFinalConfiguration(vector<char>& alphabet, string& tape) {
	int cut_to = -1, cut_from = tape.size();
	for (int i = 0; i < tape.size(); i++)
		if (tape[i] == alphabet[0]) cut_to = i;
		else break;
	for (int i = tape.size() - 1; i > 0; i--)
		if (tape[i] == alphabet[0]) cut_from = i;
		else break;
	if (cut_to >= cut_from) {
		cout << "Лента пустая";
		return;
	}
	tape = tape.substr(cut_to + 1, cut_from);
	cout << tape;
	if (tape == "") cout << "Лента пустая";
}

void launchProgram(vector<char>& alphabet, string& tape, vector<vector<string>>& commands_matrix) {
	int condition = 0, position = 0, viewed, new_cond, j;
	cout << "Начало работы машины Тьюринга\n";
	while (condition != 1) {
		if (position < 0) {
			tape.insert(tape.begin(), alphabet[0]);
			position++;
			continue;
		}
		if (position > tape.size() - 1) {
			tape.insert(tape.end(), alphabet[0]);
			continue;
		}

		printConfiguration(alphabet, tape, condition, position);
		viewed = 0;

		for (int i = 0; i < alphabet.size(); i++) {
			if (alphabet[i] == tape[position]) viewed = i;
		}

		if (condition > commands_matrix[viewed].size()) {
			cout << "Ошибка, не правильная команда.";
			return;
		}
		if (commands_matrix[viewed][condition] == "-") {
			cout << "Ошибка, не правильная команда.";
			return;
		}

		tape[position] = commands_matrix[viewed][condition][0];
		if (commands_matrix[viewed][condition][1] == 'R') position++;
		else if (commands_matrix[viewed][condition][1] == 'L') position--;
		else if (commands_matrix[viewed][condition][1] != 'H') { cout << "Ошибка, не известный символ передвижения коретки"; return; }
		new_cond = 0, j = 3;
		while (j < commands_matrix[viewed][condition].size()) { new_cond = new_cond * 10 + (commands_matrix[viewed][condition][j] - '0'); j++; }
		condition = new_cond;

	}
	printConfiguration(alphabet, tape, condition, position);
	cout << "Работа машины Тьюринга завершена.\nРезультат: ";
	printFinalConfiguration(alphabet, tape);
}

int main() {
	// Вид входных данных в файле input.txt (должен быть в той же директорий что и исполняемый файл)
	// e - пустой символ, q0 - начальное состояние, q1 - конечное состояние
	// 
	// n [n - количество символов в алфавите (вместе с пустым)]
	// ea1a2a3...an [1 символ - пустой символ, затем без пробела все символы алфавита]
	// a1ea2ea3e...
	// 1,q3 - a1Rq3 [пример команды, их может быть сколько угодно]
	
	setlocale(LC_ALL, "RU_ru");
	string tape;
	vector<char> alphabet;
	vector<vector<string>> matrixOfCommands;
	string fileName = "";
	cout << "Введите название файла с программой: ";
	cin >> fileName;
	input(alphabet, tape, matrixOfCommands, fileName);
	launchProgram(alphabet, tape, matrixOfCommands);
}