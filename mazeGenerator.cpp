#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <utility>    //pair
#include <vector>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define M 27
#define N 35
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


void initialize(std::vector< std::vector<int> >& visited, std::vector< std::vector<int> >& mh, std::vector< std::vector<int> >& mv)
{
	//inicializo matriz visitadas(todas no visitadas)
	for(int i=0;i<M; i++){
		for(int j=0;j<N;j++){
			visited[i][j] = 0;
		}
	}

	//inicializo matriz horizontal (todas paredes)
	for(int i=0;i<M; i++){
		for(int j=0;j<N-1;j++){
			mh[i][j] = 0;
		}
	}


	//inicializo matriz vertical(todas paredes)
	for(int i=0;i<M-1; i++){
		for(int j=0;j<N;j++){
			mv[i][j] = 0;
		}
	}
    return;
}

bool esPosicionValida(std::pair<int,int> pos){

	int x = pos.first;
	int y = pos.second;

	if( x < N && x>=0 && y < M && y>= 0){
		return true;
		}else{
		return false;
		}

}

std::vector< std::pair<int,int> > dameVecinos(std::pair<int,int> pos)
{
	//srand (time(NULL));
	int dir = rand() % 4;

    std::vector < std::pair<int,int> > vecinos;

	for(int i=0;i<4; i++)
	{
	int posx = pos.first;
	int posy = pos.second;

	switch ( dir ) {
	case UP:
	 posy = posy - 1;
	  break;
	case DOWN:
	 posy = posy + 1;
	  // Code
	  break;
	case RIGHT:
	 posx = posx + 1;
	  // Code
	  break;
	case LEFT:
	 posx = posx - 1;
	  break;
	}

	dir = (dir + 3) % 4;

    vecinos.push_back(std::make_pair(posx,posy));
	}

	return vecinos;

}

bool right_move(int x, int pos_vecino){
	return (pos_vecino - x) == 1;
}

bool left_move(int x, int pos_vecino){
	return (pos_vecino - x) == -1;
}


bool down_move(int y, int pos_vecino){
	return (pos_vecino - y) == 1;
}

bool up_move(int y, int pos_vecino){
	return (pos_vecino - y) == -1;
}

void crear(int x , int y, std::vector< std::vector<int> >& visited, std::vector< std::vector<int> >& mh, std::vector< std::vector<int> >& mv )
{
	std::pair <int,int> pos;
	pos.first = x; pos.second = y;
	if (visited[y][x]){
		return;
	}else{
		visited[y][x] = 1;
		std::vector< std::pair<int,int> > vecinos;
		vecinos = dameVecinos(pos);
		for(int i =0; i<4;i++){
			if(esPosicionValida(vecinos[i]) && !visited[vecinos[i].second][vecinos[i].first]){
				//si no fue visitada, rompo la pared y me llamo recursivamente
				//la fila de arista a la derech es la posicion y
				if (right_move(x, vecinos[i].first)){ mh[y][x] = 1 ;}
				if (left_move(x, vecinos[i].first)){ mh[y][x-1] = 1;}
				if (up_move(y, vecinos[i].second)){ mv[y-1][x] = 1;}
				if (down_move(y, vecinos[i].second)){ mv[y][x] = 1;}
				crear(vecinos[i].first,vecinos[i].second,visited, mh, mv);
			}
		}
	}

}

void printMaze( std::vector< std::vector<int> >& mh, std::vector< std::vector<int> >& mv )
{
    int x = 0;
    int y = 0;
    struct termios oldSettings, newSettings;

    tcgetattr( fileno( stdin ), &oldSettings );
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr( fileno( stdin ), TCSANOW, &newSettings );    

    while ( 1 )
    {

std::cout << std::endl;
// Codificación: Occidental (IBM850)
unsigned char cp = 0;
/*for (int i=0; i<256; i++) {
    std::cout << i << " " << cp << std::endl;
    cp++;
}*/
cp = 219;
for(int j = 0; j < 2*(2*N+1); j++){
    std::cout << cp;
}
std::cout << std::endl;
for(int i = 0; i < M; i++){
    std::cout << cp << cp;
    for(int j = 0; j < N; j++){
        std::cout << (x==j && y==i ? ":)" : "  ");
        if (j!=N-1)
            if (mh[i][j]==0)
                std::cout << cp << cp;
            else
                std::cout << "  ";
    }
    std::cout << cp << cp << std::endl << cp << cp;
    if (i==M-1) {
        for(int j = 0; j < 2*(2*N-1); j++){
            std::cout << cp;
        }
    } else {
        for(int j = 0; j < N; j++){
            if (mv[i][j]==0)
                std::cout << cp << cp;
            else
                std::cout << "  ";
            if (j!=N-1) {
                std::cout << cp << cp;
            }
        }
    }
    std::cout << cp << cp << std::endl;
}

        fd_set set;
        struct timeval tv;

        tv.tv_sec = 10;
        tv.tv_usec = 0;

        FD_ZERO( &set );
        FD_SET( fileno( stdin ), &set );

        int res = select( fileno( stdin )+1, &set, NULL, NULL, &tv );

        if( res > 0 )
        {
            char c;
            read( fileno( stdin ), &c, 1 );
            if (c=='A' && y>0 && mv[y-1][x]) y--;
            if (c=='B' && y<M-1 && mv[y][x]) y++;
            if (c=='D' && x>0 && mh[y][x-1]) x--;
            if (c=='C' && x<N-1 && mh[y][x]) x++;

        }
        else if( res < 0 )
        {
            perror( "select error" );
            break;
        }
        else
        {
        //    printf( "Select timeout\n" );
        }
    }

    tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
}



int main()

{
    std::vector< std::vector<int> > mh(M, std::vector<int>(N-1));
    std::vector< std::vector<int> > mv(M-1, std::vector<int>(N));
    std::vector< std::vector<int> > visited(M, std::vector<int>(N));

	initialize(visited, mh, mv);

	srand (time(NULL));
	//empiezo en una posicion random

	int posx = rand() % N;
	int posy = rand() % M;

	crear(posx, posy, visited, mh, mv);
    printMaze(mh,mv);
    return 0;
}
