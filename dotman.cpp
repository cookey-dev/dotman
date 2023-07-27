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
        ex_code = 0;
        vector<string> args(argv + 1, argv + argc);
        if (init_home()) return;
        if (load_config()) return;
    }
    int ex_code;
private:
    vector<string> args;

    bool rw_ok(fs::path path) {
        int fd = access(path.string().c_str(), F_OK & R_OK & W_OK);
        return fd >= 0;
    }

    fs::path home_dir;
    bool init_home() {
        char *home_env = getenv("HOME");
        char *home_pwd = getpwuid(getuid())->pw_dir;
        if ((string)home_env != (string)home_pwd) {
            cerr << "Warning: HOME environment variable is different from passwd entry, defaulting to $HOME" << endl;
        }
        if (home_env != NULL && rw_ok(home_env)) {
            fs::path home_dir(home_env);
        } else if (home_pwd != NULL && rw_ok(home_pwd)) {
            fs::path home_dir(home_pwd);
        } else {
            cerr << "Error: HOME environment variable and passwd entry are nonexistent or inaccessible" << endl;
            ex_code = 1;
            return true;
        }
        return false;
    }

    fs::path config_path;
    json config;
    bool load_config() {
        config_path = home_dir / "dotman.config.json";
        if (!fs::exists(config_path)) return false;
        if (!rw_ok(config_path)) {
            cerr << "Error: Config file in inaccessible (Missing adequate permissions)" << endl;
            ex_code = 66;
            return true;
        }
        ifstream configf(config_path.string());
        config = json::parse(configf);
        return false;
    }
};

int main(int argc, char **argv) {
    Dotman dman(argc, argv);
    
    return dman.ex_code;
}
