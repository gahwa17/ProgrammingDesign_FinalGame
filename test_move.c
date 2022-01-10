#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define False 0
#define True  1
#define MAX_NUMBER 1000

char** create_map();
void print_map();
void print_player_info(); 

void setup_village();
void setup_monster();
void setup_player();
void setup_trap();
void setup_gun();

int check_boundary();
int check_availability();

void go_up();
void go_down();
void go_left();
void go_right();
int check_move();

int encounter_trap();
int encounter_village();
int encounter_monster();
int encounter_gun();

void village_action();

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
    int ACK = 1;
    int money = 10;
    int DEF = 5;
    int *ptr_HP = &HP;
    int *ptr_ACK = &ACK;
    int *ptr_money = &money;
    int *ptr_DEF = &DEF;



    //Player's choice how to do
    int choice;
    char **map = create_map(ptr_maprow,ptr_mapcol);
    print_map(map,ptr_maprow,ptr_mapcol);

    setup_village(map, ptr_maprow,ptr_mapcol);
    setup_monster(map, ptr_maprow, ptr_mapcol);
    setup_trap(map, ptr_maprow, ptr_mapcol);
    setup_gun(map, ptr_maprow, ptr_mapcol);
    print_map(map,ptr_maprow,ptr_mapcol);

    //是否重建地圖?
    printf("[1] Create a new map [2] Start travel the map: ");
    scanf("%d", &choice);
    while(choice == 1){
        map = create_map(ptr_maprow,ptr_mapcol);
        setup_village(map, ptr_maprow,ptr_mapcol);
        setup_monster(map, ptr_maprow, ptr_mapcol);
        setup_trap(map, ptr_maprow, ptr_mapcol);
        setup_gun(map, ptr_maprow, ptr_mapcol);
        print_map(map,ptr_maprow,ptr_mapcol);
        printf("[1] Create a new map [2] Start travel the map: ");
        scanf("%d", &choice);
        if(choice==2)
            break;
    }

    //玩家區塊
    setup_player(map, ptr_maprow, ptr_mapcol,ptr_Prow,ptr_Pcol);
    print_map(map,ptr_maprow,ptr_mapcol);
    print_player_info(HP,ACK,DEF,money);
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
                    go_up(map,Prow,Pcol);
                    Prow--;
                    if(encounter_trap(map,Prow,Pcol) == True)
                        printf("You have %d blood now.\n",--HP);

                    printf("Debug:g[%d][%d]\n", Prow,Pcol);

                    if(encounter_gun(map,Prow,Pcol) == True){
                        ACK += 3;
                        printf("You have %d ACK now.\n",ACK);
                    }

                    if(encounter_village(map,Prow,Pcol)==True){
                        village_action(ptr_HP,ptr_ACK,ptr_money,ptr_DEF);
                    }
                }
                print_map(map,ptr_maprow,ptr_mapcol);
                break;

            case 's':
                if(check_move(Prow+1,Pcol,maprow,mapcol)==True){
                    go_down(map,Prow,Pcol);
                    Prow++;
                    if(encounter_trap(map,Prow,Pcol) == True)
                        printf("You have %d blood now.\n",--HP);
                    printf("Debug:g[%d][%d]\n", Prow,Pcol);
                    if(encounter_gun(map,Prow,Pcol) == True){
                        ACK += 3;
                        printf("You have %d ACK now.\n",ACK);
                    }
                    if(encounter_village(map,Prow,Pcol)==True){
                        village_action(ptr_HP,ptr_ACK,ptr_money,ptr_DEF);
                    }
                    
                }
                print_map(map,ptr_maprow,ptr_mapcol);
                break;

            case 'd':
                if(check_move(Prow,Pcol+1,maprow,mapcol)==True){
                    go_right(map,Prow,Pcol);
                    Pcol++;
                    if(encounter_trap(map,Prow,Pcol) == True)
                        printf("You have %d blood now.\n",--HP);
                    printf("Debug:g[%d][%d]\n", Prow,Pcol);
                    if(encounter_gun(map,Prow,Pcol) == True){
                        ACK += 3;
                        printf("You have %d ACK now.\n",ACK);
                    }
                    if(encounter_village(map,Prow,Pcol)==True){
                        village_action(ptr_HP,ptr_ACK,ptr_money,ptr_DEF);
                    }
                    
                }
                print_map(map,ptr_maprow,ptr_mapcol);
                break;

            case 'a':
                if(check_move(Prow,Pcol-1,maprow,mapcol)==True){
                    go_left(map,Prow,Pcol);
                    Pcol--;
                    if(encounter_trap(map,Prow,Pcol) == True)
                        printf("You have %d blood now.\n",--HP);
                    printf("Debug:g[%d][%d]\n", Prow,Pcol);
                    if(encounter_gun(map,Prow,Pcol) == True){
                        ACK += 3;
                        printf("You have %d ACK now.\n",ACK);
                    }
                    if(encounter_village(map,Prow,Pcol)==True){
                        village_action(ptr_HP,ptr_ACK,ptr_money,ptr_DEF);
                    }
                    
                }
                print_map(map,ptr_maprow,ptr_mapcol);
                break;
            case 'q':
                printf("Bye!");
                break;
            default:
                printf("Invalid input!\n");
                printf("[w] go up [s] go down [d] go right [a] go left [q] exit:");
                break;
        }
        if(move=='q'){
            break;
        }
    }
    // system("pause");
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

void print_player_info(int HP,int ACK,int DEF,int money){
    printf("=== PLAYER ===\n");
    printf("HP: %d\n", HP);
    printf("ACK: %d\n", ACK);
    printf("DEF: %d\n", DEF);
    printf("Money: $%d\n", money);

    // printf("Weapon: %c\n", );

    printf("=============\n");
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
    int monster_num = (*row) * (*col) / 12;
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
    int trap_num = (*row) * (*col) / 15;
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

void setup_gun(char **map,int *row,int *col){
    int gun_num = (*row) * (*col) / 20;
    int Grow ,Gcol;
    printf("You need to assign location for %d guns in total\n",gun_num);
    for (int i = 1; i <= gun_num; i++){
        printf("Input the row and column for gun[%d]:\n", i);
        scanf("%d %d", &Grow, &Gcol);

        //Range check
        if(!(check_boundary(Grow, Gcol,*row,*col))){     
            i--;
        }
        //Occupied cheack
        else if(!(check_availability(map,Grow, Gcol)))
            i--;
        else
            map[Grow][Gcol] = 'g';
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
    if(map[check_row][check_col] == 'm' || map[check_row][check_col] == 'v' || map[check_row][check_col] == 't' || map[check_row][check_col] == 'g'){
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

int encounter_village(char **map,int Prow,int Pcol){
    if(map[Prow][Pcol] == 'v'){
        printf("Welcome to a village, Adventurer!\n");
        return True;
    }
    else
        return False;
}

void village_action(int *HP,int *ACK,int *money,int *DEF){
    int stay = True;

    //Shop stuff 
    int potion = False;
    int armor = False;
    int teddy = False;

    int village_choice;
    int shop_choice = 0;
    int shop_stay = -1;
    printf("You now have $%d on you.\n",*money);

    while(stay == True){
        printf("What do you want to do in our village (1: GO to a shop;2: Go to the bar; 3:Leave the village)?");
        scanf("%d", &village_choice);
        switch(village_choice){
        case 1: //1:GO to shop
            while(shop_stay != False){
                printf("Which do you want to buy? (1: A magical strength potion; 2: An armor ; 3: A teddy bear ; 4:leave the shop.)\n");
                scanf("%d",&shop_choice);
                switch(shop_choice){
                    //Buy potion
                    case 1:
                        if(potion == True)
                            printf("You already brought a potion.\n");
                        else{
                            if((*money) == 0 || (*money) < 3)
                                printf("You have $%d,No money,No trade ! \n",*money);
                            else{
                                potion = True;
                                (*ACK)+=2;
                                (*money) -= 3;
                                printf("ACK UP! Now you have %d ACK !\n",*ACK);
                                printf("Charged $3, Now you have $%d !\n",*money);
                            }
                            
                        }
                        break;
                    //Buy armor
                    case 2:
                        if(armor == True)
                            printf("You already brought an armor.\n");
                        else{
                            if((*money) == 0 || (*money) < 3)
                                printf("You have $%d,No money,No trade ! \n",*money);
                            else{
                                armor = True;
                                (*DEF)+=2;
                                (*money) -= 3;
                                printf("DEF UP! Now you have %d DEF!\n",*DEF);
                                printf("Charged $3, Now you have $%d!\n",*money);
                            }
                            
                        }
                        break;
                    //Buy teddy bear
                    case 3:
                        if(teddy == True)
                            printf("You already brought a teddy bear.\n");
                        else{
                            if((*money) == 0 || (*money) < 5)
                                printf("You have $%d,No money,No trade ! \n",*money);
                            else{
                                teddy = True;
                                (*money) -= 5;
                                printf("A cute teddy bear,Wish it make you Lucky!\n");
                                printf("Charged $5, Now you have $%d!\n",*money);
                            }
                        }
                        break;
                    case 4:
                        printf("Good bye,Hava a nice day!\n");
                        shop_stay = False;
                        break;
                    default:
                        printf("We dont's have such thing in our shop!\n");
                        break;
                }
            }
            break;
        case 2: //2:酒吧補血
            printf("You now have $%d blood.\n",*HP);
            if((*money) == 0 || (*money) < 2){
                printf("You have $%d,No money,No trade ! \n",*money);
                break;
            }
            else{
                if( (*HP) == 10){
                    printf("Your HP is full. You don't need a beer.\n");
                    break;
                }
                else{
                    (*money) -= 2;
                    (*HP) += 1;
                    printf("Nice beer! Now you have %d HP!\n",*HP);
                    printf("Charged $2, Now you have $%d !\n",*money);
                    break;
                }
            }
            
        case 3: //離開村莊
            stay = False;
            printf("You are welcome to come back anytime!\n");
            break;
        default:
            printf("Invalid action! 1: buy a magical sword; 2: go to the bar; 3:leave the village.\n");
            break;
        }
    }
    
}

int encounter_gun(char **map,int Prow,int Pcol){
    printf("g[%d][%d]=%c\n", Prow,Pcol,map[Prow][Pcol]);
    if(map[Prow][Pcol] == 'g'){
        printf("You pick up a gun!, ACK+3!\n");
        return True;
    }
    else
        return False;
}
