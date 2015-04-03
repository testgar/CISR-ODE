#pragma once

#include <openssl/sha.h>
#include <string>
#include <armadillo>
#include <iomanip>

class sha1sum
{
public:
	static const int sha1_length=20; 

	sha1sum(std::string text)
	{
		SHA1((unsigned char*)text.c_str(), text.length(), sha1_result);
	}

	sha1sum(void *data_ptr,size_t size,unsigned int number)
	{
		SHA1((unsigned char*)data_ptr, size*number, sha1_result);
	}

	template<typename T>
	sha1sum(arma::Mat<T> matrix)
	{
		size_t ptr_lenght=sizeof(T)*matrix.n_cols*matrix.n_rows;
		SHA1((unsigned char*)matrix.memptr(), ptr_lenght, sha1_result);
	}

	std::string signature()
	{
		std::ostringstream os;
		os.fill('0');
		os<<std::hex;
		const unsigned char *begin=(unsigned char *)sha1_result;
		const unsigned char *end=(unsigned char *)(sha1_result+sha1_length);
		for(const unsigned char *ptr=begin;ptr<end;ptr++)
			os<<std::setw(2)<<(unsigned int)*ptr;
		return os.str();
	}

private:
	unsigned char sha1_result[sha1sum::sha1_length];
};