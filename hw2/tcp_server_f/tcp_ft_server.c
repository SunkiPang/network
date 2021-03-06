#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{

	int serv_sd, clnt_sd;
	FILE *fp;
	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	//fp=fopen("file_server.c", "rb");

	fp = fopen("receive.dat", "wb");
	serv_sd = socket(PF_INET, SOCK_STREAM, 0); // 서버측 소켓 주소

	memset(&serv_adr, 0, sizeof(serv_adr)); //서버측 구조체 초기화

	//구조체의 요소에 값을 넣어주는 것.
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); //host to network
	serv_adr.sin_port = htons(atoi(argv[1]));	  //포트번호
	//여기까지

	//바인딩
	bind(serv_sd, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
	//구조체의 주소를 알려주고
	listen(serv_sd, 5);
	//listen상태로 만드는 것. 소켓이 클라이언트의 리턴을 받을 수 있게 하는 것.
	//뒤의 5는 대기 메시지 queue의 갯수

	//여기 위에까지가 소켓 생성하고 바인딩하고 클라이언트에게 받을 수 있는 상태를 만든 것.

	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
	//accept에서 클라이언트의 요청이 오는 것을 대기.
	//클라이언트가 요청했으니 요청한 클라이언트의 정보를 담은 clnt_sd가 만들어짐

	//여기까진 괜츈

	while ((read_cnt = read(clnt_sd, buf, BUF_SIZE)) != 0)
		fwrite((void *)buf, 1, read_cnt, fp);

	puts("Received file data");
	write(clnt_sd, "Thank you", 10);
	fclose(fp);
	close(serv_sd);

	return 0;

	// while (1)
	// {
	// 	read_cnt = fread((void *)buf, 1, BUF_SIZE, fp);
	// 	if (read_cnt < BUF_SIZE)
	// 	{
	// 		write(clnt_sd, buf, read_cnt);
	// 		break;
	// 	}
	// 	write(clnt_sd, buf, BUF_SIZE);
	// }

	// shutdown(clnt_sd, SHUT_WR);
	// read(clnt_sd, buf, BUF_SIZE);
	// printf("Message from client: %s \n", buf);

	// fclose(fp);
	// close(clnt_sd);
	// close(serv_sd);

	// return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
