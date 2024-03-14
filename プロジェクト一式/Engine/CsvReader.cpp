#include <Windows.h>
#include "CsvReader.h"
#include <fstream>

//�R���X�g���N�^
CsvReader::CsvReader()
{
	data_.clear();
}

//�f�X�g���N�^
CsvReader::~CsvReader()
{
	//�S�f�[�^���J��
	for (int y = 0; y < data_.size(); y++)
	{
		for (int x = 0; x < data_[y].size(); x++)
		{
			data_[y][x].clear();
		}
	}
}

//CSV�t�@�C���̃��[�h
bool CsvReader::Load(std::string fileName)
{
	data_.clear();

	std::ifstream ifs(fileName);
	if (!ifs) return false;

	// BOM Skip����
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

	// �f�[�^��ǂ�
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

		// �s����,�Ő؂蕪����
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
}

//�u,�v���u���s�v�܂ł̕�������擾
void CsvReader::GetToComma(std::string *result, std::string data, DWORD* index)
{
	//�u,�v�܂ňꕶ������result�ɓ����
	while (data[*index] != ',' && data[*index] != '\n'&& data[*index] != '\r')
	{
		*result += data[*index];
		(*index)++;
	}

	//�Ō�Ɂu\0�v��t����
	*result += '\0';
	(*index)++;
}

//�w�肵���ʒu�̃f�[�^�𕶎���Ŏ擾
std::string CsvReader::GetString(DWORD x, DWORD y)
{
	if (x < 0 || x >= GetWidth() || y < 0 || y >= GetHeight())
		return "";

	return data_[y][x];
}

//�w�肵���ʒu�̃f�[�^�𐮐��Ŏ擾
int CsvReader::GetValue(DWORD x, DWORD y)
{
	return atoi(GetString(x, y).c_str());
}

//�t�@�C���̗񐔂��擾
size_t CsvReader::GetWidth()
{
	return data_[0].size();
}

//�t�@�C���̍s�����擾
size_t CsvReader::GetHeight()
{
	return data_.size();
}