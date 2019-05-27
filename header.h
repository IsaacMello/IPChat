/* msq_header.h */
#include <stdlib.h>
#include <signal.h>
/* número aleatório */
#define KEY 1234L
/* Tamanho máximo da mensagem*/
#define MSG_LEN 1024
/* restrição de acesso */
#define PERM 0666
#undef signal
/* Structs de definição para enviar e receber mensagens*/
typedef struct {
   long prioridade;
   char mensagem[MSG_LEN];
} clienteServidor;

typedef struct {
   long prioridade;
   char mensagem[MSG_LEN];
} servidorCliente;

struct geradorId {
   int id;
   struct geradorId *prox;
 };
   
struct geradorId *id_first = NULL;
   
static void addAdmin( int i ) {
   struct geradorId *ptr;
   if( id_first == NULL) {
      id_first = malloc(sizeof( struct geradorId ));
       if( id_first == NULL )
         exit(EXIT_FAILURE);
      id_first->id = i;
       id_first->prox = NULL;
   }
   else {
      ptr = id_first;
      while(ptr->prox != NULL)
         ptr=ptr->prox;
      ptr->prox = malloc(sizeof( struct geradorId ));
      ptr = ptr->prox;
      ptr->id = i;
      ptr->prox = NULL;
   }
}   
static void removeAdmin( int i ) {
   struct geradorId *ptr_tmp;
   struct geradorId *ptr;
   if(id_first == NULL)
      return;
   if( id_first->id == i ) {
      ptr = id_first->prox;
      free(id_first);
      id_first = ptr;
      printf("O usuario %d foi desconctado\n",i);
      return;
   }
   ptr = id_first;
   while(ptr->prox != NULL) {
      ptr_tmp = ptr->prox;
      if( ptr_tmp->id == i ) {
         ptr->prox = ptr_tmp->prox;
         free(ptr_tmp);
         printf("O usuario %d foi desconctado\n",i);
         break;
      }
      ptr=ptr_tmp;
   }
}
typedef void (sinalDeFuncao) (int);
sinalDeFuncao * signal (int sig_nr, sinalDeFuncao manipulaSinal) {
   struct sigaction novoSinal, sinalAntigo;
   novoSinal.sa_handler = manipulaSinal;
   sigemptyset (&novoSinal.sa_mask);
   novoSinal.sa_flags = SA_RESTART;
   if (sigaction (sig_nr, &novoSinal, &sinalAntigo) < 0)
      return SIG_ERR;
   return sinalAntigo.sa_handler;
}