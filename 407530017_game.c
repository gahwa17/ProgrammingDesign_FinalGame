#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define False 0
#define True  1
#define MAX_NUMBER 1000

char** create_map();
void print_map();

void setup_village();
void setup_monster();
void setup_player();
void setup_trap();

int check_boundary();
int check_availability();

void go_up();
void go_down();
void go_left();
void go_right();
int check_move();

int encounter_trap();

int main(void){
    //Map's data & ptr
    int maprow = MAX_NUMBER;
    int mapcol = MAX_NUMBER;
    int *ptr_maprow = &maprow;
    int *ptr_mapcol = &mapcol;
    //Player's position
    int Prow = 0;
    int Pcol = 0;
    int *ptr_Prow = &Prow;
    int *ptr_Pcol = &Pcol;

    //Player's ability
    int HP = 10;
    // int *ptr_HP = &HP;
    // int ACK = 2;
    // int *ptr_ACK = &ACK;

    //Player's choice how to do
    int choice;

    char **map = create_map(ptr_maprow,ptr_mapcol);
    print_map(map,ptr_maprow,ptr_mapcol);

    setup_village(map, ptr_maprow,ptr_mapcol);
    setup_monster(map, ptr_maprow, ptr_mapcol);
    setup_trap(map, ptr_maprow, ptr_mapcol);
    print_map(map,ptr_maprow,ptr_mapcol);

    //是否重建地圖?
    printf("[1] Create a new map [2] Start travel the map: ");
    scanf("%d", &choice);
    while(choice == 1){
        map = create_map(ptr_maprow,ptr_mapcol);
        setup_village(map, ptr_maprow,ptr_mapcol);
        setup_monster(map, ptr_maprow, ptr_mapcol);
        print_map(map,ptr_maprow,ptr_mapcol);
        printf("[1] Create a new map [2] Start travel the map: ");
        scanf("%d", &choice);
        if(choice==2)
            break;
    }

    //玩家區塊
    setup_player(map, ptr_maprow, ptr_mapcol,ptr_Prow,ptr_Pcol);
    print_map(map,ptr_maprow,ptr_mapcol);
    while(1){
        printf("[w] go up [s] go down [d] go right [a] go left [q] exit:");
        char move;
        char flush;
        //flush掉scanf沒有完全掃完的換行符號，flush用%d印出來是10
        scanf("%c", &flush);
        scanf("%c", &move);
        switch(move){
            case 'w':
                if(check_move(Prow-1,Pcol,maprow,mapcol) == True){
                    if(encounter_trap(map,Prow-1,Pcol) == True)
                        printf("You have %d blood now.\n",--HP);
                    go_up(map,Prow,Pcol);
                    Prow--;
                }
                print_map(map,ptr_maprow,ptr_mapcol);
                break;

            case 's':
                if(check_move(Prow+1,Pcol,maprow,mapcol)==True){
                    if(encounter_trap(map,Prow+1,Pcol) == True)
                        printf("You have %d blood now.\n",--HP);
                    go_down(map,Prow,Pcol);
                    Prow++;
                }
                print_map(map,ptr_maprow,ptr_mapcol);
                break;

            case 'd':
                if(check_move(Prow,Pcol+1,maprow,mapcol)==True){
                    if(encounter_trap(map,Prow,Pcol+1) == True)
                        printf("You have %d blood now.\n",--HP);
                    go_right(map,Prow,Pcol);
                    Pcol++;
                }
                print_map(map,ptr_maprow,ptr_mapcol);
                break;

            case 'a':
                if(check_move(Prow,Pcol-1,maprow,mapcol)==True){
                    if(encounter_trap(map,Prow,Pcol-1) == True)
                        printf("You have %d blood now.\n",--HP);
                    go_left(map,Prow,Pcol);
                    Pcol--;
                }
                print_map(map,ptr_maprow,ptr_mapcol);
                break;
        }
        if(move=='q'){//  if(isTrap()==true){ enCounterTrap(userHp)}
            printf("Bye!");
            break;
        }
    }
    system("pause");
    return 0;
}

char** create_map(int *row,int *col){
    printf("Input the number of row and column for the map:\n");
    scanf("%d %d", row, col);
    char **map = (char**) malloc(*row * sizeof(char *)); //替字元指標分配記憶體
    for(int i = 0; i < *row; i++)
        map[i] = (char*) malloc((*col) * sizeof(char));

    for (int i = 0; i < *row; i++)
        for (int j = 0; j < *col; j++)
            map[i][j] = '.'; 
    return map;
}

void print_map(char **map,int *row,int *col){
    printf("=== MAP ===\n");
    for (int i = 0; i <  *row ; i++){
        for (int j = 0; j < *col; j++){
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
    printf("=== MAP ===\n");

}

void setup_village(char **map,int *row,int *col){
    printf("Input the row and column for the village location:\n");
    int Vrow ,Vcol;
    scanf("%d %d", &Vrow, &Vcol);
    //Range check
    if(!(check_boundary(Vrow, Vcol,*row,*col))){     
        scanf("%d %d", &Vrow, &Vcol);
    }
    else
        map[Vrow][Vcol] = 'v';
}

void setup_monster(char **map,int *row,int *col){
    int monster_num = (*row / 10) + (*col % 10) - 1;
    int Mrow ,Mcol;
    printf("You need to assign location for %d monsters in total\n",monster_num);
    for (int i = 1; i <= monster_num; i++){
        printf("Input the row and column for monster[%d]:\n", i);
        scanf("%d %d", &Mrow, &Mcol);

        //Range check
        if(!(check_boundary(Mrow, Mcol,*row,*col))){     
            i--;
        }
        //Occupied cheack
        else if(!(check_availability(map,Mrow, Mcol)))
            i--;
        else
            map[Mrow][Mcol] = 'm';
    }
}

void setup_trap(char **map,int *row,int *col){
    int trap_num = (*row / 10) + (*col % 10) - 1;
    int Trow ,Tcol;
    printf("You need to assign location for %d traps in total\n",trap_num);
    for (int i = 1; i <= trap_num; i++){
        printf("Input the row and column for trap[%d]:\n", i);
        scanf("%d %d", &Trow, &Tcol);

        //Range check
        if(!(check_boundary(Trow, Tcol,*row,*col))){     
            i--;
        }
        //Occupied cheack
        else if(!(check_availability(map,Trow, Tcol)))
            i--;
        else
            map[Trow][Tcol] = 't';
    }
}

void setup_player(char **map,int *row,int *col,int *Prow,int *Pcol){
    printf("Input the row and column for player:\n");
    scanf("%d %d", Prow, Pcol);
    int isValid = False;
    while(isValid == False){
        if(check_boundary(*Prow, *Pcol,*row,*col)== False || check_availability(map,*Prow, *Pcol) == False){
            printf("Input the new row and column for player:\n");
            scanf("%d %d", Prow, Pcol);
        }
        else
            isValid = True;
    }
    map[*Prow][*Pcol] = 'p';
}

//判斷是否超出地圖範圍
int check_boundary(int check_row,int check_col,int row,int col){
    //超出範圍，回傳0
    if(check_row > ((row) - 1) || check_col > ((col) - 1) || check_row < 0 || check_col < 0){
        printf("the location is outside the map\n");
        return False;
    }
    else
        return True;
}

//判斷是否已經被佔領過，已佔領過回傳False
int check_availability(char **map,int check_row,int check_col){
    if(map[check_row][check_col] == 'm' || map[check_row][check_col] == 'v' || map[check_row][check_col] == 't'){
        printf("the location is occupied\n");
        return False;
    }
    else
        return True;   
}

//玩家移動
void go_up(char **map,int Prow,int Pcol){
    map[Prow][Pcol] = '.';
    map[Prow-1][Pcol] = 'p';
}
void go_down(char **map,int Prow,int Pcol){
    map[Prow][Pcol] = '.';
    map[Prow+1][Pcol] = 'p';
}
void go_left(char **map,int Prow,int Pcol){
    map[Prow][Pcol] = '.';
    map[Prow][Pcol-1] = 'p';
}
void go_right(char **map,int Prow,int Pcol){
    map[Prow][Pcol] = '.';
    map[Prow][Pcol+1] = 'p';
}

//丟入玩家位址、地圖行列，用以判斷目前玩家移動軌跡是否超出地圖範圍
int check_move(int Prow,int Pcol,int row,int col){
    if(Prow > (row - 1) || Pcol > (col - 1) || Prow < 0 || Pcol < 0){
        printf("You can't go there\n");
        return False;
    }
    return True;
}

//傳入地圖，玩家位址，判斷是否採到陷阱，踩過的陷阱會變回'.'
int encounter_trap(char **map,int Prow,int Pcol){
    if(map[Prow][Pcol] == 't'){
        printf("You encounter a trap and lose 1 blood QAQ !\n");
        map[Prow][Pcol] = '.';
        return True;
    }
    else
        return False;
}