#include <cstring>
#include "alang.hpp"
#include <iostream>

#include <climits>
#include <cstdint>
#include <type_traits>
#include <cmath>


template<class T>
constexpr typename std::enable_if<std::is_unsigned<T>::value, T>::type
bswap(T i, T j = 0u, std::size_t n = 0u) {
  return n == sizeof(T) ? j :
    bswap<T>(i >> CHAR_BIT, (j << CHAR_BIT) | (i & (T)(unsigned char)(-1)), n + 1);
}

constexpr unsigned long swap(unsigned long in){
		in = bswap(in);
		unsigned long in2 = in;
		while((unsigned char)in == 0) in >>= 8;
		
		return in;
}

inline std::any read_dat(std::vector<std::any> &dat,unsigned long id){
	if(id >= dat.size()){dat.resize(id+1); dat[id] = 0;}
	
	return dat[id];
	
}

inline void write_dat(std::vector<std::any> &dat, unsigned long id, std::any in){
	if(id >= dat.size()){dat.resize(id+1);}
	dat[id] = in;
}

static std::any parse_arg(char *arg, std::vector<std::any> &dat){

	if(arg[0] == 'c'){ // constant arg
		switch(arg[1]){
				case 's':
				{
					return std::string(&arg[2]);
					break;
				}
				case 'd':
				{
					return std::atof(&arg[2]);
					break; 
				}
				case 'i':
				{
					return (long)(std::atol(&arg[2]));
					break;
				}
		}
			
	}
	else if(arg[0] == 'v'){
		switch(arg[1]){
			case 'd':
			{
					return ((struct dat_index){std::atoi(&arg[2])});
			}
			case 'l':
			{
					
			}
		}
		
	}
	
	std::cout << "ERROR PARSING PARSE_ARG: " << arg << std::endl;
	
	return nullptr;
		
		
}

static int comp(std::any op1, std::any op2){
	double o1,o2;
	if(op1.type().name()[0] == 'd') o1 = std::any_cast<double>(op1);
	if(op2.type().name()[0] == 'd') o2 = std::any_cast<double>(op2);

	return (o1==o2);
	
}

long any_to_int(std::any in){
	int i;
	
	if(in.type() == typeid(double)) i = std::any_cast<double>(in);
	else 	if(in.type() == typeid(long)) i = std::any_cast<long>(in);
	else return 0;
		return i;
		
}

std::any a_run( struct a_run_prop &p){
	char *con;
	char *s = p.pped;
	
	auto tok = p.tokz.begin();
	
	bool decode_cmd = true;
	
	while(*tok ){
		if(decode_cmd){
				unsigned long n;
				strncpy((char*)&n,*tok,8);
				
				switch(n){
						case swap('lbl'):
						{
							++tok;
							break;
						}
						case swap('mov'):
						{	
							++tok;
							std::any op1 = parse_arg(*tok, p.memory);
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}

							++tok;
							std::any op2 = parse_arg(*tok,p.memory);

							if((op2.type()) == typeid(struct dat_index)){
								
									write_dat(p.memory,std::any_cast<struct dat_index>(op2).id,op1);
							}
							else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT" << std::endl;
							}
							break;
								
						}
						case swap('ast'):
						{	
							++tok;
							std::any op1 = parse_arg(*tok, p.memory);
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}

							++tok;
							
							std::any opid = parse_arg(*tok,p.memory);
							
							if((opid.type()) == typeid(struct dat_index)){
								
									opid = read_dat(p.memory,std::any_cast<struct dat_index>(opid).id);
							}
							long idx = any_to_int(opid);
							
							++tok;
							
														
							std::any op2 = parse_arg(*tok,p.memory);

							long midx;
							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,midx=(std::any_cast<struct dat_index>(op2).id));
							}
														


									std::any &arr = op2;
																	
									if(arr.type() != typeid(std::vector<std::any>)) arr = std::vector<std::any>();
									
									std::vector<std::any> ve = std::any_cast<std::vector<std::any>>(arr);
									
									if(idx >= ve.size() ){ve.resize(idx+1);
									}
									ve[idx] = op1;
								
									write_dat(p.memory,midx,ve);
							
						
							break;
								
						}
						case swap('agt'):
						{
							++tok;
							std::any op1 = parse_arg(*tok, p.memory);
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}

							if(op1.type() != typeid(std::vector<std::any>)){
									std::cout << "ERROR: TRYING TO USE AGT ON NON-ARRAY LVALUE" << std::endl;
									return nullptr;
							}

							++tok;
							
							std::any opid = parse_arg(*tok,p.memory);
							
							if((opid.type()) == typeid(struct dat_index)){
								
									opid = read_dat(p.memory,std::any_cast<struct dat_index>(opid).id);
							}
							long idx = any_to_int(opid);
							
							++tok;
							std::any op2 = parse_arg(*tok,p.memory);
							
							std::vector<std::any> v = std::any_cast<std::vector<std::any>>(op1);
							if(idx>=v.size())v.resize(idx+1);

							if((op2.type()) == typeid(struct dat_index)){
									write_dat(p.memory,std::any_cast<struct dat_index>(op2).id,v[idx]);
							}
							else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT" << std::endl;
									return nullptr;
							}
							break;
						}
						case swap('sin'):
						{
							++tok;
							std::any op1 = parse_arg(*tok, p.memory);
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}
							++tok;
							
							std::any op2 = parse_arg(*tok,p.memory);
							
							if((op2.type()) == typeid(struct dat_index)){
								
									write_dat(p.memory,std::any_cast<struct dat_index>(op2).id,sin(std::any_cast<double>(op1)));
							}
							else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT" << std::endl;
							}
							
							break;
															
						}
						case swap('sqrt'):
						{
							++tok;
							std::any op1 = parse_arg(*tok, p.memory);
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}
							++tok;
							
							std::any op2 = parse_arg(*tok,p.memory);
							
							if((op2.type()) == typeid(struct dat_index)){
								
									write_dat(p.memory,std::any_cast<struct dat_index>(op2).id,sqrt(std::any_cast<double>(op1)));
							}
							else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT" << std::endl;
							}
							
							break;
															
						}
						case swap('prn'):
						{
							++tok;
							while(*tok != nullptr && (*(unsigned int*)*tok) != swap('eas')){
								std::any op = parse_arg(*tok,p.memory);
								if((op.type()) == typeid(struct dat_index)){
									op = read_dat(p.memory,std::any_cast<struct dat_index>(op).id);
								}
								if(op.type() == typeid(std::string)){
									std::cout << std::any_cast<std::string>(op);
								}
								if(op.type() == typeid(double)){
									std::cout << std::any_cast<double>(op);
								}
								if(op.type() == typeid(long)){
									std::cout << std::any_cast<long>(op);
								}
								
								++tok;
							}
								
							
							break;
						}
						case swap('jio'):
						{
							++tok;
							std::any op1 = parse_arg(*tok, p.memory);
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}
							long cond = any_to_int(op1);

							if(!cond) break;
							
						}
						case swap('jmp'):
						{
							++tok;
							auto it = p.lbl_tbl.find(std::string(*tok));
							if(it != p.lbl_tbl.end()){
								tok = p.tokz.begin() + it->second;
							}
							else{
								std::cout << "ERROR: JUMP TO NONEXISTANT LABEL \"" << *tok << "\""<<std::endl;
							}
							break;
						}
						case swap('cie'):
						{
							++tok;

							std::any op1 = parse_arg(*tok, p.memory);
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}
							++tok;
							
							std::any op2 = parse_arg(*tok,p.memory);
							
							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,std::any_cast<struct dat_index>(op2).id);
							}
							
							++tok;
							
							std::any op3 = parse_arg(*tok,p.memory);
							
							if((op3.type()) == typeid(struct dat_index)){
								
									write_dat(p.memory,std::any_cast<struct dat_index>(op3).id,(double)(comp(op1,op2)));
							}
							else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT" << std::endl;
							}
							break;
						}
						case swap('cip'):
						{
							++tok;

							std::any op1 = parse_arg(*tok, p.memory);
							
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}	
							
							
							++tok;
							
							std::any op2 = parse_arg(*tok,p.memory);
														unsigned long idx;

							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,idx=std::any_cast<struct dat_index>(op2).id);
							}else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT" << std::endl;
									return nullptr;
							}
							
							
							double o1 = std::any_cast<double>(op1);
							
								
							write_dat(p.memory,idx,(double)(o1>=0));

											
							
							break;
						}
						case swap('rnd'):
						{
							++tok;

							std::any op1 = parse_arg(*tok, p.memory);
							
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}	
							
							
							++tok;
							
							std::any op2 = parse_arg(*tok,p.memory);
														unsigned long idx;

							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,idx=std::any_cast<struct dat_index>(op2).id);
							}else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT" << std::endl;
									return nullptr;
							}
							
							
							double o1 = std::any_cast<double>(op1);
							
								
							write_dat(p.memory,idx,(double)(round(o1)));

											
							
							break;
						}
						case swap('sub'):
						{
							++tok;

							std::any op1 = parse_arg(*tok, p.memory);
							
							unsigned long idx;
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,idx=std::any_cast<struct dat_index>(op1).id);
							}
							
							
							++tok;
							
							std::any op2 = parse_arg(*tok,p.memory);
							
							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,std::any_cast<struct dat_index>(op2).id);
							}
							
							
							double o1 = std::any_cast<double>(op1);
							double o2=std::any_cast<double>(op2);
							
							++tok;
							
							std::any op3 = parse_arg(*tok,p.memory);
							
							if((op3.type()) == typeid(struct dat_index)){
								
									write_dat(p.memory,std::any_cast<struct dat_index>(op3).id,o1-o2);

							}					else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT(SUB)" << std::endl;
									return nullptr;
							}
							
							break;
						}
						case swap('subi'):
						{
							++tok;

							std::any op1 = parse_arg(*tok, p.memory);
							
							unsigned long idx;
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,idx=std::any_cast<struct dat_index>(op1).id);
							}
							
							
							++tok;
							
							std::any op2 = parse_arg(*tok,p.memory);
							
							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,std::any_cast<struct dat_index>(op2).id);
							}
							
							
							long o1 = std::any_cast<long>(op1);
							long  o2=std::any_cast<long>(op2);
							
							++tok;
							
							std::any op3 = parse_arg(*tok,p.memory);
							
							if((op3.type()) == typeid(struct dat_index)){
								
									write_dat(p.memory,std::any_cast<struct dat_index>(op3).id,o1-o2);

							}					else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT(SUBI)" << std::endl;
									return nullptr;
							}
							
							break;
						}
						case swap('div'):
						{
							++tok;

							std::any op1 = parse_arg(*tok, p.memory);
							
							unsigned long idx;
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,idx=std::any_cast<struct dat_index>(op1).id);
							}
							
							
							++tok;
							
							std::any op2 = parse_arg(*tok,p.memory);
							
							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,std::any_cast<struct dat_index>(op2).id);
							}
							
							
							double o1 = std::any_cast<double>(op1);
							double o2=std::any_cast<double>(op2);
							
							++tok;
							
							std::any op3 = parse_arg(*tok,p.memory);
							
							if((op3.type()) == typeid(struct dat_index)){
								
									write_dat(p.memory,std::any_cast<struct dat_index>(op3).id,o1/o2);

							}					else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT(DIV)" << std::endl;
									return nullptr;
							}
							
							break;
						}
						case swap('ids'):
						{	
							++tok;

							
							std::any op2 = parse_arg(*tok,p.memory);
														unsigned long idx;

							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,idx=std::any_cast<struct dat_index>(op2).id);
							}else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT" << std::endl;
									return nullptr;
							}
							
							
							if(op2.type() == typeid(long)){
								write_dat(p.memory,idx,(double)(std::any_cast<long>(op2)));
							}
							else{
								
								write_dat(p.memory,idx,(long)(std::any_cast<double>(op2)));
							}
											
							
							break;
						}		
						case swap('ret'):
						{	
							++tok;

							
							std::any op2 = parse_arg(*tok,p.memory);
														unsigned long idx;

							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,idx=std::any_cast<struct dat_index>(op2).id);
							}
							
							return op2;
											
							
							break;
						}						
						case swap('add'):
						{
							++tok;

							std::any op1 = parse_arg(*tok, p.memory);
							
							unsigned long idx;
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,idx=std::any_cast<struct dat_index>(op1).id);
							}	else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT(ADD)" << std::endl;
									return nullptr;
							}
							
							
							++tok;
							
							std::any op2 = parse_arg(*tok,p.memory);
							
							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,std::any_cast<struct dat_index>(op2).id);
							}
							
							
							double o1 = std::any_cast<double>(op1);
							double o2=std::any_cast<double>(op2);
							
								
							write_dat(p.memory,idx,o1+o2);

											
							
							break;
						}
						case swap('mul'):
						{
							++tok;

							std::any op1 = parse_arg(*tok, p.memory);
							
							unsigned long idx;
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,idx=std::any_cast<struct dat_index>(op1).id);
							}	else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT(MUL)" << std::endl;
									return nullptr;
							}
							
							
							++tok;
							
							std::any op2 = parse_arg(*tok,p.memory);
							
							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,std::any_cast<struct dat_index>(op2).id);
							}
							
							
							double o1 = std::any_cast<double>(op1);
							double o2=std::any_cast<double>(op2);
							
								
							write_dat(p.memory,idx,o1*o2);

											
							
							break;
						}
				}
				
			
		}
		
		++tok;
	}

	return nullptr;
}

void replace_all(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

void a_pprocess(std::string in, struct a_run_prop &p){
			replace_all(in, "\n"," ");
		replace_all(in,"\\n","\n");
		
		p.pped = strdup(in.c_str());

		unsigned long count = 0;
		
		p.tokz.clear();
		
		char *tokp=strtok(p.pped," ");
		char *old;
		do{
			if(!strcmp(old,"lbl")){
					p.lbl_tbl.insert({std::string(tokp),count});
			}
			p.tokz.push_back(tokp);
			
			old = tokp;
			++count;
		}
		while(tokp=strtok(nullptr," "));
		p.tokz.push_back(0);

}
