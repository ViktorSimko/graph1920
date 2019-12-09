#pragma once

#include <string>
#include <fstream>
#include <streambuf>

#include "json.hpp"

using namespace std;

// https://github.com/nlohmann/json
using json = nlohmann::json;

class JsonHandler {
private:
	static const int PRETTY_PRINT_SPACES = 4;
public:
	static string JsonToString(json json) {
		return json.dump(PRETTY_PRINT_SPACES);
	}

	static json StringToJson(string str) {
		return json::parse(str);
	}

	static json ReadJson(string fileName) {
		std::ifstream s("data.json");
		std::string str((std::istreambuf_iterator<char>(s)), std::istreambuf_iterator<char>());

		json j = json::parse(str);

		s.close();

		return j;
	}

	static void WriteJson(string fileName, string json) {
		std::ofstream s(fileName);

		s << json;

		s.close();
	}
	
	static void WriteJson(string fileName, json json) {
		WriteJson(fileName, JsonToString(json));
	}
};