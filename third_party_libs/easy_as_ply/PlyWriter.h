#ifndef EASY_AS_PLY_WRITER_H
#define EASY_AS_PLY_WRITER_H

#include "PlyFormat.h"
#include "PlyData.h"
#include <ostream>
#include <fstream>

namespace EasyAsPLY
{
	class PlyWriter
	{
	public:
		PlyWriter();
		~PlyWriter();

		void Open(PlyFormat* format, PlyData* data);
		void Close();
		void Write(std::string filepath, std::initializer_list<std::string> comments = {});
		void Write(std::ostream* out, std::initializer_list<std::string> comments = {});

	private:
		void WriteHeader(std::ostream* out, std::initializer_list<std::string> comments);
		void WriteData(std::ostream* out);

		PlyFormat* format;
		PlyData* data;
	};
}

#endif