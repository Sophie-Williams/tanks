#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <conio.h>

using namespace std;

    bool wyjscie=true;
    bool wGrze=true;
    bool game=true;
    int speed_player=1;
    int number = 3;
    bool fire_ch=true;
    bool izmen=true;
    long long x=15,y=30,napravlenie=1,bx,by;

void SetWindow(int Width, int Height)       //system
{
    _COORD coord;
    coord.X = Width;
    coord.Y = Height;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle
    SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size
    SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size
    }

void gotoxy(int x, int y)                   //system
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Logo()                                 //done
{
    cout << R"(
   __|                        __ __|             |
  (      _| _` | _  /  |  |      |   _` |    \   | / (_-<
 \___| _| \__,_| ___| \_, |     _| \__,_| _| _| _\_\ ___/
                      ___/
    )" << '\n';
    Sleep(3000);
    system("CLS");
    cout << R"(
      _ _
 |_   _| |__   _ _     _ _   _
   | | | '_ \ / _ \  / _` |/ _` | '_ ` _ \ / _ \
   | | | | | |  __/ | (_| | (_| | | | | | |  __/
   |_| |_| |_|\___|  \__, |\__,_|_| |_| |_|\___|
                     |___/
    )" << '\n';
    Sleep(1500);
    system("CLS");
}

class mainPlayerObject                      //done
{
public:
    int x_coordinate=44;
    int y_coordinate=19;
    int heat_point=3;
    int speed=1;
    int shot_Direction=0;
};

class enemyObject                           //done
{
public:
    int x_coordinate=0;
    int y_coordinate=0;
    int speed; //пока не используется
    int shot_Direction;
};

class bulletObject
{
public:
    int x;
    int y;
};

char getCursorChar()                        //курсор, доделання функция, изменений не требуется
{
    char c = '\0';
    CONSOLE_SCREEN_BUFFER_INFO con;
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hcon != INVALID_HANDLE_VALUE &&
        GetConsoleScreenBufferInfo(hcon,&con))
    {
        DWORD read = 0;
        if (!ReadConsoleOutputCharacterA(hcon,&c,1,
            con.dwCursorPosition,&read) || read != 1
            )
            c = '\0';
    }
    return c;
}

char readChar(int x,int y)                  //вытягивает из курсора чаровое значение, изменений не требуется
{
    gotoxy(x,y);
    char ccccc = getCursorChar();
    return ccccc;
}

void speed_change_option()                  //настройки опции скорости, настроена и не требует изменений
{
    mainPlayerObject player_speed_option;
    system("CLS");
    char c=' ';
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(48,16);
    cout<<"CURRENT SPEED: ";
    gotoxy(41,21);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
    cout<<"<- A(-)";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout<<" MOVING SPEED ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
    cout<<"(+)D ->";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(41,24);
    cout<<"   (Press ESC to go back)   ";
    while (c!=char(27))
    {
        gotoxy(54,18);
        cout<<"                    ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
        gotoxy(54,18);
        cout<<speed_player;
        gotoxy(0,0);
        c=getch();
        if (c=='d')
        {
            speed_player+=1;
        }
        if (c=='a')
        {
            if (speed_player>=2)
            {
            speed_player-=1;
            }
        }
    }
}

void gameLegende(int *life)                 //игровая легена, должна выводиться с игровым полем
{
    int L=*life;
    //mainPlayerObject life;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
    gotoxy(87,9);
    cout<<"--------------------------";
    gotoxy(91,10);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout<<"     LEGEND: ";

    gotoxy(89,12);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<char(25)<<" -> YOU";

    gotoxy(89,14);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout<<"# -> Wall";

    gotoxy(89,16);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
    cout<<char(254)<<" -> Enemy";

    gotoxy(89,18);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
    cout<<"W,S,A,D -> Moving";

    gotoxy(89,20);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
    cout<<"Space -> Shoot";

    gotoxy(89,22);
    cout<<"Your default hp's = 3";

    gotoxy(89,24);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"Heat points left: "<<L;

    gotoxy(87,26);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
    cout<<"--------------------------";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
}

void file_levelMap(string fileName)         //вывод на экран из файла уровня(стены константы) не нужно изменять
{
    string levelMap;
    ifstream file("level.txt");
        while(getline(file, levelMap)){
        cout << levelMap << endl;
        }
    //file.close();
}

void options_menu()                                             //game options, настроены и однозначно нужны
{
    char c='0';
    int wybrane=2;
    while (c!=' ')
    {
    system("CLS");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);

    gotoxy(55,14);
    cout<<char(95);
    gotoxy(54,15);
    cout<<char(95);
    gotoxy(55,15);
    cout<<char(95);
    gotoxy(56,15);
    cout<<char(95);

    gotoxy(55,28);
    cout<<char(95);
    gotoxy(54,27);
    cout<<char(95);
    gotoxy(55,27);
    cout<<char(95);
    gotoxy(56,27);
    cout<<char(95);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(49,21);
    cout<<"MOVING SPEED";
    gotoxy(49,23);
    cout<<"    BACK    ";
    while (c!=char(13))
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
        gotoxy(46,21+wybrane);
        cout<<"->";
        gotoxy(63,21+wybrane);
        cout<<"<-";
        gotoxy(0,0);
        c=getch();
        gotoxy(46,21+wybrane);
        cout<<"  ";
        gotoxy(63,21+wybrane);
        cout<<"  ";
        if (c=='s')
        {
            if (wybrane<4)
            {
                wybrane+=2;
            }
        }
        if (c=='w')
        {
            if (wybrane>0)
            {
                wybrane-=2;
            }
        }
    }
    if (wybrane==0)
    {
        speed_change_option();
        c='0';
    }
    if (wybrane==2)
    {
        c=' '; //CR
    }
    system("CLS");
    wybrane=2;
    }
}

void theGame(){
   mainPlayerObject player;
   enemyObject enemy[3];
   bulletObject player_bullet;

   for(int i=0;i<3;i++){
    enemy[i].x_coordinate=rand()%30+1;
    enemy[i].y_coordinate=rand()%15+1;
   }

    while(game=true){
            gotoxy(0,0);
            file_levelMap("level.txt");
            gameLegende(&player.heat_point);

            for(int k=0;k<3;k++){                               //вывод на экран врагов
                gotoxy(enemy[k].x_coordinate,enemy[k].y_coordinate);
                cout<<char(254);
            }

   gotoxy(player.x_coordinate,player.y_coordinate);
   cout<<char(25);

    if(_kbhit()){                                               //передвижение танка игрока
            char cc = _getch();
    if(cc=='w'&&readChar(player.x_coordinate,player.y_coordinate-speed_player)!='#'){
          player.x_coordinate=player.x_coordinate;
          player.y_coordinate=player.y_coordinate-speed_player;
          player.shot_Direction=1;
    }
    if(cc=='s'&&readChar(player.x_coordinate,player.y_coordinate+speed_player)!='#'){
          player.x_coordinate=player.x_coordinate;
          player.y_coordinate=player.y_coordinate+speed_player;
          player.shot_Direction=3;
    }
    if(cc=='a'&&readChar(player.x_coordinate-speed_player,player.y_coordinate)!='#'){
          player.x_coordinate=player.x_coordinate-speed_player;
          player.y_coordinate=player.y_coordinate;
          player.shot_Direction=4;
    }
    if(cc=='d'&&readChar(player.x_coordinate+speed_player,player.y_coordinate)!='#'){
          player.x_coordinate=player.x_coordinate+speed_player;
          player.y_coordinate=player.y_coordinate;
          player.shot_Direction=2;
    }
   /* if(cc==char(32)||fire=true){ //выстрел пули для игрока
        if(player.shot_Direction==1){
                fire=true;
            bulletObject.x=
            bulletObject.y=
        }
        if(player.shot_Direction==3){}
        if(player.shot_Direction==4){}
        if(player.shot_Direction==2){}
        }*/
    }

   system("CLS");
   gotoxy(0,0);
   file_levelMap("level.txt");
   gameLegende(&player.heat_point);

   /*if(_kbhit()){                                              //полет и взаимодействие пулы
        char c = _getch();
   if(fire=true&&c==char(32)){

   }
   }*/

for(int j=0;j<3;j++){                                           //изменение координат танков врагов, самый простой ИИ
        if(enemy[j].x_coordinate<player.x_coordinate-1){
                if(enemy[j].x_coordinate+1==enemy[j-1].x_coordinate||enemy[j].x_coordinate+1==enemy[j+1].x_coordinate||enemy[j].x_coordinate+1==enemy[j-2].x_coordinate||enemy[j].x_coordinate+1==enemy[j+2].x_coordinate){
                    enemy[j].x_coordinate=enemy[j].x_coordinate;
                }
                else{
                    enemy[j].x_coordinate+=1;
                }
        }
        if(enemy[j].x_coordinate>player.x_coordinate+1){
                if(enemy[j].x_coordinate-1==enemy[j-1].x_coordinate||enemy[j].x_coordinate-1==enemy[j+1].x_coordinate||enemy[j].x_coordinate-1==enemy[j-2].x_coordinate||enemy[j].x_coordinate-1==enemy[j+2].x_coordinate){
                    enemy[j].x_coordinate=enemy[j].x_coordinate;
                }
                else{
                    enemy[j].x_coordinate-=1;
                }
        }
        if(enemy[j].y_coordinate<player.y_coordinate-1){
                if(enemy[j].y_coordinate+1==enemy[j-1].y_coordinate||enemy[j].y_coordinate+1==enemy[j+1].y_coordinate||enemy[j].y_coordinate+1==enemy[j-2].y_coordinate||enemy[j].y_coordinate+1==enemy[j+2].y_coordinate){
                    enemy[j].y_coordinate=enemy[j].y_coordinate;
                }
                else{
                    enemy[j].y_coordinate+=1;
                }
        }
        if(enemy[j].y_coordinate>player.y_coordinate+1){
                if(enemy[j].y_coordinate-1==enemy[j-1].y_coordinate||enemy[j].y_coordinate-1==enemy[j+1].y_coordinate||enemy[j].y_coordinate-1==enemy[j-2].y_coordinate||enemy[j].y_coordinate-1==enemy[j+2].y_coordinate){
                    enemy[j].y_coordinate=enemy[j].y_coordinate;
                }
                else{
                    enemy[j].y_coordinate-=1;
                }
        }
    }

   for(int k=0;k<3;k++){                                        //вывод на экран врагов + механика при столкновении с врагом
       if(enemy[k].x_coordinate==player.x_coordinate&&enemy[k].y_coordinate==player.y_coordinate){
        player.heat_point-=1;
        if(player.heat_point<=0){
            system("CLS");
            gotoxy(44,19);
            cout<<"YOU LOSE!!!";
            Sleep(10000);
        }
        else{
            player.x_coordinate=34;
            player.y_coordinate=19;
        }
       }
       else{
                gotoxy(enemy[k].x_coordinate,enemy[k].y_coordinate);
                cout<<char(254);}
            }

   gotoxy(player.x_coordinate,player.y_coordinate);
   cout<<char(25);
    }
}

void menu()                                                     //главное меню, настроенное и однозначно нужное
{
    char c='0';
    int wybrane=2;
    bool wyjscie=false;
    bool wGrze = false;
    system("CLS");
    while ((wGrze==false) && (wyjscie==false))
    {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    gotoxy(55,16);
    cout<<char(24);
    gotoxy(55,17);
    cout<<"W";

    gotoxy(55,28);
    cout<<char(25);
    gotoxy(55,27);
    cout<<"S";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(52,19);
    cout<<" START ";
    gotoxy(52,21);
    cout<<"OPTIONS";
    gotoxy(52,23);
    cout<<" ABOUT  ";
    gotoxy(52,25);
    cout<<" QUIT  ";
    while (c!=char(13)) //10 - подача линии, а 13 - возврат каретки.
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
        gotoxy(49,19+wybrane);
        cout<<"->";
        gotoxy(60,19+wybrane);
        cout<<"<-";
        gotoxy(0,0);
        c=getch();
        gotoxy(49,19+wybrane);
        cout<<"  ";
        gotoxy(60,19+wybrane);
        cout<<"  ";
        if (c=='s')
        {
            if (wybrane<6)
            {
                wybrane+=2;
            }
        }
        if (c=='w')
        {
            if (wybrane>0)
            {
                wybrane-=2;
            }
        }
    }
    if (wybrane==0)
    {
        wGrze=true;  //game
        system("CLS");
        Sleep(200);
        theGame();
    }
    if (wybrane==2)
    {
        options_menu(); //options
    }
    if (wybrane==4)
    {
        wGrze=false;
        wyjscie=true;
        system("CLS");
        gotoxy(0,0);
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdOut,FOREGROUND_RED);
        cout<<"CrazyTanks"<<endl;
        cout<<"Press ESC to return to main menu!"<<endl;
        system("pause");
        while (c!=char(27))
            {
                c=getch();
            }
            menu();
    }
    if (wybrane==6)
    {
        wGrze=false;
        wyjscie=true;
    }
    wybrane=2;
    c=' ';
    system("CLS");
    }
}

int main()
{
    SetWindow(113,44);
    Logo();
    system("CLS");
    menu();
    return 0;
}
