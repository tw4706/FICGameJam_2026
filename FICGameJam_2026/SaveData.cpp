#include "SaveData.h"
#include<fstream>
#include <filesystem>

namespace
{
    const wchar_t* kSaveDir = L"data/CSV";
    const wchar_t* kTutorialStagePath = L"data/CSV/turorial.csv";

}

namespace SaveData
{
	bool IsClearedTutorial()
	{
		std::ifstream ifs(kTutorialStagePath);
		return ifs.is_open();
	}

	void MarkTutorialCleared()
	{
		std::filesystem::create_directories(kSaveDir);
		std::ofstream ofs(kTutorialStagePath);
	}
}
