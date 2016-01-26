#pragma once

//
// convert transfer function to steady states
//
//  ref: http://lpsa.swarthmore.edu/Representations/SysRepTransformations/TF2SS.html
//

#include <armadillo>

class CTransferFunction
{
private:

	arma::vec v_num;
	arma::vec v_den;

	void init(arma::vec num,arma::vec den)
	{
		if(num.n_rows>den.n_rows)
			throw std::runtime_error("numerator order higher than denominator!");
		v_den=den;
		v_num=num;
		if(v_num.n_rows<v_den.n_rows)
		{
			arma::uword diff=v_den.n_rows-v_num.n_rows;
			arma::vec Z;
			Z.zeros(diff);
			v_num=arma::join_cols<arma::mat>(Z,num);
		}
		v_den=v_den/den(0);
		v_num=v_num/den(0);
	}


public:

	CTransferFunction(arma::vec num,arma::vec den)
	{
		init(num,den);
	}

#ifndef ARMA_USE_CXX11
	CTransferFunction(std::vector<double> num,std::vector<double> den)
	{
		arma::vec arma_num(arma::uword(num.size()));
		arma::vec arma_den(arma::uword(den.size()));
		arma::uword i;
		for(i=0;i<num.size();i++)
			arma_num(i)=num[i];
		for(i=0;i<den.size();i++)
			arma_den(i)=den[i];
		init(arma_num,arma_den);
	}
#endif

	void observable_canonical(arma::mat &A,arma::mat &B,arma::mat &C,arma::mat &D) const
	{// Observable Canonical Form (OCF)
		arma::uword n=v_den.n_rows-1;
		arma::mat den_pure=v_den.subvec(1,n); // [a1,a2,a3,...,an]
		arma::mat num_pure=v_num.subvec(1,n); // [b1,b2,b3,...,bn]
		A.zeros(n,n);
		A.diag(1)=arma::ones(n-1);
		A.col(0)=-den_pure;
		B=num_pure-den_pure*v_num(0);
		C.zeros(1,n);
		C(0)=1;
		D=v_num(0);
	}

	void controllable_canonical(arma::mat &A,arma::mat &B,arma::mat &C,arma::mat &D) const
	{// Controllable Canonical Form (CCF)
		arma::uword n=v_den.n_rows-1;
		arma::mat den_pure=v_den.subvec(1,n); // [a1,a2,a3,...,an]
		arma::mat num_pure=v_num.subvec(1,n); // [b1,b2,b3,...,bn]
		A.zeros(n,n);
		A.diag(1)=arma::ones(n-1);
		A.row(n-1)=-flipud(den_pure).t();
		B.zeros(n,1);
		B(n-1)=1;
		C=flipud(num_pure-den_pure*v_num(0)).t();
		D=v_num(0);
	}

	arma::uword order() const
	{
		return std::max(v_num.n_rows,v_den.n_rows)-1;
	}

};
