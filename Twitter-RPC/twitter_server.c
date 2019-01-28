#include "twitter.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>	
// #include <mysql/mysql.h>
// #include <libpq-fe.h> 

// A tabela hash inteira estava aqui
// terminando aqui!


// usuario{
//     char user;
//     Lista follow;
//     Lista followers;
// }

// topico{
//     char topico_nome;
//     Lista struct posts;
// }

// posts{
//     char texto;
//     char timestamp;
//     char topico_nome;
// }

post_users *post_user_aux;

post_users *retrieve_topic_1_svc(post_tweet_topic *argp, struct svc_req *rqstp){
	static int result;
	return &result;
}

int *list_topic_1_svc(user_follow *argp, struct svc_req *rqstp){
	static int result;
	const char *conninfo;
	PGconn *conn = NULL;
	PGresult *res;

	conninfo = "dbname =rpc_banco host=localhost user=postgres password=nxfhep3d";
	conn = PQconnectdb(conninfo); 

	char query[500];
	sprintf(query,"SELECT * FROM topico_tabela");	
	res = PQexec(conn,query);
	printf("\n");
	if(PQresultStatus(res) == PGRES_TUPLES_OK){	// Se tiver item na tabela
		printf("\nTópicos cadastrados : \n\n");
		int i = 0;
		for(i = 0 ; i < PQntuples(res); i++){	// Percorre a tabela, pegando o nome dos usuários para printá-los na tela
			printf("\n%d. %s",i,PQgetvalue(res,i,0));
		}
	}
	printf("\n\n\n\n");
	return &result;
}

post_users *post_tweet_1_svc(post_tweet_topic *argp, struct svc_req *rqstp){
	static int  result;
	struct TopicItem* data;
	struct DataItem* user;
	char key;
	const char *conninfo;
	PGconn *conn = NULL;
	PGresult *res,*res_2;

	conninfo = "dbname =rpc_banco host=localhost user=postgres password=nxfhep3d";
	conn = PQconnectdb(conninfo); 
	
	if(PQstatus(conn) != CONNECTION_OK){
		printf("Falha na conexão. Erro: %s", PQerrorMessage(conn));
        PQfinish(conn);
	}
	else{
		char query[5000];

		sprintf(query,"SELECT * FROM usuario_tabela WHERE usuario='%s'",argp->usuario);	
		res = PQexec(conn,query);

		if(PQresultStatus(res) == PGRES_TUPLES_OK){	// Verifica se tem item na tabela

			if(PQntuples(res) <= 0){					// Já existe um usuário cadastrado com esse login!
				printf("\nUsuário que está tentando postar não está cadastrado no twitter! \n");
				result = 0;
			}else{									// Não existe usuário com esse login!
				/* Preparando struct para retornar :
					Essa struct será necessária para enviar o post para todos os usuários
					que seguem o @user que criou a postagem! */
					post_user_aux = malloc(sizeof(post_users));
					strcpy(post_user_aux->user, argp->usuario);
					strcpy(post_user_aux->post, argp->post);
				/*-------------------------------------------- */
				/* Atualiza banco de dados */
					sprintf(query,"UPDATE usuario_tabela SET post='%s' WHERE usuario = '%s' AND usuario_follower != ''",argp->post,argp->usuario);
					res_2 = PQexec(conn,query);
				/* ----------------------- */
				return post_user_aux;
			}
		}
	}
		return post_user_aux;
	
}

post_users *post_topic_1_svc(post_tweet_topic *argp, struct svc_req *rqstp){
	static int  result;
	struct TopicItem* data;
	struct DataItem* user;
	char key;

	const char *conninfo;
	PGconn *conn = NULL;
	PGresult *res;

	conninfo = "dbname =rpc_banco host=localhost user=postgres password=nxfhep3d";
	conn = PQconnectdb(conninfo); 
	
	if(PQstatus(conn) != CONNECTION_OK){
		printf("Falha na conexão. Erro: %s", PQerrorMessage(conn));
        PQfinish(conn);
	}
	else{
		char query[5000];


		sprintf(query,"SELECT * FROM topico_tabela WHERE topico_nome='%s'",argp->topico);	
		res = PQexec(conn,query);

		if(PQresultStatus(res) == PGRES_TUPLES_OK){	// Verifica se tem item na tabela

			if(PQntuples(res) > 0){					// Já existe um tópico cadastrado com esse nome!
				sprintf(query,"INSERT INTO topico_tabela (topico_nome,time_stamp,post,creater) VALUES('%s','%d','%s','%s')",argp->topico,(int) time (NULL),argp->post,argp->usuario);
				res = PQexec(conn,query);
				post_user_aux = malloc(sizeof(post_users));
				strcpy(post_user_aux->user, argp->usuario);
				strcpy(post_user_aux->post, argp->post);
				printf("\nTópico '%s' criado com sucesso!\n",argp->topico);	
				return post_user_aux;
				
			}else{									// Não existe usuário com esse login!
				return post_user_aux;
			}
		}else{
			printf("\nO tópico não está cadastrado na tabela!\n ");
		}
		return &result;


	}
}

int *create_topic_1_svc(post_tweet_topic *argp, struct svc_req *rqstp){
	static int  result;
	struct TopicItem* data;
	struct DataItem* user;
	char key;

	const char *conninfo;
	PGconn *conn = NULL;
	PGresult *res;

	conninfo = "dbname =rpc_banco host=localhost user=postgres password=nxfhep3d";
	conn = PQconnectdb(conninfo); 
	
	if(PQstatus(conn) != CONNECTION_OK){
		printf("Falha na conexão. Erro: %s", PQerrorMessage(conn));
        PQfinish(conn);
	}
	else{
		char query[5000];

		sprintf(query,"SELECT * FROM topico_tabela WHERE topico_nome='%s'",argp->topico);	
		res = PQexec(conn,query);

		if(PQresultStatus(res) == PGRES_TUPLES_OK){	// Verifica se tem item na tabela

			if(PQntuples(res) > 0){					// Já existe um tópico cadastrado com esse nome!
				printf("\nJá existe um tópico cadastrado com este nome, tente novamente!\n");
				result = 0;
			}else{									// Não existe usuário com esse login!
				/* Adiciona no banco de dados ------------------------- */
					sprintf(query,"INSERT INTO topico_tabela (topico_nome,time_stamp,post,creater) VALUES('%s','','','%s')",argp->topico,argp->usuario);
					res = PQexec(conn,query);
				/* ---------------------------------------------------- */
				printf("\nTópico '%s' criado com sucesso!\n",argp->topico);	
				result = 1;
			}
		}
		return &result;
	}
	return &result;
}

/* Mostra todos os usuários cadastrados no Twitter
   em ordem alfabética
*/
int *list_user_1_svc(user_follow *argp, struct svc_req *rqstp){
	static int result;
	const char *conninfo;
	PGconn *conn = NULL;
	PGresult *res;

	conninfo = "dbname =rpc_banco host=localhost user=postgres password=nxfhep3d";
	conn = PQconnectdb(conninfo); 

	char query[500];
	sprintf(query,"SELECT * FROM usuario_tabela");	
	res = PQexec(conn,query);
	printf("\n");
	if(PQresultStatus(res) == PGRES_TUPLES_OK){	// Se tiver item na tabela
		printf("\nUsuários cadastrados : \n\n");
		int i = 0;
		for(i = 0 ; i < PQntuples(res); i++){	// Percorre a tabela, pegando o nome dos usuários para printá-los na tela
			printf("\n%d. %s",i,PQgetvalue(res,i,0));
		}
	}
	printf("\n\n\n\n");
	return &result;
}
/* Recebe a struct 'user_follow' como parâmetro
   Lá contém apenas o nome @user e @user_que_vai_parar_de_seguir
   Buscaremos o nome destes usuários na hash que contém todos os usuários cadastrados no twitter
   Após isso, iremos verificar se o @user segue @user_que_vai_parar_de_seguir
   Caso a verificação retorne um valor positivo, o usuário irá parar de seguir o outro
*/
int *unfollow_user_1_svc(user_follow *argp, struct svc_req *rqstp){
	static int  result;
	int key,key_follow;
	struct DataItem* user = NULL;
	struct DataItem* user_follow = NULL;

	const char *conninfo;
	PGconn *conn = NULL;
	PGresult *res,*res_2;

	conninfo = "dbname =rpc_banco host=localhost user=postgres password=nxfhep3d";
	conn = PQconnectdb(conninfo); 

	if(PQstatus(conn) != CONNECTION_OK){
		printf("Falha na conexão. Erro: %s", PQerrorMessage(conn));
        PQfinish(conn);
	}else{

		// Conectou com o banco local! Vamos fazer a verificação da existencia dos usuários!

		char query[5000];
		sprintf(query,"SELECT * FROM usuario_tabela WHERE usuario='%s'",argp->user_seguir);	
		res = PQexec(conn,query);

		if(PQresultStatus(res) == PGRES_TUPLES_OK){		// Verifica se tem item na tabela

			if(PQntuples(res) <= 0){					// Já existe um usuário cadastrado com esse login!
				printf("\nUsuário '%s' não existe na tabela!\n",argp->user_seguir);
				result = 0;
			}else{										// Não existe usuário com esse login!

				/* Tudo pronto para realizar a operação, vamos verificar se eles já se seguem? -------*/
					sprintf(query,"SELECT * FROM usuario_tabela WHERE usuario='%s' AND usuario_follow = '%s'",argp->user,argp->user_seguir);	
					res = PQexec(conn,query);
					sprintf(query,"SELECT * FROM usuario_tabela WHERE usuario='%s' AND usuario_follower = '%s'",argp->user_seguir,argp->user);	
					res_2 = PQexec(conn,query);

					if(PQntuples(res) <= 0 && PQntuples(res_2) <= 0){	// Ops, essa operação já foi realizada antes!
						result = 2;
						return &result;
					}else{
						/* Adiciona no banco de dados ------------------------- */
							sprintf(query,"DELETE FROM usuario_tabela WHERE usuario='%s' AND usuario_follow ='%s'",argp->user,argp->user_seguir);
							res = PQexec(conn,query);
							sprintf(query,"DELETE FROM usuario_tabela WHERE usuario='%s' AND usuario_follower ='%s'",argp->user_seguir,argp->user);
							res = PQexec(conn,query);
						/* ---------------------------------------------------- */
						printf("\nUsuário '%s' deixou de seguir '%s'!\n",argp->user,argp->user_seguir);	
						result = 1;
					}
				/* ---------------------------------------------------------------------------------- */
			}
		}
	}

	return &result;
}

/* Recebe a struct 'user_follow' como parâmetro
   Lá contém apenas o nome @user e @user_a_seguir
   Buscaremos o nome destes usuários na hash que contém todos os usuários cadastrados no twitter
   E então faremos a iserção de @user_a_seguir na lista 'follow' de @user
*/
int *follow_user_1_svc(user_follow *argp, struct svc_req *rqstp){
	static int  result;
	int key,key_follow;
	struct DataItem* user = NULL;
	struct DataItem* user_follow = NULL;

	const char *conninfo;
	PGconn *conn = NULL;
	PGresult *res,*res_2;

	conninfo = "dbname =rpc_banco host=localhost user=postgres password=nxfhep3d";
	conn = PQconnectdb(conninfo); 

	if(PQstatus(conn) != CONNECTION_OK){
		printf("Falha na conexão. Erro: %s", PQerrorMessage(conn));
        PQfinish(conn);
	}else{
		
		// Conectou com o banco local! Vamos fazer a verificação da existencia dos usuários!

		char query[5000];

		sprintf(query,"SELECT * FROM usuario_tabela WHERE usuario='%s'",argp->user);	
		res = PQexec(conn,query);

		if(PQresultStatus(res) == PGRES_TUPLES_OK){	// Verifica se o usuário que quer seguir existe na tabela

			if(PQntuples(res) <= 0){				// Já existe um usuário cadastrado com esse login!
				printf("\nUsuário '%s' não existe na tabela!\n",argp->user);
				result = 0;
			}else{									// Usuário existe, vamos ver se o usuário a ser seguido existe na tabela

				sprintf(query,"SELECT * FROM usuario_tabela WHERE usuario='%s'",argp->user_seguir);	
				res = PQexec(conn,query);

				if(PQresultStatus(res) == PGRES_TUPLES_OK){		// Verifica se tem item na tabela

					if(PQntuples(res) <= 0){					// Já existe um usuário cadastrado com esse login!
						printf("\nUsuário '%s' não existe na tabela!\n",argp->user_seguir);
						result = 0;
					}else{										// Não existe usuário com esse login!

						/* Tudo pronto para realizar a operação, vamos verificar se eles já se seguem? -------*/
							sprintf(query,"SELECT * FROM usuario_tabela WHERE usuario='%s' AND usuario_follow = '%s'",argp->user,argp->user_seguir);	
							res = PQexec(conn,query);
							sprintf(query,"SELECT * FROM usuario_tabela WHERE usuario='%s' AND usuario_follower = '%s'",argp->user_seguir,argp->user);	
							res_2 = PQexec(conn,query);

							if(PQntuples(res) > 0 && PQntuples(res_2) > 0){	// Ops, essa operação já foi realizada antes!
								result = 2;
								return &result;
							}else{
								/* Adiciona no banco de dados ------------------------- */
									sprintf(query,"INSERT INTO usuario_tabela (usuario,usuario_follow,usuario_follower) VALUES('%s','','%s')",argp->user_seguir,argp->user);
									res = PQexec(conn,query);
									sprintf(query,"INSERT INTO usuario_tabela (usuario,usuario_follow,usuario_follower) VALUES('%s','%s','')",argp->user,argp->user_seguir);
									res = PQexec(conn,query);
								/* ---------------------------------------------------- */
								printf("\nUsuário '%s' agora segue '%s'!\n",argp->user,argp->user_seguir);	
								result = 1;
							}
						/* ---------------------------------------------------------------------------------- */
					}
				}
			}
		}
	}
		return &result;
}

/* Cria um usuário - 
	O usuário criado será adicionado em uma tabela hash e também em um banco de dados
*/
int *create_user_1_svc(tweet *argp, struct svc_req *rqstp){
	static int  result;
	struct DataItem* data;

	const char *conninfo;
	PGconn *conn = NULL;
	PGresult *res;

	conninfo = "dbname =rpc_banco host=localhost user=postgres password=nxfhep3d";
	conn = PQconnectdb(conninfo); 
	
	if(PQstatus(conn) != CONNECTION_OK){
		printf("Falha na conexão. Erro: %s", PQerrorMessage(conn));
        PQfinish(conn);
	}
	else{
		char query[5000];

		sprintf(query,"SELECT * FROM usuario_tabela WHERE usuario='%s'",argp->usuario);	
		res = PQexec(conn,query);

		if(PQresultStatus(res) == PGRES_TUPLES_OK){	// Verifica se tem item na tabela

			if(PQntuples(res) > 0){					// Já existe um usuário cadastrado com esse login!
				printf("\nJá existe um usuário cadastrado com este login, tente novamente!\n");
				result = 0;
			}else{									// Não existe usuário com esse login!
				/* Adiciona no banco de dados ------------------------- */
					sprintf(query,"INSERT INTO usuario_tabela (usuario,usuario_follow,usuario_follower) VALUES('%s','','')",argp->usuario);
					res = PQexec(conn,query);
				/* ---------------------------------------------------- */
				printf("\nUsuário '%s' criado com sucesso!\n",argp->usuario);	
				result = 1;
			}
		}
		sprintf(query,"INSERT INTO topico_tabela (topico_nome,time_stamp,post,creater) VALUES('','','','')");
		res = PQexec(conn,query);
		return &result;
	}
}




