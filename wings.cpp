#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>

//Token types
#define INTEGER "INTEGER"
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


std::set< std::string > RESERVED_KEYWORDS;
std::map<std::string, std::string> GLOBAL_SCOPE;


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
	int pos, peek_pos;

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
	//Advances position of pos pointer in text, and check if EOF reached
	void advance_pos();
	//returns a multi-digit integer in form of a string starting at pos, and shift pos to next pointer
	std::string integer();
	//Most important function in Lexical analyzer.
	//This method breaks the text into tokens, and returns a token starting at pos
	Token get_next_token();
	char peek();
	Token _id();
};

void Lexer::skip_whitespaces()
{
	//as the name suggests, skip all whitespaces till any new character is encountered
	while(text[pos] == ' ')
	{
		advance_pos();
	}
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
	return str;
}

//Only peeks into input buffer without actually consuming the next character
char Lexer::peek()
{
	peek_pos++;
	//if peek_pos past the end of text, return EOF
	if(peek_pos >= text.length())
		return EOF;
	else return text[peek_pos];
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

	std::string temp_str;

	//if current_char is an integer, create an integer token and increment pos
	if(current_char >= 48 and current_char <=57)
	{
		// std::cout<<"yup int here\n";
		temp_str = integer();//remember integer() returned multi-digit interger in form of a string
		Token token(INTEGER, temp_str);
		return token;
	}

	//if token is + operator
	if(current_char == '+')
	{
		// std::cout<<"plus here\n";
		temp_str.push_back(current_char);
		Token token(PLUS, temp_str);
		advance_pos();
		return token;
	}

	//if token is - operator
	if(current_char == '-')
	{
		// std::cout<<"minus here\n";
		temp_str.push_back(current_char);
		Token token(MINUS, temp_str);
		advance_pos();
		return token;
	}

	//if token is * operator
	if(current_char == '*')
	{
		// std::cout<<"mul here\n";
		temp_str.push_back(current_char);
		Token token(MUL, temp_str);
		advance_pos();
		return token;
	}

	//if token is / operator
	if(current_char == '/')
	{
		// std::cout<<"div here\n";
		temp_str.push_back(current_char);
		Token token(DIV, temp_str);
		advance_pos();
		return token;
	}

	//if token is % operator
	if(current_char == '%')
	{
		// std::cout<<"mod here\n";
		temp_str.push_back(current_char);
		Token token(MOD, temp_str);
		advance_pos();
		return token;
	}

	//if token is ( left parenthesis
	if(current_char == '(')
	{
		// std::cout<<"lparen here\n";
		temp_str.push_back(current_char);
		Token token(LPAREN, temp_str);
		advance_pos();
		return token;
	}

	//if token is ) right paranthesis
	if(current_char == ')')
	{
		// std::cout<<"RPAREN here\n";
		temp_str.push_back(current_char);
		Token token(RPAREN, temp_str);
		advance_pos();
		return token;
	}

	//EOF reached
	if(current_char == EOF)
	{
		temp_str.push_back(current_char);
		// std::cout<<"File ends here\n";
		Token token("EOF", temp_str);//some work needs to be done here ;p
		return token;
	}

	//indentifier: some variable or a keyword
	if((current_char >= 65 and current_char <=90) or (current_char >= 97 and current_char <=122))
		return _id();

	if(current_char == '=')
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

	ASTNode parse()
	{
		ASTNode node = statement_list();
		std::cout<<"AST is as follows:\n";
		node.show(0);
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
	ASTNode node;
	if(current_token._type() == VARIABLE)
		node = assignment_statement();
	// else node = empty();
	else error();
	return node;
}

ASTNode Parser::assignment_statement()
{
	ASTNode left = variable();
	eat(ASSIGN);
	ASTNode right = expr();
	ASTNode node(Token(ASSIGN, "="));
	node.make_child(left);
	node.make_child(right);
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
	while(current_token._type() != "EOF")
	{
		node.make_child(statement());
		eat(SEMI);
	}
	return node;
}

ASTNode Parser::factor()
{
	// std::cout<<"factor called  current_token._value = "<<current_token._value()<<"\n";
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
		temp.make_child(node);
		temp.make_child(factor());
		node = temp;
	}
	// std::cout<<"term returning\n";
	// node.show(0);
	return node;
}

ASTNode Parser::expr()
{
	// std::cout<<"expr called\n";
	ASTNode node = term();
	while(true)
	{
		// std::cout<<"in expr loop"<<std::endl;
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
		// std::cout<<"one"<<std::endl;
		temp.make_child(node);
		// std::cout<<"two"<<std::endl;
		temp.make_child(term());
		// std::cout<<"three"<<std::endl;
		node = temp;
		// temp.show();
		// std::cout<<"four"<<std::endl;
		// node.show();
	}
	// std::cout<<"expr returning"<<std::endl;
	// node.show(0);
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
			return (Token(INTEGER, "0"));
		}

		if(node._token()._type() == VARIABLE)
		{
			auto it = GLOBAL_SCOPE.find(node._token()._value());
			if(it == GLOBAL_SCOPE.end())
				error("Garbage value in "+node._token()._value());
			return Token(INTEGER, it->second);
		}

		if(node._token()._type() == INTEGER)
		{
			return node._token();
		}

		//Unary plus or binary plus
		if(node._token()._type() == PLUS)
		{
			//Unary Plus
			if(node.child.size() == 1)
			{
				return visit(*(node.child[0]));
			}
			//Binary Plus
			else
			{
				Token temp1, temp2;
				temp1 = visit(*(node.child[0]));
				temp2 = visit(*(node.child[1]));
				result = std::to_string(std::stoi(temp1._value()) + std::stoi(temp2._value()));
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
				result = std::to_string( - std::stoi(temp._value()));
				return Token(INTEGER, result);
			}
			//Binary Minus
			else
			{				
				Token temp1, temp2;
				temp1 = visit(*(node.child[0]));
				temp2 = visit(*(node.child[1]));
				result = std::to_string(std::stoi(temp1._value()) - std::stoi(temp2._value()));
				return Token(INTEGER, result);
			}
		}

		if(node._token()._type() == MUL)
		{
			Token temp1, temp2;
			temp1 = visit(*(node.child[0]));
			temp2 = visit(*(node.child[1]));
			result = std::to_string(std::stoi(temp1._value()) * std::stoi(temp2._value()));
			return Token(INTEGER, result);
		}
		
		if(node._token()._type() == DIV)
		{
			Token temp1, temp2;
			temp1 = visit(*(node.child[0]));
			temp2 = visit(*(node.child[1]));
			result = std::to_string(std::stoi(temp1._value()) / std::stoi(temp2._value()));
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
	GLOBAL_SCOPE["a"] = "devrari";
	std::string text;
	while(true)
	{
		std::cout<<"Enter input   ";
		std::getline(std::cin, text);
		Lexer lexer(text);
		Parser parser(lexer);
		Interpreter interpreter(parser);
		Token result = interpreter.interpret();
		if(result._value() == "0")
			std::cout<<"Interpreted Successfully :)\n";
		else std::cout<<"Non-zero return by Interpreter\n";
		std::cout<<"GLOBAL_SCOPE:\n";
		for(auto it = GLOBAL_SCOPE.begin(); it!= GLOBAL_SCOPE.end(); it++)
		{
			std::cout<<it->first<<"  "<<it->second<<"\n";
		}
		std::cout<<"\n\n";
	}
	return 0;
}