#include <chrono>
#include <string>
#include <vector>
#include <thread>
#include "fstream"
#include "sstream"
#include "iostream"
#include "ios"

using namespace std;

std::string slurp(std::ifstream &File) {
    std::stringstream sstr;
    sstr << File.rdbuf();

    return sstr.str();
}


int main() {
	int flag_video = 0;
//    std::string _input;
    std::ifstream File;
    std::string inString;

    File.open("../../../video_test/machu_picchu_a_s111_non_B.265", std::ios::in);
//    File.open("../../../machu_picchu_8k_a_s111.265", std::ios::in);
//    File.open("input_non_b.265", std::ios::in);

    inString = slurp(File);

    std::ifstream fin("a.txt");
    std::string s;

    while(getline(fin, s)) {
    	cout << "Read from file" << s << endl;
    }

	return 0;
}