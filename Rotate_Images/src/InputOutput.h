#ifndef INPUTOUTPUT_H_
#define INPUTOUTPUT_H_

#include <map>
#include <string>
#include <vector>

/**
 * Class used to input and output files with Hard Driver
 */

class InputOutput {

	public:
		InputOutput();

		/**
		 * Method that loads the disk classifier configuration file (JSON)
		 * @param filename, JSON file name saved in HD
		 * @return map key value, which matches the keys and fields of the JSON file passed by parameter
		 */
		static std::map<std::string, std::string> readJson(std::string filename);

		/**
		 * Method that loads the file containing the class names from the disk and its corresponding index
		 * @param fileName, come from txt file saved in HD
		 * @return map key value, which corresponds to the index and the name of the classes
		 */
		static std::map<int, std::string> readIndexLabelFile(std::string fileName);

		/**
		 * Method that returns a map containing the class names as key and its index as value
		 * @param labelIndex, map containing the index of the class as key and its name as value
		 * @return map containing the name of the classes as key and the index of it as value
		 */
		static std::map<std::string, int> convertIndexLabelFile(std::map<int, std::string> labelIndex);

		/**
		 * Method used to perform a split in a string
		 * @param str, input string that will be used to compute the tokens
		 * @param tokens, split result
		 * @param delimiters, delimiter that will be used in split
		 */
		static void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);
		static std::vector<std::string> read_ocr_result(std::string filename);
		static std::map<char, char> read_dict(std::string filename);
		static std::vector<std::string> getName(std::string pathDir);

		virtual ~InputOutput();
};

#endif /* INPUTOUTPUT_H_ */
