#ifndef Executive_H
#define Executive_H
#include <string>
class Executive
{
	public:
		Executive();
		~Executive();
		void Take(std::string FileName);
		void Finalize();
		void Search();
		void Factor(int integer);
		void FrequencyAnalysis();
		void Decrypt();
	private:
		int m_size;
		int m_Hfrequency;
		int m_Length;
		int m_Shiftsize;
		std::string m_Keyword;
		int * m_IntKey;
		char * m_CharArray; //stores original cipher text
		char * m_alphabet; //stores english alphabet
		char * m_ShiftArray; //stores every n'th letter of ciphertext
		double * m_ShiftFrequency; //stores every n'th letter of ciphertext frequency
		double * m_CipherFrequency; //stores ciphertext frequency
		int * m_FactorArray; //stores every factor
		float * m_Frequency; //stores english alphabet frequency
};
#endif
