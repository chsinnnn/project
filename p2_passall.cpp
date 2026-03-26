#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>   // Env 會用
#include <map>
#include <set>
#include <cmath>
using namespace std;

int testnum;
int globalLine = 1, globalCol = 1;
bool errorInCurrentLine = false; // 記錄當前行是否發生錯誤
bool doterror = false;
int depth = 0;

enum class TokenType {
    LEFT_PAREN,   // '('
    RIGHT_PAREN,  // ')'
    INT,          // 整數
    FLOAT,        // 浮點數
    STRING,       // 字串 
    DOT,          // '.'
    NIL,          // 'nil' or '#f'
    T,            // 't' or '#t'
    QUOTE,        // 單引號 '''
    SYMBOL,       // 變數名稱
    ERROR,        // 錯誤 token
    END           // 輸入結束
};

enum class NodeType {
    ATOM,      // int、float、symbol、string
    LIST,      // (1 2 3)
    DOT_PAIR,  // Dotted Pair，(1 . 2)
    QUOTE,      // '(1 2 3)
    PROCEDURE
};

struct ASTNode {
    NodeType type;
    string value;               // 僅 ATOM 使用，例如 "123"、"abc"
    vector<ASTNode*> children;  // LIST 或 QUOTE

    ASTNode(NodeType t, const string &val = "") : type(t), value(val) {}
    ~ASTNode() {
        for (ASTNode* child : children) {
            delete child;
        }  // for()
    }  // ~ASTNode()
};
map<string, ASTNode*> userEnv; // 儲存使用者自定義的符號
const set<string> systemPrimitives = {"exit","eqv?", "equal?", "cons", "list", "quote", "+", "-", "*", "/",">","<",">=","<=","=","string-append","string>?","string<?","string=?", "define", "car", "cdr", "atom?", "pair?", "list?", "null?", "integer?", "real?", "number?", "string?", "boolean?", "symbol?", "if", "cond", "begin", "and", "or", "not", "clean-environment"};
struct Token {
    TokenType type;
    string value;
    int line;
    int col;
    int pos;
};

// 檢查字元是否為 token 分隔符：空白、括號、單引號、雙引號、分號
bool isSeparator(char ch) {
    return isspace(ch) || ch == '(' || ch == ')' || ch == '\'' || ch == '"' || ch == ';';
}  //  isSeparator

void printWithoutNewline(const string &s) {
	cout << s;
}  // printWithoutNewline

// prettyPrint 印出 S-expression
void prettyPrint(ASTNode* node, int currIndent = 0) {
	if (node->type == NodeType::PROCEDURE) {
        printWithoutNewline("#<procedure " + node->value + ">");
        return;
    }  // if
    if (node->type == NodeType::ATOM) {
        printWithoutNewline(node->value);
        return;
    }  // if
	if (node->type == NodeType::QUOTE) {
	    int M = currIndent;            // 當前已經印了多少空格
	    int newIndent = M + 2;         // 下一層的縮排
	
	    // 打開頭的 "("，接著空一格印 "quote"
	    printWithoutNewline("(");
	    printWithoutNewline(" quote");
	
	    // 換行，縮排到 newIndent，印出被 quote 的子節點
	    cout << "\n" << string(newIndent, ' ');
	    prettyPrint(node->children[0], newIndent);
	
	    // 再換行，把最外層的 ")" 對齊到 M
	    cout << "\n" << string(M, ' ') << ")";
	    return;
	}  // if 
    // 處理空 list，視為 nil
    if (node->type == NodeType::LIST && node->children.empty()) {
        printWithoutNewline("nil");
        return;
    }  // if

    int M = currIndent;
    printWithoutNewline("(");
    int newIndent = M + 2;

    
    vector<ASTNode*> elems;
    ASTNode* tail = nullptr;
    if (node->type == NodeType::DOT_PAIR) {
        ASTNode* curr = node;
        // 收集所有左側元素
        while (curr->type == NodeType::DOT_PAIR) {
            elems.push_back(curr->children[0]);
            curr = curr->children[1];
        }  // while
        // 如果右側是 LIST，就把它的 children 全部進 elems
        if (curr->type == NodeType::LIST) {
            for (ASTNode* c : curr->children)
                elems.push_back(c);
            
        }  // if
        else {
            // 只有右側不是 LIST，才當作真正的 tail
            tail = curr;
        }  // else
    }
    else if (node->type == NodeType::LIST) {
        elems = node->children;
    }  // else if

    // 印出 elems
    if (!elems.empty()) {
        printWithoutNewline(" ");
        prettyPrint(elems[0], newIndent);
        for (int i = 1; i < (int)elems.size(); i++) {
            cout << "\n" << string(newIndent, ' ');
            prettyPrint(elems[i], newIndent);
        }  // for
        // 如果有 tail（且不是 list），才印 '. tail'
        if (tail) {
            cout << "\n" << string(newIndent, ' ') << ".";
            cout << "\n" << string(newIndent, ' ');
            prettyPrint(tail, newIndent);
        }  // if
    }  // if

    // 關閉最外層的 ')'
    cout << "\n" << string(M, ' ') << ")";
}  // prettyprint

ASTNode* copyy(ASTNode* node) {
    if (!node) return nullptr;

    ASTNode* newNode = new ASTNode(node->type, node->value);
    for (ASTNode* child : node->children) {
        newNode->children.push_back(copyy(child));
    }  // for
    return newNode;
}  // copyy

class Lexer {
	private:
	    string input;
	    int pos;
	    int line, col;
	public:
		Lexer(const string &text, int initLine, int initCol) : input(text), pos(0), line(initLine), col(initCol) {}
		// 取得目前line col
	    int getLine() { 
			return line; 
		}  // getLine()
	    int getCol() { 
			return col; 
		}  // getCol()
		int getPos() { 
			return pos; 
		}  //  getPos()
	    Token getNextToken() {
	        while ( pos < input.size() ) {
	            char ch = input[pos];
//	            cout << input[pos] << " 8282\n";
	            if ( isspace(ch) ) {
	                if( ch == '\n' ){
	                	//col++;
	                    line++;
//	                    cout << globalLine << "  8484\n";
	                    col = 1;
	                }  // if
					else {
	                    col++;
//	                    globalLine++;
	                }  // else
	                pos++;
	                continue;
	            }  // if
	            if(ch == ';'){
				    // 跳過註解內容直到遇到換行
				    while(pos < input.size() && input[pos] != '\n')
				         pos++;
				    // 如果有換行符號增加一次行數
				    if(pos < input.size() && input[pos]=='\n'){
				         pos++;       
				         line++;      
				         col = 1;     
				    }  // if
				    continue;
				}  // if
	            if( ch == '(' )
	                return makeToken(TokenType::LEFT_PAREN, "(", pos++, col++);
	            if( ch == ')' )
	                return makeToken(TokenType::RIGHT_PAREN, ")", pos++, col++);
	            if( ch == '\'' )
	                return makeToken(TokenType::QUOTE, "'", pos++, col++);
	            if (ch == '.') {
				    bool prevch;  // 看前一個字元 
					if (pos == 0) {
					    // 在字串開頭
					    prevch = true;
					}  // if
					else {
					    // 非開頭，檢查前一個字元
					    if (isSeparator(input[pos-1])) 
					        prevch = true;
					    else
					        prevch = false;
					}  // else
					
					bool nextch;  // 看下一個字元 
					if (pos + 1 >= input.size()) 
					    nextch = true;  // 現在在結尾 
					else {
					    // 非結尾，檢查下一個字元
					    if (isSeparator(input[pos+1]))
					        nextch = true;
					    else
					        nextch = false;
					}  // else 
				
				    // FLOAT
				    if (prevch && pos+1 < input.size() && isdigit(input[pos+1]))
				        return readNumber();
				    // DOT
				    if (prevch && nextch) {
				        // 下一個字元
				        return makeToken(TokenType::DOT, ".", pos++, col++);
				    }  // if 
				    
				}  // if
	            if( isdigit(ch) || ch == '+' || ch == '-')
	                return readNumber();
	            if( ch == '"' )
	                return readString();
	            return readSymbol();
	        }  // while
	        return makeToken(TokenType::END, "", pos, col);
	    }  // getNextToken()
	private:
	    Token makeToken(TokenType type, string value, int position, int column) {
	        return {type, value, line, column, position};
	    }  // makeToken
	    // 讀取數字或候選數字序列
	    Token readNumber() {
	        int start = pos;
	        int startCol = col;
	        bool isFloat = false;
	        // 若開頭為 '+' 或 '-'，只接受一個，後面必須接 digit 或 '.'
	        if(pos < input.size() && (input[pos]=='+' || input[pos]=='-')) {
	            pos++; 
				col++;
            // 如果後面緊跟著 '.' 卻沒有 digit，則整體視為 symbol
	            if(pos < input.size() && input[pos]=='.') {
	                if(pos+1 >= input.size() || !isdigit(input[pos+1])) {
	                    pos = start; 
						col = startCol;
	                    return readSymbol();
	                }  // if
	            }  // if
	            else if(pos >= input.size() || (!isdigit(input[pos]) && input[pos] != '.')) {
	                pos = start; 
					col = startCol;
	                return readSymbol();
	            }  // else if
	        }  // if
	        // 檢查：如果接著是 '.' 而後面無 digit，則當作 symbol
	        if(pos < input.size() && input[pos]=='.') {
	            if(pos+1 >= input.size() || !isdigit(input[pos+1])) {
	                pos = start; 
					col = startCol;
	                return readSymbol();
	            }  // if
	        }  // if
	        // 讀取數字與點號
	        while(pos < input.size() && (isdigit(input[pos]) || input[pos] == '.')) {
	            if( input[pos] == '.' ){
	                if(isFloat) break; // 只允許一個小數點
	                isFloat = true;
	            }  // if
	            pos++; 
				col++;
	        }  // while
	        // 若接下來的字元存在且非分隔符，表示後續還屬於 symbol
	        if(pos < input.size() && !isSeparator(input[pos])) {
	            pos = start; 
				col = startCol;
	            return readSymbol();
	        }  // if
	        string number = input.substr(start, pos - start);
	        return makeToken(isFloat ? TokenType::FLOAT : TokenType::INT, number, start, startCol);
	    }  // readNumber()
		Token readString() {
		    int start = pos;
		    int startCol = col;
		    pos++; 
		    col++; // 跳過起始的 "
		    string value;
		    while(pos < input.size()){
		        char ch = input[pos];
		        if(ch == '"'){
		            // 檢查前面連續反斜線數量
		            int backslashCount = 0;
		            int temp = pos - 1;
		            while(temp >= start + 1 && input[temp] == '\\'){
		                backslashCount++;
		                temp--;
		            }  // while 
		            // 如果反斜線數為偶數，則這個雙引號是字串結束符號
		            if(backslashCount % 2 == 0){
		                pos++;
		                col++;
		                return makeToken(TokenType::STRING, "\"" + value + "\"", start, col);
		            }  // if
		            // 否則，此雙引號被 escape，應該加入字串內容
		            value.push_back('"');
		            pos++;
		            col++;
		            continue;
		        }  // if
		        if (ch == '\n') {
		        	return makeToken(TokenType::ERROR, "ERROR (no closing quote) : END-OF-LINE encountered", start, col);
		    	}  // if
		        if(ch == '\\'){
		            pos++;
		            col++;
		            if(pos >= input.size()){
		                value.push_back('\\');
		                break;
		            }  // if
		            char nextChar = input[pos];
		            if(nextChar == 'n'){
		                value.push_back('\n');
		            }  // if
		            else if(nextChar == 't'){
		                value.push_back('\t');
		            }  // else if
		            else if(nextChar == '"'){
		                value.push_back('"');
		            }  // else if
		            else if(nextChar == '\\'){
		                value.push_back('\\');
		            }  // else if
		            else {
		                // 非特殊 escape，保留反斜線與該字元
		                value.push_back('\\');
		                value.push_back(nextChar);
		            }  // else
		            pos++;
		            col++;
		            continue;
		        }  // if
		        value.push_back(ch);
		        pos++;
		        col++;
		    }  // while
		    return makeToken(TokenType::ERROR, "ERROR (no closing quote) : END-OF-LINE encountered", start, startCol);
		}  // resdString

	    Token readSymbol() {
		    int start = pos;
		    int startCol = col;
		    // 遇到任何分隔符號就停
		    while (pos < input.size() && !isSeparator(input[pos])) {
		        pos++;
		        col++;
		    }  // while
		    string symbol = input.substr(start, pos - start);
		    if (symbol == "nil" || symbol == "#f")
		        return makeToken(TokenType::NIL, "nil", start, startCol);
		    if (symbol == "t" || symbol == "#t")
		        return makeToken(TokenType::T, "#t", start, startCol);
		    return makeToken(TokenType::SYMBOL, symbol, start, startCol);
		}  // readSymbol()

};

class Parser {
	private:
	    Lexer& lexer;  // 用來取得下一個 token
	    Token current;  // 當前正在分析的 token
	    bool hasCurrent;
	    bool incomplete;  // 是否遇到 END，表示輸入尚未完整

    public:
	    // 建構子：初始化 lexer 和狀態
	    Parser(Lexer &lx) : lexer(lx), hasCurrent(false), incomplete(false) {}
	
	    ASTNode* parse() {
	        next(); // 讀第一個 token
	        if (current.type == TokenType::END) {
	            incomplete = true;  // 輸入還沒結束，等下一行
	            return nullptr;
	        } // if
	        return parseSExp();  // 呼叫遞迴檢查S-expression
	    } // parse()
	
	    // 判斷目前這個 S-expression 是否不完整 
	    bool isIncomplete() {
	        return incomplete;
	    } // isIncomplete()
	
		int getConsumedPos() {
		    // hasCurrent 代表已經先讀好下一顆 token
		    int keepFrom = 0;
		    if (hasCurrent && current.type != TokenType::END)
//		    	keepFrom = current.pos - (current.col - 1);
//        		if( keepFrom < 0 )  return 0;
//        		else return keepFrom;
		        return current.pos;          // 回傳那個 token 起始 的 index
		    return lexer.getPos();           // 已經到輸入尾巴才切掉
		}  // getConsumedPos()
	
	    private:
	    // 取得下一個 token 並存入 current
	    void next() {
	        current = lexer.getNextToken();
	        hasCurrent = true;
	    } // next()
	
	    ASTNode* parseSExp() {
	        if (errorInCurrentLine) {
	            return nullptr;
	        } // if
	
	        if (current.type == TokenType::END) {
	            incomplete = true;  // 輸入還沒結束等待下一行 
	            return nullptr;
	        } // if
	        
			// Lexer 錯誤的 token直接報錯
	        if (current.type == TokenType::ERROR) {
	            printError(current.value, current.line, current.col);
	            errorInCurrentLine = true;
	            return nullptr;
	        } // if
			// 不應該以右括號開頭
	        if (current.type == TokenType::RIGHT_PAREN) {
	            printUnexpectedTokenError(current);
	            errorInCurrentLine = true;
	            return nullptr;
	        } // if
	
	        if (current.type == TokenType::LEFT_PAREN) {
	            next();  // 消耗 '('
	            return parseList();
	        } // if
	
	        if (current.type == TokenType::QUOTE) {
	            next();  // 消耗quote
	            ASTNode* quoted = parseSExp();  // quote 接著要是一個合法的 S-expression 
	            if (quoted == nullptr) {
	                return nullptr;
	            } // if
	            ASTNode* quoteNode = new ASTNode(NodeType::QUOTE);
	            quoteNode->children.push_back(quoted);
	            return quoteNode;
	        } // if
	
	        if (current.type == TokenType::FLOAT) {
	            double num = atof(current.value.c_str());
	            char buf[64];
	            sprintf(buf, "%.3f", num);
	            next();
	            return new ASTNode(NodeType::ATOM, string(buf));
	        } // if
			// 其他合法的 atom：INT, SYMBOL, STRING, T, NIL
	        if (current.type == TokenType::INT || current.type == TokenType::SYMBOL ||
	            current.type == TokenType::STRING || current.type == TokenType::T ||
	            current.type == TokenType::NIL) {
	            string val = current.value;
	            if (current.type == TokenType::INT && val[0] == '+') {
	                val = val.substr(1);
	            } // if
	            next();
	            return new ASTNode(NodeType::ATOM, val);
	        } // if
			// 若不是合法的開頭報錯
	        printUnexpectedTokenError(current);
	        errorInCurrentLine = true;
	        return nullptr;
	    } // parseSExp()
	
	    ASTNode* parseList() {
		    if (errorInCurrentLine) {
		        return nullptr;
		    } // if
		
		    vector<ASTNode*> elements;  // 用來存放 list 中的元素 
		    bool hasDot = false;  // 記錄是否已經出現過一個 dot 
		    ASTNode* dotRight = nullptr;
		    bool isFirst = true;  // 判斷是否是第一個元素 
		
		    while (true) {
		        if (current.type == TokenType::END) {
		            incomplete = true;
		            return nullptr;
		        } // if
		
		        if (current.type == TokenType::RIGHT_PAREN) {
		            break; // 結束 list
		        } // if
		
		        if (current.type == TokenType::DOT) {
		        	// dot 不可出現在第一個或重複出現
		            if (isFirst || hasDot) {
		                printUnexpectedTokenError(current);
		                errorInCurrentLine = true;
		                return nullptr;
		            } // if
		
		            hasDot = true;
		            next();  // 消耗 '.'
		
		            dotRight = parseSExp();  // dot 後面接一個合法 S-expression 
		            if (dotRight == nullptr) {
		                return nullptr;
		            } // if
		
		            // dot 之後不能再有任何元素
		            if (current.type == TokenType::END) {
		                incomplete = true;
		                return nullptr;
		            } // if
		
		            if (current.type != TokenType::RIGHT_PAREN) {
		                printRightParenExpectedError(current);
		                errorInCurrentLine = true;
		                return nullptr;
		            } // if
		
		            break;  // 完成 dotted pair
		        } // if DOT
		
		        if (hasDot) {
		            // DOT 後不應再有元素錯誤
		            printUnexpectedTokenError(current);
		            errorInCurrentLine = true;
		            return nullptr;
		        } // if
		
		        ASTNode* expr = parseSExp();
		        if (expr == nullptr) {
		            return nullptr;
		        } // if
		
		        elements.push_back(expr);
		        isFirst = false;
		    } // while
		
		    if (current.type == TokenType::RIGHT_PAREN) {
		        next(); // consume ')'
		    } // if
		    else if (current.type == TokenType::END) {
		        incomplete = true;
		        return nullptr;
		    } // else if
		    else {
		        printRightParenExpectedError(current); 
		        errorInCurrentLine = true;
		        return nullptr;
		    } // else
		
		    if (hasDot) {
		        if (dotRight->type == NodeType::ATOM && dotRight->value == "nil") {
		            ASTNode* listNode = new ASTNode(NodeType::LIST);
		            listNode->children = elements;
		            return listNode;
		        } // if
		        return buildDottedPair(elements, dotRight);
		    } // if
			// 如果整個 list 中第一個元素是「只有一個 ATOM 的 LIST」
//			if (elements.size() > 0 &&
//			    elements[0]->type == NodeType::LIST &&
//			    elements[0]->children.size() == 1 &&
//			    elements[0]->children[0]->type == NodeType::ATOM) {
//			    
//			    // 取出內部 ATOM，替代原來的 list
//			    elements[0] = elements[0]->children[0];
//			}  // if
		    ASTNode* listNode = new ASTNode(NodeType::LIST);
		    listNode->children = elements;
		    return listNode;
		} // parseList()

	
	    ASTNode* buildDottedPair(vector<ASTNode*>& elems, ASTNode* dotRight) {
	        if (elems.empty()) {
	            printUnexpectedTokenError(current);
	            errorInCurrentLine = true;
	            return nullptr;
	        } // if
	
	        ASTNode* result = dotRight;
	        for (int i = elems.size() - 1; i >= 0; --i) {
	            ASTNode* dotPair = new ASTNode(NodeType::DOT_PAIR);
	            dotPair->children.push_back(elems[i]);
	            dotPair->children.push_back(result);
	            result = dotPair;
	        } // for
	
	        return result;
	    } // buildDottedPair()
	
	    void printError(const string &msg, int line, int col) {
	    	errorInCurrentLine = true;
	        cout << "\n\n> " << msg << " at Line " << line << " Column " << col;
	    } // printError()
	
	    void printUnexpectedTokenError(const Token &t) {
	    	errorInCurrentLine = true;
//	    	int col = 1;
//	    	if( t.value == "." )  col = t.col+1;
//	    	else  col = t.col;
	        cout << "\n\n> ERROR (unexpected token) : atom or '(' expected when token at Line "
	             << t.line << " Column " << t.col << " is >>" << t.value << "<<";
	    } // printUnexpectedTokenError()
	    void printRightParenExpectedError(const Token &t) {
		    errorInCurrentLine = true;
		    cout << "\n\n> ERROR (unexpected token) : ')' expected when token at Line "
		         << t.line << " Column " << t.col << " is >>" << t.value << "<<";
		}  // printRightParenExpectedError
};

class Evaluator {
	public:
		
		bool evalError = false;
		ASTNode* eval(ASTNode* node) {
			depth++;
		    if (node == NULL) {
		    	depth--;
		    	return nullptr;
			}  // if
		    while (node->type == NodeType::LIST &&
		           node->children.size() == 1 &&
		           node->children[0]->type == NodeType::LIST) {
		        node = node->children[0];
		    }  // while 
		
		     switch (node->type) {
		        case NodeType::ATOM: {
		            ASTNode* r = evalAtom(node);
		            --depth;                      
		            return r;
		        }
		        case NodeType::DOT_PAIR: {
				    ASTNode* fakeList = new ASTNode(NodeType::LIST);
				    ASTNode* curr = node;
				
				    while (curr->type == NodeType::DOT_PAIR) {
				        fakeList->children.push_back(curr->children[0]);
				        curr = curr->children[1];
				    }  // while
				
				    if (curr->type == NodeType::LIST) {
				        for (ASTNode* child : curr->children)
				            fakeList->children.push_back(child);
				    }  // if
				    else if (curr->type == NodeType::ATOM && curr->value == "nil") {
				        // do nothing
				    }  // else if
				    else {
				        cout << "\n\n> ERROR (non-list) : ";
				        prettyPrint(node);
				        return nullptr;
				    }  // else
				
				    // 建立新的 Evaluator只處理 fakeList
				    Evaluator subEval;
				    ASTNode* result = subEval.eval(fakeList);
				    return result;
				}


		        case NodeType::LIST: {
		            ASTNode* r = evalList(node);  
		            --depth;                      
		            return r;
		        }
		        case NodeType::QUOTE: {
		            ASTNode* r = node->children[0];
		            --depth;
		            return r;
		        }
		    }
		    depth--;
		    return nullptr;
		}  // eval

	
	private:
		int depth = 0;
	    ASTNode* evalAtom(ASTNode* node) {
	        //  (int, float, string, bool, nil)
	        if (node->value == "nil" || node->value == "#t" || isNumber(node->value) || node->value.front() == '"') {
	            return new ASTNode(NodeType::ATOM, node->value);
	        }  // if
			if (systemPrimitives.count(node->value)) {
		        // 如果這個名字被使用者重新 define 過就回傳那個定義
		        if (userEnv.count(node->value))        // 被遮蔽，當成普通符號
		            return userEnv[node->value];
		
		        // 否則它就是一支內建函式
		        return new ASTNode(NodeType::PROCEDURE, node->value);
		    }  // if
	        // 是否是已定義的 symbol
	        if (userEnv.count(node->value)) {
	            return userEnv[node->value];
	        }  // if
	
	        cout << "\n\n> ERROR (unbound symbol) : " << node->value;
	        return nullptr;
	    }  // evalAtom
	
	    bool isNumber(string &val) {
		    if (val.empty()) return false;          // 空字串一定不是數字
		
		    bool hasDigit = false;
		    for (size_t i = 0; i < val.size(); ++i) {
		        char c = val[i];
		
		        if (isdigit(c)) {                   // 0-9
		            hasDigit = true;
		            continue;
		        }  // if
		
		        if (c == '.') continue;             // 小數點
		
		        // ‘+’ 或 ‘–’ 只能出現在開頭，且後面必須還有東西
		        if ((c == '+' || c == '-') && i == 0 && val.size() > 1)
		            continue;
		
		        return false;                       // 其他字元 ? 不是數字
		    }  // for
		    return hasDigit;                        // 一定要含至少一個 0-9
		}  // isNumber

		bool isTypeCheckFunc(string& name) {
		    static set<string> types = {
		        "atom?", "pair?", "list?", "null?", "integer?",
		        "real?", "number?", "string?", "boolean?", "symbol?"
		    };
		    return types.count(name);
		}  //  isTypeCheckFunc
		bool isValidFunctionName(const string& val) {
		    if (val == "nil" || val == "#t") return false;
		    if (!val.empty() && val[0] == '"') return false;
		
		    // 禁掉完全是數字的（例如 "123" 或 "3.14"）
		    bool hasDigit = false;
		    for (char c : val) {
		        if (isdigit(c)) hasDigit = true;
		        else if (!isalpha(c) && !ispunct(c)) return true;  // 有其他字元 → 當作合法 symbol
		    }  // for
		
		    return !hasDigit;  // 純數字 → 不合法，其他都可以
		}  // isValidFunctionName
		bool isSymbolName(const string& val) {
		    //不能是字串
		    if (!val.empty() && val[0] == '"') return false;
		
		    //不能是布林或 nil
		    if (val == "#t" || val == "nil")   return false;
		
		    //不能純數字（含小數點／正負號）
		    bool hasDigit = false;
		    bool hasOther = false;
		    for (char c : val) {
		        if (isdigit(c)) hasDigit = true;
		        else if (c != '+' && c != '-' && c != '.') hasOther = true;
		    }  // for
		    if (hasDigit && !hasOther) return false;   // 全是數字相關 → 不是 symbol
		
		    return true;                               // 其餘都當符號
		}  // isSymbolName

		ASTNode* evalList(ASTNode* node) {
		    if (node->children.empty()) return new ASTNode(NodeType::ATOM, "nil");
	
		
		    ASTNode* head = node->children[0];
		    if (head->type == NodeType::LIST &&
			    head->children.size() == 1 &&
			    head->children[0]->type == NodeType::ATOM) {
			    head = head->children[0];
			    node->children[0] = head;
			}  // if
			if (head->type != NodeType::ATOM && head->type != NodeType::PROCEDURE) {
			    head = eval(head);                 
			    if (!head) return nullptr;         // eval 出錯就直接回傳
			    node->children[0] = head;          //  更新樹
			}  // if

		    if (head->type != NodeType::ATOM && head->type != NodeType::PROCEDURE ) {
		        cout << "\n\n> ERROR (attempt to apply non-function) : ";
		        prettyPrint(head);
		        return nullptr;
		    }  //if
		    string func = head->value;
			//		    cout << func << "  func\n";
			
			if (func == "clean-environment") {
			    if (depth > 1) {                         // 不在最外層
			        cout << "\n\n> ERROR (level of CLEAN-ENVIRONMENT)";
			        return nullptr;
			    }  // if
			    if ((int)node->children.size() != 1) {    // 有給多餘參數
			        cout << "\n\n> ERROR (incorrect number of arguments) : clean-environment";
			        return nullptr;
			    }  // if
			
			    userEnv.clear();                        
			    cout << "\n\n> environment cleaned";
			    return nullptr;                          
			}  // if
		    if (func == "cond")  {
		    	if ((int)node->children.size() < 1) {
		            cout << "\n\n> ERROR (incorrect number of arguments) : cond";
		            return nullptr;
		        }  // if
				return evalCond(node);
			}  // if
			if (func == "begin") {
				
			    if ((int)node->children.size() < 2) {
			        cout << "\n\n> ERROR (incorrect number of arguments) : begin";
			        return nullptr;
			    }  // if
			    // 不先 eval交給 evalBegin 處理
			    vector<ASTNode*> exprs;
			    for (int i = 1; i < node->children.size(); ++i) {
			        exprs.push_back(node->children[i]);
			    }  // for
			    return evalBegin(exprs);
			}  // if
		    // define
		    if (func == "define") {
		    	if (depth > 1) {                       // 不在最外層
			        cout << "\n\n> ERROR (level of DEFINE)";
			        return nullptr;
			    }  // if
		        if ((int)node->children.size() != 3 || node->children[1]->type != NodeType::ATOM) {
		            cout << "\n\n> ERROR (DEFINE format) : ( define\n";
				    for (int i = 1; i < node->children.size(); i++) {
				    	//cout << node->children[i] << "\n";
				        cout << "  ";
				        prettyPrint(node->children[i], 2);
				        cout << "\n";
				    }  // for
				    cout << ")";
				    return nullptr;
		        }  // if
		        ASTNode* first = node->children[1];
		        // 第一個參數不是symbol是數字之類的就錯 
		        if (first->type != NodeType::ATOM || !isSymbolName(first->value)) {
				    cout << "\n\n> ERROR (DEFINE format) : ( define\n";
				    for (int i = 1; i < node->children.size(); ++i) {
				    	cout << "  ";
				        prettyPrint(node->children[i], 2);
				        cout << "\n";
				    }  // for
				    cout << ")";
				    return nullptr;
				}  // if
		        ASTNode* value = eval(node->children[2]);
		        if (!value) return nullptr;
		        vector<ASTNode*> args = { node->children[1], value };
		        return evalDefine(args);
		    }  // if
		
		    // quote
		    if (func == "quote") {
		        if ((int)node->children.size() != 2) {
		            cout << "\n\n> ERROR (incorrect number of arguments) : quote";
		            return nullptr;
		        }  // if
		        return node->children[1];
		    }  // if		
		    // primitive 函數不用 eval
		    bool isSystemFunc = systemPrimitives.count(func);
		
		    if (!isSystemFunc) {
		        // eval user-defined symbol
		        ASTNode* evaledHead = eval(head);
		        if (!evaledHead) return nullptr;
		
		        // 不是可以當作 function 的 atom（數字、字串）
		        if ((evaledHead->type == NodeType::ATOM &&                 // 只有 ATOM 才查名字
				     !isValidFunctionName(evaledHead->value)) ||
				    (evaledHead->type != NodeType::ATOM &&                 // 不是 ATOM、也不是 PROCEDURE ? 錯
				     evaledHead->type != NodeType::PROCEDURE)) {
				    cout << "\n\n> ERROR (attempt to apply non-function) : ";
				    prettyPrint(evaledHead);
				    return nullptr;
				}  // if

		        func = evaledHead->value;
		        while (!systemPrimitives.count(func) && userEnv.count(func)) {
				    ASTNode* maybeFunc = userEnv[func];
				    if (maybeFunc->type == NodeType::ATOM) {
				        func = maybeFunc->value;
				    }  // if
					else {
				        break;
				    }  // else
				}  // while
		    }  // if
		
		    // 開始檢查參數數量（eval 前先檢查）
		    int argCount = node->children.size() - 1;
		    if (func == "exit") {
//		    	cout << depth << " depth\n";
		    	if (depth > 1) {
			        cout << "\n\n> ERROR (level of EXIT)";
			        return nullptr;
			    }  // if
			    if (argCount != 0) {                         // 只允許 (exit)
			        cout << "\n\n> ERROR (incorrect number of arguments) : exit";
			        return nullptr;
			    }  // if
		        
		    }  // if
		    if (func == "if") {
			    if (argCount != 2 && argCount != 3) {
			        cout << "\n\n> ERROR (incorrect number of arguments) : if";
			        return nullptr;
			    }  // if
			
			    ASTNode* condition = eval(node->children[1]);
			    if (!condition) return nullptr;
			
			    if (condition->value == "nil") {
			        // 條件為假
			        if (argCount == 3)
			            return eval(node->children[3]);
			        else {
			            // 少了 else → 印錯誤
			            cout << "\n\n> ERROR (no return value) : ( if";
			            for (int i = 1; i < node->children.size(); ++i) {
			                cout << "\n  ";
			                prettyPrint(node->children[i], 2);
			            }  // for
			            cout << "\n)";
			            return nullptr;
			        }  // else
			    }  // if
				else {
			        // 條件為真 → 執行 then
			        return eval(node->children[2]);
			    }  // else
			}  // if

		    if (func == "cons" && argCount != 2) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : cons";
		        return nullptr;
		    }  // if
		    if (func == "list" && argCount < 0) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : list";
		        return nullptr;
		    }  // if
		    if (func == "list" && argCount == 0) {
		        cout << "\n\n> nil";
		        return nullptr;
		    }  // if
		    if (func == "car" && argCount != 1) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : car";
		        return nullptr;
		    }  // if
		    if (func == "cdr" && argCount != 1) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : cdr";
		        return nullptr;
		    }  // if
		    if (func == "not" && argCount != 1) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : not";
		        return nullptr;
		    }  // if
		    if (func == "cond" && argCount < 1) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : not";
		        return nullptr;
		    }  // if
		    if ((func == "+" || func == "-" || func == "*" || func == "/" ||
		         func == "and" || func == "or" || func == "=" || func == "<" ||
		         func == ">" || func == "<=" || func == ">=") && argCount < 2) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : " << func;
		        return nullptr;
		    }  // if
		    if ((func == "string-append" || func == "string>?" || func == "string<?" || func == "string=?" ) && argCount < 2) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : " << func;
		        return nullptr;
		    }  // if
//		    if (func == "clean-environment" && argCount != 0) {
//		        cout << "\n\n> ERROR (incorrect number of arguments) : clean-environment";
//		        return nullptr;
//		    }  // if
		    if ((func == "eqv?" || func == "equal?") && argCount != 2) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : " << func;
		        return nullptr;
		    }  // if
		    if (func == "begin" && argCount < 1) {
			    cout << "\n\n> ERROR (incorrect number of arguments) : begin";
			    return nullptr;
			}  // if
//			if (func == "clean-environment") {
//		        evalCleanEn(args)
//		    }  // if
		    if (isTypeCheckFunc(func) && argCount != 1) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : " << func;
		        return nullptr;
		    }  // if
		    if (func == "and")  {  	
			    vector<ASTNode*> rawArgs;
			    for (int i = 1; i < node->children.size(); ++i) {
			        rawArgs.push_back(node->children[i]); 
			    }  // for
			    return evalAnd(rawArgs);
			}  // if
		    if (func == "or") {
		    	vector<ASTNode*> rawArgs;
			    for (int i = 1; i < node->children.size(); ++i) {
			        rawArgs.push_back(node->children[i]); 
			    }  // for
			    return evalOr(rawArgs);
			}  // if
		
		    // eval 所有引數
		    vector<ASTNode*> args;
		    for (int i = 1; i < node->children.size(); ++i) {
		        ASTNode* evaled = eval(node->children[i]);
		        if (!evaled) return nullptr;
		        args.push_back(evaled);
		    }  // for
		    
		    if (func == "cons") return evalCons(args);
		    if (func == "list") return evalListConstructor(args);
		    if (func == "car") return evalCar(args);
		    if (func == "cdr") return evalCdr(args);
		    if (func == "not") return evalNot(args);
//		    if (func == "and")  {  	
//			    vector<ASTNode*> rawArgs;
//			    for (int i = 1; i < node->children.size(); ++i) {
//			        rawArgs.push_back(node->children[i]); 
//			    }  // for
//			    return evalAnd(rawArgs);
//			}  // if
//		    if (func == "or") return evalOr(args);
		    if (func == "+" || func == "-" || func == "*" || func == "/")
		        return evalMathOp(args, func);
		    if (func == "=" || func == "<" || func == ">" || func == "<=" || func == ">=")
		        return evalCompareOp(args, func);
		    if (func == "string-append") return evalStringAppend(args);
			if (func == "string>?" || func == "string<?" || func == "string=?")
			    return evalStringCompare(args, func);
			if (func == "eqv?") return evalEqv(args);
			if (func == "equal?") return evalEqual(args);
//		    if (func == "clean-environment") {
//		        userEnv.clear();
//		        cout << "\n\n> environment cleaned";
//		        return nullptr;
//		    }  // if
		    
		    if (isTypeCheckFunc(func)) {
		        return evalTypeCheck(args, func.substr(0, func.length() - 1));
		    }  // if
		
		    cout << "\n\n> ERROR (unbound symbol) : " << func;
		    return nullptr;
		}  // evalList

		ASTNode* evalDefine(vector<ASTNode*> &args) {
			ASTNode* arg1 = args[0];
		    ASTNode* arg2 = args[1];
		
		    string name = args[0]->value;
		
		    if (systemPrimitives.count(name)) {
		        cout << "\n\n> ERROR (DEFINE format) : ( define\n";
		        for (ASTNode* arg : args) {
		            cout << "  ";
		            prettyPrint(arg, 2);
		            cout << "\n";
		        }  // for
		        cout << ")";
		        return nullptr;
		    }  // if

		    ASTNode* valNode = args[1];
		
//		    // 是否已存在於環境裡 
		    bool in = false;
		    for (auto& kv : userEnv) {
		        if (kv.second == valNode) {   // 指標完全相同 → 正在做別名
		            in = true;
		            break;
		        }  // if
		    }  // for
//			userEnv[name] = valNode;
		    if (in) {
		        // 右邊是舊直接共用
		        userEnv[name] = valNode;
		    }  // if
			else {
		        // 複製一份
		        userEnv[name] = copyy(valNode);
		    }  // else
//			if (name == "c") {
//			    printf("define c value = %p\n", valNode);
//			    printf("a->children[1] = %p\n", userEnv["a"]->children[1]);
//			    if (valNode == userEnv["a"]->children[1]) puts("SAME!");
//			    else puts("DIFFERENT!");
//			}

		    cout << "\n\n> " << name << " defined";
		    return nullptr;
		}  // evalDefine


		ASTNode* evalCons(vector<ASTNode*> &args) {

		    ASTNode* left  = copyy(args[0]);
		    ASTNode* right = args[1];
		
		    // (cons x nil) → (x) ；右邊是 nil 就回傳 list
		    if (right->type == NodeType::ATOM && right->value == "nil") {
		        ASTNode* listNode = new ASTNode(NodeType::LIST);
		        listNode->children.push_back(left);
		        return listNode;
		    }  // if 
		    if (right->type == NodeType::LIST) {
		        ASTNode* listNode = new ASTNode(NodeType::LIST);
		        listNode->children.push_back(left);
		        for (ASTNode* child : right->children)
		            listNode->children.push_back(copyy(child));
		        return listNode;
		    }  // if
		
		    // 其餘情況組成 dotted pair
		    ASTNode* dotPair = new ASTNode(NodeType::DOT_PAIR);
		    dotPair->children.push_back(left);
		    dotPair->children.push_back(copyy(right));
		    return dotPair;
		}  // evalCons
		ASTNode* evalListConstructor(vector<ASTNode*> &args) {
		    ASTNode* listNode = new ASTNode(NodeType::LIST);
		    for (ASTNode* arg : args) {
		        listNode->children.push_back(copyy(arg));   // 每個元素都 copy
		    }  // for
		    return listNode;
		}  // evalListConstructor
	
	    ASTNode* evalCar(vector<ASTNode*> &args) {  // 取出 pair 或 list 的第一個元素
			ASTNode* arg = args[0];
		
		    if (arg->type == NodeType::LIST && !arg->children.empty())
		        return copyy(arg->children[0]);          
		
		    if (arg->type == NodeType::DOT_PAIR)
		        return copyy(arg->children[0]);          
		
		    cout << "\n\n> ERROR (car with incorrect argument type) : ";
		    prettyPrint(arg);
		    return nullptr;
	    }  // evalCar
	
		ASTNode* evalCdr(vector<ASTNode*> &args) {
		    ASTNode* arg = args[0];
		    
		
		    // LIST 處理
		    if (arg->type == NodeType::LIST) {
		        if (arg->children.size() <= 1)
		            return new ASTNode(NodeType::LIST); // 空尾部
		
			
			    ASTNode* tail = new ASTNode(NodeType::LIST);
			    tail->children.assign(arg->children.begin() + 1, arg->children.end());
			    return tail;
		    }  // if
		
		    // DOT_PAIR 處理
		    if (arg->type == NodeType::DOT_PAIR) {
		        // cdr 是 dot pair 的 right node
		        ASTNode* right = arg->children[1];
		        // (x . nil) → ()
		        if (right->type == NodeType::ATOM && right->value == "nil")
		            return new ASTNode(NodeType::LIST);
		        return right; // 直接回傳右節點
		    }  // if
		
		    // 其他型別錯誤
		    cout << "\n\n> ERROR (cdr with incorrect argument type) : ";
		    prettyPrint(arg);
		    return nullptr;
		}  // evalcdr 

	    
	    double getNumber(ASTNode* node, bool &isFloat, const string& op) {
		    if (node->type != NodeType::ATOM) {
		        cout << "\n\n> ERROR (" << op << " with incorrect argument type) : ";
		        prettyPrint(node);
		        evalError = true;
		        return 0;
		    }  // if
		
		    string val = node->value;
		
		    if (!isNumberString(val)) {
		        cout << "\n\n> ERROR (" << op << " with incorrect argument type) : ";
		        prettyPrint(node);
		        evalError = true;
		        return 0;
		    }  // if
		
		    if (val.find('.') != string::npos)
		        isFloat = true;
		
		    return stod(val);
		}  // getNumber
		bool isNumberString(string &s) {
		    if (s.empty()) return false;
		
		    int i = 0;
		    if (s[0] == '+' || s[0] == '-') i++;  // 處理正負號
		
		    bool hasDigit = false;
		    bool hasDot = false;
		
		    for (; i < s.size(); ++i) {
		        char c = s[i];
		        if (isdigit(c)) {
		            hasDigit = true;
		        }  // if
				else if (c == '.') {
		            if (hasDot) return false;  // 不允許第二個點
		            hasDot = true;
		        }  // else if
				else {
		            return false;  // 非數字非小數點
		        }  // else 
		    }  // for
		
		    // 至少要有「數字」或「小數點後有數字」
		    return hasDigit;
		}  // isNumberString

		ASTNode* evalMathOp(vector<ASTNode*> &args, string &op) {
		    if (args.size() < 2) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : " << op;
		        return nullptr;
		    }  // if
		
		    // 檢查是否全部都是整數（都沒有 .）
		    bool allInteger = true;
		    for (ASTNode* node : args) {
		        if (node->value.find('.') != string::npos) {
		            allInteger = false;
		            break;
		        }  // if
		    }  // for
			
		    // 全是整數 int 計算
		    if (allInteger) {
		        
		        if (!isNumberString(args[0]->value)) {
				    cout << "\n\n> ERROR (" << op << " with incorrect argument type) : ";
				    prettyPrint(args[0]);
				    return nullptr;
				}  // if
				int result = stoi(args[0]->value);
		        for (int i = 1; i < args.size(); ++i) {
		        	if (!isNumberString(args[i]->value)) {
					    cout << "\n\n> ERROR (" << op << " with incorrect argument type) : ";
					    prettyPrint(args[i]);
					    return nullptr;
					}  // if
		            int n = stoi(args[i]->value);
		            if (op == "+") result += n;
		            else if (op == "-") result -= n;
		            else if (op == "*") result *= n;
		            else if (op == "/") {
		                if (n == 0) {
		                    cout << "\n\n> ERROR (division by zero) : /";
		                    return nullptr;
		                }  // if
		                result /= n;
		            }  // else if
		        }  // for
		
		        char buf[64];
		        sprintf(buf, "%d", result);
		        return new ASTNode(NodeType::ATOM, buf);
		    }  // if
		
		    // 用 float 計算
		    bool isFloat = true;
		    if (!isNumberString(args[0]->value)) {
			    cout << "\n\n> ERROR (" << op << " with incorrect argument type) : ";
			    prettyPrint(args[0]);
			    return nullptr;
			}  // if
		    double result = stod(args[0]->value);
		    for (int i = 1; i < args.size(); ++i) {
		    	if (!isNumberString(args[i]->value)) {
					cout << "\n\n> ERROR (" << op << " with incorrect argument type) : ";
					prettyPrint(args[i]);
					return nullptr;
				}  // if
		        double n = stod(args[i]->value);
		        if (op == "+") result += n;
		        else if (op == "-") result -= n;
		        else if (op == "*") result *= n;
		        else if (op == "/") {
		            if (n == 0) {
		                cout << "\n\n> ERROR (division by zero) : /";
		                return nullptr;
		            }  // if
		            result /= n;
		        }  // else if
		    }  // for
		
		    char buf[64];
		    sprintf(buf, "%.3f", result);
		    return new ASTNode(NodeType::ATOM, buf);
		}  // evalMathop

		ASTNode* evalCompareOp(vector<ASTNode*>& args, string& op) {
		    bool isFloat = false;
		     // 先檢查所有參數是否合法
		    for (ASTNode* arg : args) {
		        double tmp = getNumber(arg, isFloat, op);
		        if (evalError) return nullptr;  // 有錯直接中止
		    }  // for 
		
		    double prev = getNumber(args[0], isFloat, op);
		    if (evalError) return nullptr;
		
		    for (int i = 1; i < args.size(); ++i) {
		        double curr = getNumber(args[i], isFloat, op);
		        if (evalError) return nullptr;
		
		        if (op == "=" && fabs(prev - curr) > 1e-6) return new ASTNode(NodeType::ATOM, "nil");
		        if (op == ">" && !(prev > curr)) return new ASTNode(NodeType::ATOM, "nil");
		        if (op == "<" && !(prev < curr)) return new ASTNode(NodeType::ATOM, "nil");
		        if (op == ">=" && !(prev >= curr)) return new ASTNode(NodeType::ATOM, "nil");
		        if (op == "<=" && !(prev <= curr)) return new ASTNode(NodeType::ATOM, "nil");
		
		        prev = curr;
		    }  // for
		
		    return new ASTNode(NodeType::ATOM, "#t");
		}  // evalCompareOp

		
		// 如果是 nil →回傳 #t
		ASTNode* evalNot(const vector<ASTNode*>& args) {
		    
		    string val = args[0]->value;
		    return new ASTNode(NodeType::ATOM, (val == "nil" ? "#t" : "nil"));
		}  // evalNot
		
		ASTNode* evalAnd(const vector<ASTNode*>& args) {
		    ASTNode* result = nullptr;
		    for (ASTNode* arg : args) {
		        result = eval(arg); 
		        if (!result) return nullptr;
		        if (result->value == "nil") return new ASTNode(NodeType::ATOM, "nil");
		    }  // for
		    return result;
		}  // evaland
		
		// 只要有不是 nil 的值就直接回傳
		ASTNode* evalOr(const vector<ASTNode*>& args) {
		    for (ASTNode* arg : args) {
		        ASTNode* result = eval(arg);         // 對每個表達式執行
		        if (!result) return nullptr;         // 出錯就中止
		
		        if (result->value != "nil")          // return
		            return result;
		    }  // for
		    return new ASTNode(NodeType::ATOM, "nil"); // 所有都是 nil
		}  // evalor

		ASTNode* evalTypeCheck(const vector<ASTNode*>& args, const string& type) {
		    
		    ASTNode* node = args[0];
			if (type == "integer") {
			    if (node->type == NodeType::ATOM) {
			        string v = node->value;
			        int start = 0;
			
			        if (v.empty()) return new ASTNode(NodeType::ATOM, "nil");
			
			        // 處理 +- 
			        if (v[0] == '+' || v[0] == '-') start = 1;
			
			        if (start == (int)v.length())  // 只有符號沒有數字
			            return new ASTNode(NodeType::ATOM, "nil");
			
			        for (int i = start; i < (int)v.length(); ++i) {
			            if (!isdigit(v[i]))
			                return new ASTNode(NodeType::ATOM, "nil");
			        }  // for 
			
			        return new ASTNode(NodeType::ATOM, "#t");
			    }  // if
			    return new ASTNode(NodeType::ATOM, "nil");
			}  // if
			
			if (type == "real") {
			    if (node->type == NodeType::ATOM) {
			        string v = node->value;
			        int start = 0;
			        bool hasDot = false;
			
			        if (v.empty()) return new ASTNode(NodeType::ATOM, "nil");
			
			        if (v[0] == '+' || v[0] == '-') start = 1;
			
			        if (start == (int)v.length())  // 只有符號
			            return new ASTNode(NodeType::ATOM, "nil");
			
			        for (int i = start; i < (int)v.length(); ++i) {
			            if (v[i] == '.') {
			                if (hasDot) return new ASTNode(NodeType::ATOM, "nil");  // 不能有兩個點
			                hasDot = true;
			                continue;
			            }  // if
			            if (!isdigit(v[i]))
			                return new ASTNode(NodeType::ATOM, "nil");
			        }  // for
			
			        // 沒有小數點 也沒有數字就不是一個合法
			        if (!hasDot && v.find_first_of("0123456789", start) == string::npos)
			            return new ASTNode(NodeType::ATOM, "nil");
			
			        return new ASTNode(NodeType::ATOM, "#t");
			    }  // if
			    return new ASTNode(NodeType::ATOM, "nil");
			}  // if

		    if (type == "number") {
		        string v = node->value;
		        bool dot = false;
		        if (node->type == NodeType::ATOM) {
		            for (char c : v) {
		                if (isdigit(c) || c == '.' || c == '+' || c == '-') {
		                    if (c == '.') dot = true;
		                    continue;
		                }  // if
		                return new ASTNode(NodeType::ATOM, "nil");
		            }  // for
		            return new ASTNode(NodeType::ATOM, "#t");
		        }  // if
		        return new ASTNode(NodeType::ATOM, "nil");
		    }  // if
		
		    if (type == "string") {
		        if (node->type == NodeType::ATOM && !node->value.empty() && node->value[0] == '"')
		            return new ASTNode(NodeType::ATOM, "#t");
		        return new ASTNode(NodeType::ATOM, "nil");
		    }  // if
		
		    if (type == "boolean") {
		        if (node->value == "#t" || node->value == "nil")
		            return new ASTNode(NodeType::ATOM, "#t");
		        return new ASTNode(NodeType::ATOM, "nil");
		    }  // if
		
		    if (type == "symbol") {
			    if (node->type == NodeType::ATOM) {
			        string val = node->value;
			        // 不是數字
			        bool isNum = true;
			        for (char c : val) {
			            if (!isdigit(c) && c != '.' && c != '+' && c != '-') {
			                isNum = false;
			                break;
			            }  // if
			        }  // for
			        if (isNum) return new ASTNode(NodeType::ATOM, "nil");
			
			        // 不是字串或布林
			        if (val == "#t" || val == "nil") return new ASTNode(NodeType::ATOM, "nil");
			        if (!val.empty() && val[0] == '"') return new ASTNode(NodeType::ATOM, "nil");
			
			        // 剩下的都視為 symbol
			        return new ASTNode(NodeType::ATOM, "#t");
			    }  // if
			    return new ASTNode(NodeType::ATOM, "nil");
			}  // if

		    if (type == "null") {
		        if ((node->type == NodeType::ATOM && node->value == "nil") ||
		            (node->type == NodeType::LIST && node->children.empty()))
		            return new ASTNode(NodeType::ATOM, "#t");
		        return new ASTNode(NodeType::ATOM, "nil");
		    }  // if
		
		    if (type == "pair") {
			    if (node->type == NodeType::DOT_PAIR)
			        return new ASTNode(NodeType::ATOM, "#t");
			    if (node->type == NodeType::LIST && !node->children.empty())
			        return new ASTNode(NodeType::ATOM, "#t");
			    return new ASTNode(NodeType::ATOM, "nil");
			}  // if
		
		    if (type == "atom") {
		        if (node->type == NodeType::ATOM) return new ASTNode(NodeType::ATOM, "#t");
		        return new ASTNode(NodeType::ATOM, "nil");
		    }  // if
		
		    return new ASTNode(NodeType::ATOM, "nil");
		}  // evalTypeCheck
		bool isString(ASTNode* node) {
		    if (node->type != NodeType::ATOM)
		        return false;
		
		    if (node->value.empty())
		        return false;
		
		    if (node->value[0] != '"')
		        return false;
		
		    return true;
		}  // isString
		ASTNode* evalStringAppend(vector<ASTNode*>& args) {
		    string result = "";
		
		    for (ASTNode* node : args) {
		        if (!isString(node)) {
		            cout << "\n\n> ERROR (string-append with incorrect argument type) : ";
		            prettyPrint(node);
		            return nullptr;
		        }  // if 
		
		        string content = node->value.substr(1, node->value.length() - 2);
		        result += content;
		    }  // for
		
		    return new ASTNode(NodeType::ATOM, "\"" + result + "\"");
		}  // evalStringAppend
		ASTNode* evalStringCompare( vector<ASTNode*>& args, string& op) {
		    for (ASTNode* node : args) {
		        if (!isString(node)) {
		            cout << "\n\n> ERROR (" << op << " with incorrect argument type) : ";
		            prettyPrint(node);
		            return nullptr;
		        }  // if
		    }  // for
		
		    for (int i = 0; i < args.size() - 1; ++i) {
		        string a = args[i]->value.substr(1, args[i]->value.length() - 2);
		        string b = args[i + 1]->value.substr(1, args[i + 1]->value.length() - 2);
		
		        if (op == "string>?") {
		            if (!(a > b)) return new ASTNode(NodeType::ATOM, "nil");
		        }  // if
				else if (op == "string<?") {
		            if (!(a < b)) return new ASTNode(NodeType::ATOM, "nil");
		        }  // else if
				else if (op == "string=?") {
		            if (a != b) return new ASTNode(NodeType::ATOM, "nil");
		        }  // else if
		    }  // for
		
		    return new ASTNode(NodeType::ATOM, "#t");
		}  // evalStringCompare
		
		ASTNode* evalEqv(vector<ASTNode*>& args) {
		    ASTNode* a = args[0];
		    ASTNode* b = args[1];
		
		    if (a == b)                                         // 同一物件
		        return new ASTNode(NodeType::ATOM, "#t");
		    if (a->type != b->type)                             // 型別不同
		        return new ASTNode(NodeType::ATOM, "nil");
		    // both are empty lists
		    if (a->type == NodeType::LIST && a->children.empty() &&
		        b->type == NodeType::LIST && b->children.empty())
		        return new ASTNode(NodeType::ATOM, "#t");
		
		    if (a->type == NodeType::ATOM || a->type == NodeType::PROCEDURE) {
		        //string 只能用指標判斷
		        bool aStr = !a->value.empty() && a->value[0] == '"';
		        bool bStr = !b->value.empty() && b->value[0] == '"';
		        if (aStr && bStr)                               
		            return new ASTNode(NodeType::ATOM, "nil");
		
		        /* 其餘 ATOM（數字、布林、符號）仍看字面值 */
		        return new ASTNode(NodeType::ATOM, a->value == b->value ? "#t" : "nil");
		    }  // if
		
		    
		    return new ASTNode(NodeType::ATOM, "nil");
		}  // evaleqv


		ASTNode* evalEqual(vector<ASTNode*> &args) {  // 內容 
		    return deepEqual(args[0], args[1]) ?
		        new ASTNode(NodeType::ATOM, "#t") :
		        new ASTNode(NodeType::ATOM, "nil");
		}  // evalEqual
		
		bool deepEqual(ASTNode* a, ASTNode* b) {
		    if (a == b) return true;  // 同一物件 → 一定相等
		
//		    if (a->type != b->type) return false;
			auto normType = [](NodeType t) {
			        return (t == NodeType::PROCEDURE) ? NodeType::ATOM : t;
			};
			if (normType(a->type) != normType(b->type)) return false;
		
		    if (a->type == NodeType::ATOM || a->type == NodeType::PROCEDURE) {
		        return a->value == b->value;
		    }  // if
		
		    if ((a->type == NodeType::LIST || a->type == NodeType::QUOTE) &&
		        (b->type == NodeType::LIST || b->type == NodeType::QUOTE)) {
		        if (a->children.size() != b->children.size()) return false;
		        for (size_t i = 0; i < a->children.size(); ++i) {
		            if (!deepEqual(a->children[i], b->children[i])) return false;
		        }  // for
		        return true;
		    }  // if
		
		    if (a->type == NodeType::DOT_PAIR && b->type == NodeType::DOT_PAIR) {
		        return deepEqual(a->children[0], b->children[0]) &&
		               deepEqual(a->children[1], b->children[1]);
		    }  // if
		
		    return false;
		}  // deepEqual
		
		ASTNode* evalCond(ASTNode* node) {
		    if (node->children.size() < 2) {
		        cout << "\n\n> ERROR (COND format) : ( cond\n)";
		        return nullptr;
		    }  // if
		
		    // 檢查每個子句的語法正確性
		    for (int i = 1; i < node->children.size(); ++i) {
		        ASTNode* clause = node->children[i];
		
		        if (clause->type != NodeType::LIST || clause->children.empty()) {
		            cout << "\n\n> ERROR (COND format) : ( cond";
		            for (int j = 1; j < node->children.size(); ++j) {
		                cout << "\n  ";
		                prettyPrint(node->children[j], 2);
		            }  // for
		            cout << "\n)";
		            return nullptr;
		        }  // if
		
		        ASTNode* testNode = clause->children[0];
		        bool isLastClause = (i == node->children.size() - 1);
		
		        // else：文字是 "else" 且在最後一個 clause
		        bool isSyntaxElse = (
		            testNode->type == NodeType::ATOM &&
		            testNode->value == "else" &&
		            isLastClause
		        );
		
		        // 報錯有人用語法 else 但不在最後
		        if (testNode->type == NodeType::ATOM &&
				    testNode->value == "else" &&
				    !isSyntaxElse) {
				
				    // 查 userEnv 裡有沒有綁定這個 symbol
				    if (userEnv.find("else") == userEnv.end()) {
				        // 如果沒有綁定，這真的是語法錯誤
				        cout << "\n\n> ERROR (unbound symbol) : else";
//				        for (int j = 1; j < node->children.size(); ++j) {
//				            cout << "\n  ";
//				            prettyPrint(node->children[j], 2);
//				        }  // for
//				        cout << "\n)";
				        return nullptr;
				    }  // if
				
				    
				}  // if
		
		        // 子句只有一個元素也不合法
		        if (clause->children.size() == 1) {
		            cout << "\n\n> ERROR (COND format) : ( cond";
		            for (int j = 1; j < node->children.size(); ++j) {
		                cout << "\n  ";
		                prettyPrint(node->children[j], 2);
		            }  // for
		            cout << "\n)";
		            return nullptr;
		        }  // if
		    }  // for
		
		    // 開始逐條執行條件
		    for (int i = 1; i < node->children.size(); ++i) {
		        ASTNode* clause = node->children[i];
		        ASTNode* testNode = clause->children[0];
		        bool isLastClause = (i == node->children.size() - 1);
		
		        bool isSyntaxElse = (
		            testNode->type == NodeType::ATOM &&
		            testNode->value == "else" &&
		            isLastClause
		        );
		
		        bool shouldExecute = false;
		
		        if (isSyntaxElse) {
		            shouldExecute = true;
		        }  // if
				else {
		            // 正常 用 eval 執行確認是否非 nil
		            ASTNode* testResult = eval(testNode);
		            if (!testResult) return nullptr;
		
		            if (testResult->type != NodeType::ATOM || testResult->value != "nil") {
		                shouldExecute = true;
		            }  // if
		        }  // else
		
		        if (shouldExecute) {
		            ASTNode* result = nullptr;
		            for (int k = 1; k < clause->children.size(); ++k) {
		                result = eval(clause->children[k]);
		                if (!result) return nullptr;
		            }  // for
		            return result;
		        }  // if
		    }  // for
		
		    // 所有條件都不成立
		    cout << "\n\n> ERROR (no return value) : ( cond";
		    for (int j = 1; j < node->children.size(); ++j) {
		        cout << "\n  ";
		        prettyPrint(node->children[j], 2);
		    }  // for
		    cout << "\n)";
		    return nullptr;
		}  // evalcond

		ASTNode* evalBegin(vector<ASTNode*>& args) {
		    ASTNode* result = nullptr;
		    for (ASTNode* arg : args) {
		        result = eval(arg);  // 每個都要 eval
		        if (!result) return nullptr;  // 如果中途有錯就停止
		    }  // for
		    return result;  // 回傳最後一個 expression 的值
		}  // evalbegin
		ASTNode* evalCleanEnv(const vector<ASTNode*>& args) {
		    // (clean-environment) 不接受任何參數
		
		    // 只允許在頂層呼叫
		    if (depth > 1) {                    // 巢狀層級大於 1 → 不是頂層
		        cout << "\n\n> ERROR (level of CLEAN-ENVIRONMENT)";
		        return nullptr;
		    }  // if 

		    userEnv.clear();                   
		    cout << "\n\n> environment cleaned";
		    return nullptr;                    
		}  // evalCleanEn

};

bool isExitCommand(ASTNode* node) {
    // 普通 list: (exit)
    if (node && node->type == NodeType::LIST && node->children.size() == 1) {
        ASTNode* child = node->children[0];
        if (child && child->type == NodeType::ATOM && child->value == "exit")
            return true;
    }  // if

    // dotted pair: (exit . nil)
    if (node && node->type == NodeType::DOT_PAIR) {
        ASTNode* first = node->children[0];
        ASTNode* second = node->children[1];
        if (first && first->type == NodeType::ATOM && first->value == "exit" &&
            second && second->type == NodeType::ATOM && second->value == "nil")
            return true;
    }  // if
    return false;
}  // isExitCommand


int main() {
    string input;
    string buffer = "";
    bool exitFlag = false;

    cout << "Welcome to OurScheme!";
    cin >> testnum;
    cin.ignore(); // 忽略測試數字後的換行

    Lexer lexer("", 1, 1);  // 初始化空的 lexer

    while (getline(cin, input)) {
        buffer += input + "\n"; // 將這一行加入 buffer

        lexer = Lexer(buffer, globalLine, 1); // 每輪更新 lexer 的輸入

        while (true) {
            errorInCurrentLine = false;

            Parser parser(lexer);
            ASTNode* expr = parser.parse();

            if (errorInCurrentLine) {
                // 發生錯誤，整行丟掉
                buffer.clear();
                globalLine = 1;
                globalCol = 1;
                break;
            }  // if

            if (expr == NULL) {
                if (parser.isIncomplete()) {
                    // incomplete，等待更多輸入
                    break;
                }  // if
                else {
                    // 沒有更多 expression 可以 parse
                    break;
                }  // else
            }  // if

            if (isExitCommand(expr)) {
                cout << "\n\n> ";
                delete expr;
                exitFlag = true;
                buffer.clear();
                break;
            }  // if
//			prettyPrint(expr, 0);
            // 輸出 AST
            //cout << "\n\n> ";
            Evaluator eval;
			ASTNode* result = eval.eval(expr);
            if (result != NULL) {
			    cout << "\n\n> ";
			    prettyPrint(result, 0);
			}  // if
//            globalLine = 1;
//            globalCol = 1;
            delete expr;

            // 取得目前 Lexer 已經吃掉的位置
            int consumedPos = parser.getConsumedPos();

            while (consumedPos > 0 && buffer[consumedPos - 1] != '\n' && (buffer[consumedPos - 1] == ' ' || buffer[consumedPos - 1] == '\t')) {
			    consumedPos--;   // 往左退，讓空白留在下一輪 buffer
			}  // while
			
			if (consumedPos >= (int)buffer.size())
			    buffer.clear();
			else
			    buffer = buffer.substr(consumedPos);

            // 如果剩下全是空白也清掉
            if (buffer.find_first_not_of(" \t\r\n") == string::npos) {
                buffer.clear();
                break;
            }  // if

            
            lexer = Lexer(buffer, globalLine, globalCol);
        }  // while parse()

        if (exitFlag) break;
    }  // while getline

    if (!exitFlag && cin.eof()) {
        cout << "\n\n> ERROR (no more input) : END-OF-FILE encountered";
    }  // if

    cout << "\nThanks for using OurScheme!";
    return 0;
}  // main
