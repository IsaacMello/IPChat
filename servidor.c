#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include "header.h"
static int setup_server( key_t key, int flag ) {
   int res;
   res = msgget( key, flag );
   if( res < 0 ) {
      printf("Erro do servidor ao configurar a pilha de mensagens...\n");
      return -1;
   }
   return res;
}
static void acrescenta( const char *str, int p ) {
   int res;
   servidorCliente s2c;   
   struct geradorId *ptr = id_first;
      
   s2c.prioridade = p;
   sprintf(s2c.mensagem,"%s",str);
   
   while(ptr != NULL) {
      res = msgsnd(ptr->id, &s2c, MSG_LEN, 0);
      if( res < 0 )
         printf("Não foi possivel entregar a mensagem ao usuario: %d ...\n",ptr->id);
      else
         printf("Mensagem entregue ao usuario: %d\n", ptr->id);
      ptr=ptr->prox;
   }
}
      
int main(void) {
   int idServidor, res;
   clienteServidor c2s;
   char puffer[MSG_LEN];
   int idUsuario;
   
   idServidor = setup_server( KEY, PERM  | IPC_CREAT );
   if( idServidor < 0 )
      return EXIT_FAILURE;
   printf("Servidor em execucao ...\n");
   while(1) {
      // recebendo mensagem 
      res = msgrcv( idServidor, &c2s, MSG_LEN, 0, 0 );
      if( res < 0 ) {
         printf("Erro ao receber a mensagem!\n");
         return EXIT_FAILURE;
      }
      //administrando mensagens

      if( c2s.prioridade == 1 ) {
         
         //Remover filas de mensagens

         sscanf(c2s.mensagem,"%d",&idUsuario);
         removeAdmin( idUsuario );   
      }
      else {
         sscanf(c2s.mensagem,"%d:%s",&idUsuario, puffer);
         //Adcionando o Id do usuário à lista
         addAdmin( idUsuario );
         //Envia uma mensagem a todos os usuários
         acrescenta( puffer, idUsuario );
      }
   }
   printf(" --- Servidor Desligado ---\n");
   return EXIT_SUCCESS;
}