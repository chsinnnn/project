// 過第五題 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
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
	        return makeToken(isFloat ? TokenType::FLOAT : TokenType::INT, number, start, col);
	    }  // readNumber()
		Token readString() {
		    int start = pos;
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
		    return makeToken(TokenType::ERROR, "ERROR (no closing quote) : END-OF-LINE encountered", start, col);
		}  // resdString

	    Token readSymbol() {
		    int start = pos;
		    // 遇到任何分隔符號就停
		    while (pos < input.size() && !isSeparator(input[pos])) {
		        pos++;
		        col++;
		    }  // while
		    string symbol = input.substr(start, pos - start);
		    if (symbol == "nil" || symbol == "#f")
		        return makeToken(TokenType::NIL, "nil", start, col);
		    if (symbol == "t" || symbol == "#t")
		        return makeToken(TokenType::T, "#t", start, col);
		    return makeToken(TokenType::SYMBOL, symbol, start, col);
		}  // readSymbol()

};

class Parser {
	private:
	    vector<Token> tokens;
	    int pos;
	public:
	    Parser(const vector<Token>& tokenList) : tokens(tokenList), pos(0) {}
	    ASTNode* parse() {
	        if( pos >= tokens.size() ) return nullptr;
	        return parseSExp();
	    }  // parse()
	    int getPos() {
	        return pos;
	    }  // parse()
	private:
		ASTNode* parseList() {
		    if (errorInCurrentLine) return nullptr;
		    pos++; // 跳過 '('
		    vector<ASTNode*> elements;
		    bool hasDot = false;
		    ASTNode* dotRight = nullptr;
		    bool isFirst = true;  // 判斷是不是第一個 token
		
		    while (pos < tokens.size() && tokens[pos].type != TokenType::RIGHT_PAREN) {
		        if (errorInCurrentLine) return nullptr;
		
		        if (tokens[pos].type == TokenType::DOT) {
		            Token dotToken = tokens[pos];
		            pos++; // 跳過 '.'
		
		            // DOT 出現在第一個位置是錯的
		            if (isFirst) {
		                cout << "\n\n> ";
		                cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "
		                     << dotToken.line << " Column " << dotToken.col << " is >>" << dotToken.value << "<<";
		                globalLine = 1;
		                globalCol = 1;
		                errorInCurrentLine = true;
		                return nullptr;
		            }  // if
		
		            // 如果已經有過 DOT，也錯
		            if (hasDot) {
		                cout << "\n\n> ";
		                cout << "ERROR (unexpected token) : ')' expected when token at Line "
		                     << dotToken.line << " Column " << dotToken.col << " is >>" << dotToken.value << "<<";
		                globalLine = 1;
		                globalCol = 1;
		                errorInCurrentLine = true;
		                return nullptr;
		            }  // if
		
		            hasDot = true;
		            dotRight = parseSExp();
		            if (errorInCurrentLine) return nullptr;
		        }  // if
		        else {
		            if (hasDot) {
		                Token errToken = tokens[pos];
		                cout << "\n\n> ";
		                cout << "ERROR (unexpected token) : ')' expected when token at Line "
		                     << errToken.line << " Column " << errToken.col << " is >>" << errToken.value << "<<";
		                globalLine = 1;
		                globalCol = 1;
		                errorInCurrentLine = true;
		                return nullptr;
		            }  // if
		            ASTNode* elem = parseSExp();
		            if (errorInCurrentLine) return nullptr;
		            elements.push_back(elem);
		        }  // else
		
		        isFirst = false;  // 已處理一個 token
		    }  // while
		
		    // 檢查：如果沒遇到 RIGHT_PAREN 就是錯
		    if (pos >= tokens.size() || tokens[pos].type != TokenType::RIGHT_PAREN) {
		        if (pos < tokens.size()) {
		            Token errToken = tokens[pos];
		            cout << "\n\n> ";
		            cout << "ERROR (unexpected token) : ')' expected when token at Line "
		                 << errToken.line << " Column " << errToken.col << " is >>" << errToken.value << "<<";
		            globalLine = 1;
		            globalCol = 1;
		            errorInCurrentLine = true;
		        }  // if
		        return nullptr;
		    }  // if
		    pos++; // 跳過 ')'
		
		    // 處理 dotted pair
		    if (hasDot) {
		        if ((dotRight->type == NodeType::ATOM && dotRight->value == "nil")
		            || (dotRight->type == NodeType::LIST && dotRight->children.empty())) {
		            ASTNode* listNode = new ASTNode(NodeType::LIST);
		            listNode->children = elements;
		            return listNode;
		        }  // if
		        return buildDottedPair(elements, dotRight);
		    }  // if
		
		    // 純 list
		    ASTNode* listNode = new ASTNode(NodeType::LIST);
		    listNode->children = elements;
		    return listNode;
		}  // parseList()

	    ASTNode* buildDottedPair(vector<ASTNode*>& elems, ASTNode* dotRight) {
	        if(elems.empty()){
	        	cout << "\n\n> ";
	            cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "
	                 << tokens[pos-1].line << " Column " << tokens[pos-1].col << " is >>" << tokens[pos-1].value << "<<";
	            // 有印東西就初始化 
	            // 取得錯誤行號
				int errorLine = tokens[pos-1].line;
				// 跳過該行剩餘 token
				while(pos < tokens.size() && tokens[pos].line == errorLine) {
				    pos++;
				}  // while
	            globalLine = 1;
	        	globalCol = 1;
	        	errorInCurrentLine = true;
				return nullptr;
	        }  // if
	        ASTNode* result = dotRight;
	        for(int i = elems.size()-1; i >= 0; --i){
	            ASTNode* dotPair = new ASTNode(NodeType::DOT_PAIR);
	            dotPair->children.push_back(elems[i]);
	            dotPair->children.push_back(result);
	            result = dotPair;
	        }  // for
	        return result;
	    }  // buildDottedPair()
	    ASTNode* parseSExp() {
	    	if(errorInCurrentLine) return nullptr;  // 已發生錯誤，立即中止解析
	        if( pos >= tokens.size() ) return nullptr;
	        Token token = tokens[pos];
	        if( token.type == TokenType::ERROR ) {
	        	cout << "\n\n> ";
	            cout << token.value << " at Line " << token.line << " Column " << token.col-1 ;
	            pos++;
	            // 有印東西就初始化 
	            // 取得錯誤行號
				int errorLine = token.line;
				// 跳過該行剩餘 token
				while(pos < tokens.size() && tokens[pos].line == errorLine) {
				    pos++;
				}  // while
	            globalLine = 1;
	        	globalCol = 1;
	        	errorInCurrentLine = true;
	            return nullptr;
	        }  // if
	        if( token.type == TokenType::RIGHT_PAREN ) {
	        	cout << "\n\n> ";
	            cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "
	                 << token.line << " Column " << token.col << " is >>" << token.value << "<<";
	            pos++;
	            // 取得錯誤行號
				int errorLine = token.line;
				// 跳過該行剩餘 token
				while(pos < tokens.size() && tokens[pos].line == errorLine) {
				    pos++;
				}  // while
	            // 有印東西就初始化 
	            globalLine = 1;
	        	globalCol = 1;
	        	errorInCurrentLine = true;
	            return nullptr;
	        }  // if
	        if( token.type == TokenType::FLOAT ) {
	            pos++;
	            double num = atof(token.value.c_str());
	            char buf[64];
	            sprintf(buf, "%.3f", num);
	            return new ASTNode(NodeType::ATOM, string(buf));
	        }  // if
	        else if(token.type == TokenType::INT || token.type == TokenType::STRING ||
	                token.type == TokenType::SYMBOL || token.type == TokenType::T ||
	                token.type == TokenType::NIL) {
	            pos++;
	            string val = token.value;
	            if(token.type == TokenType::INT && !val.empty() && val[0] == '+') {
        			val = val.substr(1);
    			} // if 
	            return new ASTNode(NodeType::ATOM, val);
	        }  // else if
	        if( token.type == TokenType::LEFT_PAREN )
	            return parseList();
	        if( token.type == TokenType::QUOTE ) {
	            pos++;
	            ASTNode* quoteNode = new ASTNode(NodeType::QUOTE);
	            quoteNode->children.push_back(parseSExp());
	            return quoteNode;
	        }  // if
	        cout << "\n\n> ";
	        cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "
	             << token.line << " Column " << token.col << " is >>" << token.value << "<<";
	        pos++;
	        // 取得錯誤行號
			int errorLine = token.line;
			// 跳過該行剩餘 token
			while(pos < tokens.size() && tokens[pos].line == errorLine) {
			    pos++;
			}  // while
	        // 有印東西就初始化 
            globalLine = 1;
        	globalCol = 1;
        	errorInCurrentLine = true;
	        return nullptr;
	    }  // parseSExp()
};
	
void printWithoutNewline(const string &s) {
	cout << s;
}  // printWithoutNewline

// prettyPrint 印出 S-expression
void prettyPrint(ASTNode* node, int currIndent = 0) {
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
    cin.ignore(); // 忽略測試數字後換行
    
    // 外層讀取每行輸入
    while(getline(cin, input)) {
        buffer += input + "\n";
        //cout << buffer << " 508508\n";
        // 當 buffer 非空解析S-expression
        while(true) {
        	
            // 建立 Lexer，初始行與欄皆從 1 開始計算
            Lexer lexer(buffer, globalLine, 1);
            vector<Token> tokens;
            Token token;
            // 取得所有 token（直到 END）
            while(true) {
                token = lexer.getNextToken();
                tokens.push_back(token);
                if(token.type == TokenType::END)
                    break;
            }  // while
            // 看是不是一開始就有錯誤訊息 有的話直接出去 
            if (!tokens.empty() && tokens[0].type == TokenType::ERROR) {
//            	cout << "651651\n"; 
			    auto &t = tokens[0];
				cout << "\n\n> " << t.value << " at Line " << t.line << " Column " << t.col;
			    buffer.clear();
			    globalLine = 1;
			    globalCol  = 1;
			    errorInCurrentLine = true;
			    break;  // 跳出while讀下一行
			}  // if
			// 這行只有一個單引號先不報錯
			if (tokens.size() == 2 && tokens[0].type == TokenType::QUOTE && tokens[1].type == TokenType::END) {
			    break;
			}  // if
			bool lexError = false;  // token是不是帶有錯誤   
			for (auto &t : tokens) {
//				if( tokens[0].type != TokenType::ERROR )  break;
			    if (t.type == TokenType::ERROR && t.line != 1) {  // 印出錯誤訊息 怪怪的 
//			    	if( doterror )  cout << "\n\n> " << t.value;
//			    	else  
					cout << "\n\n> " << t.value << " at Line " << t.line << " Column " << t.col;  
			        buffer.clear();
			        globalLine = 1;
			        globalCol  = 1;
			        errorInCurrentLine = true;
			        lexError = true;
			        doterror = false;
			        break;
			    }  // if 
			}  // for
			if (lexError) {
			    // 跳出while讀下一行
			    break;
			}  // if 
			
//            for( int i = 0 ; i < tokens.size() ; i++ ) {
//            	cout << tokens[i].value << "     523523\n";
//			}  // for
            // 若僅有 END token，表示沒有可解析的 S-expression
			if (tokens.size() == 1 && tokens[0].type == TokenType::END) {
			    buffer = "";
			    //那行空白的話find_first_not_of 回傳 npos不累計 globalLine 1 丟掉空白行 
			    if (input.find_first_not_of(" \t\r") != string::npos) {  
			        globalLine = lexer.getLine();
			    }  // if
			    break;
			}  // if
            // 檢查是否為不完整的輸入（例如左括號數大於右括號數）
            // 如果右比較多才要輸出錯誤訊息 
            int leftParenCount = 0, rightParenCount = 0;
            for(auto &t : tokens) {
                if(t.type == TokenType::LEFT_PAREN)
                    leftParenCount++;
                else if(t.type == TokenType::RIGHT_PAREN)
                    rightParenCount++;
            }  // for
            if( leftParenCount > rightParenCount) {
                break; // 等待下一行輸入補齊
            }  // if
            errorInCurrentLine = false;
            Parser parser(tokens);
            ASTNode* expr = parser.parse();
            // 過程中發生錯誤，捨棄本次輸入（即整個 buffer）
            if(errorInCurrentLine) {
//	            for( int i = 0 ; i < tokens.size() ; i++ ) {
//	            	cout << tokens[i].value << "     551551\n";
//				}  // for
			    buffer = "";
                globalLine = 1;
            	globalCol = 1;
                break;
            }  // if
            if (expr != NULL ) {
                // 若遇到 (exit) 則結束程式
                if(isExitCommand(expr)) {
                    exitFlag = true;
                    delete expr;
                    buffer = "";
                    break;
                }  // if
                cout << "\n\n> ";
                prettyPrint(expr, 0);
                globalLine = 1;
            	globalCol = 1;
                delete expr;
                if (!tokens.empty() && tokens[0].type == TokenType::STRING) {  // 如果是string整個清空 
			        buffer.clear();
			    }  // if
			    else {
			        int tokenConsumed = parser.getPos();
			        int consumedPos = 0;
			        if (tokenConsumed > 0) {
			            Token &lastToken = tokens[tokenConsumed - 1];
			            consumedPos = lastToken.pos + (int)lastToken.value.size();
			        }  // if 
			        buffer = buffer.substr(consumedPos);
			    }  // else
			    // reset 行列狀態，準備下一次 parse
			    globalLine = 1;
			    globalCol  = 1;
			    continue;
            }  // if
            else {
            	if(errorInCurrentLine)  globalLine = 1;
            	else  globalLine = lexer.getLine();
            	//globalCol = lexer.getCol();
			}  // else
            // 找出最後一個非 END token 的結束位置
			int tokenConsumed = parser.getPos();
			int consumedPos = 0;
			if (tokenConsumed > 0) {
			    Token &lastToken = tokens[tokenConsumed - 1];
			    // pos 是這個 token 在原始 input 裡的起始 index
			    consumedPos = lastToken.pos + (int)lastToken.value.size();
			}  // if
			buffer = buffer.substr(consumedPos);

            // 若剩餘部分全為空白，則捨棄
            bool onlySpace = true;
            for(char c : buffer) {
                if(!isspace(c)) { 
					onlySpace = false; 
					break; 
				}  // if
            }  // for
            if(onlySpace) { 
				buffer = ""; 
				break; 
			}  // if
        }  // while
        if(exitFlag) {
        	cout << "\n\n> ";
        	break;
		}  // if
            
    }  // while
    // buffer.empty() && 
    if(!exitFlag) {
    	if( cin.eof() )
        	cout << "\n\n> ERROR (no more input) : END-OF-FILE encountered";
	}  // if
    	
    cout << "\nThanks for using OurScheme!";
    return 0;
}  // main
