#include <iostream>
#include <string>
#include <list>
#include <exception>
#include <cctype>

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

size_t	ft_atoi(double &res, std::string &str, std::string::iterator current)
{
	int 	distance;
	size_t 	next_value;

	distance = std::distance(str.begin(), current);
	res = std::stod(str.substr(distance), &next_value);
	return (next_value ? next_value - 1 : next_value);
}

class Lexer
{
public:
	explicit	Lexer(std::string &expression) :line_str(expression), token()
	{
		if (expression.length() <= 0)
			throw std::runtime_error("the str is empty");
	}
	std::list<t_tokens>	getToken()
	{
		double				 	tmp_value;
		std::list<t_tokens>::iterator	last;

		for (std::string::iterator current = this->line_str.begin(); current != this->line_str.end(); current++)
		{
			last = this->token.end();
			last--;
			if (*current == '+')
				this->token.push_back({ADD, 0});
			else if (*current == '-')
			{
				if (current + 1 != line_str.end() && isdigit(*(current + 1)) != 0 && (last->type != NUMBER || this->token.size() == 0) )
				{
					current += ft_atoi(tmp_value, line_str, current);
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
				this->token.push_back({LBRACE, 0});
			else if (*current == ')')
				this->token.push_back({RBRACE, 0});
			else if (isdigit(*current) != 0)
			{
				current += ft_atoi(tmp_value, line_str, current);
				this->token.push_back({NUMBER, tmp_value});
			}
		}
		return (this->token);
	}

private:
	std::string			line_str;
	std::list<t_tokens>	token;
};

class AST
{
public:
	virtual ~AST() = default;
	virtual double	eval() = 0;
};

class	AST_LEAF : public AST
{
public:
	~AST_LEAF() override = default ;
	explicit AST_LEAF(double value) :_value(value) {};
	double	eval() override
	{
		return (this->_value);
	}
private:
	double _value;
};

class	AST_PLUS : public AST
{
public:
	~AST_PLUS() override
	{
		delete _left;
		delete _right;
	}
	AST_PLUS(AST *left, AST *right) :_left(left), _right(right) {};
	double eval() override
	{
		return (_left->eval() + _right->eval());
	}
private:
	AST	*_left;
	AST	*_right;
};

class	AST_MINUS : public AST
{
public:
	~AST_MINUS() override
	{
		delete _left;
		delete _right;
	}
	AST_MINUS(AST *left, AST *right) :_left(left), _right(right) {};
	double eval() override
	{
		return (_left->eval() - _right->eval());
	}
private:
	AST *_left;
	AST *_right;
};

class	AST_MULTIPLY : public AST
{
public:
	~AST_MULTIPLY() override
	{
		delete _left;
		delete _right;
	}
	AST_MULTIPLY(AST *left, AST *right) :_left(left), _right(right) {};
	double eval() override
	{
		return (_left->eval() * _right->eval());
	}
private:
	AST *_left;
	AST *_right;
};


class	AST_DIVISION : public AST
{
public:
	~AST_DIVISION() override
	{
		delete _left;
		delete _right;
	}
	AST_DIVISION(AST *left, AST *right) :_left(left), _right(right) {};
	double eval() override
	{
		return (_left->eval() / _right->eval());
	}
private:
	AST *_left;
	AST *_right;
};

class Parser
{
public:
	explicit Parser(std::list<t_tokens> &tokens) :_current(tokens.begin()), _end(tokens.end()) {};

	~Parser()
	{
	//	delete res;
	}
	AST *parseExp()
	{
		AST	*res;
		AST	*right_node;

		res = factor();
		while (_current != _end && (_current->type == ADD || _current->type == MINUS))
		{
			if (_current->type == ADD)
			{
				_current++;
				right_node = factor();
				res = new AST_PLUS(res, right_node);
			}
			else if (_current->type == MINUS)
			{
				_current++;
				right_node = factor();
				res = new AST_MINUS(res, right_node);
			}
		}
		return (res);
	}
	AST	*factor()
	{
		AST	*factor;
		AST	*right_node;

		factor = Term();
		while (_current != _end && (_current->type == MULTIPLY || _current->type == DIVISION))
		{
			if (_current->type == MULTIPLY)
			{
				_current++;
				right_node = Term();
				factor = new AST_MULTIPLY(factor, right_node);
			}
			else if (_current->type == DIVISION)
			{
				_current++;
				right_node = Term();
				factor = new AST_DIVISION(factor, right_node);
			}
		}
		return (factor);
	}
	AST	*Term()
	{
		std::list<t_tokens>::iterator	next;
		AST								*term;
		double							tmp;

		term = nullptr;
		next = _current;
		next++ ;
		if (_current->type == MINUS && next->type == LBRACE)
		{
			_current++;
			_current++;
			term = parseExp();
			if (_current->type != RBRACE)
				throw std::runtime_error("Missing brace ending");
			tmp = term->eval() * -1;
			delete term;
			term = new AST_LEAF(tmp);
		}
		else if (_current->type == LBRACE)
		{
			_current++;
			term = parseExp();
			if (_current->type != RBRACE)
				throw std::runtime_error("Missing brace ending");
			tmp = term->eval();
			delete term;
			term = new AST_LEAF(tmp);
		}
		else if (_current->type == NUMBER)
			term = new AST_LEAF(_current->value);
		_current++;
		return (term);
	}
private:
	std::list<t_tokens>::iterator	_current;
	std::list<t_tokens>::iterator	_end;
};

double calc(std::string &expression)
{
	Lexer 				lexer(expression);
	std::list<t_tokens>	tokens;
	AST					*value;
	double 				res;

	tokens = lexer.getToken();
	//print_current_token(tokens);
	Parser	parser(tokens);
	value = parser.parseExp();
	res = value->eval();
	return (res);
}

int	main(void)
{
	std::string	current;
	double		res;

	while (1)
	{
		try
		{
			std::cout << "Math expr >" << std::endl;
			getline(std::cin, current);
			res = calc(current);
			std::cout << "res =>" << res << std::endl;
		}
		catch (std::exception const &e)
		{
			std::cout << "[ERROR]\t" << e.what() << std::endl;
		}
	}
	return (0);
}
/*
 * 1 + 1
8/16
3 -(-1)
 */