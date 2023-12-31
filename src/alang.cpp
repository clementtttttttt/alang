#include <cstring>
#include "alang.hpp"
#include <iostream>

#include <climits>
#include <cstdint>
#include <type_traits>
#include <cmath>
#include <iomanip>


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
					return ((struct dat_index){std::atol(&arg[2])});
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
	if(op1.type() == typeid(double)) o1 = std::any_cast<double>(op1);
	if(op2.type() == typeid(double)) o2 = std::any_cast<double>(op2);
	if(op1.type() == typeid(long)) o1 = std::any_cast<long>(op1);
	if(op2.type() == typeid(long)) o2 = std::any_cast<long>(op2);
	return (o1==o2);
	
}

long any_to_int(std::any in){
	long i;
	
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
						case swap('lst'):
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
							auto fit=p.l_var_set_tbl.find(std::string(std::any_cast<std::string>(op2)));
							a_loc_var_set_func f = fit->second;
							
							f(op1);
							
							break;
								
						}
						case swap('lgt'):
						{	
							++tok;
							

							std::any op1 = parse_arg(*tok, p.memory);
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}

							auto fit = p.l_var_get_tbl.find(std::string(std::any_cast<std::string>(op1)));
							a_loc_var_get_func f = fit->second;
							


							++tok;
							std::any op2 = parse_arg(*tok,p.memory);

							if((op2.type()) == typeid(struct dat_index)){
								
									write_dat(p.memory,std::any_cast<struct dat_index>(op2).id,f());
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
							
							if(idx <0) {std::cout << "TRYING TO ACCESS NEGATIVE IDX" << std::endl;return nullptr;}
							
							++tok;
							
														
							std::any op2 = parse_arg(*tok,p.memory);

							long midx;
							if((op2.type()) == typeid(struct dat_index)){
								
									op2 = read_dat(p.memory,midx=(std::any_cast<struct dat_index>(op2).id));
							}
														

																	
									if(op2.type() != typeid(std::vector<std::any>)) op2 = std::vector<std::any>(10);
									
									std::vector<std::any> ve = std::any_cast<std::vector<std::any>>(op2);
									
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
						case swap('cos'):
						{
							++tok;
							std::any op1 = parse_arg(*tok, p.memory);
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}
							++tok;
							
							std::any op2 = parse_arg(*tok,p.memory);
							
							if((op2.type()) == typeid(struct dat_index)){
								
									write_dat(p.memory,std::any_cast<struct dat_index>(op2).id,cos(std::any_cast<double>(op1)));
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
									std::cout << std::fixed<<std::setprecision(5) << std::any_cast<double>(op);
								}
								if(op.type() == typeid(long)){
									std::cout << std::any_cast<long>(op);
								}
								
								++tok;
							}
								
							
							break;
						}
						case swap('jio'):
						case swap('jiz'):
						{
							++tok;
							std::any op1 = parse_arg(*tok, p.memory);
							
							if((op1.type()) == typeid(struct dat_index)){
								
									op1 = read_dat(p.memory,std::any_cast<struct dat_index>(op1).id);
							}
							long cond = any_to_int(op1);
							
							if(n == swap('jiz')) cond = !cond;

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
						case swap('call'):
						{
							++tok;
							auto it = p.lbl_tbl.find(std::string(*tok));
							p.stack.push(tok - p.tokz.begin());
							if(it != p.lbl_tbl.end()){
								tok = p.tokz.begin() + it->second;
							}
							else{
								std::cout << "ERROR: CALL TO NONEXISTANT LABEL \"" << *tok << "\""<<std::endl;
							}
							break;
						}
						case swap('ret'):
						{
							long old = p.stack.top();
							p.stack.pop();
								tok = p.tokz.begin() + old;
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
						case swap('subi'):
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
							double o1=0;
							double o2=0;
							long io1=0,io2=0;
							
							bool use_float;
							if(n == swap('sub')){
								o1 = std::any_cast<double>(op1);
								o2 = std::any_cast<double>(op2);
								use_float=true;
							}
							else{
								if(op1.type() != typeid(long)){
									io1 = std::any_cast<double>(op1);
								}else{
									io1 = std::any_cast<long>(op1);
								}
								io2 = std::any_cast<long>(op2); 
								use_float = false;
							}

							
							++tok;
							
							std::any op3 = parse_arg(*tok,p.memory);
							
							if((op3.type()) == typeid(struct dat_index)){
								std::any v;
									if(use_float ==true){
										v = (o1-o2);
									}
									else{
										v= (io1-io2);									
									}
									write_dat(p.memory,std::any_cast<struct dat_index>(op3).id,v);

							}					else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT(SUB)" << std::endl;
									return nullptr;
							}
							
							break;
						}
							case swap('mod'):
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
							double o2;
							
							if(op2.type() == typeid(double))
							o2=std::any_cast<double>(op2);
							else o2 = std::any_cast<long>(op2);
														
							++tok;
							
							std::any op3 = parse_arg(*tok,p.memory);
							
							if((op3.type()) == typeid(struct dat_index)){
								
									write_dat(p.memory,std::any_cast<struct dat_index>(op3).id,fmod(o1,o2));

							}					else{
									std::cout << "ERROR: TRYING TO MOV VALUE TO CONSTANT(DIV)" << std::endl;
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
							double o2;
							
							if(op2.type() == typeid(double))
							o2=std::any_cast<double>(op2);
							else o2 = std::any_cast<long>(op2);
														
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
						case swap('ext'):
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
						case swap('addi'):
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
							
							
							double o1=0;
							double o2=0;
							long io1=0,io2=0;
							
							bool use_float = false;
							if(n == swap('add')){
								o1 = std::any_cast<double>(op1);
								o2 = std::any_cast<double>(op1);
								use_float=true;
															write_dat(p.memory,idx,(o1+o2));

							}
							else{
								io1 = std::any_cast<long>(op1);
								io2 = std::any_cast<long>(op2);
															write_dat(p.memory,idx,(long)(io1+io2));
 
							}
								

											
							
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
							
							
							double o1;
							
							if(op1.type() == typeid(double))
							o1=std::any_cast<double>(op1);
							else o1 = std::any_cast<long>(op1);
							double o2;
							
							if(op2.type() == typeid(double))
							o2=std::any_cast<double>(op2);
							else o2 = std::any_cast<long>(op2);
							
								
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
							replace_all(in, "\t"," ");

			replace_all(in, "\n"," ");
		replace_all(in,"\\n","\n");
		
		p.pped = strdup(in.c_str());

		unsigned long count = 0;
		
		p.tokz.clear();
		
		char *tokp=strtok(p.pped," ");
		char *old=nullptr;
		do{
			if(old && !strcmp(old,"lbl")){
					p.lbl_tbl.insert({std::string(tokp),count});
			}
			p.tokz.push_back(tokp);
			
			old = tokp;
			++count;
		}
		while(tokp=strtok(nullptr," "));
		p.tokz.push_back(0);

}
