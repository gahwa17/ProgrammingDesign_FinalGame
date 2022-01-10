#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>

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
int encounter_gun();
int encounter_monster();

void village_action();

void monster_fight();
int evasion();
int isDead();


//Extra Part
int isLucky = False;

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

    //Player's info
    int HP = 10;
    int ACK = 1;
    int money = 10;
    int *ptr_HP = &HP;
    int *ptr_ACK = &ACK;
    int *ptr_money = &money;

    //Player's choice how to do
    int choice;
    int endgame = False;

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
    print_player_info(HP,ACK,money);
    while(endgame == False){
        printf("[w] go up [s] go down [d] go right [a] go left [q] exit:");
        char move,flush;
        //flush掉scanf沒有完全掃完的換行符號，flush用%d印出來是10
        scanf("%c", &flush);
        scanf("%c", &move);
        switch(move){
            case 'w':
                if(check_move(Prow-1,Pcol,maprow,mapcol) == True){
                    printf("Debug[%d][%d]\n", Prow-1,Pcol);

                    if(encounter_trap(map,Prow-1,Pcol) == True){
                        printf("You have %d blood now.\n",--HP);
                    }

                    if(encounter_gun(map,Prow-1,Pcol) == True){
                        ACK += 3;
                        printf("You have %d ACK now.\n",ACK);
                    }

                    if(encounter_monster(map,Prow-1,Pcol) == True){
                        monster_fight(ptr_ACK,ptr_HP,ptr_money);
                        if(isDead(HP) == True){
                            printf("You die!\n");
                            endgame = True;
                        }  
                        print_player_info(HP,ACK,money);
                    }
                    
                    if(encounter_village(map,Prow-1,Pcol)==True){
                        printf("Debug:V[%d][%d]\n", Prow-1,Pcol);
                        village_action(ptr_HP,ptr_ACK,ptr_money);
                    }

                    go_up(map,Prow,Pcol);
                    Prow--;
                    printf("Debug:Aftergo[%d][%d]\n", Prow,Pcol);
                }
                print_map(map,ptr_maprow,ptr_mapcol);
                break;

            case 's':
                if(check_move(Prow+1,Pcol,maprow,mapcol)==True){
                    printf("Debug[%d][%d]\n", Prow+1,Pcol);

                    if(encounter_trap(map,Prow+1,Pcol) == True){
                        printf("You have %d blood now.\n",--HP);
                    }

                    if(encounter_gun(map,Prow+1,Pcol) == True){
                        ACK += 3;
                        printf("You have %d ACK now.\n",ACK);
                    }
                    
                    if(encounter_monster(map,Prow+1,Pcol) == True){
                        monster_fight(ptr_ACK,ptr_HP,ptr_money);
                        if(isDead(HP) == True){
                            printf("You die!\n");
                            endgame = True;
                        }  
                        print_player_info(HP,ACK,money);
                    }

                    if(encounter_village(map,Prow+1,Pcol)==True){
                        village_action(ptr_HP,ptr_ACK,ptr_money);
                    }
                    
                    go_down(map,Prow,Pcol);
                    Prow++;
                    printf("Debug:Aftergo[%d][%d]\n", Prow,Pcol);
                }
                print_map(map,ptr_maprow,ptr_mapcol);
                break;

            case 'd':
                if(check_move(Prow,Pcol+1,maprow,mapcol)==True){
                    printf("Debug[%d][%d]\n", Prow,Pcol+1);

                    if(encounter_trap(map,Prow,Pcol+1) == True){
                        printf("You have %d blood now.\n",--HP);
                    }

                    if(encounter_gun(map,Prow,Pcol+1) == True){
                        ACK += 3;
                        printf("You have %d ACK now.\n",ACK);
                    }

                    if(encounter_monster(map,Prow,Pcol+1) == True){
                        monster_fight(ptr_ACK,ptr_HP,ptr_money);
                        if(isDead(HP) == True){
                            printf("You die!\n");
                            endgame = True;
                        }  
                        print_player_info(HP,ACK,money);
                    }

                    if(encounter_village(map,Prow,Pcol+1)==True){
                        village_action(ptr_HP,ptr_ACK,ptr_money);
                    }
                    go_right(map,Prow,Pcol);
                    Pcol++;
                    printf("Debug:Aftergo[%d][%d]\n", Prow,Pcol);
                }
                print_map(map,ptr_maprow,ptr_mapcol);
                break;

            case 'a':
                if(check_move(Prow,Pcol-1,maprow,mapcol)==True){
                    printf("Debug[%d][%d]\n", Prow,Pcol-1);

                    if(encounter_trap(map,Prow,Pcol-1) == True){
                        printf("You have %d blood now.\n",--HP);
                    }

                    if(encounter_gun(map,Prow,Pcol-1) == True){
                        ACK += 3;
                        printf("You have %d ACK now.\n",ACK);
                    }

                    if(encounter_monster(map,Prow,Pcol-1) == True){
                        monster_fight(ptr_ACK,ptr_HP,ptr_money);
                        if(isDead(HP) == True){
                            printf("You die!\n");
                            endgame = True;
                        }  
                        print_player_info(HP,ACK,money);
                    }

                    if(encounter_village(map,Prow,Pcol-1)==True){
                        village_action(ptr_HP,ptr_ACK,ptr_money);
                    }
                    
                    go_left(map,Prow,Pcol);
                    Pcol--;
                    printf("Debug:Aftergo[%d][%d]\n", Prow,Pcol);
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
            endgame = True;
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

void print_player_info(int HP,int ACK,int money){
    printf("=== PLAYER ===\n");
    printf("HP: %d\n", HP);
    printf("ACK: %d\n", ACK);
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
    printf("Debug:BeforeLeave[%d][%d]:%c\n", Prow,Pcol,map[Prow][Pcol]);
    //玩家離開原本的格子:若離開前發現是'I'(村莊)，則改為'v'
    if(map[Prow][Pcol] == 'I')
        map[Prow][Pcol] = 'v';
    else
        map[Prow][Pcol] = '.';
    
    //玩家走到村莊，則顯示'I'，一般格子則是p
    if(encounter_village(map,Prow-1,Pcol)==True)
        map[Prow-1][Pcol] = 'I';
    else
        map[Prow-1][Pcol] = 'p';
    
}
void go_down(char **map,int Prow,int Pcol){
    printf("Debug:BeforeLeave[%d][%d]:%c\n", Prow,Pcol,map[Prow][Pcol]);
    if(map[Prow][Pcol] == 'I')
        map[Prow][Pcol] = 'v';
    else
        map[Prow][Pcol] = '.';

    if(encounter_village(map,Prow+1,Pcol)==True)
        map[Prow+1][Pcol] = 'I';
    else
        map[Prow+1][Pcol] = 'p';
}
void go_left(char **map,int Prow,int Pcol){
    printf("Debug:BeforeLeave[%d][%d]:%c\n", Prow,Pcol,map[Prow][Pcol]);
    if(map[Prow][Pcol] == 'I')
        map[Prow][Pcol] = 'v';
    else
        map[Prow][Pcol] = '.';

    if(encounter_village(map,Prow,Pcol-1)==True)
        map[Prow][Pcol-1] = 'I';
    else
        map[Prow][Pcol-1] = 'p';
    
}
void go_right(char **map,int Prow,int Pcol){
    printf("Debug:BeforeLeave[%d][%d]:%c\n", Prow,Pcol,map[Prow][Pcol]);
    if(map[Prow][Pcol] == 'I')
        map[Prow][Pcol] = 'v';
    else
        map[Prow][Pcol] = '.';

    if(encounter_village(map,Prow,Pcol+1)==True)
        map[Prow][Pcol] = 'I';
    else
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
    printf("Debug:t[%d][%d]\n", Prow,Pcol);
    if(map[Prow][Pcol] == 't'){
        printf("You encounter a trap and lose 1 blood QAQ !\n");
        map[Prow][Pcol] = '.';
        return True;
    }
    else
        return False;
}

int encounter_village(char **map,int Prow,int Pcol){
    printf("Debug:v(in)[%d][%d]\n", Prow,Pcol);
    if(map[Prow][Pcol] == 'v'){
        printf("Welcome to a village, Adventurer!\n");
        return True;
    }
    else
        return False;
}

void village_action(int *HP,int *ACK,int *money){
    int stay = True;

    //Shop stuff 
    int potion = False;
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
                printf("Which do you want to buy? (1: A magical strength potion; 2: A teddy bear ; 3:leave the shop.)\n");
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
                    //Buy teddy bear
                    case 2:
                        if(teddy == True)
                            printf("You already brought a teddy bear.\n");
                        else{
                            if((*money) == 0 || (*money) < 5)
                                printf("You have $%d,No money,No trade ! \n",*money);
                            else{
                                teddy = True;
                                isLucky = True;
                                (*money) -= 5;
                                printf("A cute teddy bear,Wish it make you Lucky!\n");
                                printf("Charged $5, Now you have $%d!\n",*money);
                            }
                        }
                        break;
                    case 3:
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
            printf("You now have $%d.\n",*money);
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
    printf("Debug:g[%d][%d]\n", Prow,Pcol);
    if(map[Prow][Pcol] == 'g'){
        printf("You pick up a gun!, ACK+3!\n");
        return True;
    }
    else
        return False;
}

int encounter_monster(char **map,int Prow,int Pcol){
    printf("Debug:m(in)[%d][%d]\n", Prow,Pcol);
    if(map[Prow][Pcol] == 'm'){
        printf("You encounter a monster group!\n");
        return True;
    }
    else
        return False;
}

void monster_fight(int *ACK,int *HP,int *money){
    srand(time(NULL));
    int monster,monster_AGI,result = 2;
    printf("Please choose which monster (Lv.1 to Lv.5) you want to fight:");
    scanf("%d", &monster);
    while(result == 2){
        switch(monster){ 
        case 1:
        case 2: 
        case 3:
        case 4:
        case 5:
            printf("You are fighting Monster(Lv.%d)!\n",monster);
            //怪物回避率隨機產生:0~100
            monster_AGI = rand() % 101;
            if((*ACK) >= monster){
                //假設怪物沒閃避前都是贏的狀態
                result = True;
                while(evasion(monster_AGI,monster,HP) != False){
                    monster_AGI = rand() % 101;
                    if(isDead(*HP)==True){
                        result = False;
                        break;
                    }
                }
            }
            else{
                printf("Your ACK is to low too fight with this monster.\n");
                result = False;
            }
            break;
        default:
            printf("No such monster, please enter 1 to 5 to choose the monster.\n");
            result = 2;
            break;
        }
    }
    
    //戰鬥回合且還沒死時
    if(isDead(*HP) == False){
        //打贏，力量+1
        if(result == True){
            printf("You win and ACK up 1!\n");
            printf("Gained $1!\n");
            (*ACK) += 1;
            (*money) += 1;
        }
        //打輸，力量不夠，扣除monsterLv
        else{
            printf("You are defeated and loss %dHP!\n",monster);
            (*HP) -= monster;
        }
    }
}

//回傳怪物是否迴避成功
int evasion(int evasionRate, int monsterLevel, int *HP){
    printf("evasionRate:%d\n", evasionRate);
    if(isLucky==True)
        evasionRate -= 20;
    if(evasionRate > 50){
        *HP -= monsterLevel;
        printf("Oops,your attack miss,The monster hit back!\n");
        printf("You lose %d HP!\n",monsterLevel);
        printf("You have %d blood now.\n",*HP);
        return True;
    }
    else{
        printf("Nice,you hit the monster!\n");
        return False;
    }
}

int isDead(int HP){
    if(HP <= 0)
        return True;
    else
        return False;
}