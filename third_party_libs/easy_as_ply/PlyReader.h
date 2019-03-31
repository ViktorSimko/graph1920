#ifndef EASY_AS_PLY_READER_H
#define EASY_AS_PLY_READER_H

#include <string>
#include <cstring>
#include <istream>

#include "PlyData.h"
#include "PlyFormat.h"

namespace EasyAsPLY
{
	class PlyReader
	{
	public:
		PlyReader();
		~PlyReader();

		void Open();
		void Close();
		bool Read(std::string filepath);
		bool Read(std::istream* in);
		PlyData* GetData() { return data; }
		PlyFormat* GetFormat() { return format; }

	private:
		bool Load(std::istream* in);
		bool ParseHeader();
		bool ParseElementFormat();
		bool ParseElements();
		bool ParseElement(PlyFormatElement* format);

		// parser utility functions
		std::string gobble();
		std::string peek();
		void nextLine();
		void skipComments();
		std::vector<std::string> tokenise(const std::string& str, const std::string& delimiter = " ");

		// parser utility variables
		int tokenPos = 0;
		int linePos = 0;
		std::vector<std::string> lines;
		std::vector<std::string> lineTokens;

		PlyData* data;
		PlyFormat* format;
	};
}
#endif