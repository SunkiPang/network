#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char *message);

typedef struct packet
{
	char data[1024];
} Packet;

typedef struct frame
{
	int frame_kind; //ACK:0, SEQ:1 FIN:2
	int sq_no;
	int ack;
	Packet packet;
} Frame;

int main(int argc, char *argv[])
{
	int bug_check = 0;
	int sd;
	FILE *fp;

	char buf[BUF_SIZE];
	char buff[BUF_SIZE];
	int read_cnt;

	int frame_id = 0;
	Frame frame_send;
	Frame frame_recv;
	int ack_recv = 1;

	struct sockaddr_in serv_adr, from_adr;
	socklen_t adr_sz;

	printf("checkcheck $d\n", bug_check++);

	if (argc != 4)
	{
		printf("Usage: %s <IP> <port> <filename>\n", argv[0]);
		exit(1);
	}
	printf("checkcheck $d\n", bug_check++);

	for (int i = 0; i < BUF_SIZE; i++)
	{
		buff[i] = 0;
	}
	printf("checkcheck $d\n", bug_check++);

	fp = fopen(argv[3], "rb");
	//fp = fopen("receive.dat", "wb");
	sd = socket(PF_INET, SOCK_DGRAM, 0);
	printf("checkcheck $d\n", bug_check++);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	printf("checkcheck $d\n", bug_check++);

	//여기까진 ㅇㅋ

	while (1)
	{
		if (ack_recv == 1)
		{
			frame_send.sq_no = frame_id;
			frame_send.frame_kind = 1;
			frame_send.ack = 0;

			read_cnt = fread((void *)buf, 1, BUF_SIZE, fp);
			if (read_cnt < BUF_SIZE)
			{
				sendto(sd, buf, read_cnt, 0, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
				break;
			}
			adr_sz = sizeof(serv_adr);
			sendto(sd, 0, BUF_SIZE, 0, (struct sockaddr *)&serv_adr, adr_sz);
		}
		// int addr_size = sizeof(serv_adr);
		int f_recv_size = recvfrom(sd, &frame_recv, sizeof(frame_recv), 0, (struct sockaddr *)&serv_adr, &adr_sz);

		if (f_recv_size > 0 && frame_recv.sq_no == 0 && frame_recv.ack == frame_id + 1)
		{
			printf("[+]Ack Received\n");
			ack_recv = 1;
		}
		else
		{
			printf("[-]Ack Not Received\n");
			ack_recv = 0;
		}

		frame_id++;
	}

	socklen_t from_adr_sz = sizeof(from_adr);
	int str_len = recvfrom(sd, buff, BUF_SIZE, 0, (struct sockaddr *)&from_adr, &from_adr_sz);

	printf("Message from client: %s \n", buff);

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