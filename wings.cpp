#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>
#include <fstream>

//Token types
#define INTEGER "INTEGER"
#define STRING "STRING"
#define PLUS "PLUS"
#define MINUS "MINUS"
#define MUL "MUL"
#define DIV "DIV"
#define MOD "MOD"
#define LPAREN "LPAREN"
#define RPAREN "RPAREN"
#define SEMI "SEMI"
#define ASSIGN "ASSIGN"
#define VARIABLE "VARIABLE"
#define STATEMENT_LIST "STATEMENT_LIST"
#define WRITE "write"
#define READ "read"
#define EQUALITY "EQUALITY"
#define IF "if"
#define LBRACKET "LBRACKET"
#define RBRACKET "RBRACKET"
#define LESSTHAN "LESSTHAN"
#define MORETHAN "MORETHAN"
#define EMPTY "EMPTY"
#define ELSE "else"
#define WHILE "while"


std::set< std::string > RESERVED_KEYWORDS;
std::map<std::string, std::string> GLOBAL_SCOPE;
std::map<std::string, std::string> VARIABLE_TYPES;


class Token
{
private:
	//token type: eg: int, plus, EOF, etc
	std::string type;
	//token value: eg: 0, 1, '+', 'EOF'
	std::string value;

public:
	Token()
	{
		type = "";
		value = "";
	}

	Token(std::string _type, std::string _value)
	{
		type = _type;
		value = _value;
	}

	~Token(){};

	//String representation of class instance
	// eg:  Token(int,3)    Token(plus,'+')
	std::string str()
	{
		return ("Token("+type+","+value+")");
	}

	//returns value of this token
	std::string _value()
	{
		return value;
	}
	//returns type of this token
	std::string _type()
	{
		return type;
	}
};








class Lexer
{
private:
	//Code in bird
	std::string text;
	//current char at text[pos]
	char current_char;
	//current index of text to be tokenized
	int pos;

public:
	Lexer()
	{
		text = "";
		pos = 0;
		current_char = EOF;
	}
	Lexer(std::string _text)
	{
		text = _text;
		pos = 0;
		current_char = text[pos];
	}
	~Lexer(){};

	void error()
	{
		std::cout<<"Lexer Error:\n";
		std::cout<<"pos = "<<pos<<"   text[pos] = "<<text[pos]<<"\n";
		std::exit(0);
	}

	//Ignores whitespaces and put pos pointer to a non-whitespace character
	void skip_whitespaces();
	//Ignores comments
	void skip_comments();
	//Advances position of pos pointer in text, and check if EOF reached
	void advance_pos();
	//returns a multi-digit integer in form of a string starting at pos, and shift pos to next pointer
	std::string integer();
	//returns a string which is present under double quotes in bird
	std::string get_string();
	//get arguments for write()
	void write_arg();
	//Most important function in Lexical analyzer.
	//This method breaks the text into tokens, and returns a token starting at pos
	Token get_next_token();
	char peek();
	Token _id();
};

void Lexer::skip_whitespaces()
{
	//as the name suggests, skip all whitespaces till any new character is encountered
	while(text[pos] == ' ' or text[pos] == '\t' or text[pos] == '\n')
	{
		advance_pos();
	}
}

void Lexer::skip_comments()
{
	advance_pos();
	advance_pos();
	while((text[pos] != '/' or peek() != '/') and text[pos] != EOF)
	{
		advance_pos();
	}
	advance_pos();
	advance_pos();
}

void Lexer::advance_pos()
{
	pos++;
	//if pos past the end of text, i.e tokenization is completed
	//return EOF token
	if(pos >= text.length())
		current_char = EOF;
	else current_char = text[pos];
}

std::string Lexer::integer()
{
	std::string str;
	while(current_char>=48 and current_char<=57)
	{
		str.push_back(current_char);
		advance_pos();
	}
	// std::cout<<"Lexer: integer():  str = "<<str<<"   current_char = "<<current_char<<"\n";
	if(current_char == '.')
	{
		str.push_back(current_char);
		advance_pos();
		while(current_char>=48 and current_char<=57)
		{
			str.push_back(current_char);
			advance_pos();
		}
		// std::cout<<"Double value here  str = "<<str<<"\n";		
	}
	return str;
}

std::string Lexer::get_string()
{
	// std::cout<<"get_string()\n";
	std::string str;
	advance_pos();
	while(current_char != '\"')
	{
		// std::cout<<"current_char = "<<current_char<<"\n";
		str.push_back(current_char);
		advance_pos();
	}
	advance_pos();
	return str;
}

//Only peeks into input buffer without actually consuming the next character
char Lexer::peek()
{
	//if peek_pos past the end of text, return EOF
	if(pos+1 >= text.length())
		return EOF;
	else return text[pos+1];
}

Token Lexer::_id()
{
	std::string result;
	Token token;
	while((current_char >= 48 and current_char <= 57) or (current_char >= 65 and current_char <=90) or current_char >= 97 and current_char <=122)
	{
		result.push_back(current_char);
		advance_pos();
	}
	auto it = RESERVED_KEYWORDS.find(result);
	if(it == RESERVED_KEYWORDS.end())
		token = Token(VARIABLE, result);
	else token = Token(result, result);
	return token;
}

Token Lexer::get_next_token()
{
	// std::cout<<"get_next_token called\n";

	//ignore whitespaces
	skip_whitespaces();

	//skip comments
	if(current_char == '/' and peek() == '/')
	{
		// std::cout<<"comment aaya    current_char = "<<current_char<<"   pos = "<<pos<<"\n";
		skip_comments();
		// std::cout<<"comment gone    current_char = "<<current_char<<"   pos = "<<pos<<"\n";
	}

	std::string temp_str;

	//if current_char is an integer, create an integer token and increment pos
	if(current_char >= 48 and current_char <=57)
	{
		// std::cout<<"yup int here\n";
		temp_str = integer();//remember integer() returned multi-digit interger in form of a string
		return Token(INTEGER, temp_str);
	}

	//if token is + operator
	if(current_char == '+')
	{
		// std::cout<<"plus here\n";
		temp_str.push_back(current_char);
		advance_pos();
		return Token(PLUS, temp_str);
	}

	//if token is - operator
	if(current_char == '-')
	{
		// std::cout<<"minus here\n";
		temp_str.push_back(current_char);
		advance_pos();
		return Token(MINUS, temp_str);
	}

	//if token is * operator
	if(current_char == '*')
	{
		// std::cout<<"mul here\n";
		temp_str.push_back(current_char);
		advance_pos();
		return Token(MUL, temp_str);
	}

	//if token is / operator
	if(current_char == '/')
	{
		// std::cout<<"div here\n";
		temp_str.push_back(current_char);
		advance_pos();
		return Token(MUL, temp_str);
	}

	//if token is % operator
	if(current_char == '%')
	{
		// std::cout<<"mod here\n";
		temp_str.push_back(current_char);
		advance_pos();
		return Token(MOD, temp_str);
	}

	//if token is ( left parenthesis
	if(current_char == '(')
	{
		// std::cout<<"lparen here\n";
		temp_str.push_back(current_char);
		advance_pos();
		return Token(LPAREN, temp_str);
	}

	//if token is ) right paranthesis
	if(current_char == ')')
	{
		// std::cout<<"RPAREN here\n";
		temp_str.push_back(current_char);
		advance_pos();
		return Token(RPAREN, temp_str);
	}

	//EOF reached
	if(current_char == EOF)
	{
		temp_str.push_back(current_char);
		// std::cout<<"File ends here\n";
		return Token("EOF", temp_str);//some work needs to be done here ;p
	}

	//indentifier: some variable or a keyword
	if((current_char >= 65 and current_char <=90) or (current_char >= 97 and current_char <=122))
		return _id();

	if(current_char == '=' and peek() == '=')
	{
		advance_pos();
		advance_pos();
		return Token(EQUALITY, "==");
	}

	if(current_char == '<')
	{
		advance_pos();
		return Token(LESSTHAN, "<");
	}

	if(current_char == '>')
	{
		advance_pos();
		return Token(MORETHAN, ">");
	}

	//Assignment operator
	if(current_char == '=' and peek() != '=')
	{
		Token token(ASSIGN, "=");
		advance_pos();
		return token;
	}

	if(current_char == ';')
	{
		advance_pos();
		return Token(SEMI, ";");
	}

	if(current_char == '{')
	{
		advance_pos();
		return Token(LBRACKET, "{");
	}

	if(current_char == '}')
	{
		advance_pos();
		return Token(RBRACKET, "}");
	}

	if(current_char == '\"')
	{
		temp_str = get_string();
		return Token(STRING, temp_str);
	}

	error();
}















//PARSER

class ASTNode
{
private:
	Token token;
public:
	std::vector< ASTNode* > child;
	ASTNode(){};
	ASTNode(Token _token)
	{
		token = _token;
	}
	~ASTNode(){};
	void make_child(ASTNode _node)
	{
		ASTNode *temp = new ASTNode(_node._token());
		temp->child = _node.child;
		child.push_back(temp);
	}
	Token _token()
	{
		return token;
	}
	//Method to print AST, AST means level of node, root is at level zero
	void show(int level)
	{
		std::cout<<"Token: level = "<<level<<"  type = "<<(token._type())<<"  value = "<<(token._value())<<std::endl;
		for(auto it = child.begin(); it != child.end(); it++)
			(*it)->show(level+1);
	}
};


class Parser
{
private:
	Lexer lexer;
	Token current_token;
public:
	Parser(){};
	Parser(Lexer _lexer)
	{
		lexer = _lexer;
		current_token = lexer.get_next_token();
	}
	~Parser(){};


	// compare the current token type with the passed token
    // type and if they match then "eat" the current token
    // and assign the next token to the self.current_token,
    // otherwise raise an exception.
	void eat(std::string token_type);
	void error()
	{
		std::cout<<"Parser Error:\n";
		std::cout<<"current_token = "<<(current_token.str())<<"\n";
		std::exit(0);
	}

	//Arithmatic expression parser/interpreter
	//factor : (PLUS|MINUS)factor | INTEGER | LPAREN expr RPAREN
	ASTNode factor();
	//term : PLUS factor | MINUS factor | INTEGER | LPAREN exor RPAREN | variable
	ASTNode term();
	//expr : term((PLUS | MINUS) term)*
	ASTNode expr();
	//assignment_statement : variable ASSIGN expr
	ASTNode assignment_statement();
	//statement : assignment_statement | empty
	ASTNode statement();
	//variable : ID
	ASTNode variable();
	//statement_list : statement | statement ENDL statement_list
	ASTNode statement_list();
	//empty statement
	// ASTNode empty();
	//write() arguments list
	// write : write LPAREN variable RPAREN
	ASTNode write();
	//read() takes only one argument, takes input from console and store it in that variable
	//read : read LPAREN expr() RPAREN
	ASTNode read();
	//conditional operator : left OPERATOR right
	ASTNode conditional();
	//if : IF LPAREN Conditional RPAREN ( ( LBACKET statement_list RBRACKET ) | statement ) (EMPTY | ELSE ( ( LBACKET statement_list RBRACKET ) | statement ) )
	ASTNode If();
	//while : WHILE LPAREN conditional RPAREN ( ( LBACKET statement_list RBRACKET ) | statement )
	ASTNode While();

	ASTNode parse()
	{
		ASTNode node = statement_list();
		// std::cout<<"AST is as follows:\n";
		// node.show(0);
		return node;
	}	
};

void Parser::eat(std::string token_type)
{
	// std::cout<<"Eat called\n";
	if(current_token._type() == token_type)
		current_token = lexer.get_next_token();
	else
	{
		std::cout<<"Error when eat() called\n";
		std::cout<<"current_token.type = "<<current_token._type()<<"     token_type = "<<token_type<<"\n";
		error();
	}
}


ASTNode Parser::statement()
{
	// std::cout<<"statement() called\n";
	ASTNode node;
	if(current_token._type() == VARIABLE)
	{
		// std::cout<<"assignment_statement h ye toh\n";
		node = assignment_statement();
		eat(SEMI);
	}
	else if(current_token._type() == WRITE)
	{
		node = write();
		eat(SEMI);
	}
	else if(current_token._type() == READ)
	{
		node = read();
		eat(SEMI);
	}
	else if(current_token._type() == IF)
		node = If();
	else if(current_token._type() == WHILE)
		node = While();
	// else node = empty();
	else error();
	return node;
}

ASTNode Parser::assignment_statement()
{
	// std::cout<<"assignment_statement() called\n";
	ASTNode left = variable();
	eat(ASSIGN);
	ASTNode right = expr();
	ASTNode node(Token(ASSIGN, "="));
	// node.show(0);
	node.make_child(left);
	node.make_child(right);
	// node.show(0);
	return node;
}

ASTNode Parser::variable()
{
	ASTNode node(current_token);
	eat(VARIABLE);
	return node;
}

// ASTNode Parser::empty()
// {
// 	ASTNode node(current_token);
// 	return node;
// }

ASTNode Parser::statement_list()
{
	ASTNode node(Token("STATEMENT_LIST", "STATEMENT_LIST"));
	// node.make_child(statement());
	// std::cout<<"statement_list called node is\n";
	// node.show(0);
	// while(current_token._type() == "EOF")
	// {
	// 	eat(SEMI);
	// 	if(current_token._type() != "EOF")
	// 		node.make_child(statement());
	// 	else std::cout<<"Program parsed\n";

	// }
	while(current_token._type() != "EOF" and current_token._type() != RBRACKET)
	{
		node.make_child(statement());
	}
	return node;
}

ASTNode Parser::factor()
{
	if(current_token._type() == INTEGER)
	{
		ASTNode node(current_token);
		eat(INTEGER);
		return node;
	}
	else if(current_token._type() == LPAREN)
	{
		eat(LPAREN);
		ASTNode node = expr();
		eat(RPAREN);
		return node;
	}
	else if(current_token._type() == PLUS)
	{
		ASTNode node(current_token);
		eat(PLUS);
		node.make_child(factor());
		return node;
	}
	else if(current_token._type() == MINUS)
	{
		ASTNode node(current_token);
		eat(MINUS);
		node.make_child(factor());
		return node;
	}
	else if(current_token._type() == VARIABLE)
	{
		ASTNode node(variable());
		return node;
	}
	else if(current_token._type() == STRING)
	{
		ASTNode node(current_token);
		eat(STRING);
		return node;
	}
	else error();
}

ASTNode Parser::term()
{
	// std::cout<<"term called\n";
	ASTNode node = factor();
	while(true)
	{		
		ASTNode temp;
		if(current_token._type() == MUL)
		{
			eat(MUL);
			temp = ASTNode(Token(MUL, "*"));
		}
		else if(current_token._type() == DIV)
		{
			eat(DIV);
			temp = ASTNode(Token(DIV, "/"));
		}
		else if(current_token._type() == MOD)
		{
			eat(MOD);
			temp = ASTNode(Token(MOD, "%"));
		}
		else break;
		//child[1] holds value to be multiplied/divided from child[0]
		temp.make_child(node);
		temp.make_child(factor());
		node = temp;
	}
	return node;
}

ASTNode Parser::expr()
{
	ASTNode node = term();
	while(true)
	{
		ASTNode temp;
		if(current_token._type() == PLUS)
		{
			eat(PLUS);
			temp = ASTNode(Token(PLUS, "+"));
		}
		else if(current_token._type() == MINUS)
		{
			eat(MINUS);
			temp = ASTNode(Token(MINUS, "-"));
		}
		else break;
		//child[1] holds value to be added/subtracted from child[0]
		temp.make_child(node);
		temp.make_child(term());
		node = temp;
	}
	return node;
}

ASTNode Parser::write()
{
	eat(WRITE);
	eat(LPAREN);
	ASTNode node(Token(WRITE, WRITE));
	node.make_child(expr()); //child[0] holds expr() to be printed
	eat(RPAREN);
	return node;
}

ASTNode Parser::read()
{
	eat(READ);
	eat(LPAREN);
	ASTNode node(Token(READ, READ));
	node.make_child(variable()); //child[0] holds variable whose value is to be taken as input
	eat(RPAREN);
	return node;
}

ASTNode Parser::If()
{
	//child[0] holds conditional statement, and child[1] holds statement_list to be executed if conditional returns true
	//child[2] stores ELSE part
	eat(IF);
	eat(LPAREN);
	ASTNode node(Token(IF, IF));
	node.make_child(conditional());
	eat(RPAREN);
	if(current_token._type() == LBRACKET)
	{
		eat(LBRACKET);
		node.make_child(statement_list());
		eat(RBRACKET);
	}
	else node.make_child(statement());
	//if ELSE part is present, then make it child, otherwise make empty statement as ELSE part
	if(current_token._type() == ELSE)
	{
		eat(ELSE);
		if(current_token._type() == LBRACKET)
		{
			eat(LBRACKET);
			node.make_child(statement_list());
			eat(RBRACKET);
		}
		else node.make_child(statement());
	}
	else node.make_child(Token(EMPTY, EMPTY));
	return node;
}

ASTNode Parser::conditional()
{
	//child[0] holds Left Hand Side of conditional
	//child[1] holds Right Hand Side of conditional
	ASTNode node;
	ASTNode left = expr();
	if(current_token._type() == EQUALITY)
	{
		node = Token(EQUALITY, EQUALITY);
		eat(EQUALITY);
	}
	else if(current_token._type() == LESSTHAN)
	{
		node = Token(LESSTHAN, LESSTHAN);
		eat(LESSTHAN);
	}
	else if(current_token._type() == MORETHAN)
	{
		node = Token(MORETHAN, MORETHAN);
		eat(MORETHAN);
	}
	else error();
	node.make_child(left);
	node.make_child(expr());
	return node;
}

ASTNode Parser::While()
{
	//child[0] holds conditional statement, and child[1] holds statement_list to be executed while conditional returns true
	eat(WHILE);
	eat(LPAREN);
	ASTNode node(Token(WHILE, WHILE));
	node.make_child(conditional());
	eat(RPAREN);
	if(current_token._type() == LBRACKET)
	{
		eat(LBRACKET);
		node.make_child(statement_list());
		eat(RBRACKET);
	}
	else node.make_child(statement());
	return node;
}








//Interpreter




class Interpreter
{
private:
	Parser parser;
	
public:
	Interpreter(Parser _parser)
	{
		parser = _parser;
	}
	~Interpreter(){};
	
	Token visit(ASTNode node)
	{
		// std::cout<<"visit called node.type = "<<(node._token()._type())<<"   node.value = "<<(node._token()._value())<<std::endl;
		

		std::string result;

		if(node._token()._type() == STATEMENT_LIST)
		{
			for(auto it = node.child.begin(); it != node.child.end(); it++)
			{
				Token token = visit(**it);
				if(token._value() != "0")
					error("Failed to interpret statement:  type = "+token._type()+"  value = "+token._value());
			}
			return (Token(INTEGER, "0"));
		}

		if(node._token()._type() == ASSIGN)
		{
			Token token = visit(*node.child[1]);
			GLOBAL_SCOPE[node.child[0]->_token()._value()] = token._value();
			VARIABLE_TYPES[node.child[0]->_token()._value()] = token._type();
			return (Token(INTEGER, "0"));
		}

		if(node._token()._type() == VARIABLE)
		{
			auto it = GLOBAL_SCOPE.find(node._token()._value());
			if(it == GLOBAL_SCOPE.end())
				error("Garbage value in "+node._token()._value());
			auto i = VARIABLE_TYPES.find(node._token()._value());
			if(i == GLOBAL_SCOPE.end())
				error("Error while determining datatype of "+node._token()._value());
			if(i->second == INTEGER)
				return Token(INTEGER, it->second);
			else if(i->second == STRING)
				return Token(STRING, it->second);
		}

		if(node._token()._type() == INTEGER)
		{
			return node._token();
		}

		if(node._token()._type() == STRING)
		{
			return node._token();
		}

		//Unary plus or binary plus
		if(node._token()._type() == PLUS)
		{

			//Unary Plus: Applicable for both integer addition and string concatanation
			if(node.child.size() == 1)
			{
				return visit(*(node.child[0]));
			}

			Token temp1, temp2;
			temp1 = visit(*(node.child[0]));
			temp2 = visit(*(node.child[1]));

			//if any of child is string, then do simple string concatanation
			if(temp1._type() == STRING or temp2._type() == STRING)
			{
				result = temp1._value() + temp2._value();
				return Token(STRING, result);
			}
			//INTEGER ADDITION
			else
			{
				std::string str1 = temp1._value();
				std::string str2 = temp2._value();
				//If both operands are integer only then return without decimals
				if(str1.find('.') == std::string::npos and str2.find('.') == std::string::npos)
					result = std::to_string(std::stoi(temp1._value()) + std::stoi(temp2._value()));
				//if any of th operands is double type, then return with decimals
				else result = std::to_string(std::stod(temp1._value()) + std::stod(temp2._value()));
				return Token(INTEGER, result);				
			}
		}
		
		//Unary minus or binary minus
		if(node._token()._type() == MINUS)
		{
			//Unary Minus
			if(node.child.size() == 1)
			{
				Token temp = visit(*(node.child[0]));
				std::string str = temp._value();
				if(str.find('.') == std::string::npos)
					result = std::to_string( - std::stoi(temp._value()));
				else result = std::to_string( - std::stod(temp._value()));
				return Token(INTEGER, result);
			}
			//Binary Minus
			else
			{				
				Token temp1, temp2;
				temp1 = visit(*(node.child[0]));
				temp2 = visit(*(node.child[1]));
				std::string str1 = temp1._value();
				std::string str2 = temp2._value();
				//If both operands are integer only then return without decimals
				if(str1.find('.') == std::string::npos and str2.find('.') == std::string::npos)
					result = std::to_string(std::stoi(temp1._value()) - std::stoi(temp2._value()));
				//if any of th operands is double type, then return wiht decimals
				else result = std::to_string(std::stod(temp1._value()) - std::stod(temp2._value()));
				return Token(INTEGER, result);
			}
		}

		if(node._token()._type() == MUL)
		{
			Token temp1, temp2;
			temp1 = visit(*(node.child[0]));
			temp2 = visit(*(node.child[1]));
			std::string str1 = temp1._value();
			std::string str2 = temp2._value();
			//If both operands are integer only then do integer multiplication and return without decimals
			if(str1.find('.') == std::string::npos and str2.find('.') == std::string::npos)
				result = std::to_string(std::stoi(temp1._value()) * std::stoi(temp2._value()));
			//if any of th operands is double type, then return with decimal digits
			else result = std::to_string(std::stod(temp1._value()) * std::stod(temp2._value()));
			return Token(INTEGER, result);
		}
		
		if(node._token()._type() == DIV)
		{
			Token temp1, temp2;
			temp1 = visit(*(node.child[0]));
			temp2 = visit(*(node.child[1]));
			std::string str1 = temp1._value();
			std::string str2 = temp2._value();
			//If both operands are integer only then do integer division and return quotient of division
			if(str1.find('.') == std::string::npos and str2.find('.') == std::string::npos)
				result = std::to_string(std::stoi(temp1._value()) / std::stoi(temp2._value()));
			//if any of th operands is double type, do double division
			else result = std::to_string(std::stod(temp1._value()) / std::stod(temp2._value()));
			return Token(INTEGER, result);
		}
		
		if(node._token()._type() == MOD)
		{
			Token temp1, temp2;
			temp1 = visit(*(node.child[0]));
			temp2 = visit(*(node.child[1]));
			result = std::to_string(std::stoi(temp1._value()) % std::stoi(temp2._value()));
			return Token(INTEGER, result);
		}

		if(node._token()._type() == WRITE)
		{
			Token token = visit(*node.child[0]);
			std::cout<<token._value();
			return Token(INTEGER, "0");
		}

		if(node._token()._type() == READ)
		{
			std::string str, var;
			int dec_count = 0; //to count number of dots '.' in input
			var = node.child[0]->_token()._value(); //var stores name of variable
			std::getline(std::cin, str); //str stores value of var
			GLOBAL_SCOPE[var] = str;
			//flag = true means input is string type, flag = false means input is integer type
			bool flag = false;
			for(auto it = str.begin(); it != str.end(); it++)
			{
				if((*it < 48 or *it > 57) and *it != '.')
					flag = true;
				if(*it == '.')
					dec_count++;
			}
			//One decimal is allowed in integer, two decimal points means STRING
			if(dec_count > 1)
				flag = true;
			auto it = VARIABLE_TYPES.find(var);
			//if input is a string, no matter what was earlier datatype, new datatype of var will be STRING
			if(flag)
				VARIABLE_TYPES[var] = STRING;
			//input is a number and initially var was not used or has INTEGER datatype
			else if(it == VARIABLE_TYPES.end() or it->second == INTEGER)
				VARIABLE_TYPES[var] = INTEGER;
			else VARIABLE_TYPES[var] = STRING;
			return Token(INTEGER, "0");
		}

		if(node._token()._type() == IF)
		{
			Token token = visit(*node.child[0]);
			Token temp;
			if(token._value() == "1") //if conditional() returns 1(means TRUE) then execute statement_list, else do nothing
				temp = visit(*node.child[1]);
			else temp = visit(*node.child[2]); //else part
			return temp;
		}

		if(node._token()._type() == EQUALITY)
		{
			Token token0 = visit(*node.child[0]);
			Token token1 = visit(*node.child[1]);
			if(token0._value() == token1._value() and token0._type() == token1._type())
				return Token(INTEGER, "1");
			else return Token(INTEGER, "0");
		}

		if(node._token()._type() == LESSTHAN)
		{
			Token token0 = visit(*node.child[0]);
			Token token1 = visit(*node.child[1]);
			if(token0._type() != token1._type())
			{
				error("Comparing different datatypes");
			}
			else if(token0._type() == INTEGER and std::stod(token0._value()) < std::stod(token1._value()))
				return Token(INTEGER, "1");
			else if(token0._type() == STRING and token0._value() < token1._value())
				return Token(INTEGER, "1");
			else return Token(INTEGER, "0");
		}

		if(node._token()._type() == MORETHAN)
		{
			Token token0 = visit(*node.child[0]);
			Token token1 = visit(*node.child[1]);
			if(token0._type() != token1._type())
			{
				error("Comparing different datatypes");
			}
			else if(token0._type() == INTEGER and std::stod(token0._value()) > std::stod(token1._value()))
				return Token(INTEGER, "1");
			else if(token0._type() == STRING and token0._value() > token1._value())
				return Token(INTEGER, "1");
			else return Token(INTEGER, "0");
		}

		if(node._token()._type() == EMPTY)
		{
			return Token(INTEGER, "0");
		}

		if(node._token()._type() == WHILE)
		{
			Token token = visit(*node.child[0]);
			Token temp;
			while(token._value() == "1") //if conditional() returns 1(means TRUE) then execute statement_list, else do nothing
			{
				temp = visit(*node.child[1]);
				token = visit(*node.child[0]);
			}
			return temp;
		}
	}

	Token interpret()
	{
		ASTNode tree = parser.parse();
		// return tree._token();
		return visit(tree);
	}

	void error(std::string str)
	{
		std::cout<<"Interpreter error: "<<str<<"\n";
		exit(0);
	}
};





int main(int argc, char const *argv[])
{

	RESERVED_KEYWORDS.insert("write");
	RESERVED_KEYWORDS.insert("read");
	RESERVED_KEYWORDS.insert("if");
	RESERVED_KEYWORDS.insert("else");
	RESERVED_KEYWORDS.insert("while");


	if(argc < 2)
	{
		std::cout<<"Please Enter name of file as commandline argument\n";
		exit(0);
	}

	std::string text, temp;
	std::ifstream file;
	file.open(argv[1]);

	while(std::getline(file, temp))
	{
		text += temp;
	}
	file.close();
	Lexer lexer(text);
	Parser parser(lexer);
	Interpreter interpreter(parser);
	
	if(argc == 3)
		temp = argv[2]; //to create C++ string class object, easier to compare
	else temp = "devrari"; //Why use garbage, when you can use your name :p
	//Show internal details
	if(temp == "details")
	{
		ASTNode tree = parser.parse();
		std::cout<<"AST :\n";
		tree.show(0);
		std::cout<<"Starts Interpreting\n***************************************************\n";
		Token result = interpreter.interpret();
		std::cout<<"\n***************************************************\nStops Interpreting\n\n";
		std::cout<<"Status: "<<((result._value() == "0")?"Success\n":"Failed\n\n");		
		std::cout<<"GLOBAL_SCOPE:\n";
		for(auto it = GLOBAL_SCOPE.begin(); it!= GLOBAL_SCOPE.end(); it++)
		{
			std::cout<<it->first<<"  "<<it->second<<"\n";
		}
		std::cout<<"VARIABLE_TYPES:\n";
		for(auto it = VARIABLE_TYPES.begin(); it!= VARIABLE_TYPES.end(); it++)
		{
			std::cout<<it->first<<"  "<<it->second<<"\n";
		}		
	}
	//only runs code
	else
	{
		Token result = interpreter.interpret();
	}
	return 0;
}