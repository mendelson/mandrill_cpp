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
	const unsigned int FPS;

	std::shared_ptr<cv::Mat> getFrame(unsigned int index);
	cv::Mat getLatestFrame();
	unsigned int getLatestFrameIndex();
	unsigned int getFrameAvailability(unsigned int frame);
	void setFrameAsBusy(unsigned int frameIndex);
	void setFrameAsFree(unsigned int frameIndex, unsigned int moduleIndex);
	void run();
	void setStreamSource(std::string url, std::string model);

	/*! \brief Retorna a instância global do FramesManager
	 *
	 * Necessário para o padrão Singleton.
	 *
	 */
	static FramesManager *getManager();

	/*! \brief Desabilita o construtor por cópia.
	 *
	 * Necessário para o padrão Singleton
	 *
	 */
	FramesManager(FramesManager const &) = delete;

	/*! \brief Desabilita o operador de cópia.
	 *
	 * Necessário para o padrão Singleton.
	 *
	 */
	void operator=(FramesManager const &) = delete; 

	// Subject-observer pattern
	void attach(Observer *);
	void detach(Observer *);

	double getFramesWidth();
	double getFramesHeight();
	double getCameraFPS();
	const std::string getModel();

	bool lostCamera() { return _lostCamera; }

private:
	class BufferManager
	{
	public:
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
	std::string _model;
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

