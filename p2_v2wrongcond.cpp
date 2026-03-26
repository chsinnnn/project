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
    QUOTE      // '(1 2 3)
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
const set<string> systemPrimitives = {"eqv?", "equal?", "cons", "list", "quote", "+", "-", "*", "/",">","<",">=","<=","=","string-append","string>?","string<?","string=?", "define", "car", "cdr", "atom?", "pair?", "list?", "null?", "integer?", "real?", "number?", "string?", "boolean?", "symbol?", "if", "cond", "begin", "and", "or", "not", "clean-environment"};
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
	if (node->type == NodeType::ATOM) {
	    if (currIndent > 0) cout << string(currIndent, ' ');
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
//	            if (ch == ';' ){
//				    while(pos < input.size() && input[pos] != '\n')
//				        pos++;
//				    cout << input[pos] << " 9696\n";
//				    cout << input.size() << "   " << pos << "  9797\n";
//					line++;
//					col = 1;
//				    continue;
//				}  // if 
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
//					    if (pos+1 < input.size()) {
//					        char nextch = input[pos+1];
//					        // 只有下一個字元直接跟著 '(' 或 digit 才算合法的 DOT
//					        if (nextch == '(' || isdigit(nextch)) {
//					            return makeToken(TokenType::DOT, ".", pos++, col++);
//					        }  // if 
//					    }  // if
//					    // 否則一律報錯
//					    int errLine = line;
//						int errCol = col;
//				        pos++; 
//						col++;
//						doterror = true;
//				        return makeToken(TokenType::ERROR, "ERROR (unexpected token) : atom or '(' expected when token at Line " + to_string(errLine) + " Column " + to_string(errCol) + " is >>.<<", errLine, errCol );
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
//		        if(ch == '\n'){
//		            return makeToken(TokenType::ERROR, "ERROR (no closing quote) : END-OF-LINE encountered", start, col);
//		        }
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
		    if (node == NULL) return nullptr;
		
		    switch (node->type) {
		        case NodeType::ATOM:
		            return evalAtom(node);
		        case NodeType::LIST:
		        case NodeType::DOT_PAIR: {
		            // 如果是 DOT_PAIR 但不能視為 LIST，就報錯
		            if (node->type == NodeType::DOT_PAIR) {
		                cout << "\n\n> ERROR (non-list) : ";
		                prettyPrint(node);
		                return nullptr;
		            }  // if 
		            return evalList(node);
		        }
		        case NodeType::QUOTE:
		            return node->children[0];
		    }
		    return nullptr;
		}  // eval

	
	private:

	    ASTNode* evalAtom(ASTNode* node) {
	        //  (int, float, string, bool, nil)
	        if (node->value == "nil" || node->value == "#t" || isNumber(node->value) || node->value.front() == '"') {
	            return new ASTNode(NodeType::ATOM, node->value);
	        }  // if
			if (systemPrimitives.count(node->value)) {
			    return new ASTNode(NodeType::ATOM, node->value);  // 將 primitive 視為合法 function
			}  // if
	        // 是否是已定義的 symbol
	        if (userEnv.count(node->value)) {
	            return userEnv[node->value];
	        }  // if
	
	        cout << "\n\n> ERROR (unbound symbol) : " << node->value;
	        return nullptr;
	    }  // evalAtom
	
	    bool isNumber(string &val) {
	        for (char c : val) {
	            if (!isdigit(c) && c != '.' && c != '+' && c != '-') return false;
	        }  // for
	        return true;
	    }  // isNumber
	
		bool isTypeCheckFunc(string& name) {
		    static set<string> types = {
		        "atom?", "pair?", "list?", "null?", "integer?",
		        "real?", "number?", "string?", "boolean?", "symbol?"
		    };
		    return types.count(name);
		}  //  isTypeCheckFunc
		bool isValidFunctionName(const string& val) {
		    // 不是純數字、不是字串、不是 #t / nil
		    if (val == "nil" || val == "#t") return false;
		    if (!val.empty() && val[0] == '"') return false;
		
		    // 判斷是否為數字
		    bool digitOnly = true;
		    for (char c : val) {
		        if (!isdigit(c) && c != '.' && c != '+' && c != '-') {
		            digitOnly = false;
		            break;
		        }  // if
		    }  // for
		    if (digitOnly) return false;
		
		    // 其他都算是合法 symbol / 函數名
		    return true;
		}  // isValidFunctionName

		ASTNode* evalList(ASTNode* node) {
		    if (node->children.empty()) return new ASTNode(NodeType::ATOM, "nil");
		
		    ASTNode* head = node->children[0];
//			cout << "\n[debug] head type: " << static_cast<int>(head->type);
//			cout << "\n[debug] head value: [" << head->value << "]";
		    if (head->type != NodeType::ATOM) {
		        cout << "\n\n> ERROR (attempt to apply non-function) : ";
		        prettyPrint(head);
		        return nullptr;
		    }  // if

		    string func = head->value;
		
		    // define
		    if (func == "define") {
		        if ((int)node->children.size() != 3 || node->children[1]->type != NodeType::ATOM) {
		            cout << "\n\n> ERROR (DEFINE format) : ( define";
				    for (int i = 1; i < node->children.size(); i++) {
				    	//cout << node->children[i] << "\n";
//				        cout << "\n  ";
				        prettyPrint(node->children[i], 2);
				    }  // for
				    cout << "\n)";
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
		        if (evaledHead->type != NodeType::ATOM || !isValidFunctionName(evaledHead->value)) {
		            cout << "\n\n> ERROR (attempt to apply non-function) : ";
		            prettyPrint(evaledHead);
		            return nullptr;
		        }  // if
		
		        func = evaledHead->value;
		    }  // if
		
		    // 開始檢查參數數量（eval 前先檢查）
		    int argCount = node->children.size() - 1;
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
//			                cout << "\n  ";
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
		    if (func == "list" && argCount < 1) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : list";
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
		    if (func == "clean-environment" && argCount != 0) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : clean-environment";
		        return nullptr;
		    }  // if
		    if ((func == "eqv?" || func == "equal?") && argCount != 2) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : " << func;
		        return nullptr;
		    }  // if
		    if (isTypeCheckFunc(func) && argCount != 1) {
		        cout << "\n\n> ERROR (incorrect number of arguments) : " << func;
		        return nullptr;
		    }  // if
		
		    // eval 所有引數
		    vector<ASTNode*> args;
		    for (int i = 1; i < node->children.size(); ++i) {
		        ASTNode* evaled = eval(node->children[i]);
		        if (!evaled) return nullptr;
		        args.push_back(evaled);
		    }  // for
//			if (func == "eqv?" || func == "equal?") {
//			    // 保留原始 symbol（例如 a, b）
//			    for (int i = 1; i < node->children.size(); ++i) {
//			        args.push_back(node->children[i]);
//			    }  // for 
//			}  // if
//			else {
//			    for (int i = 1; i < node->children.size(); ++i) {
//			        ASTNode* evaled = eval(node->children[i]);
//			        if (!evaled) return nullptr;
//			        args.push_back(evaled);
//			    }  // for
//			}  // else
		
		    
		    if (func == "cons") return evalCons(args);
		    if (func == "list") return evalListConstructor(args);
		    if (func == "car") return evalCar(args);
		    if (func == "cdr") return evalCdr(args);
		    if (func == "not") return evalNot(args);
		    if (func == "and") return evalAnd(args);
		    if (func == "or") return evalOr(args);
		    if (func == "+" || func == "-" || func == "*" || func == "/")
		        return evalMathOp(args, func);
		    if (func == "=" || func == "<" || func == ">" || func == "<=" || func == ">=")
		        return evalCompareOp(args, func);
		    if (func == "string-append") return evalStringAppend(args);
			if (func == "string>?" || func == "string<?" || func == "string=?")
			    return evalStringCompare(args, func);
			if (func == "eqv?") return evalEqv(args);
			if (func == "equal?") return evalEqual(args);
			if (func == "cond")  return evalCond(node);
		    if (func == "clean-environment") {
		        userEnv.clear();
		        cout << "\n\n> Environment cleaned";
		        return nullptr;
		    }  // if
		    if (isTypeCheckFunc(func)) {
		        return evalTypeCheck(args, func.substr(0, func.length() - 1));
		    }  // if
		
		    cout << "\n\n> ERROR (unbound symbol) : " << func;
		    return nullptr;
		}  // evalList
		
//		ASTNode* evalDefine(vector<ASTNode*> &args) {
//		    string name = args[0]->value;
//		
//		    if (systemPrimitives.count(name)) {
//		        cout << "\n\n> ERROR (DEFINE format) : ( define";
//		        for (ASTNode* arg : args) {
//		            cout << "\n  ";
//		            prettyPrint(arg, 2);  // 加 indent
//		        }  // for
//		        cout << "\n)";
//		        return nullptr;
//		    }  // if
//		
//		    userEnv[name] = copyy(args[1]);
//		    //userEnv[name] = args[1];
//		    cout << "\n\n> " << name << " defined";
//		    return nullptr;
//		}  // evalDefine
		ASTNode* evalDefine(vector<ASTNode*> &args) {
		    string name = args[0]->value;
		
		    if (systemPrimitives.count(name)) {
		        cout << "\n\n> ERROR (DEFINE format) : ( define";
		        for (ASTNode* arg : args) {
//		            cout << "\n  ";
		            prettyPrint(arg, 2);
		        }  // for
		        cout << "\n)";
		        return nullptr;
		    }  // if
		

		    ASTNode* valNode = args[1];
		
		    // 斷是否已存在於環境裡 
		    bool in = false;
		    for (auto& kv : userEnv) {
		        if (kv.second == valNode) {   // 指標完全相同 → 正在做別名
		            in = true;
		            break;
		        }  // if
		    }  // for
		
		    if (in) {
		        // 右邊是舊直接共用
		        userEnv[name] = valNode;
		    }  // if
			else {
		        // 複製一份
		        userEnv[name] = copyy(valNode);
		    }  // else

		
		    cout << "\n\n> " << name << " defined";
		    return nullptr;
		}  // evalDefine


		ASTNode* evalCons(vector<ASTNode*> &args) {
		
		    // 如果是 cons 到 nil，回傳 list
		    if (args[1]->type == NodeType::ATOM && args[1]->value == "nil") {
		        ASTNode* listNode = new ASTNode(NodeType::LIST);
		        listNode->children.push_back(args[0]);
		        return listNode;
		    }  // if 
		
		    // 否則就是 dotted pair
		    ASTNode* dotPair = new ASTNode(NodeType::DOT_PAIR);
		    dotPair->children.push_back(args[0]);
		    dotPair->children.push_back(args[1]);
		    return dotPair;
		}  // evalCons
		ASTNode* evalListConstructor(vector<ASTNode*> &args) {
		    ASTNode* listNode = new ASTNode(NodeType::LIST);
		    for (ASTNode* arg : args) {
		        listNode->children.push_back(arg);
		    }  // for
		    return listNode;
		}  // evalListConstructor
	
	    ASTNode* evalCar(vector<ASTNode*> &args) {  // 取出 pair 或 list 的第一個元素
	        
	        ASTNode* arg = args[0];
	        if (arg->type == NodeType::LIST && !arg->children.empty()) {
	            return arg->children[0];
	        }  // if
	        if (arg->type == NodeType::DOT_PAIR) {
	            return arg->children[0];
	        }  // if
	        cout << "\n\n> ERROR (car with incorrect argument type) : ";
	        prettyPrint(arg); 
			return nullptr;
	    }  // evalCar
	
	    ASTNode* evalCdr(vector<ASTNode*> &args) {  //取出 pair 或 list 的最右元素
	        
		
		    ASTNode* arg = args[0];
		
		    // 如果是 LIST
		    if (arg->type == NodeType::LIST) {
		        if (arg->children.empty()) {
		            cout << "\n\n> ERROR (cdr with incorrect argument type) : ";
		            prettyPrint(arg);
		            return nullptr;
		        }  // if
		
		        // 建立新的 LIST，內容是去掉第一個元素後的其餘元素
		        ASTNode* list = new ASTNode(NodeType::LIST);
		        for (size_t i = 1; i < arg->children.size(); ++i) {
		            list->children.push_back(arg->children[i]);
		        }  // for
		
		        return list;
		    }  // if
		
		    // 如果是 DOT_PAIR
		    if (arg->type == NodeType::DOT_PAIR) {
		        ASTNode* right = arg->children[1];
		
		        // 如果是 (x . nil)，應該回傳空 list
		        if (right->type == NodeType::ATOM && right->value == "nil") {
		            return new ASTNode(NodeType::LIST); // 空 list
		        }  // if
		
		        // 如果右邊是串成的 DOT_PAIR轉LIST
		        if (right->type == NodeType::DOT_PAIR) {
		            ASTNode* curr = right;
		            ASTNode* list = new ASTNode(NodeType::LIST);
		
		            while (curr && curr->type == NodeType::DOT_PAIR) {
		                list->children.push_back(curr->children[0]);
		                curr = curr->children[1];
		            }  // while
		
		            // 如果最後是 nil，就可以視為合法 list
		            if (curr->type == NodeType::ATOM && curr->value == "nil") {
		                return list;
		            }  // if
		
		            // 如果最後不是 nil DOTTED PAIR
//		            ASTNode* tail = new ASTNode(NodeType::DOT_PAIR);
//		            tail->children.push_back(list);
//		            tail->children.push_back(curr);
//		            return tail;
					if (list->children.size() == 1) {
					    // 不要包 list，直接用裡面的元素
					    ASTNode* tail = new ASTNode(NodeType::DOT_PAIR);
					    tail->children.push_back(list->children[0]);
					    tail->children.push_back(curr);
					    return tail;
					}  // if
					else {
					    ASTNode* tail = new ASTNode(NodeType::DOT_PAIR);
					    tail->children.push_back(list);
					    tail->children.push_back(curr);
					    return tail;
					}  // else
		        }  // if
		
		        // 其他情況直接回傳右邊
		        return right;
		    }  // if
		
		    // 其他型別報錯
		    cout << "\n\n> ERROR (cdr with incorrect argument type) : ";
		    cout << "\n\n> ";
		    prettyPrint(arg);
		    return nullptr;
	    }  // evalCdr
	    
	    double getNumber(ASTNode* node, bool &isFloat) {
		    if (node->type != NodeType::ATOM) {
		        //cout << "\n\n> ERROR (non-number) : ";
		        prettyPrint(node); 
		        evalError = true;
		        return 0;
		    }  // if
		
		    string val = node->value;
		    try {
		        if (val.find('.') != string::npos) isFloat = true;
		        return stod(val);
		    } catch (...) {
		        //cout << "\n\n> ERROR (non-number) : " << val;
		        evalError = true;
		        return 0;
		    }
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
		        int result = stoi(args[0]->value);
		        for (int i = 1; i < args.size(); ++i) {
		        	if (!isNumberString(args[i]->value)) {
					    cout << "\n\n> ERROR (" << op << " with incorrect argument type) : " << args[i]->value;
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
		    double result = stod(args[0]->value);
		    for (int i = 1; i < args.size(); ++i) {
		    	if (!isNumberString(args[i]->value)) {
					cout << "\n\n> ERROR (" << op << " with incorrect argument type) : " << args[i]->value;
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
		    double prev = getNumber(args[0], isFloat);
		    if (evalError) return nullptr;
		
		    for (int i = 1; i < args.size(); ++i) {
		    	if (!isNumberString(args[i]->value)) {
					cout << "\n\n> ERROR (" << op << " with incorrect argument type) : " << args[i]->value;
					return nullptr;
				}  // if
		        double curr = getNumber(args[i], isFloat);
		        if (evalError) return nullptr;
				// 如果 prev 和 curr 的差異大於0.000001就視為不相等
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
		    for (ASTNode* arg : args) {
		        if (arg->value == "nil") return new ASTNode(NodeType::ATOM, "nil");
		    }  // for
		    return args.back();
		}  // evalAnd
		
		// 只要有不是 nil 的值就直接回傳
		ASTNode* evalOr(const vector<ASTNode*>& args) {
		    for (ASTNode* arg : args) {
		        if (arg->value != "nil") return arg;
		    }  // for
		    return new ASTNode(NodeType::ATOM, "nil");
		}  // evalOr
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
		ASTNode* evalEqv(vector<ASTNode*> &args) {  // 記憶體 
		    ASTNode* a = args[0];
		    ASTNode* b = args[1];
		
		    // 型別不同 → false
		    if (a->type != b->type) return new ASTNode(NodeType::ATOM, "nil");
		
		    // 是 ATOM
		    if (a->type == NodeType::ATOM) {
		        // 是字串一律 false（內容一樣也 false）
		        if (!a->value.empty() && a->value[0] == '"') {
		            return new ASTNode(NodeType::ATOM, "nil");
		        }  // if
		
		        // 數字、布林、symbol 用 value 比較
		        return new ASTNode(NodeType::ATOM, a->value == b->value ? "#t" : "nil");
		    }  // if
		
		    // 同一個指標（記憶體位置相同）
		    if (a == b) return new ASTNode(NodeType::ATOM, "#t");
		
		    // 其他 → false
		    return new ASTNode(NodeType::ATOM, "nil");
		}  // evalEqv
		ASTNode* evalEqual(vector<ASTNode*> &args) {  // 內容 
		    return deepEqual(args[0], args[1]) ?
		        new ASTNode(NodeType::ATOM, "#t") :
		        new ASTNode(NodeType::ATOM, "nil");
		}  // evalEqual
		
		bool deepEqual(ASTNode* a, ASTNode* b) {
		    if (a == b) return true;  // 同一物件 → 一定相等
		
		    if (a->type != b->type) return false;
		
		    if (a->type == NodeType::ATOM) {
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
		
		    // (cond ...) 至少要一個子句
		    if (node->children.size() < 2) {
		        cout << "\n\n> ERROR (COND format) : ( cond\n)";
		        return nullptr;
		    }
		
		    // 依序檢查每個子句
		    for (size_t i = 1; i < node->children.size(); ++i) {
		
		        ASTNode* clause = node->children[i];
		
		        // 子句一定是非空 LIST
		        if (clause->type != NodeType::LIST || clause->children.empty()) {
		            cout << "\n\n> ERROR (COND format) : ( cond";
		            for (size_t j = 1; j < node->children.size(); ++j) {
		                cout << "\n  ";
		                prettyPrint(node->children[j], 2);
		            }
		            cout << "\n)";
		            return nullptr;
		        }
		
		        ASTNode* testNode = clause->children[0];
		        bool isElse = (testNode->type == NodeType::ATOM && testNode->value == "else");
		
		        // else 只能在最後一個子句
		        if (isElse && i != node->children.size() - 1) {
		            cout << "\n\n> ERROR (unbound symbol) : else";
		            return nullptr;
		        }
		
		        // 求值 test（直接交給 eval，不限制型別）
		        ASTNode* testResult = nullptr;
		        if (!isElse) {
		            testResult = eval(testNode);
		            if (!testResult) return nullptr;
		        }
		
		        // test 成立或 else → 執行 body
		        if (isElse || testResult->value != "nil") {
		
		            // 只有 test / else 本身
		            if (clause->children.size() == 1) {
		                return isElse
		                       ? new ASTNode(NodeType::ATOM, "nil")
		                       : testResult;
		            }
		
		            // 有 body：依序 eval，回傳最後結果
		            ASTNode* result = nullptr;
		            for (size_t k = 1; k < clause->children.size(); ++k) {
		                result = eval(clause->children[k]);
		                if (!result) return nullptr;
		            }
		            return result;
		        }
		    }
		
		    // 所有子句皆不成立
		    cout << "\n\n> ERROR (no return value) : ( cond";
		    for (size_t j = 1; j < node->children.size(); ++j) {
		        cout << "\n  ";
		        prettyPrint(node->children[j], 2);
		    }
		    cout << "\n)";
		    return nullptr;
		}


};

bool isExitCommand(ASTNode* node) {
    if(node && node->type == NodeType::LIST && node->children.size() == 1) {
        ASTNode* child = node->children[0];
        if(child && child->type == NodeType::ATOM && child->value == "exit")
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


