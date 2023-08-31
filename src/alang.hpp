#include <string>
#include <vector>
#include <unordered_map>
#include <any>

#include <stack>

struct a_run_prop{
	std::unordered_map<std::string,unsigned long> lbl_tbl;
	std::vector<std::any> memory;
	char *pped;	
	std::stack<unsigned long> stack;
	std::vector<char*> tokz;
};

struct loc_var_info{
	std::string name;
	std::any dat;
};

struct dat_index{unsigned long id;};

std::any a_run(struct a_run_prop &p);
void a_pprocess(std::string in, struct a_run_prop &p);


struct conv_ret{
	bool is_float;
	union{ double flo; long inte;};
};
