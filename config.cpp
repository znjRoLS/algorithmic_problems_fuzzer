//TODO: make template
inline void SetRandomVar(int x_min, int x_max, int &x) {
	x = rand() % (x_max - x_min + 1) + x_min;
}


class Variable {
	public:
		void SetUpperLimit(shared_ptr<Variable> var)
		{ upperVar = var; isUpperLimitVar = true; }
		void SetLowerLimit(shared_ptr<Variable> var)
		{ lowerVar = var; isLowerLimitVar = true;	}
		string GetValue() = 0;
		void GenerateValue() = 0;
	private:
		bool isUpperLimitVar;
		bool isLowerLimitVar;

		shared_ptr<Variable> lowerVar;
		shared_ptr<Variable> upperVar;	
};

class VariableInt: public Variable {
	public:
		override string GetValue()
		{ return itos(value); };
		override void GenerateValue();

		void SetUpperLimit(int val)
		{ upperInt = val; isUpperLimitVar = false; }
		void SetLowerLimit(int val)
		{ lowerInt = val; isLowerLimitVar = false; }

	private:
		int upperInt;
		int lowerInt;
		int value;
}

void VariableInt::GenerateValue() {
	if (isUpperLimitVar) {
		upperInt = upperVar->GetValue();
	}
	if (isLowerLimitVar) {
		lowerInt = lowerVar->GetValue();
	}

	SetRandomVar(lowerInt, upperInt, &value);
}


Config::Config(char *filename) {
	ifstream config_file(filename);

	if (!config_file.is_open()) {
		cout << "Error opening file " << filename << endl;
		exit(255);
	}

	Config::Parse(config_file);
}

void Config::Parse(ifstream& config_file) {
	string line;

	unordered_map<string,string> conf;

	while (1) {
		config_file >> line;
		
		if (line == INPUT_START_LINE) {
			stringstream input_config;
			while (1) {
				config_file >> line;
				if (line == INPUT_END_LINE) {
					config_file >> line;
					Config::ParseInput(input_config.str());
					break;
				}
				input_config << line;
			}
		}
		if (line == VAR_START_LINE) {
			stringstream input_config;
			while (1) {
				config_file >> line;
				if (line == VAR_END_LINE) {
					config_file >> line;
					Config::ParseVar(input_config.str());
					break;
				}
				input_config << line;
			}
		}

		if (line == CONSTRAINTS_START_LINE) {
			stringstream input_config;
			while (1) {
				config_file >> line;
				if (line == CONSTAINTS_END_LINE) {
					config_file >> line;
					Config::ParseConstraints(input_config.str());
					break;
				}
				input_config << line;
			}
		}


		if (config_file.eof()) break;

		vector<string> tokens = split(line, '=');
		if (tokens.size() != 2) {
			cout << "Error while parsing config!" << endl;
			exit(255);
		}

		conf[tokens[0]] = tokens[1];
	}

	string bin_prefix = conf["binary_prefix"];
	string binaries_string = conf["binaries"];

	vector<string> binaries_vector = split(binaries_string, ',');
	
	for(string& binary: binaries_vector) {
		binaries.push_back(bin_prefix + binary);
	}

}

void Config::ParseConstraints(string input) {

}

void Config::ParseVar(string input) {
	//TODO add if already parsed this section.
	
	vector<string> tokens = split(line, '=');

	vector<string> vars = split(tokens, ',');

	if (tokens[0] == "int") {
		
	}
}

void Config::ParseInput(string input) {
	
}

vector<string> Config::GetBinaries() {
	return binaries;
}


