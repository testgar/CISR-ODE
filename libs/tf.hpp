#pragma once

//
// convert transfer function to steady states
//
//  ref: http://lpsa.swarthmore.edu/Representations/SysRepTransformations/TF2SS.html
//

#include <armadillo>

using namespace arma;

class CTransferFunction
{
private:

	vec v_num;
	vec v_den;

public:

	CTransferFunction(vec num,vec den)
	{
		if(num.n_rows>den.n_rows)
			throw std::runtime_error("numerator order higher than denominator!");
		v_den=den;
		v_num=num;
		if(v_num.n_rows<v_den.n_rows)
		{
			int diff=v_den.n_rows-v_num.n_rows;
			vec Z;
			Z.zeros(diff);
			v_num=join_cols<mat>(Z,num);
		}
		v_den=v_den/den(0);
		v_num=v_num/den(0);
	}

	void observable_canonical(mat &A,mat &B,mat &C,mat &D)
	{// Observable Canonical Form (OCF)
		int n=v_den.n_rows-1;
		mat den_pure=v_den.subvec(1,n); // [a1,a2,a3,...,an]
		mat num_pure=v_num.subvec(1,n); // [b1,b2,b3,...,bn]
		A.zeros(n,n);
		A.diag(1)=ones(n-1);
		A.col(0)=-den_pure;
		B=num_pure-den_pure*v_num(0);
		C.zeros(1,n);
		C(0)=1;
		D=v_num(0);
	}

	void controllable_canonical(mat &A,mat &B,mat &C,mat &D)
	{// Controllable Canonical Form (CCF)
		int n=v_den.n_rows-1;
		mat den_pure=v_den.subvec(1,n); // [a1,a2,a3,...,an]
		mat num_pure=v_num.subvec(1,n); // [b1,b2,b3,...,bn]
		A.zeros(n,n);
		A.diag(1)=ones(n-1);
		A.row(n-1)=-flipud(den_pure).t();
		B.zeros(n,1);
		B(n-1)=1;
		C=flipud(num_pure-den_pure*v_num(0)).t();
		D=v_num(0);
	}

};
