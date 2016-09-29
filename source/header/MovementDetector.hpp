#pragma once

#include <iostream>
#include <string>
#include <gst/gst.h>

class MovementDetector
{
public:
	/*! \brief Construtor da classe.
	 *
	 * Inicializa o ponteiro de erros como nulo e o contador de movimentos é zerado.
	 *
	 */
	MovementDetector();

	/*! \brief Analisa um arquivo e indica se foi detectado algum movimento nele.
	 *
	 * \param inputFile Nome do arquivo de entrada a ser analisado.
	 *
	 * \retval true Indica que foi detectado movimento.
	 * \retavl false Indica que não foi detectado movimento.
	 *
	 */
	bool detectMovement(char *inputFile);

private:
	/*! \brief Ponteiro estático para controle de loop do GStreamer.
	 *
	 */
	static GMainLoop *_loop;

	/*! \brief Contador de mensagens de detecção de movimento enviadas no bus do pipeline.
	 *
	 */
	static int _movementCounter;

	/*! \brief Referência para o pipeline do GStreamer a ser executado.
	 *
	 */
	GstElement *_pipeline;

	/*! \brief Referência para o elemento do GStreamer que define o arquivo de entrada a ser analisado.
	 *
	 */
	GstElement *_src;

	/*! \brief Referência para o elemento do GStreamer que detecta movimentos no arquivo de entrada.
	 *
	 */
	GstElement *_motioncells;

	/*! \brief Referência para o elemento do GStreamer que armazena o tipo de erro ocorrido (se houver).
	 *
	 */
	GError *_error;

	/*! \brief Referência para o bus do pipeline do GStreamer.
	 *
	 */
	GstBus *_bus;

	/*! \brief Id do watch para o bus.
	 *
	 */
	guint _busWatchId;

	/*! \brief Arquivo de entrada a ser analisado.
	 *
	 */
	char *_inputFile;

	/*! \brief Callback para a detecção de movimento.
	 *
	 * Ao alcançar o fim do arquivo de entrada ou ao encontrar algum erro, notifica o GStreamer que a execução do loop deve ser encerrada. Tais tarefas são realizadas através do monitoramento do bus do pipeline e da checagem dos tipos de mensagens recebidas. Seus parâmetros de entrada são definidos por padrão pelo GStreamer. Não altere a assinatura deste método.
	 *
	 */
	static gboolean countMovementCallback (GstBus *bus, GstMessage *message, gpointer data);

	/*! \brief Inicializa o pipeline.
	 *
	 */
	void initPipeline();


	/*! \brief Configura a referência do pipeline para o arquivo de entrada.
	 *
	 */
	void setPipelineInputFile();

	/*! \brief Executa o pipeline até que o fim do arquivo de entrada seja alcançado.
	 *
	 */
	void startPipeline();

	/*! \brief Finaliza a execução do pipeline e prepara o ambiente para que o próximo arquivo de entrada possa ser processado.
	 *
	 */
	void stopPipeline();
};