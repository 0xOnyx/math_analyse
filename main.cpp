#include <iostream>
#include <string>
#include <list>
#include <exception>
#include <ctype.h>

typedef enum e_token
{
	NUMBER,

	ADD,
	MINUS,
	MULTIPLY,
	DIVISION,

	RBRACE,
	LBRACE,
}	t_token;

typedef struct	s_tokens
{
	t_token		type;
	double		value;
}	t_tokens;

double ft_atoi(int &res, str::string::iterator current, std::string::iterator end)
{
	bool	neg;

	neg = false;
	if (*current == '-')
		neg = true
	for (;current != end; current++)
	{

	}
}

class Lexer
{
public:
	Lexer(std::string &expression) :line_str(expression), token()
	{
		if (expression.length() <= 0)
			throw std::runtime_error("the str is empty");
	}
	std::list<t_token>	getToken(void)
	{
		int 	tmp_value;

		for (std::string::iterator current = line_str.begin(); current != line_str.end(); line_str++)
		{
			if (*current == '+')
				this->token.push_back({ADD, 0});
			else if (*current == '-')
			{
				if (*(current + 1) != line_str.end() && isdigit(*(current + 1)) != 0)
				{
					current += ft_atoi(tmp_value, current, line_str.end());
					this->token.push_back({NUMBER, tmp_value});
				}
				else
					this->token.push_back({MINUS, 0});
			}
			else if (*current == '*')
				this->token.push_back({MULTIPLY, 0});
			else if (*current == '/')
				this->token.push_back({DIVISION, 0});
			else if (*current == '(')
				this->token.push_back({RBRACE, 0});
			else if (*current == ')')
				this->token.push_back({LBRACE, 0});
			else if (isdigit(*current) != 0)
			{
				current += ft_atoi(tmp_value, current, line_str.end());
				this->token.push_back({NUMBER}, tmp_value);
			}
		}
		return (this->token);
	}
private:
	std::string			line_str;
	std::list<t_tokens>	token;
};


double calc(std::string &expression)
{
	Lexer lexer(expression);
	std::list<t_tokens>	tokens;

	tokens = lexer.getToken();
	return (0);
}

int	main(void)
{
	std::string	current;

	try{
		std::cout << "Math expr >" << std::endl;
		getline(std::cin, current);
		std::cout << "res =>" << calc(current);
	}
	catch(std::exception const &e)
	{
		std::cout << "[ERROR]\t" << e.what() << std::endl;
	}
	return (0);
}
