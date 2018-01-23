#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace  std;

void CreateCumulativeTemporalOrderSVG(const vector<string>& svgLines,const vector<unsigned int> &strokeLineIds,const string& dst_temporal_dir_png,const string& dst_temporal_dir_svg)
{
    //cout << strokeLineIds[0] << endl;
	unsigned int first_stroke_line_id = *min_element(strokeLineIds.begin(),strokeLineIds.end());  // some thing wrong
	unsigned int last_stroke_line_id  = *max_element(strokeLineIds.begin(),strokeLineIds.end());
    //cout << first_stroke_line_id << endl;
    cout << "last id =" << last_stroke_line_id << endl;
	int counter = 1;// how many lines?
	stringstream ss;
	for(unsigned int i = first_stroke_line_id ; i <= last_stroke_line_id ; i++)
	{
		// Dump SVG
		ss << dst_temporal_dir_svg << "/" << counter << ".svg";
		string svg_path = ss.str();
		cout << svg_path << endl;
		ofstream ofs_svg(ss.str().c_str());
		// Add lines up to and including i
		for(unsigned int j = 0 ; j <= i ; j++)
			ofs_svg << svgLines.at(j) << endl;
		// Add lines from last_stroke_line_id + 1 -> end
		for(unsigned int j = last_stroke_line_id + 1 ; j < svgLines.size() ; j++)
			ofs_svg << svgLines.at(j) << endl;
		ofs_svg.close();
		ss.str("");
		// Dump PNG
		ss << dst_temporal_dir_png << "/" << counter << ".png";
		string png_path = ss.str();
		cout << ss.str() << endl;
		ss.str("");
		ss << "convert -flatten -depth 8 -density 1200 -resize 256x256 \"" << svg_path << "\"  \"" << png_path << "\"";\
		cout << ss.str() << endl;
		system(ss.str().c_str());
		ss.str("");

		++counter;
	}
}
// src_svg = SVG filepath corresponding to input sketch
// png_css_root_dir = Root directory where all temporally accumulated sketch sequences are to be stored as pngs
// svg_css_root_dir = Root directory where all temporally accumulated sketch sequences are to be stored as svgs
// category = Label name of category (e.g. "airplane")
// counter = Id of PNG or svg within the category directory (as per Eitz data layout) [1-80:airplane,81-160: ...]
int ProcessSVG(const string& src_svg,const char* png_css_root_dir,const char* svg_css_root_dir,const string& category,unsigned int counter)
{
	// Create destination category folders
	stringstream ss;
	// Create destination cateory directories
	ss << "mkdir -m 777 -p \"" << png_css_root_dir << "/" << category << "\" " ;
	cout << ss.str() << endl;
	system(ss.str().c_str());//Execute system command Invokes the command processor to execute a command.
	ss.str("");//clear the ss, ss.clear() can't clear the memory
	ss << "mkdir -m 777 -p \"" << svg_css_root_dir << "/" << category << "\" " ;
	cout << ss.str() << endl;
	system(ss.str().c_str());
	ss.str("");

	// Read svg file
	vector<string> svgLines;
	//cout << "SVG:" << src_svg << endl;
    ifstream ifs(src_svg.c_str());	// how many lines
    cout << "original svg:" << src_svg.c_str() << endl;
	string line;
	cout << "line lists:" << endl;
	while (std::getline(ifs, line))
	{
			svgLines.push_back(line);
			cout << line << endl;
	}


	// Get indices of all lines containg stroke info(string 'path id')
	vector<unsigned int> strokeLineIds;
	string strk_token ="path id";
	for(unsigned int i = 0 ; i < svgLines.size() ; i++)
	{
		string line = svgLines.at(i);
		size_t found = line.find(strk_token);//查找是否包含子字符串
		if (found != string::npos) //find that
			strokeLineIds.push_back(i);
       // cout << strokeLineIds.size() << endl;
	}

        // Create a sequence of svgs/pngs where strokes are accumulated in cumultemporal order
        ss << png_css_root_dir << "/" << category ;
        cout << ss.str() << endl;
        string dst_temporal_dir_png = ss.str();
        ss.str("");
        ss << svg_css_root_dir << "/" << category ;
        cout << ss.str() << endl;
        string dst_temporal_dir_svg = ss.str();
        ss.str("");
        cout << "for testing:" << strokeLineIds[0] << endl;
        CreateCumulativeTemporalOrderSVG(svgLines,strokeLineIds,dst_temporal_dir_png,dst_temporal_dir_svg);

	return strokeLineIds.size();//how many lines the sketch is divided

}

int main(int argc,char **argv )
{
       string src_svg = "flower.svg";
       string png_css_root_dir = "./png";
       string svg_css_root_dir ="./svg";
       unsigned int id = 12;
       cout << ProcessSVG(src_svg.c_str(),png_css_root_dir.c_str(),svg_css_root_dir.c_str(),"flower",0) << endl;

       return 0;
}
/*
#include <syslog.h>

int main(int argc, char **argv)
{
        openlog("mytest",LOG_CONS| LOG_PID,0);
        syslog(LOG_DEBUG,"TEST",argv[0]);
        closelog();
        return 0;
}
*/

