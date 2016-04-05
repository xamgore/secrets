#include <string>
#include "utils.h"
#include "md5.h"
#include <ctime>
#include <cstdio>
#include <linux/limits.h>

using namespace std;

string REQUEST_METHOD, QUERY_STRING, REMOTE_ADDR, root;
map<string, string> $_GET, $_POST;


void init() {
    REQUEST_METHOD = { getenv("REQUEST_METHOD") };
    QUERY_STRING = { getenv("QUERY_STRING") };
    REMOTE_ADDR = { getenv("REMOTE_ADDR") };

    char full_path[PATH_MAX];
    realpath("./../", full_path);
    root = string(full_path);

    if (REQUEST_METHOD == "GET") {
        split_query(uri_decode(QUERY_STRING), $_GET);
    } else if (REQUEST_METHOD == "POST") {
        string query{ read_all_text(cin) };
        split_query(uri_decode(query), $_POST);
    }
}

void authorize(string login, string pass, bool errl = 0, bool errp = 0) {
    ifstream ifs(root + "/resource/views/index.twig");
    string content{ read_all_text(ifs) };

    replace_all_strings(content, "{{ login }}", escape(login));
    replace_all_strings(content, "{{ pass }}", escape(pass));
    replace_all_strings(content, "{{ errl }}", errl ? "error" : "");
    replace_all_strings(content, "{{ errp }}", errp ? "error" : "");
    replace_all_strings(content, "{{ aufl }}", errp ? "" : "autofocus");
    replace_all_strings(content, "{{ aufr }}", errp ? "autofocus" : "");

    cout << "Content-Type: text/html; charset=utf-8\n\n" << content;
}

void show_secret(string login) {
    ifstream ifs(root + "/resource/views/secret.twig");
    string content{ read_all_text(ifs) };

    ifstream efs(root + "/app/secrets/" + to_lower(login) + ".twig");
    string secret{ read_all_text(efs) };

    replace_string(content, "{{ content }}", secret);
    cout << "Content-Type: text/html; charset=utf-8\n\n" << content;
}

void visits(const string &ip, time_t &v1, time_t &v2, time_t &v3) {
    ifstream f(root + "/app/time/" + ip);

    v1 = v2 = v3 = 0;
    if (f.good()) f >> v1 >> v2 >> v3;
}

void update_visits(const string &ip) {
    time_t v1, v2, v3;
    visits(ip, v1, v2, v3);

    ofstream f(root + "/app/time/" + ip);
    f << v2 << " " << v3 << " " << time(nullptr);
}

time_t last_visit(const string &ip) {
    time_t v1, v2, v3;
    visits(ip, v1, v2, v3);
    return v1;
}

bool ip_is_banned(const string &ip) {
    time_t v1, v2, v3;
    visits(ip, v1, v2, v3);
    bool has_ban = time(nullptr) - v1 < 60;

    if (has_ban) {
        ofstream f(root + "/app/time/" + ip);
        f << v1 << " " << 0 << " " << 0;
    }

    return has_ban;
}

void show_ban() {
    ifstream f_content(root + "/resource/views/secret.twig");
    string content{ read_all_text(f_content) };

    ifstream f_secret(root + "/resource/views/banned.twig");
    string secret{ read_all_text(f_secret) };

    time_t ban_time = 60 - (time(nullptr) - last_visit(REMOTE_ADDR));
    string minutes = { ban_time == 1 ? " second" : " seconds" };

    replace_string(secret, "{{ time }}", to_string(ban_time).append(minutes));
    replace_string(content, "{{ content }}", secret);
    cout << "Content-Type: text/html; charset=utf-8\n\n" << content;
}


int main() {
    init();

    if (ip_is_banned(REMOTE_ADDR)) {
        show_ban();
        return 0;
    }

    string login{ $_POST["login"] }, password{ $_POST["pass"] };

    if (login == "" | password == "")
        authorize(login, password);
    else {
        ifstream ifs(root + "/app/users.txt");
        string data{ read_all_text(ifs) };
        map<string, string> users;
        split_query(data, users);

        // Display page
        if (users.find(to_lower(login)) == users.end()) {
            authorize(login, password, true);
        } else if (users[to_lower(login)] != md5(password)) {
            authorize(login, password, false, true);
            update_visits(REMOTE_ADDR);
        } else show_secret(login);
    };

    return 0;
}
