#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <sys/time.h>
#include <iostream>
#include <mysql/mysql.h>
#include "error.h"


const int sql_maxlen = 10000;
struct Mysql_conf {
    char ip[80];
    int port;
    char db_name[80];
    char user_name[80];
    char password[80];
};

class Mysql {
    private:
        MYSQL * _mysql;
        std::string sql_msg;
        Errno sql_err;
    public:
        Mysql() {
            _mysql = new MYSQL();
        }
        virtual ~Mysql() {
            delete _mysql;
        }

        Errno init(Mysql_conf * mysql_conf) {
            mysql_init(_mysql);
            if(NULL == _mysql) {
                return sql_err = init_err;
            }
            mysql_options(_mysql, MYSQL_SET_CHARSET_NAME, "utf8");
            char reconnect_value = 1;
            mysql_options(_mysql, MYSQL_OPT_RECONNECT, (char*)&reconnect_value);

            if (!mysql_real_connect(_mysql, mysql_conf->ip, mysql_conf->user_name,
                mysql_conf->password, mysql_conf->db_name, mysql_conf->port, NULL, 0)) {
                return sql_err = init_err;
            }
            return 0;
        }

        Errno insert(char sql[]) {
            int ret;
            ret = mysql_query(_mysql, sql);
            if(ret) {
                sql_msg = sql;
                return sql_err = insert_err;
            }
            return 0;
        }
        Errno update(char sql[]) {
            int ret;
            ret = mysql_query(_mysql, sql);
            if(ret) {
                sql_msg = sql;
                return sql_err = update_err;
            }
            return 0;
        }
        Errno del(char sql[]) {
            int ret;
            ret = mysql_query(_mysql, sql);
            if(ret) {
                sql_msg = sql;
                return sql_err = delete_err;
            }
            return 0;
        }
        int select(char *sql, std::vector<std::vector<std::string> >&ret, int field_size);

        int explode(char s[], std::vector<std::string> &vet, char split_ch = '\t'); //分割失败返回-1,成功返回分割的个数
};

