#pragma once

#include <armadillo>
#include <string>
#include <fstream>
#include "types.hpp"

class CSVG
{
public:

	struct Margin
	{
		double top;
		double right;
		double bottom;
		double left;
	};

	struct Attribute
	{
		double stroke_width=1;
		std::string stroke="black";
		std::string fill="none";
		std::string stroke_dasharray="";
	};

	struct Rectangle
	{
	public:
		double x1;
		double x2;
		double y1;
		double y2;
		Rectangle(const figure_frame &fframe)
		{
			x1=fframe.min_time;
			x2=fframe.max_time;
			y1=fframe.min_y;
			y2=fframe.max_y;
		} 
	};

	struct Point
	{
		double x;
		double y;
		Point operator+(Point const &p2)
		{
			return {x+p2.x,y+p2.y};
		}

		void operator+=(Point const &p2)
		{
			x+=p2.x;
			y+=p2.y;
		}

		bool operator<=(Point const &p2)
		{
			return x<=p2.x && y<=p2.y;
		}
	};

	struct Path_Config
	{
		std::string color;
		std::string stroke_width;
		std::string fill;

		Path_Config():color("blue"),stroke_width("1"),fill("none"){}
	};

private:
	std::ofstream svg_file;
	int width;
	int height;
	figure_frame frame;
	CSVG::Margin margin;
	const std::string thicks_color="gray";
	const std::string grids_color="lightgray";
	const double tick_depth_y=5;
	const double tick_depth_t=5;
	const double y_label_padding=5;
	const double t_label_padding=19;
	const int y_label_fonstsize=20;
	const int t_label_fonstsize=20;

public:

	CSVG(std::string filename,figure_frame frame,int width,int height,Margin margin):
		width(width),height(height),frame(frame), margin(margin)
	{
		svg_file.open(filename);
	}

	CSVG(std::string filename,figure_frame frame): 
		CSVG(filename,frame,1000,600,{70,130,100,180})
	{
// _(frame.min_time);
// _(frame.min_time);
// _(frame.max_time);
// _(frame.min_y);
// _(frame.max_y);
	}

	void begin()
	{
		svg_file<<"<svg height=\""<<height<<"\" width=\""<<width<<"\">\n";
		// for white background
		svg_file<<"<rect x=\"0\" y=\"0\" width=\""<<width+1<<"\" height=\""<<height+1<<"\" fill=\"white\" stroke-width=\"1\" />\n";
	}

	void end()
	{
		svg_file<<"Sorry, your browser does not support inline SVG.\n</svg>";
	}

	void close()
	{
		svg_file.close();
	}

	inline double trans_t(double t)
	{
		return (t-frame.min_time)/(frame.max_time-frame.min_time)*(width-margin.left-margin.right)+margin.left;
	}

	inline double trans_y(double y)
	{
		return (frame.max_y-y)/(frame.max_y-frame.min_y)*(height-margin.top-margin.bottom)+margin.top;
	}

	inline Point trans(Point p)
	{
		return {trans_t(p.x),trans_y(p.y)};
	}

	void comment(std::string comment_text)
	{

		svg_file<<"<!-- "<<comment_text<<"-->\n";
	}

	void path_l(arma::mat fig,int f_index,Path_Config pconf)
	{
		arma::uword n_cols=fig.n_cols;
		if(n_cols<2)
			throw std::runtime_error("unexpected small matrix!");
		svg_file<<"<path d=\"M ";
		double P0x=fig(0,0);
		double P0y=fig(f_index,0);
		svg_file<<trans_t(P0x)<<" "<<trans_y(P0y); // first point
		for(uint i=1;i<n_cols;i++)
		{
			svg_file<<" L ";
			double Px=fig(0,i);
			double Py=fig(f_index,i);
			svg_file<<trans_t(Px)<<" "<<trans_y(Py);
			// removed camma insertation due to a bug related to inkscape (it does not work with camma)
			// - if(i<n_cols-1)
			// - 	svg_file<<",";
		}
		svg_file<<"\"";
		svg_file<<" stroke=\""<<pconf.color<<"\""
				<<" stroke-width=\""<<pconf.stroke_width<<"\""
				<<" fill=\""<<pconf.fill<<"\""
				<<" />\n";
	}

	// void path_c(mat fig,int f_index,Path_Config pconf)
	// {
	// 	// implementing Catmull-Rom algorithm to obtain Bezier control point
	// 	const double F16=1.0/6.0;
	// 	const double F13=1.0/3.0;
	// 	const double F23=2.0/3.0;
	// 	int n_cols=fig.n_cols;
	// 	if(n_cols<2)
	// 		throw std::runtime_error("unexpected small matrix!");
	// 	//supposed fig is 2xn
	// 	svg_file<<"<path d=\"M ";
	// 	double P0x=fig(0,0);
	// 	double P0y=fig(1+f_index,0);
	// 	svg_file<<trans_t(P0x)<<" "<<trans_y(P0y); // first point
	// 	svg_file<<" C ";
	// 	double C0x=fig(0,0)*F23+fig(0,1)*F13;
	// 	double C0y=fig(1+f_index,0)*F23+fig(1+f_index,1)*F13;
	// 	svg_file<<trans_t(C0x)<<" "<<trans_y(C0y)<<","; // second point
	// 	for(int i=1;i<n_cols-1;i++)
	// 	{
	// 		mat R;
	// 		R 	<<F16<<1<<-F16<<endr
	// 			<<0<<1<<0<<endr
	// 			<<-F16<<1<<F16<<endr;
	// 		mat M;
	// 		M 	<<fig(0,i-1)<<fig(1+f_index,i-1)<<endr
	// 			<<fig(0,i)<<fig(1+f_index,i)<<endr
	// 			<<fig(0,i+1)<<fig(1+f_index,i+1)<<endr;
	// 		// =fig.submat(0,i-1,1,i+1).t();
	// 		mat L=R*M;
	// 		for(int j=0;j<3;j++)
	// 			svg_file<<trans_t(L(j,0))<<" "<<trans_y(L(j,1))<<","; // middle points
	// 	}
	// 	double Pn_1x=fig(0,n_cols-1);
	// 	double Pn_1y=fig(1+f_index,n_cols-1);
	// 	double Pn_2x=fig(0,n_cols-2);
	// 	double Pn_2y=fig(1+f_index,n_cols-2);
	// 	double CLx=Pn_1x*F23+Pn_2x*F13;
	// 	double CLy=Pn_1y*F23+Pn_2y*F13;		
	// 	svg_file<<trans_t(CLx)<<" "<<trans_y(CLy)<<","; // (3n-3)th point
	// 	svg_file<<trans_t(Pn_1x)<<" "<<trans_y(Pn_1y); // (3n-2)th point
	// 	svg_file<<"\"";
	// 	svg_file<<" stroke=\""<<pconf.color<<"\""
	// 			<<" stroke-width=\""<<pconf.stroke_width<<"\""
	// 			<<" fill=\""<<pconf.fill<<"\""
	// 			<<" />\n";
	// }

	void append_attribute(Attribute attribute)
	{
		svg_file<<"stroke-width=\""<<attribute.stroke_width<<"\" ";
		svg_file<<"fill=\""<<attribute.fill<<"\" ";
		if(attribute.stroke!="")
			svg_file<<"stroke=\""<<attribute.stroke<<"\" ";
		if(attribute.stroke_dasharray!="")
			svg_file<<"stroke-dasharray=\""<<attribute.stroke_dasharray<<"\" ";
	}

	void rectangle(Rectangle frame,Attribute attribute,bool scale)
	{
		double x1=(scale?trans_t(frame.x1):frame.x1);
		double x2=(scale?trans_t(frame.x2):frame.x2);
		double y1=(scale?trans_y(frame.y1):frame.y1);
		double y2=(scale?trans_y(frame.y2):frame.y2);
		svg_file<<"<path d=\"M";
		svg_file<<x1<<","<<y1<<" ";
		svg_file<<x2<<","<<y1<<" ";
		svg_file<<x2<<","<<y2<<" ";
		svg_file<<x1<<","<<y2<<" ";
		svg_file<<x1<<","<<y1;
		svg_file<<"\" ";
		append_attribute(attribute);
		svg_file<<"/>\n";
	}

	void line(Point p1,Point p2,Attribute attribute,bool scale)
	{
		if(!scale)
		{
			svg_file<<"<line ";
			svg_file<<"x1=\""<<p1.x<<"\" ";
			svg_file<<"x2=\""<<p2.x<<"\" ";
			svg_file<<"y1=\""<<p1.y<<"\" ";
			svg_file<<"y2=\""<<p2.y<<"\" ";
			append_attribute(attribute);
			svg_file<<"/>\n";
		}
		else
			throw std::runtime_error("not implemented!");
	}

	double get_tickspace(double frame_length,int min_tick,int max_tick)
	{
		if(frame_length<=0)
			throw std::runtime_error("invalid frame coordinates!");
		if(min_tick>=max_tick)
			throw std::runtime_error("invalid arguments!");
		const int digit_length=3;
		double digits[digit_length]={1,2,5};
		double space_power=1;
		int digit_index=1;
		while(frame_length/(digits[digit_index]*space_power)<min_tick)
		{ // to divide the space
			digit_index--;
			if(digit_index<0)
			{
				digit_index+=digit_length;
				space_power*=0.1;
			}
		}
		while(frame_length/(digits[digit_index]*space_power)>max_tick)
		{ // to enlargen the space
			digit_index++;
			if(digit_index>=digit_length)
			{
				digit_index-=digit_length;
				space_power*=10;
			}
		}
		return digits[digit_index]*space_power;
	}

	double get_tickspace_t()
	{
		const int tick_number[2]={5,10};// maximum number of thicks in t axis
		return get_tickspace(frame.max_time-frame.min_time,tick_number[0],tick_number[1]);
	}

	double get_tickspace_y()
	{
		const int tick_number[2]={5,10};// maximum number of thicks in y axis
		if(std::isinf(frame.max_y)||std::isinf(frame.min_y))
			throw std::runtime_error("Infinity value detected.");

		return get_tickspace(frame.max_y-frame.min_y,tick_number[0],tick_number[1]);
	}

	void label_ticks_y(double label_number,Point position)
	{
		svg_file<<"<text "
				<<"x=\""<<position.x-y_label_padding<<"\" "
				<<"y=\""<<position.y<<"\" "
				<<"font-size=\""<<y_label_fonstsize<<"\" "
				<<"text-anchor=\"end\" "
				<<"alignment-baseline=\"middle\">"
				<<label_number // text
				<<"</text>";
	}


	void label_ticks_t(double label_number,Point position)
	{
		svg_file<<"<text "
				<<"x=\""<<position.x<<"\" "
				<<"y=\""<<position.y+t_label_padding<<"\" "
				<<"font-size=\""<<t_label_fonstsize<<"\" "
				<<"text-anchor=\"middle\" "
				// <<"alignment-baseline=\"hanging\""
				<<">"
				<<label_number // text
				<<"</text>";
	}

	void draw_ticks_y()
	{
		double tickspace=get_tickspace_y();
		double tick_end=frame.max_y;
		double tick_original=ceil(frame.min_y/tickspace)*tickspace;
		Attribute attribute;
		attribute.stroke=thicks_color;
		double tick_position=tick_original;
		while(tick_position<=tick_end)
		{
			Point tip=trans({frame.min_time,tick_position});
			line(tip,tip+Point({tick_depth_y,0}),attribute,false);
			label_ticks_y(tick_position,tip);
			tick_position+=tickspace;
		}
	}

	void draw_grids_y()
	{
		const double tick_grid_distance=0;
		double tickspace=get_tickspace_y();
		double tick_end=frame.max_y;
		double tick_original=ceil(frame.min_y/tickspace)*tickspace;
		Attribute attribute;
		attribute.stroke=grids_color;
		attribute.stroke_dasharray="5,5";
		double tick_position=tick_original;
		while(tick_position<=tick_end)
		{
			line(trans({frame.max_time,tick_position}),trans({frame.min_time,tick_position})+Point({tick_depth_y+tick_grid_distance+1,0}),attribute,false);
			tick_position+=tickspace;
		}
	}

	void draw_ticks_t()
	{
		double tickspace=get_tickspace_t();
		double tick_end=frame.max_time;
		double tick_original=ceil(frame.min_time/tickspace)*tickspace;
		Attribute attribute;
		attribute.stroke=thicks_color;
		double tick_position=tick_original;
		while(tick_position<=tick_end)
		{
			Point tip=trans({tick_position,frame.min_y});
			line(tip,tip+Point({0,-tick_depth_t}),attribute,false);
			label_ticks_t(tick_position,tip);
			tick_position+=tickspace;
		}
	}

	void draw_grids_t()
	{
		const double tick_grid_distance=0;
		double tickspace=get_tickspace_t();
		double tick_end=frame.max_time;
		double tick_original=ceil(frame.min_time/tickspace)*tickspace;
		Attribute attribute;
		attribute.stroke=grids_color;
		attribute.stroke_dasharray="5,5";
		double tick_position=tick_original;
		while(tick_position<=tick_end)
		{
			line(trans({tick_position,frame.max_y}),trans({tick_position,frame.min_y})+Point({0,-tick_depth_t-tick_grid_distance-1}),attribute,false);
			tick_position+=tickspace;
		}
	}

	void draw_grids()
	{
		draw_grids_t();
		draw_grids_y();
	}

	void draw_frame()
	{
		draw_ticks_t();
		draw_ticks_y();
		CSVG::Attribute frame_attr;
		rectangle(frame,frame_attr,true);
// _(frame.min_time);
// _(frame.min_time);
// _(frame.max_time);
// _(frame.min_y);
// _(frame.max_y);
	}


};
