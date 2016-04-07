#pragma once

#include "../n110_square__/n110_150_file.hpp"

class UtilFile {
public:
	// 左右変換
	static inline constexpr File Inverse(const File f) { return FileNum - 1 - f; }

	static inline bool ContainsOf(const File f) { return (0 <= f) && (f < FileNum); }
	static inline char ToCharUSI(const File f) { return '1' + f; }
	static inline char ToCharCSA(const File f) { return '1' + f; }
	static inline File FromCharCSA(const char c) { return static_cast<File>(c - '1'); }
	static inline File FromCharUSI(const char c) { return static_cast<File>(c - '1'); }
};