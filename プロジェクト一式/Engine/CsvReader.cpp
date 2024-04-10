#include <Windows.h>
#include "CsvReader.h"
#include <fstream>

//コンストラクタ
CsvReader::CsvReader()
{
	data_.clear();
}

//デストラクタ
CsvReader::~CsvReader()
{
	//全データを開放
	for (int y = 0; y < data_.size(); y++)
	{
		for (int x = 0; x < data_[y].size(); x++)
		{
			data_[y][x].clear();
		}
	}
}

//CSVファイルのロード
bool CsvReader::Load(std::string fileName)
{
	data_.clear();

	std::ifstream ifs(fileName);
	if (!ifs) return false;

	// BOM Skipする
	unsigned char BOMS[] = { 0xEF, 0xBB, 0xBF };
	bool found = true;
	for (int i = 0; i < 3; i++) {
		if (ifs.get() != BOMS[i]) {
			found = false;
			break;
		}
	}
	if (!found)
		ifs.seekg(std::ios_base::beg);

	// データを読む
	std::string lineStr;
	while (getline(ifs, lineStr)) {
		while (true) {
			int dq = 0;
			for (int i = 0; i < lineStr.size(); i++) {
				if (lineStr[i] == '"')
					dq++;
			}
			if (dq % 2 == 0)
				break;
			std::string s;
			getline(ifs, s);
			lineStr += "\n" + s;
		}
		for (auto it = lineStr.begin(); it != lineStr.end();) {
			if (*it == '"')
				it = lineStr.erase(it);
			if (it != lineStr.end())
				it++;
		}

		// 行内を,で切り分ける
		std::vector<std::string> record;
		int top = 0;
		bool indq = false;
		for (int n = 0; n < lineStr.size(); n++) {
			if (lineStr[n] == ',') {
				if (!indq) {
					record.emplace_back(lineStr.substr(top, (size_t)(n - top)));
					top = n + 1;
				}
			}
			else if (lineStr[n] == '"')
				indq = !indq;
		}
		record.emplace_back(lineStr.substr(top, lineStr.size() - top));
		data_.emplace_back(record);
	}
	ifs.close();
	return true;
}

//「,」か「改行」までの文字列を取得
void CsvReader::GetToComma(std::string *result, std::string data, DWORD* index)
{
	//「,」まで一文字ずつresultに入れる
	while (data[*index] != ',' && data[*index] != '\n'&& data[*index] != '\r')
	{
		*result += data[*index];
		(*index)++;
	}

	//最後に「\0」を付ける
	*result += '\0';
	(*index)++;
}

//指定した位置のデータを文字列で取得
std::string CsvReader::GetString(DWORD x, DWORD y)
{
	if (x < 0 || x >= GetWidth() || y < 0 || y >= GetHeight())
		return "";

	return data_[y][x];
}

//指定した位置のデータを整数で取得
int CsvReader::GetValue(DWORD x, DWORD y)
{
	return atoi(GetString(x, y).c_str());
}

//ファイルの列数を取得
size_t CsvReader::GetWidth()
{
	return data_[0].size();
}

//ファイルの行数を取得
size_t CsvReader::GetHeight()
{
	return data_.size();
}