#include "block.h"
#include "variable.h"

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

const static string INPUT_START_LINE = "input";
const static string INPUT_END_LINE = "/input";
const static string VAR_START_LINE = "var";
const static string VAR_END_LINE = "/var";
const static string CONSTRAINTS_START_LINE = "constraints";
const static string CONSTRAINTS_END_LINE = "/constraints";


class Config {
	friend class TestConfig;
	public:
        Config();
		void OpenFile(char *filename);
		vector<string> GetBinaries();
	//TODO: naming
		string GetInput();
	private:
		void Parse(ifstream& config_file);
		void ParseVar(string input);
		void ParseInput(string input);
		void ParseConstraints(string input);
		
		vector<string> binaries;
		unordered_map<string,shared_ptr<Variable>> vars;
		unique_ptr<BlockComposition> rootBlock;
};
