#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Tree_parse.h"
using namespace std;


struct Rule {
    string first;
    vector<vector<string>> second;

};

class syntactical_analyzer
{
private:

    struct rule {
        string first;
        vector<string> second;
    };
    struct Situation {
        Rule rule;
        int cur_point; // текущая точка; 
        int parent_point; // точка в строке, а не в правиле; 
        // точка указывает на строку в векторе  
        bool flag = false;
        Situation() {};
        Situation(Rule rule, int point, int p_point) : rule(rule), cur_point(point), parent_point(p_point) {};
        bool operator == (Situation another) {
            bool flag = true;
            if (rule.second.size() == another.rule.second.size()) {
                for (int i = 0; i < another.rule.second.size(); i++) {
                    if (another.rule.second[i] != rule.second[i]) {
                        flag = false;
                        break;
                    }
                }
            }
            else {
                flag = false;
            }
            if ((cur_point == another.cur_point) && (parent_point == another.parent_point) && flag) {
                return true;
            }
            else {
                return false;
            }
        }
    };

    vector<Rule> Grammar;
    vector<vector<Situation>> States;
    vector<Rule> rules; // их перепишем потом в дерево разбора;  
    int coumt_states = 0;

public:

    syntactical_analyzer() {}
    //Когда точка перед нетерминальным символом. Тогда ситуация, в которой из этого нетерминала
    //что-то выводится, добавляется в текущее состояние(то есть состояние, например, j - то сколько символов прочли)
    // Пример:  [A -> a*B betta, i] принадлежит Dj, тогда добавляем ситуацию, [B -> *gamma, j] принадлежит Dj, заметим,
    //что точку прочтения у родителя сместили с i позиции на j;
    bool find(vector<Situation>& st, Situation s) {
        bool flag = false;
        for (int i = 0; i < st.size(); i++) {
            flag = false;
            //Проверяем, что содержится Procedure

            if (st[i].rule.first == s.rule.first) {
                int count_same = 0;
                for (int j = 0; j < st[i].rule.second.size(); j++) {
                    for (int k = 0; k < s.rule.second.size(); k++) {
                        if (st[i].rule.second[j] == s.rule.second[k]) {
                            count_same++;
                        }
                    }
                }
                if (count_same == st[i].rule.second.size()) {
                    flag = true;
                }
            }

            if ((st[i].cur_point == s.cur_point)
                && (st[i].parent_point == s.parent_point)
                && flag) {
                return true;
            }
        }

        return false;
    }
    int size_s(string s) {
        int count = 0;
        for (int i = 0; i < s.size(); i++) {
            count++;
        }
        return count;
    }
    void Predict(int j) {
        vector <Situation> new_situations;
        for (int i = 0; i < States[j].size(); i++) {
            new_situations.push_back(States[j][i]);
        }
        //Берем состояние, Пробегаемся по его ситуациям и добавляем ситуации из грамматики, если в (situation)  ситуации из нашего состояния
        //точка стоит перед нетерминальным символом(то есть rule.second[situation.cur_point] точка указывает на строку обозначающая нетерминал).
        // Из которой потом что-то ещё выведем 

        for (Situation situation : States[j]) {
            for (Rule rule : Grammar) {

                for (int i = 0; i < situation.rule.second.size(); i++) {
                    if ((situation.cur_point != situation.rule.second[i].size())
                        && (rule.first == situation.rule.second[i][situation.cur_point])) {
                        for (int k = 0; k < rule.second.size(); k++) {
                            Rule new_r;
                            new_r.first = rule.first;
                            new_r.second.push_back(rule.second[k]);

                            Situation new_st = Situation(new_r, 0, j);

                            if (!find(new_situations, new_st)) {
                                new_situations.push_back(new_st);
                            }
                            clear_rule(new_r);
                        }
                    }
                }
            }
        }
        if (new_situations.size() != 0) {
            for (int i = 0; i < new_situations.size(); i++) {
                if (!find(States[j], new_situations[i])) {
                    States[j].push_back(new_situations[i]);
                }
            }
        }
    }
    //Если встретили терминальный символ, то есть точка перед терминальным символом, тогда
    //точку смещаем за терминальный символ. Эта ситуация относиться к следующему состоянию, так как прочли еще 
    //один новый символом. Пример: [A -> alph*aBetta, i] принадлежит Dj. Тогда [A -> alpha a*Betta, i] принадлежит Dj+1;
    void Scan(int j, string& x) {
        //пробегаемся по предыдущему состоянию, добавляем в новое состояние то, в котором точка стоит перед терминальным символом x;
        //то есть точка указывает на строку x в векторе

        vector<Situation> new_state;
        for (Situation situation : States[j]) {
            //Проверяем, что точка стоит перед x;
            for (int i = 0; i < situation.rule.second.size(); i++) {
                if ((situation.cur_point != situation.rule.second[i].size())
                    && (x == situation.rule.second[i][situation.cur_point])) {
                    //Добавляем новую ситуацию в новое состояние, смещаем точку;

                    Rule new_r;
                    new_r.first = situation.rule.first;
                    new_r.second.push_back(situation.rule.second[i]);
                    Situation new_sit = Situation(new_r, situation.cur_point + 1, situation.parent_point);

                    // if (!find(new_state, new_sit)) {
                    new_state.push_back(new_sit);
                    // }
                    clear_rule(new_r);

                }
            }
        }
        //if (new_state.size() != 0) {
        States.push_back(new_state);
        //}
        new_state.clear();
        new_state.shrink_to_fit();
    }

    //Если точка стоит в конце, а у родителя стоит перед нетерминальным символом,
    //тогда смещаем точку у родитьельского узла, добавляем в текущее состояние
    //[B -> gamma*, j] принадлежит Dk и [A-> alpha*Bbetta, i] принадлежит Dj. 
    // Тогда добавляем в Dk ситуацию [A-> alphaB*betta, i], где точка стоит уже после нетерминального символа;
    void Complete(int j) {
        vector<Situation> new_situations;
        for (int i = 0; i < States[j].size(); i++) {
            new_situations.push_back(States[j][i]);
        }

        for (Situation situation : States[j]) {
            //Проверка, что точка не стоит в конце, тогда продолжаем;
            if ((situation.cur_point != situation.rule.second[0].size())) {
                continue;
            }
            //Пробегаемся по родительскому состоянию, если нашли такое состояние, в котором перед нетерминалом стоит точка,
            //а из нетерминала ситуации текущего состояния выодится что-то и в конце стоит точка, то добавляем из родительского состояния в текущее состояние ситуацию,
            //сдвигая, при этом точку за нетерминал;
            for (Situation situation_in_parent_State : States[situation.parent_point]) {
                for (int i = 0; i < situation_in_parent_State.rule.second.size(); i++) {
                    if ((situation_in_parent_State.cur_point
                        != situation_in_parent_State.rule.second[i].size()) &&
                        (situation_in_parent_State.rule.second[i][situation_in_parent_State.cur_point] ==
                            situation.rule.first)) {

                        Rule new_r;
                        new_r.first = situation_in_parent_State.rule.first;
                        new_r.second.push_back(situation_in_parent_State.rule.second[i]);
                        Situation new_st = Situation(new_r, situation_in_parent_State.cur_point + 1,
                            situation_in_parent_State.parent_point);
                        if (!find(new_situations, new_st)) {
                            new_situations.push_back(new_st);
                        }
                        clear_rule(new_r);

                    }

                }
            }
        }
        if (new_situations.size() != 0) {
            for (int i = 0; i < new_situations.size(); i++) {
                if (!find(States[j], new_situations[i])) {
                    States[j].push_back(new_situations[i]);
                }
            }
        }
    }
    void clear_rule(Rule& add_rule) {
        add_rule.first.clear();

        for (int i = 0; i != add_rule.second.size(); i++) {
            for (int j = 0; j != add_rule.second[i].size(); j++) {
                add_rule.second[i][j].clear();
                add_rule.second[i][j].shrink_to_fit();
            }
            add_rule.second[i].clear();
            add_rule.second[i].shrink_to_fit();
        }

        add_rule.second.clear();
        add_rule.second.shrink_to_fit();
    }
    void clear_v(vector<string>& v) {
        for (int i = 0; i < v.size(); i++) {
            v[i].clear();
            v[i].shrink_to_fit();
        }
        v.clear();
        v.shrink_to_fit();
    }
    void Initialisation_grammar() {
        Rule add_rule;
        vector<string> right;
        /////////////////////////////// S -> Procedure
        /////////////////////////////// Procedure -> Begin Descriptions Operators End
        add_rule.first = "Procedure";
        right.push_back("Begin");
        right.push_back("Descriptions");
        right.push_back("Operators");
        right.push_back("End");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////  Begin -> procedure ProcedureName; begin
        add_rule.first = "Begin";
        right.push_back("procedure");
        right.push_back("ProcedureName");
        right.push_back(";");
        right.push_back("begin");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////   End -> end
        add_rule.first = "End";
        right.push_back("end");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////   ProcedureName -> Id
        add_rule.first = "ProcedureName";
        right.push_back("Id");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////   Descriptions -> var DescrList
        add_rule.first = "Descriptions";
        right.push_back("var");
        right.push_back("DescrList");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////    DescrList -> Descr | Descr DescrList
        add_rule.first = "DescrList";
        right.push_back("Descr");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("Descr");
        right.push_back("DescrList");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        /////////////////////////////// Descr -> VarList : Type;
        add_rule.first = "Descr";
        right.push_back("VarList");
        right.push_back(":");
        right.push_back("Type");
        right.push_back(";");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////   VarList -> Id | Id, VarList
        add_rule.first = "VarList";
        right.push_back("Id");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("Id");
        right.push_back(",");
        right.push_back("VarList");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////   Type -> integer
        add_rule.first = "Type";
        right.push_back("integer");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);


        ///////////////////////////////    Operators -> Op | Op Operators
        add_rule.first = "Operators";
        right.push_back("Op");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("Op");
        right.push_back("Operators");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////    Op -> Id : = Expr; | case Expr of Options end;
        add_rule.first = "Op";
        right.push_back("Id");
        right.push_back(":=");
        right.push_back("Expr");
        right.push_back(";");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("case");
        right.push_back("Expr");
        right.push_back("of");
        right.push_back("Options");
        right.push_back("end");
        right.push_back(";");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////    Options -> Const : Operators | Const : Operators Options
        add_rule.first = "Options";

        right.push_back("Const");
        right.push_back(":");
        right.push_back("Operators");
        right.push_back("Options");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("Const");
        right.push_back(":");
        right.push_back("Operators");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////   Expr -> SimpleExpr | SimpleExpr + Expr | SimpleExpr - Expr
        add_rule.first = "Expr";
        right.push_back("SimpleExpr");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("SimpleExpr");
        right.push_back("+");
        right.push_back("Expr");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("SimpleExpr");
        right.push_back("-");
        right.push_back("Expr");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////   SimpleExpr -> Id | Const | (Expr)
        add_rule.first = "SimpleExpr";
        right.push_back("Id");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("Const");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("(");
        right.push_back("Expr");
        right.push_back(")");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////    Id -> Letter | Letter Id

        add_rule.first = "Id";
        right.push_back("Letter");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("Letter");
        right.push_back("Id");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////    Const Digit | Digit Const

        add_rule.first = "Const";
        right.push_back("Digit");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("Digit");
        right.push_back("Const");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////     Digit 0 | 1 | 2 | 3 | 4| 5 | 6 | 7 | 8 | 9 
        add_rule.first = "Digit";
        right.push_back("0");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("1");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("2");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("3");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("4");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("5");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("6");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("7");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("8");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("9");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        ///////////////////////////////  Letter ->  
        // a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z
        // A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z
        add_rule.first = "Letter";
        right.push_back("a");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("b");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("c");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("d");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("e");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("f");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("g");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("h");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("i");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("j");
        add_rule.second.push_back(right);
        clear_v(right);
        // r | s | t | u | v | w | x | y | z

        right.push_back("k");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("l");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("m");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("n");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("o");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("p");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("q");
        add_rule.second.push_back(right);
        clear_v(right);
        // u | v | w | x | y | z
        right.push_back("r");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("s");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("t");
        add_rule.second.push_back(right);
        clear_v(right);

        ///////////////////////////////////////////////////
        // u | v | w | x | y | z
        right.push_back("u");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("v");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("w");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("x");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("y");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("z");
        add_rule.second.push_back(right);
        clear_v(right);
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z
        right.push_back("A");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("B");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("C");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("D");
        add_rule.second.push_back(right);
        clear_v(right);
        //////////////////////////////////////////
        right.push_back("E");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("F");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("G");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("H");
        add_rule.second.push_back(right);
        clear_v(right);
        // P | Q | R | S | T | U | V | W | X | Y | Z
        right.push_back("I");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("J");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("K");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("L");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("M");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("N");
        add_rule.second.push_back(right);
        clear_v(right);
        ///////////////////////////////////////////////
        right.push_back("O");
        add_rule.second.push_back(right);
        clear_v(right);
        // Y | Z
        right.push_back("P");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("Q");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("R");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("S");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("T");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("U");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("V");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("W");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("X");
        add_rule.second.push_back(right);
        clear_v(right);
        //////////////////////////////////////
         // Y | Z
        right.push_back("Y");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("Z");
        add_rule.second.push_back(right);
        clear_v(right);
        ////////////////////////////////////////
        Grammar.push_back(add_rule);
        clear_rule(add_rule);
    }
    /*void Initialisation_grammar1() {
        Rule add_rule;
        vector<string> right;
        /////////////////////////////// E1 -> E


        /////////////////////////////// E -> T + E | T
        add_rule.first = "E";
        right.push_back("T");
        right.push_back("+");
        right.push_back("E");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("T");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);



        //////////////////////////////// T -> F*T|F
        add_rule.first = "T";
        right.push_back("F");
        right.push_back("*");
        right.push_back("T");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("F");
        add_rule.second.push_back(right);
        clear_v(right);

        Grammar.push_back(add_rule);
        clear_rule(add_rule);

        //////////////////////////////////// F -> (E)
        add_rule.first = "F";
        right.push_back("(");
        right.push_back("E");
        right.push_back(")");
        add_rule.second.push_back(right);
        clear_v(right);

        right.push_back("a");
        add_rule.second.push_back(right);
        clear_v(right);
        Grammar.push_back(add_rule);
        clear_rule(add_rule);
    }*/
    void Start_Initialization() {
        Initialisation_grammar();

        vector<Situation> new_situations;
        States.push_back(new_situations);

        Rule add_rule;
        vector<string> right;

        add_rule.first = "Procedure";
        right.push_back("Begin");
        right.push_back("Descriptions");
        right.push_back("Operators");
        right.push_back("End");
        add_rule.second.push_back(right);

        clear_v(right);
        States[0].push_back(Situation(add_rule, 0, 0));
        clear_rule(add_rule);

        int current_size;

        do {
            current_size = States[0].size();
            Predict(0);
            Complete(0);
        } while (States[0].size() != current_size);
    }
    //вызов функции symbol_analyze идет после прочтения очередного
    //символа программой в функции f, где считываем слово посимвольно;
    //то есть после каждого вызова встроенной функции считывания символа из файла; 

    void symbol_analyze(string& type_lexeme, string& lexeme, int& index_states) {
        size_t current_size;
        //если лексема это ключевое слово, то обраюатываем как целое слово
        //иначе посимвольно идет обработка
        //передаем токен
        //в токене есть информация
        //сохраняем в грамматику не только буковки

        if ((type_lexeme == "key_word") || (type_lexeme == "operator") ||
            (type_lexeme == "separator")) {
            Scan(index_states - 1, lexeme);
            coumt_states++;
            do {
                current_size = States[index_states].size();
                Predict(index_states);
                Complete(index_states);
            } while (States[index_states].size() != current_size);

            index_states++;
        }
        else {
            string help;

            for (int i = 0; i < lexeme.size(); ++i) {
                help = lexeme[i];
                Scan(index_states - 1, help);
                coumt_states++;
                do {
                    current_size = States[index_states].size();
                    Predict(index_states);
                    Complete(index_states);
                } while (States[index_states].size() != current_size);
                index_states++;

                help.clear();
                help.shrink_to_fit();
            }
        }
    }








    bool find1(vector<Situation>& st, Situation s) {
        bool flag = false;
        for (int i = 0; i < st.size(); i++) {
            flag = false;
            //Проверяем, что содержится Procedure

            if (st[i].rule.first == s.rule.first) {
                int count_same = 0;
                if (st[i].rule.second[0].size() == s.rule.second[0].size())
                    for (int j = 0; j < st[i].rule.second.size(); j++) {
                        if (st[i].rule.second[0][j] == s.rule.second[0][j]) {
                            count_same++;
                        }
                    }
                if (count_same == st[i].rule.second.size()) {
                    flag = true;
                }
            }

            if ((st[i].cur_point == s.cur_point)
                && (st[i].parent_point == s.parent_point)
                && flag) {
                return true;
            }
        }

        return false;
    }
    vector<vector< Situation> > Const; 
    vector<vector< Situation> > id;
    vector<vector< Situation> > options;
    vector<vector< Situation> > varList;
    vector<vector< Situation> > descrList;
    vector<vector< Situation> > operators;
    vector<vector< Situation> > expr;
    bool check(vector<string> Digits) {
        Rule add_rule; 
        vector<string> right; 
        
       
        add_rule.first = "Procedure";

        right.push_back("Begin"); 
        right.push_back("Descriptions"); 
        right.push_back("Operators"); 
        right.push_back("End"); 
        add_rule.second.push_back(right);  
        clear_v(right); 
        Situation s(add_rule, 4, 0); 
        clear_rule(add_rule);
        
        
   
        
       /* Rule add_rule;
        vector<string> right;

        add_rule.first = "VarList";
        right.push_back("Id");
        add_rule.second.push_back(right);
        clear_v(right);
        Situation s(add_rule, 1, 0);
        clear_rule(add_rule);

        add_rule.first = "VarList";
        right.push_back("Id");
        right.push_back(",");
        right.push_back("VarList");
        add_rule.second.push_back(right);
        clear_v(right);
        
        add_rule.second.push_back(right);
        clear_v(right);

        Situation s1 (add_rule, 3, 0);
        clear_rule(add_rule);*/

        if (find1(States[States.size() - 1], s)) {
           
            int p_in_str = coumt_states;

            for (int i = 0; i < States.size(); i++) {
                for (int j = 0; j < States[i].size(); j++) {
                    States[i][j].rule.second[0].insert(States[i][j].rule.second[0].begin(), "_");
                }
            }
            varList.resize(States.size());
            for (int i = 0; i < varList.size(); i++) {
                for (int j = 0; j < States[i].size(); j++)
                    if(States[i][j].rule.first == "VarList")
                        varList[i].push_back(States[i][j]);
            }
            descrList.resize(States.size());
            for (int i = 0; i < descrList.size(); i++) {
                for (int j = 0; j < States[i].size(); j++)
                    if (States[i][j].rule.first == "DescrList")
                        descrList[i].push_back(States[i][j]);
            }
            operators.resize(States.size());
            for (int i = 0; i < operators.size(); i++) {
                for (int j = 0; j < States[i].size(); j++)
                    if (States[i][j].rule.first == "Operators")
                        operators[i].push_back(States[i][j]);
            }
            expr.resize(States.size());
            for (int i = 0; i < expr.size(); i++) {
                for (int j = 0; j < States[i].size(); j++)
                    if (States[i][j].rule.first == "Expr")
                        expr[i].push_back(States[i][j]);
            }
            options.resize(States.size());
            for (int i = 0; i < options.size(); i++) {
                for (int j = 0; j < States[i].size(); j++)
                    if (States[i][j].rule.first == "Options")
                        options[i].push_back(States[i][j]);
            }
            id.resize(States.size());
            for (int i = 0; i < id.size(); i++) {
                for (int j = 0; j < States[i].size(); j++)
                    if (States[i][j].rule.first == "Id")
                        id[i].push_back(States[i][j]);
            }
            Const.resize(States.size());
            for (int i = 0; i < Const.size(); i++) {
                for (int j = 0; j < States[i].size(); j++)
                    if (States[i][j].rule.first == "Const")
                        Const[i].push_back(States[i][j]);
            }
            ofstream fout;
            fout.open("states.txt");
            for (int i = 0; i < States.size(); i++) {
                fout << "\n" << i  << " \n";
                for (int j = 0; j < States[i].size(); j++) {
                    fout << States[i][j].rule.first  << "   - >  ";
                    for (int k = 0; k < States[i][j].rule.second[0].size(); k++) {
                        fout << States[i][j].rule.second[0][k] << " ";
                    }
                    fout << States[i][j].cur_point << " " << States[i][j].parent_point;
                    fout << " \n";
                }
                
            }
            fout.close();
            add_rule.first = "Procedure";
            right.push_back("_");
            right.push_back("Begin");
            right.push_back("Descriptions");
            right.push_back("Operators");
            right.push_back("End");
            add_rule.second.push_back(right);
            clear_v(right);
            /*add_rule.first = "VarList";
            right.push_back("_");
            right.push_back("Id");
            right.push_back(",");
            right.push_back("VarList");
            add_rule.second.push_back(right);
            clear_v(right);*/
            Situation s3(add_rule, 4, 0);
            clear_rule(add_rule);
            this->right_parse(s3, p_in_str); 
           
          
           
            return true; 
            
        } 
        else 
             
            return false; 
    } 
    void clear_rule1(rule &r) {
        r.first.clear();
        r.first.shrink_to_fit();
        for (int i = 0; i < r.second.size(); i++) {
            r.second[i].clear();
            r.second[i].shrink_to_fit();
        }
        r.second.clear();
        r.second.shrink_to_fit();
    }
    bool ravenst_r(rule f, rule s) {
        if (f.first == s.first) {
            if (f.second.size() == s.second.size()) {
                for (int i = 0; i < f.second.size(); i++) {
                    if (f.second[i] != s.second[i]) {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }
    void rules_for_tree(vector<vector<string>>&build_tree) {
        build_tree.resize(rules.size());

        for (int i = 0; i < rules.size(); i++) { 
            build_tree[i].push_back(rules[i].first);
            for (int j = 1; j < rules[i].second[0].size(); j++) {
                build_tree[i].push_back(rules[i].second[0][j]);
            }
        }
      
       /* for (int i = 0; i < build_tree.size(); i++) {
            cout << build_tree[i][0] << "  ->  ";
            for (int j = 1; j < build_tree[i].size(); j++) {
                cout << build_tree[i][j] << " ";
            }
            cout << "\n";
        }*/
    }

    //Определяем, что перед нами терминал или нетерминал; 
    //Если нетерминал выдаст 0, если терминал, то выдаст 1;
    int is_terminal(string s) {
        string alph = "abcdefghijklmnopqrstuvwxyz";
        string Alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        string Digit = "0123456789";

        for (int i = 0; i < alph.size(); i++) {
            if (s[0] == alph[i]) {
                return 1;
            }
        }

        //если заглавная, строчная буквы, разделители;
        if (s.size() == 1) {
            for (int i = 0; i < Alph.size(); i++) {
                if (s[0] == Alph[i]) {
                    return 1;
                }
            }
            for (int i = 0; i < Digit.size(); i++) {
                if (s[0] == Digit[i]) {
                    return 1;
                }
            }
            if ((s[0] == ';') || (s[0] == ':') || (s[0] == ',') || (s[0] == '+') || (s[0] == '-')
                || (s[0] == '(') || (s[0] == ')')) {
                return 1;
            }
        }

        if (s == ":=") {
            return 1;
        }
        
        return 0;
    }
    bool raven(vector<string> f, vector<string> s) {
        if (f.size() == s.size()) {
            for (int i = 0; i < f.size(); i++) {
                if (f[i] != s[i]) {
                    return false;
                }
            }
            return true;
        }
        return false;

    }

    //Проверка, что у переданной в нашу функцию ситуации
    //есть в r-ом состоянии ситуация, где перед нетерминалом,
    //(в переданной сюда ситуации будет слева нетерминал)
    //стоит точка
    bool check_situation(Situation st, int r, Situation cur_st) {
                                       //r
        for (Situation cur : States[st.parent_point]) {
           
               if  ((cur.rule.second[0][cur.cur_point] == cur_st.rule.second[0][r-1]) &&
                ((cur.rule.first == cur_st.rule.first) && raven(cur.rule.second[0], cur_st.rule.second[0])) ) {

                    return true;
            }
        }

        return false;
    }

    int size_vect(vector<string> s) {
        int count = 0;
        for (int i = 0; i < s.size(); i++) {
            count++;
        }
        return count;
    }


    //Найти ситуацию в состоянии с, ([xk -> gamma*, r]) где точка в правиле стоит в конце,
    // также где r такое, что в состоянии r есть ситуация, в которой точка стоит перед xk
    // Функция выдает ситуацию, удовлетворяющая вышеописанным условиям;
    Situation nessasery_situation(int c, int r, Situation cur_st) {
        //Пробешаемся по ситуации c, проверяя условия для каждой ситуации;
        if (cur_st.rule.second[0][r] == "Const") {
            for (int i = Const[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации; expr
                if ((((Const[c][i].cur_point == size_vect(Const[c][i].rule.second[0]) - 1)
                    && (Const[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(Const[c][i], r, cur_st) && (Const[c][i].rule.second[0].size() == 3) && (Const[c][i].flag != true)) {
                        Const[c][i].flag = true;
                        return Const[c][i];
                    }
                }
            }
            for (int i = Const[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации;
                if ((((Const[c][i].cur_point == size_vect(Const[c][i].rule.second[0]) - 1)
                    && (Const[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(Const[c][i], r, cur_st) && (Const[c][i].flag != true) && (Const[c][i].rule.second[0].size() == 2)) {
                        Const[c][i].flag = true;
                        return Const[c][i];
                    }
                }
            }

        }
        else if (cur_st.rule.second[0][r] == "Id") {
            for (int i = id[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации; expr
                if ((((id[c][i].cur_point == size_vect(id[c][i].rule.second[0]) - 1)
                    && (id[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(id[c][i], r, cur_st) && (id[c][i].rule.second[0].size() == 3) && (id[c][i].flag != true)) {
                        id[c][i].flag = true;
                        return id[c][i];
                    }
                }
            }
            for (int i = id[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации;
                if ((((id[c][i].cur_point == size_vect(id[c][i].rule.second[0]) - 1)
                    && (id[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(id[c][i], r, cur_st) && (id[c][i].flag != true) && (id[c][i].rule.second[0].size() == 2)) {
                        id[c][i].flag = true;
                        return id[c][i];
                    }
                }
            }

        }
        else if (cur_st.rule.second[0][r] == "Options") {
            for (int i = options[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации; expr
                if ((((options[c][i].cur_point == size_vect(options[c][i].rule.second[0]) - 1)
                    && (options[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(options[c][i], r, cur_st) && (options[c][i].rule.second[0].size() == 5) && (options[c][i].flag != true)) {
                        options[c][i].flag = true;
                        return options[c][i];
                    }
                }
            }
            for (int i = options[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации;
                if ((((options[c][i].cur_point == size_vect(options[c][i].rule.second[0]) - 1)
                    && (options[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(options[c][i], r, cur_st) && (options[c][i].flag != true) && (options[c][i].rule.second[0].size() == 4)) {
                        options[c][i].flag = true;
                        return options[c][i];
                    }
                }
            }

        }
        else if (cur_st.rule.second[0][r] == "Expr") {
            for (int i = expr[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации; expr
                if ((((expr[c][i].cur_point == size_vect(expr[c][i].rule.second[0]) - 1)
                    && (expr[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(expr[c][i], r, cur_st) && (expr[c][i].rule.second[0].size() == 4) && (expr[c][i].flag != true)) {
                        expr[c][i].flag = true;
                        return expr[c][i];
                    }
                }
            }
            for (int i = expr[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации;
                if ((((expr[c][i].cur_point == size_vect(expr[c][i].rule.second[0]) - 1)
                    && (expr[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(expr[c][i], r, cur_st) && (expr[c][i].flag != true) && (expr[c][i].rule.second[0].size() == 2)) {
                        expr[c][i].flag = true;
                        return expr[c][i];
                    }
                }
            }

        }
        else if (cur_st.rule.second[0][r] == "Operators") {
            for (int i = operators[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации;
                if ((((operators[c][i].cur_point == size_vect(operators[c][i].rule.second[0]) - 1)
                    && (operators[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(operators[c][i], r, cur_st) && (operators[c][i].rule.second[0].size() == 3) && (operators[c][i].flag != true)) {
                        operators[c][i].flag = true;
                        return operators[c][i];
                    }
                }
            }
            for (int i = operators[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации;
                if ((((operators[c][i].cur_point == size_vect(operators[c][i].rule.second[0]) - 1)
                    && (operators[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(operators[c][i], r, cur_st) && (operators[c][i].flag != true) && (operators[c][i].rule.second[0].size() == 2)) {
                        operators[c][i].flag = true;
                        return operators[c][i];
                    }
                }
            }

        }
        else if (cur_st.rule.second[0][r] == "DescrList") {
            for (int i = descrList[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации;
                if ((((descrList[c][i].cur_point == size_vect(descrList[c][i].rule.second[0]) - 1)
                    && (descrList[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(descrList[c][i], r, cur_st) && (descrList[c][i].rule.second[0].size() == 3) && (descrList[c][i].flag != true)) {
                        descrList[c][i].flag = true;
                        return descrList[c][i];
                    }
                }
            }
            for (int i = descrList[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации;
                if ((((descrList[c][i].cur_point == size_vect(descrList[c][i].rule.second[0]) - 1)
                    && (descrList[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(descrList[c][i], r, cur_st) && (descrList[c][i].flag != true) && (descrList[c][i].rule.second[0].size() == 2)) {
                        descrList[c][i].flag = true;
                        return descrList[c][i];
                    }
                }
            }

        }else  if (cur_st.rule.second[0][r] == "VarList") {

            for (int i = varList[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации;
                if ((((varList[c][i].cur_point == size_vect(varList[c][i].rule.second[0]) - 1)
                    && (varList[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(varList[c][i], r, cur_st)&&(varList[c][i].rule.second[0].size() == 4) && (varList[c][i].flag != true)) {
                        varList[c][i].flag = true;
                        return varList[c][i];
                    }
                }
            }
            for (int i = varList[c].size() - 1; i >= 0; i--) {
                //смотрим только на разобранные ситуации;
                if ((((varList[c][i].cur_point == size_vect(varList[c][i].rule.second[0]) - 1)
                    && (varList[c][i].rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(varList[c][i], r, cur_st) && (varList[c][i].flag != true) && (varList[c][i].rule.second[0].size() == 2)) {
                        varList[c][i].flag = true;
                        return varList[c][i];
                    }
                }
            }
        }
        else {
            for (Situation& st : States[c]) {
                //смотрим только на разобранные ситуации;
                if ((((st.cur_point == size_vect(st.rule.second[0]) - 1)
                    && (st.rule.first == cur_st.rule.second[0][r])))) {
                    if (check_situation(st, r, cur_st))

                        return st;
                }
            }
        }
        Situation s;
        vector<string> right;
        s.rule.first = "-";
        right.push_back("-");
        right.push_back("Digit");
        right.push_back("Const");
        s.cur_point = r;
        s.parent_point = c;
        s.rule.second.push_back(right);
        return s;
    }


    void right_parse(Situation cur_st, int j) {
        // 1). Вместо добавления номера правила в грамматике; 
        rules.push_back(cur_st.rule);
        // k; 
        int point_in_rule = cur_st.cur_point;

        //точка в строке, которую мы распознаем; c; 
        int point_in_string = j;
     
        while ((point_in_rule != 0)) {
            if (is_terminal(cur_st.rule.second[0][point_in_rule]) == 1) {
                //Терминал;
                point_in_rule--;
                point_in_string--;
            }
            else {
                Situation s;
                s = nessasery_situation(point_in_string, point_in_rule, cur_st);
                if ((s.rule.first != "-")) {
                    right_parse(s, point_in_string);
                }
               
                    point_in_rule--;
                
                //paren_point до этого обозначал номер состояния, в котором находится ситуация,
                //а это и есть номер буквы в распозноваемой строке;
                point_in_string = s.parent_point;
            }
        }
    }


    void print_parse() {
        ofstream fout;
        fout.open("right_parse.txt");
        
        for (int i = 0; i < rules.size(); i++) {
            fout << rules[i].first << "    -> " << "    ";
            for (int j = 0; j < rules[i].second[0].size(); j++) {
                fout << rules[i].second[0][j] << "  ";
            }
            fout << "\n";
        }

        fout.close(); 
    }
};