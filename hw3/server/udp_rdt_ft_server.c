#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/stat.h>

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
	int serv_sd, clnt_sd;
	FILE *fp;
	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	int frame_id = 0;
	Frame frame_recv;
	Frame frame_send;

	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	fp = fopen("receive.dat", "wb");
	serv_sd = socket(PF_INET, SOCK_DGRAM, 0); // 서버측 소켓 주소

	memset(&serv_adr, 0, sizeof(serv_adr)); //서버측 구조체 초기화

	//구조체의 요소에 값을 넣어주는 것.
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); //host to network
	serv_adr.sin_port = htons(atoi(argv[1]));	  //포트번호
	//여기까지

	//바인딩
	bind(serv_sd, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
	//구조체의 주소를 알려주고
	// listen(serv_sd, 5);
	//listen상태로 만드는 것. 소켓이 클라이언트의 리턴을 받을 수 있게 하는 것.
	//뒤의 5는 대기 메시지 queue의 갯수

	//여기 위에까지가 소켓 생성하고 바인딩하고 클라이언트에게 받을 수 있는 상태를 만든 것.
	clnt_adr_sz = sizeof(clnt_adr);
	// clnt_sd = accept(serv_sd, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
	//accept에서 클라이언트의 요청이 오는 것을 대기.
	//클라이언트가 요청했으니 요청한 클라이언트의 정보를 담은 clnt_sd가 만들어짐

	//여기까진 괜츈

	// while ((read_cnt = recvfrom(serv_sd, buf, BUF_SIZE, 0, (struct sockaddr *)&clnt_adr, &clnt_adr_sz)) != 0)
	// {
	// 	printf("check4 %s\n", buf);
	// 	fwrite((void *)buf, 1, read_cnt, fp);
	// }
	while (1)
	{
		// int f_recv_size = recvfrom(serv_sd, &)
		read_cnt = recvfrom(serv_sd, &frame_recv, sizeof(Frame), 0, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
		// printf("***out**** \nread_cnt : %d\n frame_recv.frame_kind : %d\n frame_recv.sq_no: %d\n frame_id : %d\n", read_cnt, frame_recv.frame_kind, frame_recv.sq_no, frame_id);
		if (read_cnt > 0 && frame_recv.frame_kind == 1)
		{
			fwrite(frame_recv.packet.data, 1, read_cnt, fp);
			// printf("%s", frame_recv.packet.data);
			frame_send.sq_no = 0;
			frame_send.frame_kind = 0;
			// printf("***in**** \nread_cnt : %d\n frame_recv.frame_kind : %d\n frame_recv.sq_no: %d\n frame_id : %d\n", read_cnt, frame_recv.frame_kind, frame_recv.sq_no, frame_id);
			frame_send.ack = frame_recv.sq_no + 1;
			puts("Received file data");
			sendto(serv_sd, "Thank you", 10, 0, (struct sockaddr *)&clnt_adr, clnt_adr_sz);
		}
		else
		{
			printf("Not Recived\n");
		}
		frame_id++;
	}
	fclose(fp);
	close(serv_sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
