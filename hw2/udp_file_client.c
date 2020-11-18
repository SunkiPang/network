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
	int sd;
	FILE *fp;

	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr, from_adr;
	socklen_t adr_sz;

	if (argc != 4)
	{
		printf("Usage: %s <IP> <port> <filename>\n", argv[0]);
		exit(1);
	}
	fp = fopen(argv[3], "rb");
	//fp = fopen("receive.dat", "wb");
	sd = socket(PF_INET, SOCK_DGRAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	// connect(sd, (struct sockaddr *)&serv_adr, sizeof(serv_adr));

	//여기까진 ㅇㅋ

	// while(1)
	// {
	// 	fputs("Insert message(q to quit): ", stdout);
	// 	fgets(message, sizeof(message), stdin);
	// 	if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
	// 		break;

	// 	sendto(sock, message, strlen(message), 0,
	// 				(struct sockaddr*)&serv_adr, sizeof(serv_adr));
	// 	adr_sz=sizeof(from_adr);
	// 	str_len=recvfrom(sock, message, BUF_SIZE, 0,
	// 				(struct sockaddr*)&from_adr, &adr_sz);

	// 	message[str_len]=0;
	// 	printf("Message from server: %s", message);
	// }

	while (1)
	{
		read_cnt = fread((void *)buf, 1, BUF_SIZE, fp);
		if (read_cnt < BUF_SIZE)
		{
			sendto(sd, buf, read_cnt, 0, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
			// write(sd, buf, read_cnt);
			break;
		}
		sendto(sd, buf, BUF_SIZE, 0, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
		// write(sd, buf, BUF_SIZE);
	}

	// shutdown(sd, SHUT_WR);
	int str_len = recvfrom(sd, buf, BUF_SIZE, 0, (struct sockaddr *)&from_adr, sizeof(from_adr));
	// read(sd, buf, BUF_SIZE);
	buf[str_len] = 0;
	printf("Message from client: %s \n", buf);

	fclose(fp);
	close(sd);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}