#include <QCoreApplication>
#include <QTextStream>
#include <QTextCodec>

#include "lib.cpp"

void log(const QString text){
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM 866"));
    QTextStream cout(stdout);
    cout << text;
}


static std::vector<QString> arrayInput  = {
    "Карта",
    "Подтверждение #1",
    "Некорректный пароль #1",
    "Корректный пароль #1",
    "Таймер #1",
    "Подтверждение #2",
    "Некорректный пароль #2",
    "Корректный пароль #2",
    "Таймер #2",
    "Подтверждение #3",
    "Некорректный пароль #3",
    "Корректный пароль #3",
    "Таймер #3",
    "Запрос баланса",
    "Внести сумму",
    "Выдать сумму",
    "Продолжить #1",
    "Продолжить #2",
    "Продолжить #3",
    "Подтверждение #4",
    "Подтверждение #5",
    "Таймер #4",
    "Выход #1",
    "Выход #2",
    "Выход #3"
};

static std::vector<QString> arrayOutput  = {
    "Принятие карты",
    "Подтверждение правильности пароля",
    "Ожидание 5 сек.",
    "Открытие окна с информацией о балансе",
    "Открытие купюроприемника",
    "Открытие окна с полем ввода суммы",
    "Выдача карты клиенту"
};

static std::vector<QString> arrayCondition = {
    "Начало",
    "Ввод пароля #1",
    "Авторизация #1",
    "Сообщение об ошибке #1",
    "Ввод пароля #2",
    "Авторизация #2",
    "Сообщение об ошибке #2",
    "Ввод пароля #3",
    "Авторизация #3",
    "Сообщение о блокировке",
    "Меню",
    "Вывод сообщения о балансе",
    "Окно с количеством принятой суммы",
    "Окно с полем ввода для суммы",
    "Выдача денег",
    "Сообщение о принятии суммы",
    "Сообщение о выдаче суммы"
};

static std::vector<std::vector<unsigned>> jumpTable = {
    {2,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17},  //1
    {1,3,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17},  //2
    {1,2,4,4,5,6,7,8,9,10,11,12,13,14,15,16,17},  //3
    {1,2,11,4,5,6,7,8,9,10,11,12,13,14,15,16,17}, //4
    {1,2,3,5,5,6,7,8,9,10,11,12,13,14,15,16,17},  //5
    {1,2,3,4,6,6,7,8,9,10,11,12,13,14,15,16,17},  //6
    {1,2,3,4,5,7,7,8,9,10,11,12,13,14,15,16,17},  //7
    {1,2,3,4,5,11,7,8,9,10,11,12,13,14,15,16,17}, //8
    {1,2,3,4,5,6,8,8,9,10,11,12,13,14,15,16,17},  //9
    {1,2,3,4,5,6,7,9,9,10,11,12,13,14,15,16,17},  //10
    {1,2,3,4,5,6,7,8,10,10,11,12,13,14,15,16,17}, //11
    {1,2,3,4,5,6,7,8,11,10,11,12,13,14,15,16,17}, //12
    {1,2,3,4,5,6,7,8,9,1,11,12,13,14,15,16,17},   //13
    {1,2,3,4,5,6,7,8,9,10,12,12,13,14,15,16,17},  //14
    {1,2,3,4,5,6,7,8,9,10,13,12,13,14,15,16,17},  //15
    {1,2,3,4,5,6,7,8,9,10,14,12,13,14,15,16,17},  //16
    {1,2,3,4,5,6,7,8,9,10,11,11,13,14,15,16,17},  //17
    {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,11,17},  //18
    {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,11},  //19
    {1,2,3,4,5,6,7,8,9,10,11,12,16,14,15,16,17},  //20
    {1,2,3,4,5,6,7,8,9,10,11,12,13,15,15,16,17},  //21
    {1,2,3,4,5,6,7,8,9,10,11,12,13,14,17,16,17},  //22
    {1,2,3,4,5,6,7,8,9,10,11,1,13,14,15,16,17},   //23
    {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,1,17},   //24
    {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,1},   //25
};

static std::vector<std::vector<unsigned>> outputTable = {
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  //1
    {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  //2
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  //3
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  //4
    {0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0},  //5
    {0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0},  //6
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  //7
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //8
    {0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0},  //9
    {0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0},  //10
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //11
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //12
    {0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0},  //13
    {0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0}, //14
    {0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0}, //15
    {0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0}, //16
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //17
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //18
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //19
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //20
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //21
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //22
    {0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0},  //23
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0},  //24
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},  //25
};


static QTextStream cin(stdin);

void jump(unsigned index){
    int select = 0, currentSelect = 0;
    unsigned newCondition;
    std::vector<unsigned> selectArray;
    std::vector<unsigned> outputArray;


    log("Текущее состояние: "+arrayCondition[index]+"\n");
    log("Действия:\n");

    for(unsigned i = 0; i < jumpTable.size(); ++i){
        if((index+1) != jumpTable[i][index]) {
            log(QString::number(select+1) + ". " + arrayInput[i]+"\n");
            select++;
            selectArray.push_back(jumpTable[i][index]);
            outputArray.push_back(i);
        }
    }


repeatEnter:
    log("Выберите действие: ");
    cin >> currentSelect;

    if(currentSelect < 1 || currentSelect > select){
        log("Неверные входные данные\n");
        goto repeatEnter;
    }

    newCondition = selectArray[currentSelect-1]-1;

    log("\nПереход из состояния \""+arrayCondition[index]+"\" в состояние \""+arrayCondition[newCondition]+"\"\n");


    if(outputTable[outputArray[currentSelect-1]][index] != 0) log("Выходные данные: "+arrayOutput[outputTable[outputArray[currentSelect-1]][index]-1]+"\n\n");
        else log("\n");


    log("------------------------------\n\n");
    jump(newCondition);
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    jump(0);
    return a.exec();
}




















