#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
// 定義雙向鏈結串列的節點結構(for創建各條顏色捷運)
struct Node {
    char *train_ID;
    char *name;  
    struct Node* prev; // 指向前一個節點的指標
    struct Node* next; // 指向後一個節點的指標
};

// 在雙向鏈結串列的尾部插入新節點(建立捷運各站)
void append(struct Node** head_ref, char *train_ID , char *name) {
    // 分配新節點的空間
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    
    // 設定新節點的資料
    new_node->train_ID = train_ID;
    new_node->name = name;
    new_node->next = NULL; // 新節點位於尾部，所以後一個節點為NULL

    // 如果雙向鏈結串列是空的，將新節點設為頭部
    if (*head_ref == NULL) {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }

    // 否則，找到尾部節點，並設定指標
    struct Node* last = *head_ref;
    while (last->next != NULL)
        last = last->next;

    last->next = new_node;
    new_node->prev = last;
}

// 顯示雙向鏈結串列的節點(呈現創建好的捷運各站,for檢查用)
void display(struct Node* node) {
    while (node != NULL) {
        printf("%s - ",node->name);
        node = node->next;
    }
    printf("\n");
}

// 在雙向鏈結串列中搜尋指定的火車站名稱(for確認某個站名是否出現在某條顏色的捷運路線上)
// 若某條顏色捷運沒有該站,則回傳NULL
struct Node* searchByName(struct Node* head, const char* name) {
    struct Node* current = head;

    while (current != NULL) {
        // 使用strcmp函數比較火車站名稱
        if (strcmp(current->name, name) == 0) {
            // 找到匹配，返回當前節點
            return current;
        }
        current = current->next;
    }
    // 無法找到匹配，返回NULL
    return NULL;
}

//搜尋兩站最短路徑
//輸入:  起始站和終點站，以及若有故障站點要避開的站名
//輸出:  兩站最短路徑，以及經過站數
//使用dynamic programming 遞迴呼叫
//e.g. A-B-C-D-E (search A ->E)
//呼叫(A,E)->呼叫(B,E)->呼叫(C,E)->呼叫(D,E)->呼叫(E,E)
//依次回傳path並篩選最短路徑，最後整合全部最短路徑

void find_shortest_path(char* source_station, char* dest_station, char* broken_station,int *count,char  *paths,int *success_find,\
struct Node* red_list , struct Node* blue_list , struct Node* green_list , struct Node* brown_list ,\
struct Node* yellow_list ,struct Node* purple_list , struct Node* lightgreen_list ,\
struct Node* orange1_list , struct Node* orange2_list , struct Node* pink_list){
//若兩站名稱相同,回傳Path:  (->站名)
if (strcmp(source_station, dest_station) == 0){
    strcat(paths, "->");
    strcat(paths, source_station);
    *success_find=1;
    *count=*count+1;
    return;
}
else{
    int  red_next_local_count=0;char red_next_local_paths[1000]="";int  red_next_local_success_find=0;  
    int  red_prev_local_count=0;char red_prev_local_paths[1000]="";int  red_prev_local_success_find=0; 
    int  blue_next_local_count=0;char blue_next_local_paths[1000]="";int  blue_next_local_success_find=0;  
    int  blue_prev_local_count=0;char blue_prev_local_paths[1000]="";int  blue_prev_local_success_find=0;
    int  green_next_local_count=0;char green_next_local_paths[1000]="";int  green_next_local_success_find=0;  
    int  green_prev_local_count=0;char green_prev_local_paths[1000]="";int  green_prev_local_success_find=0;
    int  brown_next_local_count=0;char brown_next_local_paths[1000]="";int  brown_next_local_success_find=0;  
    int  brown_prev_local_count=0;char brown_prev_local_paths[1000]="";int  brown_prev_local_success_find=0; 
    int  yellow_next_local_count=0;char yellow_next_local_paths[1000]="";int  yellow_next_local_success_find=0;  
    int  yellow_prev_local_count=0;char yellow_prev_local_paths[1000]="";int  yellow_prev_local_success_find=0; 
    int  purple_next_local_count=0;char purple_next_local_paths[1000]="";int  purple_next_local_success_find=0;  
    int  purple_prev_local_count=0;char purple_prev_local_paths[1000]="";int  purple_prev_local_success_find=0; 
    int  lightgreen_next_local_count=0;char lightgreen_next_local_paths[1000]="";int  lightgreen_next_local_success_find=0;  
    int  lightgreen_prev_local_count=0;char lightgreen_prev_local_paths[1000]="";int  lightgreen_prev_local_success_find=0; 
    int  orange1_next_local_count=0;char orange1_next_local_paths[1000]="";int  orange1_next_local_success_find=0;  
    int  orange1_prev_local_count=0;char orange1_prev_local_paths[1000]="";int  orange1_prev_local_success_find=0; 
    int  orange2_next_local_count=0;char orange2_next_local_paths[1000]="";int  orange2_next_local_success_find=0;  
    int  orange2_prev_local_count=0;char orange2_prev_local_paths[1000]="";int  orange2_prev_local_success_find=0; 
    int  pink_next_local_count=0;char pink_next_local_paths[1000]="";int  pink_next_local_success_find=0;  
    int  pink_prev_local_count=0;char pink_prev_local_paths[1000]="";int  pink_prev_local_success_find=0; 

    strcat( paths,"->");
    strcat(paths, source_station);
    //尋找出發站出現在那些顏色線上,若非null則查找該顏色線的相鄰站點
    //-----------------------------------------------------------------
    
    if(searchByName(red_list,source_station)!=NULL){   //搜尋紅線

        //該線上的next結點
        if((searchByName(red_list,source_station)->next)!=NULL){
            char *red_next;
            strcat( red_next_local_paths,paths);    
            red_next=searchByName(red_list,source_station)->next->name;
            if(strstr(red_next_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                red_next_local_success_find=0;//放棄該條路徑
                red_next_local_count=999;
                strcpy(red_next_local_paths,"");//清空字串
            }            
            else if(strstr(red_next_local_paths,searchByName(red_list,source_station)->next->name)!=NULL){ //出現在path過(重複站名)
                red_next_local_success_find=0;//放棄該條路徑
                red_next_local_count=999;
                strcpy(red_next_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(red_next,dest_station , broken_station,&red_next_local_count, red_next_local_paths,\
                &red_next_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list , \
                lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(red_next_local_success_find){
                    //red_next_local_success_find==1
                    //red_next_local_paths
                    red_next_local_count+=1;
                };
            }    
        }
        //該線上的prev結點
        if((searchByName(red_list,source_station)->prev)!=NULL){
            char *red_prev;
            strcat( red_prev_local_paths,paths);    
            red_prev=searchByName(red_list,source_station)->prev->name;
            if(strstr(red_prev_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                red_prev_local_success_find=0;//放棄該條路徑
                red_prev_local_count=999;
                strcpy(red_prev_local_paths,"");//清空字串
            } 
            else if(strstr(red_prev_local_paths,searchByName(red_list,source_station)->prev->name)!=NULL){ //出現在path過(重複站名)
                red_prev_local_success_find=0;//放棄該條路徑
                red_prev_local_count=999;
                strcpy(red_prev_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(red_prev,dest_station , broken_station, &red_prev_local_count, red_prev_local_paths,\
                &red_prev_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list ,\
                 lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(red_prev_local_success_find){
                    //red_prev_local_success_find==1
                    //red_prev_local_paths
                    red_prev_local_count+=1;
                };
            }    
        }
    }
    //-----------------------------------------------------------------
    if(searchByName(blue_list,source_station)!=NULL){    //搜尋blue
        if((searchByName(blue_list,source_station)->next)!=NULL){
            char *blue_next;
            strcat( blue_next_local_paths,paths);    
            blue_next=searchByName(blue_list,source_station)->next->name;
            if(strstr(blue_next_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                blue_next_local_success_find=0;//放棄該條路徑
                blue_next_local_count=999;
                strcpy(blue_next_local_paths,"");//清空字串
            }            
            else if(strstr(blue_next_local_paths,searchByName(blue_list,source_station)->next->name)!=NULL){ //出現在path過(重複站名)
                blue_next_local_success_find=0;//放棄該條路徑
                blue_next_local_count=999;
                strcpy(blue_next_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(blue_next,dest_station , broken_station,&blue_next_local_count, blue_next_local_paths,\
                &blue_next_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list , \
                lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(blue_next_local_success_find){
                    //blue_next_local_success_find==1
                    //blue_next_local_paths
                    blue_next_local_count+=1;
                };
            }    
        }
        if((searchByName(blue_list,source_station)->prev)!=NULL){
            char *blue_prev;
            strcat( blue_prev_local_paths,paths);    
            blue_prev=searchByName(blue_list,source_station)->prev->name;
            if(strstr(blue_prev_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                blue_prev_local_success_find=0;//放棄該條路徑
                blue_prev_local_count=999;
                strcpy(blue_prev_local_paths,"");//清空字串
            } 
            else if(strstr(blue_prev_local_paths,searchByName(blue_list,source_station)->prev->name)!=NULL){ //出現在path過(重複站名)
                blue_prev_local_success_find=0;//放棄該條路徑
                blue_prev_local_count=999;
                strcpy(blue_prev_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(blue_prev,dest_station , broken_station, &blue_prev_local_count, blue_prev_local_paths,\
                &blue_prev_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list , \
                lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(blue_prev_local_success_find){
                    //blue_prev_local_success_find==1
                    //blue_prev_local_paths
                    blue_prev_local_count+=1;
                };
            }    
        }
    }
    //-----------------------------------------------------------------
    if(searchByName(green_list,source_station)!=NULL){    //搜尋green

        //該線上的前,後結點
        if((searchByName(green_list,source_station)->next)!=NULL){
            char *green_next;
            strcat( green_next_local_paths,paths);    
            green_next=searchByName(green_list,source_station)->next->name;

            if(strstr(green_next_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                green_next_local_success_find=0;//放棄該條路徑
                green_next_local_count=999;
                strcpy(green_next_local_paths,"");//清空字串
            }            
            else if(strstr(green_next_local_paths,searchByName(green_list,source_station)->next->name)!=NULL){ //出現在path過(重複站名)
                green_next_local_success_find=0;//放棄該條路徑
                green_next_local_count=999;
                strcpy(green_next_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(green_next,dest_station , broken_station,&green_next_local_count, green_next_local_paths,\
                &green_next_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list , \
                lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(green_next_local_success_find){
                    //green_next_local_success_find==1
                    //green_next_local_paths
                    green_next_local_count+=1;
                };
            }    
        }
        if((searchByName(green_list,source_station)->prev)!=NULL){
            char *green_prev;
            strcat( green_prev_local_paths,paths);    
            green_prev=searchByName(green_list,source_station)->prev->name;
            if(strstr(green_prev_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                green_prev_local_success_find=0;//放棄該條路徑
                green_prev_local_count=999;
                strcpy(green_prev_local_paths,"");//清空字串
            } 
            else if(strstr(green_prev_local_paths,searchByName(green_list,source_station)->prev->name)!=NULL){ //出現在path過(重複站名)
                green_prev_local_success_find=0;//放棄該條路徑
                green_prev_local_count=999;
                strcpy(green_prev_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(green_prev,dest_station , broken_station, &green_prev_local_count, green_prev_local_paths,\
                &green_prev_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list , \
                lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(green_prev_local_success_find){
                    //green_prev_local_success_find==1
                    //green_prev_local_paths
                    green_prev_local_count+=1;
                };
            }    
        }
    }
    //-----------------------------------------------------------------
    if(searchByName(brown_list,source_station)!=NULL){    //搜尋brown

        //該線上的前,後結點
        if((searchByName(brown_list,source_station)->next)!=NULL){
            char *brown_next;
            strcat( brown_next_local_paths,paths);    
            brown_next=searchByName(brown_list,source_station)->next->name;
            if(strstr(brown_next_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                brown_next_local_success_find=0;//放棄該條路徑
                brown_next_local_count=999;
                strcpy(brown_next_local_paths,"");//清空字串
            }            
            else if(strstr(brown_next_local_paths,searchByName(brown_list,source_station)->next->name)!=NULL){ //出現在path過(重複站名)
                brown_next_local_success_find=0;//放棄該條路徑
                brown_next_local_count=999;
                strcpy(brown_next_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(brown_next,dest_station , broken_station,&brown_next_local_count, brown_next_local_paths,\
                &brown_next_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list , \
                lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(brown_next_local_success_find){
                    //brown_next_local_success_find==1
                    //brown_next_local_paths
                    brown_next_local_count+=1;
                };
            }    
        }
        if((searchByName(brown_list,source_station)->prev)!=NULL){
            char *brown_prev;
            strcat( brown_prev_local_paths,paths);    
            brown_prev=searchByName(brown_list,source_station)->prev->name;
            if(strstr(brown_prev_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                brown_prev_local_success_find=0;//放棄該條路徑
                brown_prev_local_count=999;
                strcpy(brown_prev_local_paths,"");//清空字串
            } 
            else if(strstr(brown_prev_local_paths,searchByName(brown_list,source_station)->prev->name)!=NULL){ //出現在path過(重複站名)
                brown_prev_local_success_find=0;//放棄該條路徑
                brown_prev_local_count=999;
                strcpy(brown_prev_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(brown_prev,dest_station , broken_station, &brown_prev_local_count, brown_prev_local_paths,\
                &brown_prev_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list , \
                lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(brown_prev_local_success_find){
                    //brown_prev_local_success_find==1
                    //brown_prev_local_paths
                    brown_prev_local_count+=1;
                };
            }    
        }
    }
    //-----------------------------------------------------------------
    if(searchByName(yellow_list,source_station)!=NULL){    //搜尋yellow

        //該線上的前,後結點
        if((searchByName(yellow_list,source_station)->next)!=NULL){
            char *yellow_next;
            strcat( yellow_next_local_paths,paths);    
            yellow_next=searchByName(yellow_list,source_station)->next->name;
            if(strstr(yellow_next_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                yellow_next_local_success_find=0;//放棄該條路徑
                yellow_next_local_count=999;
                strcpy(yellow_next_local_paths,"");//清空字串
            }            
            else if(strstr(yellow_next_local_paths,searchByName(yellow_list,source_station)->next->name)!=NULL){ //出現在path過(重複站名)
                yellow_next_local_success_find=0;//放棄該條路徑
                yellow_next_local_count=999;
                strcpy(yellow_next_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(yellow_next,dest_station , broken_station,&yellow_next_local_count, yellow_next_local_paths,\
                &yellow_next_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list ,\
                 lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(yellow_next_local_success_find){
                    //yellow_next_local_success_find==1
                    //yellow_next_local_paths
                    yellow_next_local_count+=1;
                };
            }    
        }
        if((searchByName(yellow_list,source_station)->prev)!=NULL){
            char *yellow_prev;
            strcat( yellow_prev_local_paths,paths);    
            yellow_prev=searchByName(yellow_list,source_station)->prev->name;
            if(strstr(yellow_prev_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                yellow_prev_local_success_find=0;//放棄該條路徑
                yellow_prev_local_count=999;
                strcpy(yellow_prev_local_paths,"");//清空字串
            } 
            else if(strstr(yellow_prev_local_paths,searchByName(yellow_list,source_station)->prev->name)!=NULL){ //出現在path過(重複站名)
                yellow_prev_local_success_find=0;//放棄該條路徑
                yellow_prev_local_count=999;
                strcpy(yellow_prev_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(yellow_prev,dest_station , broken_station, &yellow_prev_local_count, yellow_prev_local_paths,\
                &yellow_prev_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list , \
                lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(yellow_prev_local_success_find){
                    //yellow_prev_local_success_find==1
                    //yellow_prev_local_paths
                    yellow_prev_local_count+=1;
                };
            }    
        }
    }
    //-----------------------------------------------------------------
    if(searchByName(purple_list,source_station)!=NULL){

        //該線上的前,後結點
        if((searchByName(purple_list,source_station)->next)!=NULL){
            char *purple_next;
            strcat( purple_next_local_paths,paths);    
            purple_next=searchByName(purple_list,source_station)->next->name;
            if(strstr(purple_next_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                purple_next_local_success_find=0;//放棄該條路徑
                purple_next_local_count=999;
                strcpy(purple_next_local_paths,"");//清空字串
            }            
            else if(strstr(purple_next_local_paths,searchByName(purple_list,source_station)->next->name)!=NULL){ //出現在path過(重複站名)
                purple_next_local_success_find=0;//放棄該條路徑
                purple_next_local_count=999;
                strcpy(purple_next_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(purple_next,dest_station , broken_station,&purple_next_local_count, purple_next_local_paths,\
                &purple_next_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list ,\
                 lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(purple_next_local_success_find){
                    //purple_next_local_success_find==1
                    //purple_next_local_paths
                    purple_next_local_count+=1;
                };
            }    
        }
        if((searchByName(purple_list,source_station)->prev)!=NULL){
            char *purple_prev;
            strcat( purple_prev_local_paths,paths);    
            purple_prev=searchByName(purple_list,source_station)->prev->name;
            if(strstr(purple_prev_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                purple_prev_local_success_find=0;//放棄該條路徑
                purple_prev_local_count=999;
                strcpy(purple_prev_local_paths,"");//清空字串
            } 
            else if(strstr(purple_prev_local_paths,searchByName(purple_list,source_station)->prev->name)!=NULL){ //出現在path過(重複站名)
                purple_prev_local_success_find=0;//放棄該條路徑
                purple_prev_local_count=999;
                strcpy(purple_prev_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(purple_prev,dest_station , broken_station, &purple_prev_local_count, purple_prev_local_paths,\
                &purple_prev_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list ,\
                 lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(purple_prev_local_success_find){
                    //purple_prev_local_success_find==1
                    //purple_prev_local_paths
                    purple_prev_local_count+=1;
                };
            }    
        }
    }
    //-----------------------------------------------------------------
    if(searchByName(lightgreen_list,source_station)!=NULL){    //搜尋lightgreen

        //該線上的前,後結點
        if((searchByName(lightgreen_list,source_station)->next)!=NULL){
            char *lightgreen_next;
            strcat( lightgreen_next_local_paths,paths);    
            lightgreen_next=searchByName(lightgreen_list,source_station)->next->name;
            if(strstr(lightgreen_next_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                lightgreen_next_local_success_find=0;//放棄該條路徑
                lightgreen_next_local_count=999;
                strcpy(lightgreen_next_local_paths,"");//清空字串
            }            
            else if(strstr(lightgreen_next_local_paths,searchByName(lightgreen_list,source_station)->next->name)!=NULL){ //出現在path過(重複站名)
                lightgreen_next_local_success_find=0;//放棄該條路徑
                lightgreen_next_local_count=999;
                strcpy(lightgreen_next_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(lightgreen_next,dest_station , broken_station,&lightgreen_next_local_count, lightgreen_next_local_paths,\
                &lightgreen_next_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list ,\
                 lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(lightgreen_next_local_success_find){
                    //lightgreen_next_local_success_find==1
                    //lightgreen_next_local_paths
                    lightgreen_next_local_count+=1;
                };
            }    
        }
        if((searchByName(lightgreen_list,source_station)->prev)!=NULL){
            char *lightgreen_prev;
            strcat( lightgreen_prev_local_paths,paths);    
            lightgreen_prev=searchByName(lightgreen_list,source_station)->prev->name;
            if(strstr(lightgreen_prev_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                lightgreen_prev_local_success_find=0;//放棄該條路徑
                lightgreen_prev_local_count=999;
                strcpy(lightgreen_prev_local_paths,"");//清空字串
            } 
            else if(strstr(lightgreen_prev_local_paths,searchByName(lightgreen_list,source_station)->prev->name)!=NULL){ //出現在path過(重複站名)
                lightgreen_prev_local_success_find=0;//放棄該條路徑
                lightgreen_prev_local_count=999;
                strcpy(lightgreen_prev_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(lightgreen_prev,dest_station , broken_station, &lightgreen_prev_local_count, lightgreen_prev_local_paths,\
                &lightgreen_prev_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list ,\
                 lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(lightgreen_prev_local_success_find){
                    //lightgreen_prev_local_success_find==1
                    //lightgreen_prev_local_paths
                    lightgreen_prev_local_count+=1;
                };
            }    
        }
    }
    //-----------------------------------------------------------------
    if(searchByName(orange1_list,source_station)!=NULL){    //搜尋orange1

        //該線上的前,後結點
        if((searchByName(orange1_list,source_station)->next)!=NULL){
            char *orange1_next;
            strcat( orange1_next_local_paths,paths);    
            orange1_next=searchByName(orange1_list,source_station)->next->name;
            if(strstr(orange1_next_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                orange1_next_local_success_find=0;//放棄該條路徑
                orange1_next_local_count=999;
                strcpy(orange1_next_local_paths,"");//清空字串
            }            
            else if(strstr(orange1_next_local_paths,searchByName(orange1_list,source_station)->next->name)!=NULL){ //出現在path過(重複站名)
                orange1_next_local_success_find=0;//放棄該條路徑
                orange1_next_local_count=999;
                strcpy(orange1_next_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(orange1_next,dest_station , broken_station,&orange1_next_local_count, orange1_next_local_paths,\
                &orange1_next_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list ,\
                 lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(orange1_next_local_success_find){
                    //orange1_next_local_success_find==1
                    //orange1_next_local_paths
                    orange1_next_local_count+=1;
                };
            }    
        }
        if((searchByName(orange1_list,source_station)->prev)!=NULL){
            char *orange1_prev;
            strcat( orange1_prev_local_paths,paths);    
            orange1_prev=searchByName(orange1_list,source_station)->prev->name;
            if(strstr(orange1_prev_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                orange1_prev_local_success_find=0;//放棄該條路徑
                orange1_prev_local_count=999;
                strcpy(orange1_prev_local_paths,"");//清空字串
            } 
            else if(strstr(orange1_prev_local_paths,searchByName(orange1_list,source_station)->prev->name)!=NULL){ //出現在path過(重複站名)
                orange1_prev_local_success_find=0;//放棄該條路徑
                orange1_prev_local_count=999;
                strcpy(orange1_prev_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(orange1_prev,dest_station , broken_station, &orange1_prev_local_count, orange1_prev_local_paths,\
                &orange1_prev_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list , \
                lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(orange1_prev_local_success_find){
                    //orange1_prev_local_success_find==1
                    //orange1_prev_local_paths
                    orange1_prev_local_count+=1;
                };
            }    
        }
    }
    //-----------------------------------------------------------------
    if(searchByName(orange2_list,source_station)!=NULL){    //搜尋orange2

        //該線上的前,後結點
        if((searchByName(orange2_list,source_station)->next)!=NULL){
            char *orange2_next;
            strcat( orange2_next_local_paths,paths);    
            orange2_next=searchByName(orange2_list,source_station)->next->name;
            if(strstr(orange2_next_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                orange2_next_local_success_find=0;//放棄該條路徑
                orange2_next_local_count=999;
                strcpy(orange2_next_local_paths,"");//清空字串
            }            
            else if(strstr(orange2_next_local_paths,searchByName(orange2_list,source_station)->next->name)!=NULL){ //出現在path過(重複站名)
                orange2_next_local_success_find=0;//放棄該條路徑
                orange2_next_local_count=999;
                strcpy(orange2_next_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(orange2_next,dest_station , broken_station,&orange2_next_local_count, orange2_next_local_paths,\
                &orange2_next_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list , \
                lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(orange2_next_local_success_find){
                    //orange2_next_local_success_find==1
                    //orange2_next_local_paths
                    orange2_next_local_count+=1;
                };
            }    
        }
        if((searchByName(orange2_list,source_station)->prev)!=NULL){
            char *orange2_prev;
            strcat( orange2_prev_local_paths,paths);    
            orange2_prev=searchByName(orange2_list,source_station)->prev->name;
            if(strstr(orange2_prev_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                orange2_prev_local_success_find=0;//放棄該條路徑
                orange2_prev_local_count=999;
                strcpy(orange2_prev_local_paths,"");//清空字串
            } 
            else if(strstr(orange2_prev_local_paths,searchByName(orange2_list,source_station)->prev->name)!=NULL){ //出現在path過(重複站名)
                orange2_prev_local_success_find=0;//放棄該條路徑
                orange2_prev_local_count=999;
                strcpy(orange2_prev_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(orange2_prev,dest_station , broken_station, &orange2_prev_local_count, orange2_prev_local_paths,\
                &orange2_prev_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list ,\
                 lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(orange2_prev_local_success_find){
                    //orange2_prev_local_success_find==1
                    //orange2_prev_local_paths
                    orange2_prev_local_count+=1;
                };
            }    
        }
    }
    //-----------------------------------------------------------------
    if(searchByName(pink_list,source_station)!=NULL){    //搜尋pink

        //該線上的前,後結點
        if((searchByName(pink_list,source_station)->next)!=NULL){
            char *pink_next;
            strcat( pink_next_local_paths,paths);    
            pink_next=searchByName(pink_list,source_station)->next->name;
            if(strstr(pink_next_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                pink_next_local_success_find=0;//放棄該條路徑
                pink_next_local_count=999;
                strcpy(pink_next_local_paths,"");//清空字串
            }            
            else if(strstr(pink_next_local_paths,searchByName(pink_list,source_station)->next->name)!=NULL){ //出現在path過(重複站名)
                pink_next_local_success_find=0;//放棄該條路徑
                pink_next_local_count=999;
                strcpy(pink_next_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(pink_next,dest_station , broken_station,&pink_next_local_count, pink_next_local_paths,\
                &pink_next_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list ,\
                 lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(pink_next_local_success_find){
                    //pink_next_local_success_find==1
                    //pink_next_local_paths
                    pink_next_local_count+=1;
                };
            }    
        }
        if((searchByName(pink_list,source_station)->prev)!=NULL){
            char *pink_prev;
            strcat( pink_prev_local_paths,paths);    
            pink_prev=searchByName(pink_list,source_station)->prev->name;
            if(strstr(pink_prev_local_paths,broken_station)!=NULL){ //要避開的站名出現在path
                pink_prev_local_success_find=0;//放棄該條路徑
                pink_prev_local_count=999;
                strcpy(pink_prev_local_paths,"");//清空字串
            } 
            else if(strstr(pink_prev_local_paths,searchByName(pink_list,source_station)->prev->name)!=NULL){ //出現在path過(重複站名)
                pink_prev_local_success_find=0;//放棄該條路徑
                pink_prev_local_count=999;
                strcpy(pink_prev_local_paths,"");//清空字串
            }
            else{
                find_shortest_path(pink_prev,dest_station , broken_station, &pink_prev_local_count, pink_prev_local_paths,\
                &pink_prev_local_success_find,red_list , blue_list , green_list , brown_list , yellow_list , purple_list , \
                lightgreen_list , orange1_list , orange2_list , pink_list);  
                if(pink_prev_local_success_find){
                    //pink_prev_local_success_find==1
                    //pink_prev_local_paths
                    pink_prev_local_count+=1;
                };
            }    
        }
    }
    //-----------------------------------------------------------------



    //考慮每條顏色的可能後,paths加上每條篩選過後的最短路徑
    // red blue green brown yellow purple lightgreen orange1 orange2 pink
    if (red_prev_local_success_find||red_next_local_success_find||blue_prev_local_success_find||blue_next_local_success_find||\
    green_prev_local_success_find||green_next_local_success_find ||brown_prev_local_success_find||brown_next_local_success_find||\
    yellow_prev_local_success_find||yellow_next_local_success_find||purple_prev_local_success_find||purple_next_local_success_find||\
    lightgreen_prev_local_success_find||lightgreen_next_local_success_find||orange1_prev_local_success_find||orange1_next_local_success_find||\
    orange2_prev_local_success_find||orange2_next_local_success_find||pink_prev_local_success_find||pink_next_local_success_find){
        *success_find=1; 
        char tmp_paths[1000]="";
        int tmp_count=990;
        if(red_next_local_count!=0&&red_next_local_count<tmp_count){strcpy(tmp_paths,red_next_local_paths); tmp_count=red_next_local_count;}
        if(red_prev_local_count!=0&&red_prev_local_count<tmp_count){strcpy(tmp_paths,red_prev_local_paths); tmp_count=red_prev_local_count; }
        if(blue_next_local_count!=0&&blue_next_local_count<tmp_count){strcpy(tmp_paths,blue_next_local_paths); tmp_count=blue_next_local_count;}
        if(blue_prev_local_count!=0&&blue_prev_local_count<tmp_count){strcpy(tmp_paths,blue_prev_local_paths); tmp_count=blue_prev_local_count;}
        if(green_next_local_count!=0&&green_next_local_count<tmp_count){strcpy(tmp_paths,green_next_local_paths); tmp_count=green_next_local_count;}
        if(green_prev_local_count!=0&&green_prev_local_count<tmp_count){strcpy(tmp_paths,green_prev_local_paths); tmp_count=green_prev_local_count;}
        if(lightgreen_next_local_count!=0&&lightgreen_next_local_count<tmp_count){strcpy(tmp_paths,lightgreen_next_local_paths); tmp_count=lightgreen_next_local_count;}
        if(lightgreen_prev_local_count!=0&&lightgreen_prev_local_count<tmp_count){strcpy(tmp_paths,lightgreen_prev_local_paths); tmp_count=lightgreen_prev_local_count;}
        if(brown_next_local_count!=0&&brown_next_local_count<tmp_count){strcpy(tmp_paths,brown_next_local_paths); tmp_count=brown_next_local_count;}
        if(brown_prev_local_count!=0&&brown_prev_local_count<tmp_count){strcpy(tmp_paths,brown_prev_local_paths); tmp_count=brown_prev_local_count;}
        if(yellow_next_local_count!=0&&yellow_next_local_count<tmp_count){strcpy(tmp_paths,yellow_next_local_paths); tmp_count=yellow_next_local_count;}
        if(yellow_prev_local_count!=0&&yellow_prev_local_count<tmp_count){strcpy(tmp_paths,yellow_prev_local_paths); tmp_count=yellow_prev_local_count;}
        if(purple_next_local_count!=0&&purple_next_local_count<tmp_count){strcpy(tmp_paths,purple_next_local_paths); tmp_count=purple_next_local_count;}
        if(purple_prev_local_count!=0&&purple_prev_local_count<tmp_count){strcpy(tmp_paths,purple_prev_local_paths); tmp_count=purple_prev_local_count;}
        if(orange1_next_local_count!=0&&orange1_next_local_count<tmp_count){strcpy(tmp_paths,orange1_next_local_paths); tmp_count=orange1_next_local_count;}
        if(orange1_prev_local_count!=0&&orange1_prev_local_count<tmp_count){strcpy(tmp_paths,orange1_prev_local_paths); tmp_count=orange1_prev_local_count;}
        if(orange2_next_local_count!=0&&orange2_next_local_count<tmp_count){strcpy(tmp_paths,orange2_next_local_paths); tmp_count=orange2_next_local_count;}
        if(orange2_prev_local_count!=0&&orange2_prev_local_count<tmp_count){strcpy(tmp_paths,orange2_prev_local_paths); tmp_count=orange2_prev_local_count;}
        if(pink_next_local_count!=0&&pink_next_local_count<tmp_count){strcpy(tmp_paths,pink_next_local_paths); tmp_count=pink_next_local_count;}
        if(pink_prev_local_count!=0&&pink_prev_local_count<tmp_count){strcpy(tmp_paths,pink_prev_local_paths); tmp_count=pink_prev_local_count;}

        strcpy(paths,tmp_paths);          
       *count=tmp_count;
    }



    else{//完全沒有可行路徑
        *success_find=0;
        *count=999; 
        }

}
}

int main(){
    // 初始化一個空的雙向鏈結串列(依不同顏色創建)
    struct Node* red_list = NULL;struct Node* blue_list = NULL;struct Node* green_list = NULL;struct Node* brown_list = NULL;
    struct Node* yellow_list = NULL;struct Node* purple_list = NULL;struct Node* lightgreen_list = NULL;struct Node* orange1_list = NULL;
    struct Node* orange2_list = NULL;struct Node* pink_list = NULL;
    //創建各站點
    append(&red_list,"r01","象山");append(&red_list,"r02","台北101/世貿");append(&red_list,"r03","信義安和");append(&red_list,"r04","(大安)");
    append(&red_list,"r05","大安森林公園");append(&red_list,"r06","東門");append(&red_list,"r07","中正紀念堂");append(&red_list,"r08","臺大醫院");
    append(&red_list,"r09","台北車站");append(&red_list,"r10","中山");append(&red_list,"r11","雙連");append(&red_list,"r12","民權西路");
    append(&red_list,"r13","圓山");append(&red_list,"r14","劍潭");append(&red_list,"r15","士林");append(&red_list,"r16","芝山");
    append(&red_list,"r17","明德");append(&red_list,"r18","石牌");append(&red_list,"r19","唭哩岸");append(&red_list,"r20","奇岩");
    append(&red_list,"r21","北投");append(&red_list,"r22","復興崗");append(&red_list,"r23","忠義");append(&red_list,"r24","關渡");
    append(&red_list,"r25","竹圍");append(&red_list,"r26","紅樹林");append(&red_list,"r27","淡水");append(&blue_list,"b01","頂埔");
    append(&blue_list,"b02","永寧");append(&blue_list,"b03","土城");append(&blue_list,"b04","海山");append(&blue_list,"b05","亞東醫院");
    append(&blue_list,"b06","府中");append(&blue_list,"b07","板橋");append(&blue_list,"b08","新埔");append(&blue_list,"b09","江子翠");
    append(&blue_list,"b10","龍山寺");append(&blue_list,"b11","西門");append(&blue_list,"b12","台北車站");append(&blue_list,"b13","善導寺");
    append(&blue_list,"b14","忠孝新生");append(&blue_list,"b15","忠孝復興");append(&blue_list,"b16","忠孝敦化");append(&blue_list,"b17","國父紀念館");
    append(&blue_list,"b18","市政府");append(&blue_list,"b19","永春");append(&blue_list,"b20","後山埤");append(&blue_list,"b21","昆陽");
    append(&blue_list,"b22","南港");append(&blue_list,"b23","南港展覽館");append(&green_list,"g01","新店");append(&green_list,"g02","新店區公所");
    append(&green_list,"g03","七張");append(&green_list,"g04","大坪林");append(&green_list,"g05","景美");append(&green_list,"g06","萬隆");
    append(&green_list,"g07","公館");append(&green_list,"g08","台電大樓");append(&green_list,"g09","古亭");append(&green_list,"g10","中正紀念堂");
    append(&green_list,"g11","小南門");append(&green_list,"g12","西門");append(&green_list,"g13","北門");append(&green_list,"g14","中山");
    append(&green_list,"g15","松江南京");append(&green_list,"g16","南京復興");append(&green_list,"g17","台北小巨蛋");append(&green_list,"g18","南京三民");
    append(&green_list,"g19","松山");append(&brown_list,"b01","動物園");append(&brown_list,"b02","木柵");append(&brown_list,"b03","萬芳社區");
    append(&brown_list,"b04","萬芳醫院");append(&brown_list,"b05","辛亥");append(&brown_list,"b06","麟光");append(&brown_list,"b07","六張犁");
    append(&brown_list,"b08","科技大樓");append(&brown_list,"b09","大安");append(&brown_list,"b10","忠孝復興");append(&brown_list,"b11","南京復興");
    append(&brown_list,"b12","中山國中");append(&brown_list,"b13","松山機場");append(&brown_list,"b14","大直");append(&brown_list,"b15","劍南路");
    append(&brown_list,"b16","西湖");append(&brown_list,"b17","港墘");append(&brown_list,"b18","文德");append(&brown_list,"b19","內湖");
    append(&brown_list,"b20","大湖公園");append(&brown_list,"b21","葫洲");append(&brown_list,"b22","東湖");append(&brown_list,"b23","南港軟體園區");
    append(&brown_list,"b24","南港展覽館");append(&yellow_list,"y01","大坪林");append(&yellow_list,"y02","十四張");append(&yellow_list,"y03","秀朗橋");
    append(&yellow_list,"y04","景平");append(&yellow_list,"y05","景安");append(&yellow_list,"y06","中和");append(&yellow_list,"y07","橋和");
    append(&yellow_list,"y08","中原");append(&yellow_list,"y09","板新");append(&yellow_list,"y10","板橋");append(&yellow_list,"y11","新埔民生");
    append(&yellow_list,"y12","頭前庄");append(&yellow_list,"y13","幸福");append(&yellow_list,"y14","新北產業園區");append(&purple_list,"p01","西門");
    append(&purple_list,"p02","三重");append(&purple_list,"p03","新北產業園區");append(&purple_list,"p04","機場捷運");append(&lightgreen_list,"lg01","小碧潭");
    append(&lightgreen_list,"lg02","七張");append(&orange1_list,"or01","南勢角");append(&orange1_list,"or02","景安");append(&orange1_list,"or03","永安市場");
    append(&orange1_list,"or04","頂溪");append(&orange1_list,"or05","古亭");append(&orange1_list,"or06","東門");append(&orange1_list,"or07","忠孝新生");
    append(&orange1_list,"or08","松江南京");append(&orange1_list,"or09","行天宮");append(&orange1_list,"or10","中山國小");append(&orange1_list,"or11","民權西路");
    append(&orange1_list,"or12","大橋頭");append(&orange1_list,"or13","台北橋");append(&orange1_list,"or14","菜寮");append(&orange1_list,"or15","三重");
    append(&orange1_list,"or16","先嗇宮");append(&orange1_list,"or17","頭前庄");append(&orange1_list,"or18","新莊");append(&orange1_list,"or19","輔大");
    append(&orange1_list,"or20","丹鳳");append(&orange1_list,"or21","迴龍");append(&orange2_list,"orr01","大橋頭");append(&orange2_list,"orr02","三重國小");
    append(&orange2_list,"orr03","三和國中");append(&orange2_list,"orr04","徐匯中學");append(&orange2_list,"orr05","三民高中");append(&orange2_list,"orr06","蘆洲");
    

    int count=0;
    char paths[1000]="";
    int success_find=0;



    char source[100];
    char destination[100];
    char broken_station[100];
    printf("輸入起始站: ");
    scanf("%s", source);
    printf("輸入終點站: ");
    scanf(" %s", destination);
    printf("輸入避開的站: ");
    scanf(" %s", broken_station);

    find_shortest_path(source,destination,broken_station , &count, paths,&success_find,red_list , blue_list , \
    green_list , brown_list , yellow_list , purple_list , lightgreen_list , orange1_list , orange2_list , pink_list);
    //輸出結果
    if((*&count)!=999){
    printf("經過 %d 站\n花費 : %d 元\n",*&count,(*&count)*5);
    printf("最短path:%s\n",paths);}
    else{
    printf("%d",*&count);
    printf("未找到任何路徑\n");
    }


    return 0;
}