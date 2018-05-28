/*
 * InputOutput.h
 *
 *  Created on: Jul 4, 2017
 *      Author: carlos
 */

#ifndef INPUTOUTPUT_H_
#define INPUTOUTPUT_H_

#include <map>
#include <string>
#include <vector>

/**
 * Classe usada para fazer entrada e saida de arquivos com o HD
 */
class InputOutput {
public:
	InputOutput();

	/**
	 * Método que carrega o arquivo de configuração (JSON) do classificador do disco
	 * @param filename, nome do arquivo JSON salvo no HD
	 * @return mapa chave valor, que corresponde as chaves e os campos do arquivo JSON passado por parâmetro
	 */
	static std::map<std::string, std::string> readJson(std::string filename);

	/**
	 * Método que carrega do disco o arquivo que contém os nomes das classe e seu indice correspondente
	 * @param fileName, come do arquivo txt salvo no HD
	 * @return mapa chave valor, que corresponde ao indice e o nome das classes
	 */
	static std::map<int, std::string> readIndexLabelFile(std::string fileName);

	/**
	 * Método que retorna um mapa contendo o nome das classes como chave e o indice dela como valor
	 * @param labelIndex, mapa contendo o indice da classe como chave e o nome dela como valor
	 * @return mapa contendo o nome das classes como chave e o indice dela como valor
	 */
	static std::map<std::string, int> convertIndexLabelFile(std::map<int, std::string> labelIndex);

	/**
	 * Método usado para realizar um split numa string
	 * @param str, string de entrada que será usada para computar os tokens
	 * @param tokens, resultado do split
	 * @param delimiters, delimitador que será usado no split
	 */
	static void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);

	static std::vector<std::string> read_ocr_result(std::string filename);
	static std::map<char, char> read_dict(std::string filename);
	static std::vector<std::string> getName(std::string pathDir);


	virtual ~InputOutput();
};

#endif /* INPUTOUTPUT_H_ */
