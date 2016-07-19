#pragma once

#include <iostream>
#include <mutex>
#include <string>

class FramesManager;

/*! \brief Classe abstrata básica para uso com o padrão de projeto Observer.
 *
 * Esta classe abstrata apresenta os comportamentos básicos necessários para a implementação de um subscriber/observer e, portanto, deve ser herdada por classes que desempenham o papel de observers.
 *
 */
class Observer
{
public:
	/*! \brief Construtor da classe
	 *
	 * Inicializa _subject e _currentFrameIndex com valores nulos.
	 *
	 */
	Observer();

	/*! \brief Destrutor vazio.
	 *
	 * Caso seja necessário realizar alguma operação na destruição do objeto, este método pode ser sobrescrito pela classe filha.
	 *
	 */
	virtual ~Observer() {}
	
	/*! \brief Método invocado para que o objeto seja notificado sobre a ocorrência de um evento que lhe é de interesse.
	 *
	 * Por se tratar de um método puramente virtual, é obrigatório que cada uma das classes filhas implemente seu próprio comportamento.
	 *
	 */
	virtual void Update() = 0;

	/*! \brief Define o publisher.
	 *
	 * \param subject Referência para o objeto publisher, ou seja, objeto que irá notificar este objeto aqui sobre a ocorrência de eventos de interesse.
	 * \param id Número de identificação do objeto, definido pela política interna do objeto publisher.
	 */
	virtual void setSubject(FramesManager *subject, unsigned int id);

protected:
	/*! \ brief Referência para o objeto publisher.
	 *
	 */
	FramesManager *_subject;

	/*! \ brief Índice do frame atual.
	 *
	 */
	unsigned int _currentFrameIndex;

	/*! \ brief Mutex para uso interno.
	 *
	 */
	std::mutex _mutex;

	/*! \ brief Número de identificação do objeto.
	 *
	 */
	unsigned int _id;
};

