#include "game.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

Game::Game() : p1("Jogador 1", 'X', false), p2("Jogador 2", 'O', false), p1Turn(true), histSize(0) {
    srand((unsigned)time(nullptr));
}

void Game::start() {
    mainMenu();
}

void Game::mainMenu() {
    while (true) {
        cout << "\n Jogo da Trilha irru tei tei \n";
        cout << "1. Novo jogo 2 jogadores\n";
        cout << "2. Novo jogo vs IA\n";
        cout << "3. Carregar jogo\n";
        cout << "4. Sair\n";
        cout << "Escolha: ";
        int op;
        if (!(cin >> op)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        cin.ignore(10000,'\n');
        if (op == 1) {
            board.reset();
            histSize = 0;
            p1 = Player("Jogador 1", 'X', false);
            p2 = Player("Jogador 2", 'O', false);
            cout << "Digite nome do Jogador 1: "; string n1; getline(cin, n1);
            if (!n1.empty()) p1.setName(n1);
            cout << "Digite nome do Jogador 2: "; string n2; getline(cin, n2);
            if (!n2.empty()) p2.setName(n2);
            p1Turn = true;
            playLoop();
        } else if (op == 2) {
            board.reset();
            histSize = 0;
            p1 = Player("Voce", 'X', false);
            p2 = Player("CPU", 'O', true);
            cout << "Digite seu nome: "; string n; getline(cin, n);
            if (!n.empty()) p1.setName(n);
            p1Turn = true;
            playLoop();
        } else if (op == 3) {
            cout << "Nome do arquivo para carregar: ";
            string fname; getline(cin, fname);
            if (loadFromFile(fname.c_str())) {
                cout << "Jogo carregado com sucesso.\n";
                playLoop();
            } else {
                cout << "Falha ao carregar.\n";
            }
        } else if (op == 4) {
            cout << "Adios\n";
            break;
        }
    }
}

void Game::playLoop() {
    while (true) {
        printHeader();
        board.print();
        if (board.checkWin(p1.getSymbol())) {
            cout << p1.getName() << " (" << p1.getSymbol() << ") venceu!\n";
            break;
        }
        if (board.checkWin(p2.getSymbol())) {
            cout << p2.getName() << " (" << p2.getSymbol() << ") venceu!\n";
            break;
        }
        if (board.isFull()) {
            cout << "Empate!\n";
            break;
        }

        if (p1Turn) {
            playerTurn(p1);
        } else {
            if (p2.isCPUPlayer()) cpuTurn(p2);
            else playerTurn(p2);
        }
        p1Turn = !p1Turn;
    }

    cout << "Deseja salvar o estado do jogo? (s/n): ";
    char c; cin >> c; cin.ignore(10000,'\n');
    if (c == 's' || c == 'S') {
        cout << "Nome do arquivo: ";
        string fname; getline(cin, fname);
        saveToFile(fname.c_str());
        cout << "Jogo salvo em '" << fname << "'\n";
    }
}

void Game::playerTurn(const Player &p) {
    while (true) {
        cout << p.getName() << " (" << p.getSymbol() << ") - escolha:\n";
        cout << "1. Jogar  2. Desfazer ultima jogada  3. Sair para menu\n";
        cout << "Opcao: ";
        int op; if (!(cin >> op)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        if (op == 1) {
            int r, c;
            cout << "Digite linha e coluna (ex: 0 2): ";
            if (!(cin >> r >> c)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Entrada invalida.\n"; continue; }
            if (board.setCell(r, c, p.getSymbol())) {
                pushHistory(r, c, p.getSymbol());
                break;
            } else {
                cout << "Celula invalida ou ocupada. Tente novamente.\n";
            }
        } else if (op == 2) {
            if (undoLast()) {
                cout << "Ultima jogada desfeita.\n";
                
                p1Turn = !p1Turn;
                break;
            } else {
                cout << "Nada para desfazer.\n";
            }
        } else if (op == 3) {
            cout << "Voltando ao menu principal.\n";
           
            throw 0;
        } else {
            cout << "Opcao invalida.\n";
        }
    }
}

void Game::cpuTurn(const Player &p) {
    cout << p.getName() << " (" << p.getSymbol() << ") esta pensando...\n";

    int bestR = -1, bestC = -1;
    int bestScore = -10000;

    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (board.getCell(r, c) == ' ') {
                board.setCell(r, c, p.getSymbol());
                int score = minimax(board, (p.getSymbol() == 'X' ? 'O' : 'X'), p.getSymbol());
                board.setCell(r, c, ' ');
                if (score > bestScore) {
                    bestScore = score;
                    bestR = r;
                    bestC = c;
                }
            }
        }
    }

    if (bestR != -1) {
        board.setCell(bestR, bestC, p.getSymbol());
        pushHistory(bestR, bestC, p.getSymbol());
    } else {
       
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                if (board.getCell(r,c) == ' ') { board.setCell(r,c,p.getSymbol()); pushHistory(r,c,p.getSymbol()); return;}
    }
}

int Game::minimax(Board &b, char playerSymbol, char maximizingSymbol) {
    
    char opponent = (maximizingSymbol == 'X') ? 'O' : 'X';
    if (b.checkWin(maximizingSymbol)) return 10;
    if (b.checkWin(opponent)) return -10;
    if (b.isFull()) return 0;

    bool isMaximizing = (playerSymbol == maximizingSymbol);
    int bestVal = isMaximizing ? -1000 : 1000;

    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (b.getCell(r,c) == ' ') {
                b.setCell(r,c, playerSymbol);
                int val = minimax(b, (playerSymbol == 'X' ? 'O' : 'X'), maximizingSymbol);
                b.setCell(r,c, ' ');
                if (isMaximizing) {
                    if (val > bestVal) bestVal = val;
                } else {
                    if (val < bestVal) bestVal = val;
                }
            }
        }
    }
    return bestVal;
}

void Game::pushHistory(int r, int c, char s) {
    if (histSize >= 9) return;
    hist_r[histSize] = r;
    hist_c[histSize] = c;
    hist_sym[histSize] = s;
    histSize++;
}

bool Game::undoLast() {
    if (histSize == 0) return false;
    histSize--;
    int r = hist_r[histSize];
    int c = hist_c[histSize];

    board.setCell(r, c, ' '); 
    board.reset();
    for (int i = 0; i < histSize; ++i) {
        board.setCell(hist_r[i], hist_c[i], hist_sym[i]);
    }
    return true;
}

void Game::saveToFile(const char *filename) const {
    ofstream ofs(filename);
    if (!ofs) {
        cout << "Erro ao abrir arquivo para salvar, tente novamente!\n";
        return;
    }
    ofs << board.serialize() << "\n";
    ofs << p1.getName() << "\n" << p1.getSymbol() << "\n" << p1.isCPUPlayer() << "\n";
    ofs << p2.getName() << "\n" << p2.getSymbol() << "\n" << p2.isCPUPlayer() << "\n";
    ofs << (p1Turn ? 1 : 0) << "\n";
    ofs << histSize << "\n";
    for (int i = 0; i < histSize; ++i) {
        ofs << hist_r[i] << " " << hist_c[i] << " " << hist_sym[i] << "\n";
    }
    ofs.close();
}

bool Game::loadFromFile(const char *filename) {
    ifstream ifs(filename);
    if (!ifs) return false;
    string boardData;
    if (!getline(ifs, boardData)) return false;
    if (!board.deserialize(boardData)) return false;
    string name;
    int cpuFlag;
    char sym;
    
    if (!getline(ifs, name)) return false;
    getline(ifs, boardData); 
    if (boardData.size() < 1) return false;
    sym = boardData[0];
    ifs >> cpuFlag;
    ifs.ignore(10000,'\n');
    p1 = Player(name, sym, cpuFlag != 0);

    if (!getline(ifs, name)) return false;
    getline(ifs, boardData);
    if (boardData.size() < 1) return false;
    sym = boardData[0];
    ifs >> cpuFlag;
    ifs.ignore(10000,'\n');
    p2 = Player(name, sym, cpuFlag != 0);

    int t;
    ifs >> t; p1Turn = (t != 0);
    ifs >> histSize;
    for (int i = 0; i < histSize; ++i) {
        ifs >> hist_r[i] >> hist_c[i] >> hist_sym[i];
    }
    
    Board btmp;
    for (int i = 0; i < histSize; ++i) btmp.setCell(hist_r[i], hist_c[i], hist_sym[i]);
    board = btmp;
    ifs.close();
    return true;
}

void Game::printHeader() const {
    cout << "\n Estado do game \n";
    cout << p1.getName() << " (X)  vs  " << p2.getName() << " (O)\n";
}