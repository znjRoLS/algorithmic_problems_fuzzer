#include <vector>
#include <string>
#include <unordered_map>

class Config {
	public:
		Config(char *filename);
		vector<string> GetBinaries();
	private:
		void Parse(ifstream& config_file);
		void ParseInput(string input);
		
		vector<string> binaries;
		unordered_map<string,shared_ptr<Variable>> vars;
};
