class CSysHandler
{
private:
	buffer_type buffer;
	arma::mat results;
	int buffer_index=0;
	time_type last_observed_time;
	state_type last_observed_states;
public:
	time_type start_time;
	time_type end_time;
	time_type dt;

    const value_type eps_rel=1E-10;
    const value_type eps_abs=1E-10;

	CSysHandler():results(outputs::output_size+1,0)
	{
		// constructor
		filesystem::ensure_folder(files::output_folder);
		
	}

	void observer(const state_type &x , const double t)
	{
		observer_type ymat;
		CSimulator::observer(x,t,ymat);
		results_push(t,ymat);
		last_observed_time=t;
		last_observed_states=x;
	}

	void rhs(const state_type &x, state_type &dxdt, const double t)
	{	
		CSimulator::rhs(x,dxdt,t,last_observed_states,last_observed_time);
	}

	double timer(const state_type &x, const double t)
	{
		_unused(x);
		return t+0.1;
	}

	void export_output(string filename,int out_index)
	{
		arma::mat ymat;
		ymat=get_results();
		figure_frame results_range=view_range(out_index,out_index);
		CSVG svg_image(filename+".svg",results_range);
		svg_image.begin();
		svg_image.draw_frame();
		svg_image.comment("grids");
		svg_image.draw_grids();
		svg_image.comment("main curve");
		CSVG::Path_Config pconfig;
		svg_image.path_l(ymat,out_index,pconfig);
		svg_image.comment("frame");
		svg_image.draw_frame();
		svg_image.end();
		svg_image.close();
	}

	void export_outputs(string filename)
	{
		int explist_size=extent<decltype(config::exportlist)>::value;

		for(int i=0;i<explist_size;i++)
		{
			string filename_aug=filename+"_"+config::exportlist[i].file_suffix;
			export_output(filename_aug,config::exportlist[i].output_index);
		}
	}

	void post_solve()
	{
		std::string filename=files::output_results;
		ofstream file(filename);
		results_export(&file);
		file.close();
		export_outputs(filename);
	}

	void results_push(time_type t,observer_type& y)
	{
		buffer(0,buffer_index)=t;
		buffer.submat(1,buffer_index,outputs::output_size,buffer_index)=y;
		buffer_index++;
		if(buffer_index==config::buffer_size)
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
				buffer.submat(0,0,outputs::output_size,buffer_index-1)
				);
			buffer_index=0;
		}
	}

	void results_export(std::ostream *output_media=&std::cout)
	{
		if(buffer_index>0)
			throw std::runtime_error("observer is not finalized!");
		int n_cols=results.n_cols;
		int n_rows=results.n_rows;
		for(int i=0;i<n_cols;i++)
		{
			for(int j=0;j<n_rows;j++)
			{
				*output_media << results(j,i);
				if(j!=n_rows-1)
					*output_media<<'\t';
			}
			*output_media<<std::endl;
		}
	}

	figure_frame view_range(int y_first=0,int y_last=-1)
	{
		if(y_last==-1)
			y_last=results.n_rows-2;
		if(buffer_index>0)
			throw std::runtime_error("observer is not finalized!");
		if(y_first>y_last)
			throw std::runtime_error("wrong matrix dimention!");
		arma::mat sub_t=results.submat(0,0,0,results.n_cols-1);
		arma::mat sub_y=results.submat(y_first+1,0,y_last+1,results.n_cols-1);

		double max_time=sub_t.max();
		double min_time=sub_t.min();
		double max_y=sub_y.max();
		double min_y=sub_y.min();
		double d=(max_y-min_y)/2;
		if(d==0)
			d=1;
		max_y+=d/10;
		min_y-=d/10;
		figure_frame frame;
		frame.min_time=min_time;
		frame.max_time=max_time;
		frame.min_y=min_y;
		frame.max_y=max_y;
		return frame;
	}

	const arma::mat get_results()
	{
		return results;
	}
};
