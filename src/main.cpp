#include "alang.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char **argv){
		if(argc <= 1){
				std::cout << "Input file not specified" << std::endl;
				return 1;
		}
		
		std::ifstream prog;
		prog.open(argv[1]);
		if(!prog.is_open()){ std::cout << "Invalid file path" << std::endl;}
		
		std::stringstream buf;
		
		buf << prog.rdbuf();
		
		struct a_run_prop p;
		
		a_pprocess(std::string(buf.str()), p);
		a_run(p);
		
		prog.close();
				
		return 0;
}
