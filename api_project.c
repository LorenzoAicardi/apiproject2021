#include <stdio.h>
#include <stdlib.h>
#define long_MAX 2147483647

long d, k;

typedef struct nodevalue{ //questa struct mi definisce i due valori chiave delle matrici di adiacenza: quanto vale la somma dei cammini (key) e il numero identificativo della matrice
    long numeromat;
    long key;
}nodevalue;

long dkalgo(long [][d], long, long); //djkstra
long minlen(long [], long [], long); //funzione di supporto per djkstra
long equalstr(char [], char []); //funzione per prendere il comando
void build_max_heap(long, nodevalue []);
void MaxHeapify(nodevalue [], long, long);
void doitall(FILE *, long, long, long, nodevalue []); //specificato nel main
void dotopk(FILE * fp, long, nodevalue [], long, long); //anche

////////MAIN////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                                                                                                                                    
int main(int argc, char * argv[]){                                                                                                                                  
                                                                                                                                                                    
    FILE * fp;                                                                                             
    long count = 0, chk, chktopn;
    long retina;
    long * ret;          
    //long chktop;                                                                                                                      
    char comando[20], aggiungigrafo[20] = "AggiungiGrafo\n", topkn[20] = "TopK\n";       
    //char topk[20] = "TopK";                                                                                
                                                                                                                                                                
    if((fp = fopen("input_1", "r"))){                                                                                                                           
        retina = fscanf(fp,"%ld %ld\n", &d, &k); //leggo rispettivamente la dimensione della matrice di adiacenza e il numero di grafi che mi longeressano   GIUSTO            
        retina++;                         
        struct nodevalue list[k];  //creo l'array di struct di dimensione k
        for(long i = 0; i < k; i++){
            list[i].key = 0;
            list[i].numeromat = -1;
        }
        while(!feof(fp)){ //fino alla fine del file
            ret = (long*)fgets(comando, 20, fp); //leggo il comando che segue i due parametri LEGGE IL COMANDO BENE
            chk = equalstr(comando, aggiungigrafo);
            chktopn = equalstr(comando, topkn);
            if(chk == 1){  
                doitall(fp, d, k, count, list); //questo è il problema
                count++;
                chk = 0;
            }
            if(chktopn == 1 && !feof(fp)){ // !feof serve per non fare due topk
                dotopk(fp, d, list, k, count);
                printf("\n");
                chktopn = 0;
            }
        }
        ret++;
        fclose(fp);
    }else{
        printf("errore di apertura del file");
    }
    return 0;
}

//////MAIN//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void doitall(FILE * fp, long d, long k, long count, nodevalue list[]){  //djkstra with extra steps

    long dkvalue;
    long mat[d][d]; 
    int *niente;
    char riga[200000];
    char *car;
    long elem;
    /*for(long i = 0; i < d; i++){ 
        for(long j = 0; j < d; j++){
            niente = fscanf(fp, "%ld,", &mat[i][j]);
        }
    }*/
    for(long i = 0; i < d; i++){
        niente = (int*)fgets(riga, long_MAX, fp);
        //prlongf("%s", riga);
        elem = strtol(riga, &car, 10);
        mat[i][0] = elem;
        for(long j = 1; j < d;j++){
            elem = strtol(car+1,&car,10);
            mat[i][j] = elem;
        }
    }
    niente++;
    dkvalue = dkalgo(mat, 0, d); //algoritmo di dijkstra + sum dei pesi degli archi da 0 a ogni altro nodo
    if(count < k){   
            list[count].key = dkvalue; //insert in max heap
            list[count].numeromat = count;
            MaxHeapify(list, count, k);
        }else if(count >= k){  //da cambiare: in max heap il più grande è sempre in testa, basta confrontare il primo elemento col nuovo. se è minore, sostituisco e ricostruisco max heap
                if(dkvalue < list[0].key){
                    list[0].key = dkvalue;
                    list[0].numeromat = count;
                    build_max_heap(k, list);
                }
        }
}

long dkalgo(long mat[d][d], long src, long dimgraph){ //NON GESTISCE IL CASO IN CUI SE DIST == long_MAX, ALLORA VALE 0

    long dist[dimgraph];
    long existspath[dimgraph]; //se ho processato il nodo
    long i, val, sum = 0;
    for (long i = 0; i < dimgraph; i++){
        dist[i] = long_MAX;
        existspath[i] = 0;
    }

    dist[src] = 0;

    for(i = 0; i < dimgraph-1; i++){
        val = minlen(dist, existspath, dimgraph);
        existspath[val] = 1;
        for(long v = 0; v < dimgraph; v++)
            if(existspath[v] == 0 && mat[val][v] && dist[val] != long_MAX && dist[val] + mat[val][v] < dist[v])
                dist[v] = dist[val] + mat[val][v];
    }
    //prlongf("Ecco i pesi degli archi che vanno dal nodo 0 ad ogni altro nodo, in ordine:\n");

    for(long i = 0; i < dimgraph; i++){
        if(dist[i] == long_MAX)
            dist[i] = 0;
        sum = sum + dist[i];
    }

    return sum;
}

long minlen(long dist[], long existspath[], long dimgraph){ //FUNZIONE DI SUPPORTO PER DJKSTRA, IGNORE
    long min = long_MAX;
    long min_index = -1; //forse qui c'è un errore, warning
 
    for (long v = 0; v < dimgraph; v++)
        if (existspath[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}

//FUNZIONI DI HEAP

void dotopk(FILE * fp, long dim, nodevalue list[], long k, long count){ //FUNZIONE TOPK

    if(count < k){
        for(long l = 0; l < count-1; l++){
            printf("%ld ", list[l].numeromat);
        }
        if(count != 0)
            printf("%ld", list[count-1].numeromat);
    }else if(count >= k){
        for(long l = 0; l < k-1; l++){
            printf("%ld ", list[l].numeromat);
        }
        printf("%ld", list[k-1].numeromat);
    }
}


void build_max_heap(long n, nodevalue arr[]){ //n = lungh arr, arr 
    for(long i = n/2; i >= 0; i--)
        MaxHeapify(arr, i, n);
}

void MaxHeapify(nodevalue arr[], long i, long n){ //n è len array, i è posizione in questione 

    long l = 2*i + 1;
    long r = 2*i + 2;
    long posmax, tp, tp2;

    if(l < n && arr[l].key > arr[i].key){
        posmax = l;
    }else{
        posmax = i;
    }
    if(r < n && arr[r].key > arr[posmax].key){
        posmax = r;
    }
    if(posmax != i){
        //swap arr[n] e arr[posmax]
        tp = arr[i].key;
        arr[i].key = arr[posmax].key;
        arr[posmax].key = tp;
        tp2 = arr[i].numeromat;
        arr[i].numeromat = arr[posmax].numeromat;
        arr[posmax].numeromat = tp2;
        MaxHeapify(arr, posmax, n);
    }
}


//IGNORE



long equalstr(char comando[20], char option[20]){ //FUNZIONE PER CHECK DI EQUALSTRING FUNZIONA, NON RICONTROLLARE

    long i = 0, check = 1;
    while(comando[i]!='\0' || option[i]!='\0'){
        if(comando[i]!=option[i]){
            check = 0;
            break;
        }
        i++;
    }
    return check;
}

