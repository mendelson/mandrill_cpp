#pragma once

#include "Camera.hpp"
#include "Observer.hpp"
#include "Printer.hpp"
#include "ThreadPool.hpp"
#include <bitset>
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <thread>
#include <unordered_map>
#include <vector>

const unsigned int MAXFRAMES = 10;
const unsigned int TIMESPAN  = 100;  // miliseconds

typedef std::list<Observer *> observersList;
typedef std::unordered_map<unsigned int, std::shared_ptr<cv::Mat>> FramesSet;
typedef std::unordered_map<unsigned int, std::shared_ptr<std::bitset<16>>>
	FramesMetadata;
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

/*! \brief Classe responsável pelo gerenciamento dos frames.
 *
 * Esta classe realiza a comunicação diretas com as câmeras, realiza o
 * gerenciamento dos frames recebidos e segue os
 * padrões Singleton e Observer. No padrão Observer, esta classe realiza o papel
 * de publisher, ou seja, à cada vez que um frame é recebido, todos os objetos
 * inscritos (subscribers/observers) são notificados.
 *
 */
class FramesManager
{
public:
	/*! \brief Restringe o acesso ao método getFrameAvailability somente a
	 * objetos da
	 * classe BufferManager.
	 *
	 * É desejado que apenas a classe BufferManager tenha acesso ao estado dos
	 * frames, uma vez que esta é responsável por decidir o momento em que cada
	 * frame deve ser removido da memória.
	 *
	 */
	friend class BufferManager;

	/*! \brief Armazena a taxa de frames por segundo em que o vídeo de saída
	 * deve ser escrito em disco.
	 *
	 */
	const unsigned int FPS;

	/*! \brief Retorna a referência para o frame de índice desejado.
	 *
	 * \param frameIndex Índice do frame desejado.
	 * \return Referência para o frame desejado, por meio de um shared pointer.
	 *
	 */
	std::shared_ptr<cv::Mat> getFrame(unsigned int frameIndex);

	/*! \brief Retorna o frame mais recente recebido da câmera.
	 *
	 * \return O frame mais recente.
	 *
	 */
	cv::Mat getLatestFrame();

	/*! \brief Retorna o índice do frame mais recente recebido da câmera.
	 *
	 * \return O índice do frame mais recente.
	 *
	 */
	unsigned int getLatestFrameIndex();


	/*! \brief Define que um frame está em uso.
	 *
	 * \param frameIndex Índice do frame desejado.
	 *
	 */
	void setFrameAsBusy(unsigned int frameIndex);

	/*! \brief Define que um frame não está mais em uso por um elemento
	 * específico de _observers.
	 *
	 * \param frameIndex Índice do frame desejado.
	 * \param moduleIndex Posição do elemento em _observers que está liberando o
	 * frame.
	 *
	 */
	void setFrameAsFree(unsigned int frameIndex, unsigned int moduleIndex);

	/*! \brief Loop principal da classe, responsável por se comunicar
	 * diretamente com a câmera e armazenar os frames em memória.
	 *
	 */
	void run();

	/*! \brief Instancia a câmera de acordo com uma URL e UUID.
	 *
	 * \param url URL da câmera.
	 * \param uuid UUID da câmera.
	 *
	 */
	void setStreamSource(std::string url, std::string uuid);

	/*! \brief Retorna a instância global do FramesManager.
	 *
	 * Necessário para o padrão Singleton.
	 *
	 */
	static FramesManager *getManager();

	/*! \brief Desabilita o construtor por cópia.
	 *
	 * Necessário para o padrão Singleton.
	 *
	 */
	FramesManager(FramesManager const &) = delete;

	/*! \brief Desabilita o operador de cópia.
	 *
	 * Necessário para o padrão Singleton.
	 *
	 */
	void operator=(FramesManager const &) = delete;

	/*! \brief Adiciona um novo elemento à lista _observers.
	 *
	 * \param newObserver Elemento a ser adicionado.
	 *
	 */
	void attach(Observer *newObserver);

	/*! \brief Remove um elemento da lista _observers.
	 *
	 * \param observer Elemento a ser removido.
	 *
	 */
	void detach(Observer *observer);

	/*! \brief Retorna a largura dos frames da câmera.
	 *
	 * \return O valor, em pixels, da largura dos frames da câmera.
	 *
	 */
	double getFramesWidth();

	/*! \brief Retorna a altura dos frames da câmera.
	 *
	 * \return O valor, em pixels, da altura dos frames da câmera.
	 *
	 */
	double getFramesHeight();

	/*! \brief Retorna a taxa de frames por segundo com que a câmera envia
	 * frames.
	 *
	 * \return A taxa de frames (FPS) de gravação da câmera.
	 *
	 */
	double getCameraFPS();

	/*! \brief Retorna o UUID da câmera.
	 *
	 * \return Retorna o UUID da câmera.
	 *
	 */
	const std::string getUUID();

	/*! \brief Indica se a conexão com a câmera foi perdida.
	 *
	 * \retval true Indica que a conexão com a câmera foi perdida.
	 * \retval false Indica que a conexão com a câmera não foi perdida.
	 *
	 */
	bool lostCamera() { return _lostCamera; }

protected:
	/*! \brief Checa se um determinado frame está em uso ou não.
	 *
	 * \param frameIndex Índice do frame desejado
	 * \retval 0 Indica que o frame não está mais em uso por nenhum elemento de
	 * _observers.
	 * \retval 1 Indica que o frame está em uso por pelo menos um elemento de
	 * _observers.
	 *
	 */
	unsigned int getFrameAvailability(unsigned int frameIndex);

private:
	/*! \brief Classe responsável por decidir por quanto tempo um frame deve
	 * permanecer em memória.
	 *
	 * Esta classe realiza a comunicação diretas com as câmeras, realiza o
	 * gerenciamento dos frames recebidos e segue os
	 * padrões Singleton e Observer. No padrão Observer, esta classe realiza o
	 * papel
	 * de publisher, ou seja, à cada vez que um frame é recebido, todos os
	 * objetos
	 * inscritos (subscribers/observers) são notificados.
	 *
	 */
	class BufferManager
	{
	public:
		/*! \brief Construtor da classe
		 *
		 * Salva a referência a instância global do FramesManager.
		 *
		 */
		BufferManager();
		void run();
		void stop();
		// void stopWhenEmpty();

	private:
		FramesManager *_framesManager;
		BufferManager(BufferManager const &) = delete;  // Desabling copy
		// constructor
		void operator=(BufferManager const &) = delete;  // Desabling copy
		// operator

		bool _mustStop;
		// bool _mustStopWhenEmpty;
		std::mutex _stopMutex;
		bool _lostCamera;
	};

	void addFrame(cv::Mat frame);
	void Notify();
	unsigned int getNewId();

	FramesSet _framesSet;
	FramesMetadata _busyFrames;
	std::string _url;
	std::string _uuid;
	Camera *_camera;
	unsigned int _latestFrameIndex;
	observersList _observers;
	std::mutex _mutex;
	ThreadPool *_threadPool;
	static BufferManager *_bufferManager;
	bool _lostCamera;
	fsec timeDiff;
	ms miliDiff;

	/*! \brief Construtor privado.
	 *
	 * Necessário para o padrão Singleton, pois torna inviável que outros entes
	 * instanciem diretamente um FramesManager.
	 *
	 */
	FramesManager();

	/*! \brief Referência para a instância global do FramesManager.
	 *
	 * Necessário para o padrão Singleton.
	 *
	 */
	static FramesManager *_instance;

	/*! \brief Mutex para controlar acesso a instância global do
	 * FramesManager.
	 *
	 * Necessário para o padrão Singleton.
	 *
	 */
	static std::mutex _instanceMutex;

	// Allowing parallelism
	// static void
	// updateHelper(std::list<Observer*>::iterator
	// it);

	void saveFrame(cv::Mat frame, cv::VideoWriter outputStream);

	static void
	bufferManagerRunHelper(FramesManager::BufferManager *bufferManager);
};

