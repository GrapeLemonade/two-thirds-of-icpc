#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <utility>
#include <fstream>
#include <cassert>
#include <cctype>
#include <vector>
#include <array>
#include <set>

#include "core.h"
#include "main.h"

using namespace std;
namespace fs = filesystem;

const string allowed_options = "chmnrtw";
const string output_filename = "solution.txt";

int main(int argc, char* argv[]) {
	try {
		main_serve(argc, argv);
	}
	catch (invalid_argument const& e) {
		cerr << "WordList: " << e.what() << endl;
		cerr << "Usage: WordList.exe OPTION... FILE" << endl;
	}
	catch (logic_error const& e) {
		cerr << "WordList: " << e.what() << endl;
	}
	catch (runtime_error const& e) {
		cerr << "WordList: " << e.what() << endl;
	}
	return 0;
}

int main_serve(int argc, char* argv[]) {
	bool count = false;
	bool normal = false;
	bool weighted = false;
	bool file_output = false;
	bool enable_ring = false;
	bool enable_self_loop = true;
	char head = 0, tail = 0;
	if (argc <= 1) {
		throw invalid_argument("missing arguments");
	}
	set<char> options;
	string filename = "";
	for (int _ = 1; _ < argc; ++_) {
		string arg = argv[_];
		if (arg.length() == 2
			&& arg[0] == '-'
			// we always consider strings like -x as options
			// && allowed_options.find(arg[1]) != allowed_options.npos
			) {
			// option format
			if (allowed_options.find(arg[1]) != allowed_options.npos) {
				// check dupicate options
				if (options.find(arg[1]) != options.end()) {
					throw invalid_argument("option duplicated -- '" + arg.substr(1) + "'");
				}
				else {
					options.insert(arg[1]);
				}
			}
			switch (arg[1]) {
			case 'n':
				count = true;
				break;
			case 'w':
				normal = true;
				file_output = true;
				break;
			case 'm':
				enable_self_loop = false;
				file_output = true;
				break;
			case 'c':
				weighted = true;
				file_output = true;
				break;
			case 'h':
			case 't':
			{
				if (_ + 1 == argc) throw invalid_argument("option requires an argument -- '" + arg.substr(1) + "'");
				string arg2 = argv[_ + 1];
				if (arg2.length() > 1) throw invalid_argument("argument of option " + arg + " should be a single alphabet, " + arg2 + " received");
				if (!isalpha(arg2[0])) throw invalid_argument("argument of option " + arg + " should be an alphabet, " + arg2 + " received");
				// ignoring case, this must be specification now
				if (arg[1] == 'h') {
					head = arg2[0];
				}
				else {
					tail = arg2[0];
				}
				++_; // skip next argument
			}
			break;
			case 'r':
				enable_ring = true;
				break;
			default:
				throw invalid_argument("invalid option -- '" + arg.substr(1) + "'");
			}
		}
		else {
			// this must be filename
			if (filename != "") {
				throw invalid_argument("multiple text files given: \"" + filename + "\" and \"" + arg + "\"");
			}
			else {
				filename = arg;
			}
		}
	}
	// option sanity checks before run
	if (count && (normal
		|| !enable_self_loop
		|| weighted
		|| head
		|| tail
		|| enable_ring)) {
		throw invalid_argument("cannot combine -n with other options");
	}
	if (!enable_self_loop && (head || tail || enable_ring)) {
		throw invalid_argument("cannot combine -m with -h, -t or -r");
	}
	if (normal && (!enable_self_loop || weighted) || !enable_self_loop && weighted) {
		throw invalid_argument("conflicting option combinations");
	}
	if (options.empty() || !(count || file_output)) {
		throw invalid_argument("no option specified");
	}
	// let's check the file
	fs::path input_path(filename);
	if (!fs::exists(input_path)) {
		throw runtime_error(filename + ": No such file");
	}
	if (!fs::is_regular_file(input_path)) {
		throw runtime_error(filename + ": Not a regular file");
	}
	ifstream input(filename, ios::in | ios::binary | ios::ate); // seek to end
	if (!input.is_open()) {
		throw runtime_error(filename + ": Cannot open as read-only");
	}

	ios::pos_type size = input.tellg();
	input.seekg(0);

	string raw_input(size, 0);
	input.read(raw_input.data(), size);

	vector<char*> words;
	for (int i = 0, las = -1; i < size; ++i) {
		char& c = raw_input.data()[i];
		if (isalpha(c)) {
			if (i != las) words.push_back(&c);
			las = i + 1;
			c = tolower(c);
		}
		else {
			c = 0;
		}
	}

	if (words.empty()) {
		throw runtime_error(filename + ": File does not contain words");
	}

	// now we have all the words, let's have the engine whirring!
	vector<char*> result(32768, 0);

	int ret_val = engine(
		words.data(),
		(int) words.size(),
		result.data(),
		head,
		tail,
		count,
		weighted,
		enable_self_loop,
		enable_ring);

	ofstream output;
	ostream &out = file_output ? output : cout;
	if (file_output) {
		output.open(output_filename, ios::out | ios::binary | ios::trunc);
		if (!output.is_open()) {
			throw runtime_error(output_filename + ": Cannot open for writing");
		}
	}
	
	if (count) {
		out << ret_val << endl;
	}

	for (int i = 0; result[i]; ++i) {
		out << result[i] << endl;
	}

	if (file_output) {
		output.close();
	}

	// seems working?
	return 0;
}