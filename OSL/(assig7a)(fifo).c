#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    int fd;
    char *  myfifo = "/tmp/myfifo";
    mkfifo(myfifo,0777);
    char arr1[80],arr2[80];
    while(1){
        fd = open(myfifo,O_WRONLY);
        printf("Enter Messeage:");
        if(fgets(arr1,80,stdin)!=NULL){
            write(fd,arr1,strlen(arr1)+1);
        }
        close(fd);

        fd = open(myfifo,O_RDONLY);
        read(fd,arr2,sizeof(arr2));
        printf("User2:%s\n",arr2);
        close(fd);
    }
    return 0;
}
