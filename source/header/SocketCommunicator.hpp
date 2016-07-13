#pragma once

#include <mutex>
#include <string>

const char HOSTNAME[14] = "10.190.60.139";
// const int BUFFERSIZE	= 512;

/*! \brief Classe responsável pela comunicação com o Mid.
 *
 * Esta classe realiza a comunicação com o Mid através de sockets e segue o
 * padrão Singleton.
 *
 */
class SocketCommunicator
{
	/*! \brief Restringe o acesso ao método setupSocket somente a objetos da
	 * classe Core.
	 *
	 * É desejado que apenas a classe Core tenha acesso ao ponto de definição do
	 * número da porta e da inicialização do socket.
	 *
	 */
	friend class Core;

public:
	/*! \brief Retorna a instância global do SocketCommunicator
	 *
	 * Necessário para o padrão Singleton.
	 *
	 */
	static SocketCommunicator *getSocket();

	/*! \brief Desabilita o construtor por cópia.
	 *
	 * Necessário para o padrão Singleton
	 *
	 */
	SocketCommunicator(SocketCommunicator const &) = delete;

	/*! \brief Desabilita o operador de cópia.
	 *
	 * Necessário para o padrão Singleton.
	 *
	 */
	void operator=(SocketCommunicator const &) = delete;

	/*! \brief Notifica o Mid sobre a existência de uma nova filmagem.
	 *
	 */
	void notifyNewFootage();

protected:
	/*! \brief Inicializa o socket.
	 *
	 * De acordo com o número da porta especificada, este método inicializa o
	 * socket.
	 *
	 * \param portNumber Número da porta desejada.
	 *
	 */
	void setupSocket(int portNumber);

private:
	/*! \brief Construtor privado.
	 *
	 * Necessário para o padrão Singleton, pois torna inviável que outros entes
	 * instanciem diretamente um SocketCommunicator.
	 *
	 */
	SocketCommunicator(){};

	/*! \brief Destrutor da classe.
	 *
	 * Fecha o socket.
	 *
	 */
	~SocketCommunicator();

	/*! \brief Referência para a instância global do SocketCommunicator.
	 *
	 * Necessário para o padrão Singleton.
	 *
	 */
	static SocketCommunicator *_instance;

	/*! \brief Mutex para controlar acesso a instância global do
	 * SocketCommunicator.
	 *
	 * Necessário para o padrão Singleton.
	 *
	 */
	static std::mutex _instanceMutex;

	/*! \brief Descritor de arquivo para o socket em uso.
	 *
	 */
	int _socketFileDescriptor;

	/*! \brief Contém a notificação a ser enviada ao Mid.
	 *
	 */
	std::string _notification;
};

