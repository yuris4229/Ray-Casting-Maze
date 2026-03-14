#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#pragma region 모든 구조체
typedef struct deque deque;
typedef struct deque_node deque_node;
#pragma endregion
#pragma region 모든 함수 목록
// ===== ===== deque ===== =====
void deque_init(deque* dq);
void node_init(deque_node* node);
void push_front(deque* dq, int x);
void push_back(deque* dq, int x);
void push_after(deque* dq, int x, int idx);
void pop_front(deque* dq);
void pop_back(deque* dq);
int deque_empty(deque* dq);
int deque_size(deque* dq);
int deque_front(deque* dq);
int deque_back(deque* dq);

// ===== ===== ray & print ===== =====
void maze_simulate();
void calc_ascii(int x, long double calc_distance);
void screen_init();
void screen_print();
void map_print();
void path_print();
void gotoxy(int x, int y);

// ===== ===== maze ===== ====
void maze_generation();

// ===== ===== game logic ===== =====
int get_input();
void run_input(char c);
void moving_player(long double angle);
void print_key(int i);
void start_screen();
void end_screen();

// ===== ===== etc ===== =====
void malloc_init();
#pragma endregion
#pragma region 덱 구현
//deque를 만들어 봅시다

//deque 선언 후에는 deque_init 할 것.
//node 새로 생성 후에는 node_init 할 것.
typedef struct deque_node {
    struct deque_node* prv;
    struct deque_node* nxt;
    int data;
} deque_node;

typedef struct deque {
    deque_node* head;
    deque_node* tail;
    int deque_size;
} deque;

//deque를 초기화 하는 함수, deque 생성시 마다 호출 할 것
void deque_init(deque* dq) {
    dq->head = NULL;
    dq->tail = NULL;
    dq->deque_size = 0;
}

//노드를 초기화 하는 함수, 노드 생성시 마다 호출 할 것
void node_init(deque_node* node) {
    node->prv = NULL;
    node->nxt = NULL;
    node->data = 0;
}

//맨 앞에 노드를 출가하는 함수
void push_front(deque* dq, int x) {
    deque_node* new_node = malloc(sizeof(deque_node));
    //메모리 할당 실패
    if (new_node == NULL) {
        printf("push_front 함수에서 메모리 할당에 실패했습니다.\n");
        exit(1);
    }
    node_init(new_node);
    new_node->data = x;
    if (dq->deque_size == 0) { //원소가 0개인 경우
        dq->head = new_node;
        dq->tail = new_node;
    }
    else { // 그 외
        new_node->nxt = dq->head;
        dq->head->prv = new_node;
        dq->head = new_node;
    }
    dq->deque_size++;
    return;
}

//맨 뒤에 노드를 추가하는 함수
void push_back(deque* dq, int x) {
    deque_node* new_node = malloc(sizeof(deque_node));
    //메모리 할당 실패
    if (new_node == NULL) {
        printf("push_back 함수에서 메모리 할당에 실패했습니다.\n");
        exit(1);
    }
    node_init(new_node);
    new_node->data = x;
    if (dq->deque_size == 0) { //원소가 0개인 경우
        dq->head = new_node;
        dq->tail = new_node;
    }
    else { // 그 외
        new_node->prv = dq->tail;
        dq->tail->nxt = new_node;
        dq->tail = new_node;
    }
    dq->deque_size++;
    return;
}

//특정한 지점 이후에 노드를 삽입하는 함수
void push_after(deque* dq, int x, int idx) {
    deque_node* cur_node = dq->head;
    deque_node* new_node = malloc(sizeof(deque_node));
    if (new_node == NULL) {
        printf("push_after 함수에서 메모리 할당에 실패했습니다.\n");
        exit(1);
    }
    node_init(new_node);
    new_node->data = x;
    for (int i = 0; i < idx; i++) {
        if (cur_node == NULL) {
            printf("노드 개수 부족");
            exit(1);
        }
        cur_node = cur_node->nxt;
    }

    if (cur_node == NULL) {
        printf("노드 개수 부족");
        exit(1);
    }

    if (cur_node->nxt == NULL) { //마지막 뒤에 삽입할 경우 
        new_node->prv = dq->tail;
        dq->tail->nxt = new_node;
        dq->tail = new_node;
    }
    else { //마지막이 아닌 경우
        new_node->nxt = cur_node->nxt;
        new_node->prv = cur_node;
        new_node->nxt->prv = new_node;
        cur_node->nxt = new_node;
    }

    dq->deque_size++;
}

void pop_front(deque* dq) {
    if (dq->deque_size == 0) { //만약 0개인 경우
        printf("빈 deque를 pop_front 시킬 수 없습니다.\n");
        return;
    }
    else if (dq->deque_size == 1) { //1개짜리인 경우에서의 예외 처리
        free(dq->head);
        dq->head = NULL;
        dq->tail = NULL;
    }
    else { // 그 외
        dq->head = dq->head->nxt;
        free(dq->head->prv);
        dq->head->prv = NULL;
    }
    dq->deque_size--;
    return;
}

void pop_back(deque* dq) {
    if (dq->deque_size == 0) { //만약 0개인 경우
        printf("빈 deque를 pop_back 시킬 수 없습니다.\n");
        return;
    }
    else if (dq->deque_size == 1) { //1개짜리인 경우에서의 예외 처리
        free(dq->tail);
        dq->head = NULL;
        dq->tail = NULL;
    }
    else { //그외
        dq->tail = dq->tail->prv;
        free(dq->tail->nxt);
        dq->tail->nxt = NULL;
    }
    dq->deque_size--;
    return;
}

int deque_empty(deque* dq) {
    return !(dq->deque_size);
}

int deque_size(deque* dq) {
    return dq->deque_size;
}

int deque_front(deque* dq) {
    if (deque_empty(dq)) {
        printf("빈 deque에서 원소를 꺼낼 수 없습니다.");
        return -1;
    }
    return dq->head->data;
}

int deque_back(deque* dq) {
    if (deque_empty(dq)) {
        printf("빈 deque에서 원소를 꺼낼 수 없습니다.");
        return -1;
    }
    return dq->tail->data;
}
#pragma endregion
#pragma region 레이캐스팅 구현 & 화면 출력 관련
//파이
#define PI 3.14159265358979323846

//상태 저장용 값들
int is_endpoint_reached = 0;
int is_debug_mode_enabled = 0;
int is_printing_map = 0;
int is_printing_path = 0;
int is_current_input_special = 0;
char current_input = '0';

//각종 플레이어 관련 변수
long double player_x = 1.5;
long double player_y = 1.5;
long double player_angle = 45;
long double player_angle_sensivity = 15.5;
long double player_field_of_view = 60.0;
long double player_visible_distance = 5.5;
long double player_moving_distance = 0.4;

//상하좌우 - 미로 생성과 길 출력에 사용됨
int search_x[4] = { 0, 0, 1, -1 };
int search_y[4] = { 1, -1, 0, 0 };

//거리 관련 변수들
//distance-adding 이 적을수록 부드러움, 근데 그만큼 느림
long double distance_adding = 0.05;
long double* distance_array;
int* ascii_length;
char* ascii_using;

//출력 관련 변수들
int max_screen_width = 160;
int max_screen_height = 50;
char** screen;

//미로 (grid) 생성 변수들
//가로와 세로는 홀수를 권장함니다
//짝수면 마지막 두칸이 벽입니다
int max_maze_width = 7;
int max_maze_height = 7;
int** grid;
int** grid_visited;
int** grid_path;

const char ascii_table[20] = { '@', '#', '*', '=', '+', '-', '\'', ' ' };
const int ascii_count = 8;
const long double brightness_changer = 0.6;

void maze_simulate() {
    while (1) {
        if (is_endpoint_reached) return;

        screen_init();
        gotoxy(0, 0);

        if (player_angle > 360) player_angle -= 360;
        if (player_angle < 0) player_angle += 360;

        //angle += fov/2 가 플레이어 시야의 시작 -> 끝점
        //max_screen_width의 크기만큼 연산하면서
        //player_field_of_view / max_screen_width씩 각도를 돌리면 됨
        long double player_angle_start = player_angle - player_field_of_view / 2;
        long double player_angle_increase = player_field_of_view / max_screen_width;

        //x_position
        for (int x_pos = 0; x_pos < max_screen_width; x_pos++) {
            //현재 계산할 각도 = 각도 시작점 + 각도 증가량 * 현재 연산 횟수
            long double current_calc_angle = player_angle_start + player_angle_increase * x_pos;

            //플레이어 위치에서 바라보는 방향으로 떨어진 거리, 벽인지 검사하는 용도로 사용됨
            long double current_distance = 0;

            //C언어에서의 sin, cos는 라디안 각도 단위를 사용하기 때문에 * (PI / 180) 를 붙여주어야 함
            //그리고, sin(뭐시기), cos(뭐시기)의 값이 0이라면 프로그램이 펑펑 터지기 때문에 값을 바꿔 주어야 함 (= 0도, 90도, 180도, 270도 일 때)
            if (abs(sin(current_calc_angle * (PI / 180))) < 0.01 || abs(cos(current_calc_angle * (PI / 180))) < 0.01) current_calc_angle += 0.1;

            distance_array[x_pos] = player_visible_distance;
            while (current_distance <= player_visible_distance) {
                long double current_x = player_x + cos(current_calc_angle * (PI / 180)) * current_distance;
                long double current_y = player_y + sin(current_calc_angle * (PI / 180)) * current_distance;

                //만약 벽에 도달한 경우
                if (grid[(int)current_x][(int)current_y]) {
                    distance_array[x_pos] = current_distance;
                    grid_visited[(int)current_x][(int)current_y] = 1;
                    break;
                }

                //distance_adding의 단위씩을 더해 전진시킴
                current_distance += distance_adding;
            }

            //아스키 코드 배열에 저장
            calc_ascii(x_pos, current_distance);
        }

        Sleep(10);
        //지도나 맵을 출력하고 있는 상황이라면, 그걸 출력합니다
        if (is_printing_path) path_print();
        else if (is_printing_map) map_print();
        else screen_print(); //아니라면 3D 화면 출력

        //그리고 밑에 조작법을 출력합니다
        print_key(current_input);
        current_input = get_input();
        run_input(current_input);
        Sleep(10);
    }
}

//각종 아스키 처리
void calc_ascii(int x, long double calc_distance) {
    //0으로나누면 안됩니다
    if (calc_distance < 0.01) calc_distance = 0.01;
    //너무 커지더라도 문제가 생깁니다
    if (calc_distance > player_visible_distance) calc_distance = player_visible_distance;

    //거리에 따른 사용할 문자와 출력할 문자 갯수를 계산
    int ascii_index = 0 + (int)round((calc_distance / brightness_changer));
    //아스키 원소 개수보다 index가 커져도 문제가 생깁니다
    if (ascii_index >= ascii_count) ascii_index = ascii_count - 1;
    ascii_using[x] = ascii_table[ascii_index];
    ascii_length[x] = (int)(max_screen_height / calc_distance);
    //비슷하게 아스키 세로 길이가 화면 세로 길이보다 크더라도 문제가 생깁니다
    if (ascii_length[x] > max_screen_height) ascii_length[x] = max_screen_height;

    //그러니까 ascii_length[x]는 screen[x][?]에 중간이 max_screen_height / 2에 걸치도록 해야 합니다
    int ascii_begin = (max_screen_height / 2) - (ascii_length[x] / 2);
    for (int i = ascii_begin; i < ascii_begin + ascii_length[x]; i++) {
        screen[x][i] = ascii_using[x];
    }
}

//스크린 배열을 공백으로 초기화
void screen_init() {
    for (int i = 0; i < max_screen_height; i++) {
        for (int j = 0; j < max_screen_width; j++) {
            screen[j][i] = ' ';
        }
    }
}

//스크린 출력, gcc/clang의 putchar_unlock 대신 msvc는 _putchar_nolock 사용
void screen_print() {
    for (int i = 0; i < max_screen_height; i++) {
        for (int j = 0; j < max_screen_width; j++) {
            _putchar_nolock(screen[j][i]);
        }
        _putchar_nolock('\n');
    }
}

//맵 출력하는 함수
void map_print() {
    system("cls");
    for (int i = 0; i < max_maze_height; i++) {
        for (int j = 0; j < max_maze_width; j++) {

            if (j == (int)player_x && i == (int)player_y) { //현재 위치인 경우
                printf("\033[33m");
                _putchar_nolock('O');
                printf("\033[37m");
            }
            else if (grid_visited[j][i] == 1) { //본 벽인경우
                _putchar_nolock('#');
            }
            else if (j == 1 && i == 1) { //시작점인 경우
                printf("\033[34m");
                _putchar_nolock('S');
                printf("\033[37m");
            }
            else if (i == max_maze_height - 2 && j == max_maze_width - 2) { //시작점인 경우
                printf("\033[35m");
                _putchar_nolock('E');
                printf("\033[37m");
            }
            else if (i == 0 || j == 0 || i == max_maze_height - 1 || j == max_maze_width - 1) { //테두리인경우
                _putchar_nolock('#');
            }
            else _putchar_nolock(' ');
            _putchar_nolock(' ');
        }
        _putchar_nolock('\n');
    }
}

//올바른 길을 포함한 맵을 출력하는 함수
void path_print() {
    system("cls");
    //길과 거리를 양수로 저장할 것이기 때문에 벽을 -1로 저장
    for (int i = 0; i < max_maze_height; i++) {
        for (int j = 0; j < max_maze_width; j++) {
            if (grid[j][i]) grid_path[j][i] = -1;
            else grid_path[j][i] = 0;
        }
    }

    //똑같이 x좌표와 y좌표를 각각 다른 deque를 써서 관리
    deque path_deque_x;
    deque_init(&path_deque_x);
    deque path_deque_y;
    deque_init(&path_deque_y);

    push_back(&path_deque_x, (int)player_x);
    push_back(&path_deque_y, (int)player_y);

    //시작점을 1로 두기
    grid_path[(int)player_x][(int)player_y] = 1;

    //일단 모든 길을 시작점에서 떨어진 거리로 채워 줍니다.
    while (!deque_empty(&path_deque_x) && !deque_empty(&path_deque_y)) {
        int current_x = deque_front(&path_deque_x);
        int current_y = deque_front(&path_deque_y);

        pop_front(&path_deque_x);
        pop_front(&path_deque_y);

        for (int i = 0; i < 4; i++) {
            //미로 생성할 때 만들어둔 search_x, search_y 재활용
            int next_x = current_x + search_x[i];
            int next_y = current_y + search_y[i];

            //이미 방문한 곳이거나, 배열의 끝이라면 건너뜀
            if (next_x == 0 || next_y == 0 || next_x == max_maze_width - 1 || next_y == max_maze_height - 1) continue;
            if (grid_path[next_x][next_y] != 0) continue;
            
            push_back(&path_deque_x, next_x);
            push_back(&path_deque_y, next_y);

            //칸에는 최솟값만이 저장되게
            if (grid_path[next_x][next_y] == 0) {
                grid_path[next_x][next_y] = grid_path[current_x][current_y] + 1;
            }
            else {
                if (grid_path[next_x][next_y] > grid_path[current_x][current_y] + 1) grid_path[current_x][current_y] + 1;
                else continue;
            }
        }
    }
    
    //경로를 역추적하기 위한 도착점 저장
    int trace_x = max_maze_width - 2;
    int trace_y = max_maze_height - 2;

    //경로를 역추적 해 줍니다.
    //도착 지점에 저장된 현재 위치에서 떨어진 위치가 최단 경로를 통해 저장된 거리이므로
    //1씩 빼주며 돌아가면 최단 경로를 역추적 할 수 있습니다.
    while (1) {
        //시작점의 값을 1로 설정해 두었으므로
        if (grid_path[trace_x][trace_y] == 1) break;

        for (int i = 0; i < 4; i++) {
            //미로 생성할 때 만들어둔 search_x, search_y 재활용
            int next_x = trace_x + search_x[i];
            int next_y = trace_y + search_y[i];

            //벽이거나 , 현재 위치와 관련 없는 길이라면 건너뜀
            if (next_x == 0 || next_y == 0 || next_x == max_maze_width - 1 || next_y == max_maze_height - 1) continue;
            if (grid_path[next_x][next_y] != grid_path[trace_x][trace_y] - 1) continue;

            //역추적 경로에 포함된다면 표시
            grid_path[trace_x][trace_y] = -31337;

            trace_x = next_x;
            trace_y = next_y;
            break;
        }
    }

    //추적이 완료되었다면, 경로를 포함한 지도를 출력 해 줍니다.
    for (int i = 0; i < max_maze_height; i++) {
        for (int j = 0; j < max_maze_width; j++) {

            if (j == (int)player_x && i == (int)player_y) { //현재 위치인 경우
                printf("\033[33m");
                _putchar_nolock('O');
                printf("\033[37m");
            }
            else if (grid_path[j][i] == -1) { //벽인경우, 여기선 -1임
                _putchar_nolock('#');
            }
            else if (j == 1 && i == 1) { //시작점인 경우
                printf("\033[34m");
                _putchar_nolock('S');
                printf("\033[37m");
            }
            else if (i == max_maze_height - 2 && j == max_maze_width - 2) { //시작점인 경우
                printf("\033[35m");
                _putchar_nolock('E');
                printf("\033[37m");
            }
            else if (i == 0 || j == 0 || i == max_maze_height - 1 || j == max_maze_width - 1) { //테두리인경우
                _putchar_nolock('#');
            }
            else if (grid_path[j][i] == -31337) { //역추적 경로일경우
                printf("\033[31m");
                _putchar_nolock('>');
                printf("\033[37m");
            }
            else _putchar_nolock(' ');
            _putchar_nolock(' ');
        }
        _putchar_nolock('\n');
    }
}

//출력 위치 바꾸는 함수
void gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//메모용 주석.

/*
θ = 현재 검사할 angle
0도   : +X
90도  : +Y
180도 : -X
270도 : -Y

cos = 밑변/빗변
sin = 높이/빗변
tan = 높이/밑변

시야를 직선으로 표현하자면
y = mx + b (m은 기울기, 항상 0,0에서 출발하지 않으므로 b라는 절편 추가)
y = tanθ * x + b
하지만, tan은 90일 때 정의 불가하므로, sin / cos로 표현할 것이다.
y = sinθ / cosθ * x + b
=> x가 cosθ * t만큼 증가할 때
y가 sinθ * t만큼 증가한다.

다르게 표현하자면
x = player_x + cosθ(현재 검사할 각도) * t
y = player_y + sinθ(현재 검사할 각도) * t
*여기서 이 식에서는 시작점이 표현되므로, b는 사라지게 됨

여기서 거리는 빗변이므로,
t * sinθ = y의 증가된 거리 (높이)
t * cosθ = x의 증가된 거리 (밑변) 이 된다
그러니 결국, x y값이 성립하는 t는 거리를 의미한다

============================================================================================

원래는 DDA 기반 구현을 고려했으나, 구조를 단순하게 유지하기 위해 일정 간격으로 전진하며 벽 충돌을 검사하는 방식을 사용했습니다.
플레이어 x좌표, y좌표가 있을 때
바라보는 방향(현재 검사하는 직선)에서 distance_adding씩 거리를 더한 뒤에
그 위치가 벽인지 검사하고, 만약 벽이라면 그게 벽까지의 거리가 되는 것이고
아니라면 벽과 닿을 때까지 / 최대 검사 거리에 도달할 때 까지 반복합니다.

그 후 거리가 멀 수록 #(하얀 빛이 많음) -> `(하얀 빛이 적음) 문자를 출력하고
거리가 멀 수록 세로로 긋는 선의 길이가 짧게 출력합니다.
*/
#pragma endregion
#pragma region 미로 생성

/*
기본 뚫려있는 벽이 아닌 칸의 개수는
가로 : (maze_width - 1) / 2
세로 : (maze_height - 1) / 2
가로 c_n번째, 세로 c_m번째 기본적으로 뚫려있지 않은 칸의 위치는 (0에서 시작한다 치고)
grid[c_n * 2 + 1][c_m * 2 + 1]이 됨

기본적인 growing-tree 알고리즘의 미로 생성 방식은 이러함
'방문한 칸들 중 임의의 칸을 선택해 아직 방문하지 않은 칸 쪽으로 벽을 뚫는다'
를 방문하지 않은 칸이 남지 않을 때 까지 반복하는 것 인데,

이 코드에서는 시작점을 랜덤으로 정한 후에, 상하좌우 4방향을 랜덤으로 골라 방문하지 않은 칸이면 길을 뚫고 deque의 랜덤의 index에 추가할 것임
그리고 deque를 맨 앞에서부터 지워나가면서 맨 뒤에 추가될 칸이 없을 경우에만 앞 원소를 지울 것임
*/

int cell_x_total;
int cell_y_total;

//랜덤으로 탐색할 방향을 골라 탐색 안한 칸이 나올 때 까지 다시 돌리는 것은 꽤나 비효율적입니다
//그렇기 때문에 그냥 모든 가능한 탐색 순서를 정해두고, 처음 한번만 어떤 랜덤 순서를 사용할지를 정해주기로 했습니다
int search_seed_table[24][4] = {
    {0, 1, 2, 3},{0, 1, 3, 2},{0, 2, 1, 3},{0, 2, 3, 1},{0, 3, 1, 2},{0, 3, 2, 1},
    {1, 0, 2, 3},{1, 0, 3, 2},{1, 2, 0, 3},{1, 2, 3, 0},{1, 3, 0, 2},{1, 3, 2, 0},
    {2, 0, 1, 3},{2, 0, 3, 1},{2, 1, 0, 3},{2, 1, 3, 0},{2, 3, 0, 1},{2, 3, 1, 0},
    {3, 0, 1, 2},{3, 0, 2, 1},{3, 1, 0, 2},{3, 1, 2, 0},{3, 2, 0, 1},{3, 2, 1, 0},
};

int** search_progress;
int** search_seed;

int acted_count = 0;

void maze_generation() {
    //deque에 한번에 두 원소를 넣을 수 없으므로, 두 deque를 만들어 좌표를 관리할 것임
    //그러니까 방문했지만-상하좌우로 지울 칸이 남아있는 칸들만 남아있음
    deque maze_deque_x;
    deque_init(&maze_deque_x);
    deque maze_deque_y;
    deque_init(&maze_deque_y);

    srand((unsigned int)time(NULL));

    //미로 초기화
    for (int i = 0; i < max_maze_height; i++) {
        for (int j = 0; j < max_maze_width; j++) {
            grid[j][i] = 1;
        }
    }

    //미로 탐색용 시듯값 쵝화
    for (int i = 0; i < cell_y_total; i++) {
        for (int j = 0; j < cell_x_total; j++) {
            //이건 탐색 시듯값 정해주기
            search_seed[j][i] = rand() % 24;

            //이건 탐색 진행도 초기화
            search_progress[j][i] = 0;
        }
    }

    system("cls");

    //현재 선택된 셀과 아직 방문하지 않은 이웃 셀 사이의 벽을 제거한다. (를 방문된 칸에서만 해야 함)
    int cell_x_begin = rand() % cell_x_total;
    int cell_y_begin = rand() % cell_y_total;

    grid[cell_x_begin * 2 + 1][cell_y_begin * 2 + 1] = 0;

    //x좌표, y좌표 두가지 정보를 저장해야 하므로, x와 y용 deque를 각각 만들어 둠
    push_back(&maze_deque_x, cell_x_begin);
    push_back(&maze_deque_y, cell_y_begin);

    while (!deque_empty(&maze_deque_x) && !deque_empty(&maze_deque_y)) { //비어있을 때 까지, 그러니까 탐색이 종료(미로 생성이 완성) 될 때 까지 반복
        int current_cell_x = deque_front(&maze_deque_x);
        int current_cell_y = deque_front(&maze_deque_y);

        int current_maze_x = current_cell_x * 2 + 1;
        int current_maze_y = current_cell_y * 2 + 1;

        for (int i = 0; i < 4; i++) {
            //4번 다 방문했을 경우 삭제 후 break
            if (search_progress[current_cell_x][current_cell_y] == 4) {
                pop_front(&maze_deque_x);
                pop_front(&maze_deque_y);
                break;
            }

            int current_progress = search_progress[current_cell_x][current_cell_y];
            int current_seed = search_seed[current_cell_x][current_cell_y];

            int next_cell_x = current_cell_x + search_x[search_seed_table[current_seed][current_progress]];
            int next_cell_y = current_cell_y + search_y[search_seed_table[current_seed][current_progress]];

            //grid 범위를 벗어나는 경우 continue
            if (next_cell_x < 0 || next_cell_x >= cell_x_total || next_cell_y < 0 || next_cell_y >= cell_y_total) {
                search_progress[current_cell_x][current_cell_y]++;
                continue;
            }

            //grid 상에서의 위치
            int next_maze_x = next_cell_x * 2 + 1;
            int next_maze_y = next_cell_y * 2 + 1;

            //이미 방문했던 곳인 경우 continue
            if (!grid[next_maze_x][next_maze_y]) {
                search_progress[current_cell_x][current_cell_y]++;
                continue;
            }

            //현재 칸의 진행도 증가
            search_progress[current_cell_x][current_cell_y]++;

            //벽 뚫기
            grid[(current_maze_x + next_maze_x) / 2][(current_maze_y + next_maze_y) / 2] = 0;
            grid[next_maze_x][next_maze_y] = 0;

            //방문된-칸-목록에 추가 & 현재 칸 뒤로 밀기
            int random_index = rand() % deque_size(&maze_deque_x);
            push_after(&maze_deque_x, next_cell_x, random_index);
            push_after(&maze_deque_y, next_cell_y, random_index);

            random_index = rand() % deque_size(&maze_deque_x);
            push_after(&maze_deque_x, current_cell_x, random_index);
            push_after(&maze_deque_y, current_cell_y, random_index);

            pop_front(&maze_deque_x);
            pop_front(&maze_deque_y);
            break;
        }

        acted_count++;

        //매번 이 곳에 도착할 때 마다 출력하는 건 너무 느리기 때문에
        //변화가 어느정도 될 때 마다 출력합니다.
        //상수 값으로 나눠주면 maze의 가로 세로 크기가 커졌을 때 느려지기 때문에
        //적당히 maze 크기에 따라 나누는 값을 조정해 주었습니다.
        if (acted_count % (max_maze_width / 3) != 0) continue;

        gotoxy(0, 0);
        printf("미로 생성 중...\n");
        for (int i = 0; i < max_maze_height; i++) {
            for (int j = 0; j < max_maze_width; j++) {
                if (!grid[j][i]) {
                    //ANSI escape code
                    printf("\033[31m");
                    _putchar_nolock(grid[j][i] + '0');
                    _putchar_nolock(' ');
                    printf("\033[37m");
                }
                else {
                    _putchar_nolock(grid[j][i] + '0');
                    _putchar_nolock(' ');
                }
            }
            printf("\n");
        }
        Sleep(300);
    }

    gotoxy(0, 0);
    system("cls");
    printf("생성 완료!\n");
    for (int i = 0; i < max_maze_height; i++) {
        for (int j = 0; j < max_maze_width; j++) {
            if (!grid[j][i]) {
                printf("\033[31m");
                _putchar_nolock(grid[j][i] + '0');
                _putchar_nolock(' ');
                printf("\033[37m");
            }
            else {
                _putchar_nolock(grid[j][i] + '0');
                _putchar_nolock(' ');
            }
        }
        printf("\n");
    }
    printf("아무 키나 눌러 시작하세요!");
    _getch();
}
#pragma endregion
#pragma region 게임 로직
//키보드 값
const int key_tab = 9;
const int key_enter = 13;
const int key_left = 75;
const int key_right = 77;

//현재 입력을 받아오는 함수
int get_input() {
    //키 입력
    //특수키가 입력되었다면 0(예를 들어 f1 ~ f12) 또는 224(esc, enter 등)가 입력되고, 이후 키 값 입력됨
    int special_check = _getch();
    if (special_check == 0 || special_check == 224) {
        int current_key = _getch();
        is_current_input_special = 1;
        return current_key;
    }
    else {
        is_current_input_special = 0;
        return special_check;
    }
}

//화면 밑에 키/좌표 출력
void print_key(int i) {
    if (is_debug_mode_enabled) {
        printf("입력받은 키의 값 : %d                                        \n", i);
        printf("현재 플레이어 각도 : %Lf                                        \n", player_angle);
        printf("플레이어 x 좌표 : %Lf                                        \n", player_x);
        printf("플레이어 y 좌표 : %Lf                                        \n", player_y);
    }
    else {
        printf("[W, A, S, D] - 이동                                        \n");
        printf("[←, →] - 시야 회전                                        \n");
        printf("[M] - 지도 열기                                        \n");
        printf("[Tab] - 디버그 모드 활성화/비활성화                                        \n");
        if (is_printing_map) {
            printf("[N] - 전체 지도와 올바른 길 출력\n");
        }
    }
}

//입력을 처리합니다
//w, a, s, d가 입력되면 플레이어 이동
//key_left 또는 key_right가 입력되면 화면을 조정합니다
void run_input(char c) {
    //대문자 M이랑 오른쪽 화살표랑 값이 같아 is_current_input_special로 구분해야 합니다.
    if (is_current_input_special) {
        if (c == key_left) player_angle -= player_angle_sensivity;
        if (c == key_right) player_angle += player_angle_sensivity;
    }
    else {
        //맵이 열려있을 때에는 움직여지면 안 됨
        if (!is_printing_map) {
            if ((c == 'w' || c == 'W')) moving_player(player_angle);
            if (c == 'a' || c == 'A') moving_player(player_angle - 90);
            if (c == 's' || c == 'S') moving_player(player_angle - 180);
            if (c == 'd' || c == 'D') moving_player(player_angle - 270);
        }
        else {
            if (c == 'n' || c == "N") is_printing_path = !is_printing_path;
        }

        if (c == 'm' || c == 'M') is_printing_map = !is_printing_map;
        if (c == key_tab) is_debug_mode_enabled = !is_debug_mode_enabled;
    }
}

//플레이어 위치를 이동시킴니다
void moving_player(long double angle) {
    long double current_x = player_x + cos(angle * (PI / 180)) * player_moving_distance;
    long double current_y = player_y + sin(angle * (PI / 180)) * player_moving_distance;

    if (grid[(int)current_x][(int)current_y]) return;
    else {
        player_x = current_x;
        player_y = current_y;
    }

    if ((int)player_x == (cell_x_total - 1) * 2 + 1 && (int)player_y == (cell_y_total - 1) * 2 + 1) is_endpoint_reached = 1;
}

//시작화면
void start_screen(){
    system("echo off");
    while (1) {
        system("cls");
        printf("3D 미로 찾기 - 아무 키나 눌러 시작\n");

        printf("\n\n==================================================================\n\n\n");
        printf("다음과 같은 옵션으로 게임이 시작됩니다\n");
        printf("수정을 원한다면, ENTER 키를 눌러주세요\n\n");
        printf("화면 가로 크기 : %d\n", max_screen_width);
        printf("화면 세로 크기 : %d\n\n", max_screen_height);
        printf("미로 가로 크기 : %d\n", max_maze_width);
        printf("미로 세로 크기 : %d\n\n", max_maze_height);
        printf("플레이어 시야각 각도 : %Lf\n", player_field_of_view);
        printf("플레이어 시야 회전 감도 : %Lf\n", player_angle_sensivity);
        printf("\n\n==================================================================\n\n");

        char c = get_input();
        //while (getchar() != '\n'); 는 입력 버퍼 비우기 역할입니다

        //설정값 수정을 하는 경우
        //오버플로우는 고려 안했습니다.
        if (c == key_enter) {
            //max_screen_width 입력 받기
            system("cls");
            printf("화면 가로 크기 값을 입력하세요 (현재 크기 : %d), (정숫값 입력)\n", max_screen_width);
            scanf("%d", &max_screen_width);
            while (getchar() != '\n');
            //max_screen_height 입력 받기
            system("cls");
            printf("화면 세로 크기 값을 입력하세요 (현재 크기 : %d), (정숫값 입력)\n", max_screen_height);
            scanf("%d", &max_screen_height);
            while (getchar() != '\n');
            //max_maze_width 입력 받기
            system("cls");
            printf("미로 가로 크기 값을 입력하세요. 홀수를 권장합니다 (현재 크기 : %d), (정숫값 입력)\n", max_maze_width);
            scanf("%d", &max_maze_width);
            while (getchar() != '\n');
            //max_screen_width 입력 받기
            system("cls");
            printf("미로 세로 크기 값을 입력하세요. 홀수를 권장합니다 (현재 크기 : %d), (정숫값 입력)\n", max_maze_height);
            scanf("%d", &max_maze_height);
            while (getchar() != '\n');
            //player_field_of_view 입력 받기
            system("cls");
            printf("플레이어 시야각 각도를 입력하세요 (현재 크기 : %Lf), (소수값 입력)\n", player_field_of_view);
            scanf("%Lf", &player_field_of_view);
            while (getchar() != '\n');
            //player_angle_sensivity 입력 받기
            system("cls");
            printf("플레이어 시야 회전 감도값을 입력하세요 (현재 크기 : %Lf), (정숫값 입력)\n", player_angle_sensivity);
            scanf("%Lf", &player_angle_sensivity);
            while (getchar() != '\n');
        }
        else {
            break;
        }
    }
}

//끝화면
void end_screen() {
    gotoxy(0, 0);

    //간단한 연출
    int printed_count = 0;
    for (int i = 0; i < max_screen_height; i++) {
        for (int j = 0; j < max_screen_width; j++) {
            _putchar_nolock(' ');
            printed_count++;
            if (printed_count % (max_screen_width / 3) != 0) continue;
            Sleep(3);
        }
        _putchar_nolock('\n');
    }
    system("cls");
    printf("게임 클리어!!!");
    printf("\n\n\n\n\n\n\n\n\n\n\n");
}
#pragma endregion
#pragma region etc
//각종 동적할당을 할당하는 함수
void malloc_init() {
    //distance_array
    distance_array = malloc(sizeof(long double) * max_screen_width);
    if (distance_array == NULL) {
        printf("메모리 부족. distance_array 할당 실패\n");
        exit(1);
    }
    //ascii_length
    ascii_length = malloc(sizeof(int) * max_screen_width);
    if (ascii_length == NULL) {
        printf("메모리 부족. ascii_length 할당 실패\n");
        exit(1);
    }
    //ascii_using
    ascii_using = malloc(sizeof(int) * max_screen_width);
    if (ascii_using == NULL) {
        printf("메모리 부족. ascii_using 할당 실패\n");
        exit(1);
    }
    //screen
    screen = malloc(sizeof(char*) * (max_screen_width));
    if (screen == NULL) {
        printf("메모리 부족. screen 할당 실패\n");
        exit(1);
    }
    for (int i = 0; i < max_screen_width; i++) {
        screen[i] = malloc(sizeof(char) * max_screen_height);
        if (screen[i] == NULL) {
            printf("메모리 부족. 2차 screen 할당 실패\n");
            exit(1);
        }
    }
    //grid
    grid = malloc(sizeof(int*) * (max_maze_width));
    if (grid == NULL) {
        printf("메모리 부족. grid 할당 실패\n");
        exit(1);
    }
    for (int i = 0; i < max_maze_width; i++) {
        grid[i] = malloc(sizeof(int) * max_maze_height);
        if (grid[i] == NULL) {
            printf("메모리 부족. 2차 grid 할당 실패\n");
            exit(1);
        }
    }

    //grid_visited
    grid_visited = malloc(sizeof(int*) * (max_maze_width));
    if (grid_visited == NULL) {
        printf("메모리 부족. grid_visited 할당 실패\n");
        exit(1);
    }
    for (int i = 0; i < max_maze_width; i++) {
        grid_visited[i] = malloc(sizeof(int) * max_maze_height);
        if (grid_visited[i] == NULL) {
            printf("메모리 부족. 2차 grid_visited 할당 실패\n");
            exit(1);
        }
    }

    //grid_path
    grid_path = malloc(sizeof(int*) * (max_maze_width));
    if (grid_path == NULL) {
        printf("메모리 부족. grid_path 할당 실패\n");
        exit(1);
    }
    for (int i = 0; i < max_maze_width; i++) {
        grid_path[i] = malloc(sizeof(int) * max_maze_height);
        if (grid_path[i] == NULL) {
            printf("메모리 부족. 2차 grid_path 할당 실패\n");
            exit(1);
        }
    }

    //search_progress
    cell_x_total = (max_maze_width - 1) / 2;
    cell_y_total = (max_maze_height - 1) / 2;
    search_progress = malloc(sizeof(int*) * (cell_x_total));
    if (search_progress == NULL) {
        printf("메모리 부족. search_progress 할당 실패\n");
        exit(1);
    }
    for (int i = 0; i < cell_x_total; i++) {
        search_progress[i] = malloc(sizeof(int) * cell_y_total);
        if (search_progress[i] == NULL) {
            printf("메모리 부족. 2차 search_progress 할당 실패\n");
            exit(1);
        }
    }

    //search_seed
    search_seed = malloc(sizeof(int*) * (cell_x_total));
    if (search_seed == NULL) {
        printf("메모리 부족. search_seed 할당 실패\n");
        exit(1);
    }
    for (int i = 0; i < cell_x_total; i++) {
        search_seed[i] = malloc(sizeof(int) * cell_y_total);
        if (search_seed[i] == NULL) {
            printf("메모리 부족. 2차 search_seed 할당 실패\n");
            exit(1);
        }
    }
}

#pragma endregion

int main() {
    start_screen();
    malloc_init();
    maze_generation();
    maze_simulate();
    end_screen();
}
