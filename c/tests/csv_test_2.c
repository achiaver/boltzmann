#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char buffer[1024] ;
    char *record,*line;
    int i=0,j=0,aux=0;
    int mat[100][100];
    FILE *fstream = fopen("csv_test_2.csv", "r");
    if(fstream == NULL)
    {
        printf("\n file opening failed ");
        return -1 ;
    }
    while((line = fgets(buffer, sizeof(buffer), fstream)) != NULL)
    {
        printf("i - %d, j - %d \n", i, j);
        record = strtok(line, ",");
        while(record != NULL)
        {
            printf("record : %s \n", record);    //here you can put the record into the array as per your requirement.
            mat[i][j++] = atoi(record);
            record = strtok(NULL, ",");
            printf("i - %d, j - %d \n", i, j);
        }
        aux = j;
        j = 0;
        printf("\n");
        ++i ;
    }
    printf("\n"); 
    printf("i - %d, j - %d \n", i, aux);
    for (int k = 0; k < i; k++)
    {
        for (int l = 0; l < aux; l++)
        {
            printf("%d ", mat[k][l]);
        }
        printf("\n");
    }
    fclose(fstream);
    return 0 ;
}
