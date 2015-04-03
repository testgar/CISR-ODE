#pragma once

#include "svg.hpp"
#include "filesystem.hpp"
#include "config_files.hpp"

class CSystem_Base
{
protected:

	buffer_type buffer;
	arma::mat results;
	int buffer_index=0;

public:

	CSystem_Base()//:results(config::buffer_headers,0)
	{
		// constructor
		Reset();
		filesystem::ensure_folder(files::output_folder);
		int out_header_size=std::extent<decltype(outputs::output_header)>::value;
		if(out_header_size!=outputs::output_size)
			throw std::runtime_error("output header size mismatch!");
	}

	void Reset()
	{
		buffer_index=0;
		results=arma::mat(config::buffer_headers,0);
	}

protected:

	void export_output(std::string filename,const int fig_index)
	{
		int offset=config::results_additions;
		arma::mat ymat;
		ymat=get_results();
		figure_frame results_range=view_range(fig_index,fig_index);
		CSVG svg_image(filename+".svg",results_range);
		svg_image.begin();
		// svg_image.draw_frame();
		svg_image.comment("grids");
		svg_image.draw_grids();
		svg_image.comment("main curve");
		CSVG::Path_Config pconfig;
		svg_image.path_l(ymat,fig_index+offset,pconfig);
		svg_image.comment("frame");
		svg_image.draw_frame();
		svg_image.end();
		svg_image.close();
	}

	void export_figures(std::string basename)
	{
		int explist_size=std::extent<decltype(outputs::figure_list)>::value;

		for(int i=0;i<explist_size;i++)
		{
			std::string filename_aug=basename+"_"+outputs::figure_list[i].file_suffix;
			export_output(filename_aug,outputs::figure_list[i].output_index);
		}
	}

	void post_solve()
	{
		std::string basename=files::output_results;
		std::ofstream file(basename);
		results_export(&file);
		file.close();
		export_figures(basename);
	}

	void results_push(const time_type t,const time_type &next_dt,observer_type& y)
	{
		buffer(0,buffer_index)=t;
		buffer(1,buffer_index)=next_dt;
		buffer.submat(config::results_additions,buffer_index,config::buffer_headers-1,buffer_index)=y;
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
				buffer.submat(0,0,config::buffer_headers-1,buffer_index-1)
				);
			buffer_index=0;
		}
	}

	void results_export(std::ostream *output_media=&std::cout)
	{
		if(buffer_index>0)
			throw std::runtime_error("observer is not finalized!");
		if(results.n_rows==0)
			throw std::runtime_error("empty results!");

		int n_cols=results.n_cols;
		int n_rows=results.n_rows;

		*output_media <<"t\tdt";
		int out_header_size=std::extent<decltype(outputs::output_header)>::value;
		for(int i=0;i<out_header_size;i++)
		{
			*output_media << '\t' << outputs::output_header[i];
		}
		*output_media << std::endl;

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
		int offset=config::results_additions;
		arma::mat sub_y=results.submat(y_first+offset,0,y_last+offset,results.n_cols-1);

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
