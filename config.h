#include "block.h"
#include "variable.h"
#include "learn_param.h"

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
const static string PARAMS_START_LINE = "params";
const static string PARAMS_END_LINE = "/params";
const static string END_LINE = "end";


class Config {
	friend class TestConfig;
	public:
        Config();
		void OpenFile(char *filename);
		vector<string> GetBinaries();
		string GetParam(string key, string defaultVal);
	//TODO: naming
		string GetInput();
		vector<LearnParam> GetLearnParameters();
	private:
		void Parse(ifstream& config_file);
	    void ParseParams(string input);
		void ParseVar(string input);
		void ParseInput(string input);
		void ParseConstraints(string input);

        unordered_map<string,string> confParams;
		
		vector<string> binaries;
		unordered_map<string,shared_ptr<Variable>> vars;
		unique_ptr<BlockComposition> rootBlock;
		vector<LearnParam> learn_parameters;
};
