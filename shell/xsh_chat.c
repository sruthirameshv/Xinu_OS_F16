
#include <xinu.h>

shellcmd xsh_chat(int nargs, char *args[]) {

    uint32 dstIp;
    uint16 dstPort;
    uint32 srcIp;
    uint16 srcPort=100;
    uint32 con;
    pid32 print_proc;
    char input[15];
    char send[15];
    char send_large[150];
    int i=0;
    uint32 lenOfData;
    int len_f=5;
    char str_f[6];
    char n_str_f[6];
    int n_len_f=0;
    future* f_chat = future_alloc(FUTURE_EXCLUSIVE);

    if (nargs != 4){
     printf("Usage: Command dst_ip dst_port messageToSend \n");
    }

    // Test fut
    // strncpy(str_f, "hello", 5);
    // printf("str_f: %s\n", str_f);
    // printf("len_f: %d\n", len_f);
    // future_set_str(f_chat, &len_f, str_f);

    // future_get_str(f_chat, &n_len_f, n_str_f);
    // printf("new str_f: %s\n", n_str_f);
    // printf("new_len_f: %d\n", n_len_f);


    //print_process = create(print_toConsole, 1024, 20, "print_toConsole", 1, chat);
    // resume(print_process);
    // printf("1IP before %d\n", dstIp);
    printf("1IP %d\n", dstIp);
    if (dot2ip(args[1], &dstIp) == SYSERR) {
      printf("Syserr in dot2ip\n");
    }
    dstPort = 8009;
    printf("2IP %d\n", dstIp);
    printf("dstPort %d\n", dstPort);
    con = udp_register (dstIp,dstPort,srcPort);
    if (con == SYSERR) {
      printf ("Could not register connection\n");
      return 1;
    }

    printf("UDP registered\n");
  while (i<1) {
    i++;
      
      bzero (input,15);
      bzero (send,15);

      strncpy(send, "hello xinu!", 11);
      strncpy(send_large, "hello xinu!hello xinu!hello xinu!hello xinu!hello xinu!hello xinu!hello xinu!hello xinu!hello xinu!hello xinu!hello xinu!", 121);
      printf("Copying: %s \n", send);
      // send = {'h','e','l','l','o',' ',' ',' ',' ',' ','x','i','n','u','\0'};

      if((lenOfData = udp_send (con, send_large, 121))  == SYSERR) {
         printf ("Error on sending data %d\n", lenOfData);
         return 1;
      }

      printf("Sent: %s and len: %d\n", send_large, lenOfData);
      if ((lenOfData = udp_recv (con, input, 15, 10000)) == SYSERR) {
          printf (" UDP recieve error\n");
          return 1;
      }
      if (lenOfData != TIMEOUT) {
          //future_set_str(f_talk, 15, input);
        printf (" Recived data %s\n",input);
      }

      
  }
  //kill (print_process);
  udp_release (con);
  return 0;
}
