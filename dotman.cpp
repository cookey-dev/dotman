#include <pwd.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <sys/types.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

class Dotman {
public:
    Dotman(int argc, char **argv) {
        cout << argc << endl;
    }
private:
};

int main(int argc, char **argv) {
    Dotman dman(argc, argv);
    return 0;
}
