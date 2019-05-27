#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include "header.h"
static int final = 1;
static int configUsuario (key_t key, int flag) {
   int res;
   res = msgget (key, flag);
   if (res == -1) {
      printf ("Erro do usuario ao configurar a pilha de mensagens...\n");
      return -1;
   }
   return res;
}
 // Desligamento do usuario
static void desligaUsuario (int signr) {
      final = 0;
}
int main (void) {
   int idServidor, idUsuario;
   int res;
   clienteServidor c2s;
   servidorCliente s2c;
   // Configura o manipulador de sinal CTRL + C 
   signal (SIGINT, desligaUsuario);
   // Fila de mensagens para o servidor 
   idServidor = configUsuario (KEY, 0);
   if (idServidor < 0)
      return EXIT_FAILURE;
   // Fila de mensagens para o usuário 
   idUsuario = configUsuario (IPC_PRIVATE, PERM | IPC_CREAT);
   if (idUsuario < 0)
      return EXIT_FAILURE;
   // Envia uma mensagem para o servidor 
   c2s.prioridade = 2;
   sprintf (c2s.mensagem, "%d:Login\n", idUsuario);
   res = msgsnd (idServidor, &c2s, MSG_LEN, 0);
   if (res == -1) {
      printf ("Nao foi possivel enviar a mensagem ...\n");
      return EXIT_FAILURE;
   }
   // Confirmação do servidor ou descarte 
   // Recebimento de outros usuários      
   res = msgrcv (idUsuario, &s2c, MSG_LEN, 0, 0);
   if (res == -1) {
      printf ("Erro ao receber a mensagem ...\n");
      return EXIT_FAILURE;
   }
   //Ler, editar ou descartar
   printf ("%ld: %s\n", s2c.prioridade, s2c.mensagem);
   while (final) {

    /* Aqui está o código importante para comunicação 
     são escritos entre processos. 
     Filas de mensagens emitidas como um novo usuário 
     Loop está aguardando o sinal SIGINT == CTRL + C 
     Confirma ou descarta a mensagem */

      res= msgrcv (idUsuario, &s2c, MSG_LEN, 0, IPC_NOWAIT);
      if (res != -1) {
         printf ("(%s) Usuário na fila: %ld\n", s2c.mensagem, s2c.prioridade);
         }
       usleep( 1000 );
   }
   // Ctrl + C para disparar o sinal ... 
   c2s.prioridade = 1;
   sprintf (c2s.mensagem, "%d", idUsuario);
   res = msgsnd (idServidor, &c2s, MSG_LEN, 0);
   if (res == -1) {
      printf ("Nao foi possivel enviar a mensagem ...\n");
      return EXIT_FAILURE;
   }
   // Remove a fila de mensagens 
   msgctl (idUsuario, IPC_RMID, NULL);
   return EXIT_SUCCESS;
}