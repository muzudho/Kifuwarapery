#pragma once

#include "../../header/n260_evaluate/n260_400_EvaluaterBase.hpp"

class KppCacheIo {
public:
	std::string GetLv2FilePath(const std::string& dirName, int k1);
	std::string GetLv3FilePath(const std::string& dirName, int k1, int p1);

	bool SynthesizeLv3To2(const std::string& dirName, int k1);

	bool WriteLv3Files(const std::string& dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end]);
	void WriteLv3FilesBody(const std::string& cache3Filepath, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end]);

	bool ReadLv2Files(const std::string& dirName, int k1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end]);
	bool ReadLv3Files(const std::string& dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end]);

};