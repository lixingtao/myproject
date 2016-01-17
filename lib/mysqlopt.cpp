#include "mysqlopt.h"

int Mysql::explode(char s[], std::vector<std::string> &vet, char split_ch) { //'\t' 分割字符川
    //vet.clear();
    std::vector<std::string>().swap(vet);
    std::string str = "";
    for(int i=0; i < strlen(s); i++) {
        if(s[i] == split_ch) {
            vet.push_back(str);
            str = "";
        } else {
            str = str + s[i];
        }
    }

    vet.push_back(str);
    return vet.size();
}


int Mysql::select(char *sql, std::vector<std::vector<std::string> >&vet, int field_size) {
    MYSQL_ROW m_row;
    MYSQL_RES *m_res;
    int rnum = 0;

    if(mysql_query(_mysql,sql) != 0)
    {
        sql_msg = "select ps_info Error";
        return sql_err = select_err;
    }
    m_res = mysql_store_result(_mysql);

    if(m_res==NULL)
    {
        sql_msg = "select username Error";
        return sql_err = select_err;
    }

    while(m_row = mysql_fetch_row(m_res))
    {
        vet.push_back(std::vector<std::string>());
        for(int i = 0;i < field_size;i++)
        {
            if(m_row[i] != NULL)vet[vet.size()-1].push_back(m_row[i]);
            else vet[vet.size()-1].push_back("");
        }
        rnum++;
    }

    mysql_free_result(m_res);

    return 0;
}

