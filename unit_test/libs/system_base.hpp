#pragma once

class CSystem_UnitTest
{
public:

	typedef double value_type;
	typedef double time_type;
	// typedef arma::vec::fixed<state_size> state_type;
	// typedef state_type deriv_type;
	typedef state_type deriv_type;
	typedef arma::vec::fixed<state_size> observer_type;
	typedef arma::mat::fixed<buffer_headers,buffer_size> buffer_type;

protected:

	buffer_type buffer;
	arma::mat results;
	int buffer_index=0;

public:

	CSystem_UnitTest()
	{
		Reset();
	}

	void Reset()
	{
		buffer_index=0;
		results=arma::mat(buffer_headers,0);
	}

protected:

	void results_push(const time_type t,const time_type &next_dt,observer_type& y)
	{
		buffer(0,buffer_index)=t;
		buffer(1,buffer_index)=next_dt;
		buffer.submat(results_additions,buffer_index,buffer_headers-1,buffer_index)=y;
		buffer_index++;
		if(buffer_index==buffer_size)
		{
			results=join_horiz(results,buffer);
			buffer_index=0;
		}
	}

	void results_finalize()
	{
		if(buffer_index>0)
		{
			results=join_horiz(
				results,
				buffer.submat(0,0,buffer_headers-1,buffer_index-1)
				);
			buffer_index=0;
		}
	}

	const arma::mat get_results()
	{
		return results;
	}

};
