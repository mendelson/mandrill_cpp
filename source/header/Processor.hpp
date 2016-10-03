#pragma once

#include "Observer.hpp"
#include <memory>
#include <opencv2/opencv.hpp>

/*! \brief Classe abstrata básica para uso em entidades que realizam quaisquer
 * tipos de operações sobre frames.
 *
 * Esta classe abstrata apresenta os comportamentos básicos necessários para a
 * implementação de entidades que realizam operações sobre frames, seja para
 * salvar um vídeo, alterar resolução, alterar espaço de cores, etc. É classe
 * filha da classe Observer.
 *
 */
class Processor : public Observer
{
public:
	/*! \brief Método invocado para que o objeto seja notificado sobre a chegada
	 * de um novo frame da câmera.
	 *
	 * Por se tratar de um método puramente virtual, é obrigatório que cada uma
	 * das classes filhas implemente seu próprio comportamento.
	 *
	 */
	virtual void Update() = 0;

protected:
	/*! \brief Este método requisita o frame atual ao publisher.
	 *
	 */
	void getCurrentFrame();

	/*! \brief Referência para o frame atualmente em uso.
	 *
	 */
	std::shared_ptr<cv::Mat> _frame;

	/*! \brief Nome do codec em uso.
	 *
	 */
	const std::string _codecName;

	/*! \ brief Índice do frame atual.
	 *
	 */
	unsigned int _currentFrameIndex;
};

