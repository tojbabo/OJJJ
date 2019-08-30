// UDP 서버
// 우분투. C언어
// 클라이언트에서 받은 데이터를 그대로 다시 클라이언트에게로 전송함

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 9000
#define BUF 1024

void main(){
  int sock;
  struct sockaddr_in_addr, client_addr;
  char buffer[BUF];
  int recv_len;
  int addr_len;
  
  if((sock = socket(AF_INET, SOCK_DGRAM,0))<0){
    puts("socket() error");
    return;
  }
  memset(&addr,0x00,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(PORT);
  if(bind(sock,(sturct sockaddr *)&addr,sizeof(addr))<0){
    puts("bind() error");
    return;
  }
  while(1){
    printf("watiting for message\n");
    addr_len= sizeof(client_addr);
    if((recv_len = recvfrom(sock,buffer,BUF,0,(struct sockaddr*)
                &client_addr,&addr_len)) <0){
      puts("recv() eeror");
      return;
    }
    buffer[recv_len] = '\0';
    printf("ip : %s\n",inet_ntoa(client_addr.sin_addr));
    printf("recv data : %s \n",buffer);
    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)
                &client_addr,sizeof(client_addr));
    }
  close(sock);  
}
