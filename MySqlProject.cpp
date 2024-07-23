#include <iostream>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

int main()
{

    try {

        //создаём переменные для подключения к базе данных, для запроса и для ответа
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        //подключаемся к базе по ip, порту, логину и паролю
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "user", "password");

        //подключаемся к нужной таблице по названию и создаём запрос
        con->setSchema("test");
        stmt = con->createStatement();

        //выбор между входом и регистрацией
        cout << "log or reg?\n";
        string ans1;
        cin >> ans1;
        if (ans1 == "log")
        {
            //вход
            
            //вводим имя и пароль пользователя
            cout << "username: ";
            string user;
            cin >> user;
            cout << "password: ";
            string password;
            cin >> password;

            //запрос ищущий в таблице зарегистрированных пользователей пользователя с совпадающим логином и паролем
            string request = "select * from test where login='" + user + "' and password='" + password + "'";
            res = stmt->executeQuery(request);

            //если пользователь зарегистрирован, происходит вход, иначе ошибка
            if (res->next())
                cout << "welcome!\n";
            else
                cout << "wrong user data\n";

            delete res;
            delete stmt;
            delete con;
        }
        else if (ans1 == "reg")
        {
            //регистрация

            //вводим имя и пароль пользователя
            cout << "username: ";
            string user;
            cin >> user;
            cout << "password: ";
            string password;
            cin >> password;

            //запрос ищущий в таблице зарегистрированных пользователей пользователя с совпадающим логином
            string request = "select * from test where login='" + user + "'";
            res = stmt->executeQuery(request);

            //если пользователь с таким логином найден, регистрация не происходит, иначе добавляем нового пользователя в базу
            if (res->next())
                cout << "this user already exist\n";
            else
            {
                string inreq = "insert into test values('" + user + "','" + password + "')";
                stmt->execute(inreq);
                cout << "success\n";
            }
            delete res;
            delete stmt;
            delete con;
        }
    }
    catch (sql::SQLException& e) {
        //вывод данных об ошибке
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    cout << endl;

    return EXIT_SUCCESS;
}