#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <cmath>
#include <random>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<pair<int,int>> vp;
typedef pair<int,int> pi;
typedef priority_queue<int> maxHeap;
typedef priority_queue<int, vector<int>, greater<int>> minHeap;

int calculateNextEvenSquareSize(int minSize){
    int raiz = static_cast<int>(ceil(sqrt(minSize)));
    return (raiz % 2 != 0? raiz + 1 : raiz); 
}

char randomChar(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 25);
    return 'A' + dis(gen);
}

string padTextToMultipleOfFour(string text){
    string padding = "";
    for(int i = text.size(); i % 4 != 0; i++){
        padding += randomChar();
    }
    return text + padding;
}


vector<int> selectRandomPositionsWithReservoir(int totalPositions, int selectCount) {
    vector<int> resultado;
    random_device rd;
    mt19937 gen(rd());
    
    for(int i = 0; i < totalPositions; i++) {
        if(resultado.size() < selectCount) {
            resultado.push_back(i);
        } else {
            uniform_int_distribution<int> dis(1, i);
            int j = dis(gen);
            if(j <= selectCount) {
                resultado[j-1] = i;
            }
        }
    }
    return resultado;
}

vector<int> assignRandomQuadrants(int positionCount){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 3);
    vector<int> cuadrantes(positionCount);
    for(int i = 0; i < positionCount; i ++){
        cuadrantes[i] = dis(gen); 
    }
    return cuadrantes;
}

vector<pair<int,int>> convertToGrilleCoordinates(const vector<int>& positions, const vector<int>& quadrants, int gridSize){
    vector<pair<int,int>> rejilla;
    for(int i = 0; i < quadrants.size(); i++){
        int fila = positions[i] / (gridSize / 2);
        int col = positions[i] % (gridSize / 2); 

        if(quadrants[i] == 0){
            rejilla.push_back({fila, col});
        } 
        else if(quadrants[i] == 1){
            rejilla.push_back({col, gridSize - fila - 1});
        } 
        else if(quadrants[i] == 2){
            rejilla.push_back({gridSize - col - 1, fila});
        }
        else if(quadrants[i] == 3){
            rejilla.push_back({gridSize - fila - 1, gridSize - col - 1});
        }
    }
    return rejilla;
}

vector<pair<int,int>> generateCardanGrillePattern(int gridSize, int holeCount){
    int sectorsPerQuadrant = pow((gridSize / 2), 2);
    vector<int> selectedPositions = selectRandomPositionsWithReservoir(sectorsPerQuadrant, holeCount);
    vector<int> quadrants = assignRandomQuadrants(selectedPositions.size());
    return convertToGrilleCoordinates(selectedPositions, quadrants, gridSize);
}

void rotateGrille90Degrees(vector<pair<int,int>>& grille, int gridSize){
    for(auto &pos : grille){
        int fila = pos.first;
        int col = pos.second;
        pos.first = col;
        pos.second = gridSize - fila - 1;
    }
}

void fillMatrixWithRotatedGrille(vector<vector<char>>& matrix, const string& text, const vector<pair<int,int>>& grille, int& textIndex){
    vector<pair<int,int>> sortedGrille = grille;
    sort(sortedGrille.begin(), sortedGrille.end());
    for(const auto& pos : sortedGrille){
        if(textIndex < text.size()){
            matrix[pos.first][pos.second] = text[textIndex++];
        }
    }
}

vector<vector<char>> encryptWithCardanGrille(string text, vector<pair<int,int>> grille, int gridSize){
    vector<vector<char>> matriz(gridSize, vector<char>(gridSize, '*'));
    int index = 0;
    for(int rotacion = 0; rotacion < 4; rotacion++){
        fillMatrixWithRotatedGrille(matriz, text, grille, index);
        rotateGrille90Degrees(grille, gridSize);
    }
    return matriz;
}


void fillRemainingWithRandomLetters(vector<vector<char>>& matrix) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 25);

    for(int i = 0; i < matrix.size(); i++) {
        for(int j = 0; j < matrix[i].size(); j++) {
            if(matrix[i][j] == '*') {
                char randomChar = 'A' + dis(gen);
                matrix[i][j] = randomChar;
            }
        }
    }
}

void concatenar(fstream &in, string &word){
    string linea;
    while(getline(in, linea)){
        word += linea;
    }
}

vector<pair<int,int>> readGrilleFromFile(const string& filename, int& sizeOriginal, int& gridSize) {
    ifstream in(filename);
    in >> sizeOriginal >> gridSize;
    
    vector<pair<int,int>> grille;
    string line;
    getline(in, line);
    
    for(int i = 0; i < gridSize; i++) {
        getline(in, line);
        for(int j = 0; j < line.size(); j++) {
            if(line[j] == '0') {
                grille.push_back({i, j});
            }
        }
    }
    
    in.close();
    return grille;
}

vector<vector<char>> readEncryptedMatrix(const string& filename, int gridSize) {
    ifstream in(filename);
    int sizeOriginal, gridSizeFromFile;
    in >> sizeOriginal >> gridSizeFromFile;
    
    string line;
    getline(in, line);
    
    for(int i = 0; i < gridSize; i++) {
        getline(in, line);
    }
    
    vector<vector<char>> matrix(gridSize, vector<char>(gridSize));
    for(int i = 0; i < gridSize; i++) {
        getline(in, line);
        for(int j = 0; j < line.size() && j < gridSize; j++) {
            matrix[i][j] = line[j];
        }
    }
    
    in.close();
    return matrix;
}

string decryptWithCardanGrille(const vector<vector<char>>& matrix, vector<pair<int,int>> grille, int gridSize) {
    string decryptedText = "";
    
    for(int rotacion = 0; rotacion < 4; rotacion++) {
        vector<pair<int,int>> sortedGrille = grille;
        sort(sortedGrille.begin(), sortedGrille.end());
        
        for(const auto& pos : sortedGrille) {
            decryptedText += matrix[pos.first][pos.second];
        }
        
        rotateGrille90Degrees(grille, gridSize);
    }
    
    return decryptedText;
}


int main() {
    srand(time(0));
    
    string word; 
    fstream in("MENSAJE.txt");
    concatenar(in, word);
    int sizeOriginal = word.size();
    word = padTextToMultipleOfFour(word);
    int sizeRejilla = calculateNextEvenSquareSize(word.size());
    int holes = word.size() / 4; 
    vector<pair<int,int>> rejilla = generateCardanGrillePattern(sizeRejilla, holes);
    ofstream out("MENSAJE-CIFRADO.txt"); 
    out << sizeOriginal << '\n';
    out << sizeRejilla << '\n';
    for(int i = 0; i < sizeRejilla; i++){
        for(int j = 0; j < sizeRejilla; j++){
            for(int k = 0; k < rejilla.size(); k++){
                if(rejilla[k].first == i && rejilla[k].second == j){
                    out << "0";
                    break;
                }
                if(k == rejilla.size() - 1){
                    out << "#";
                }
            }
        }
        out << '\n';
    } 
    
    vector<vector<char>> matriz = encryptWithCardanGrille(word, rejilla, sizeRejilla);
    fillRemainingWithRandomLetters(matriz);
    for(int i = 0; i < matriz.size(); i++){
        for(int j = 0; j < matriz[i].size(); j++){
            out << matriz[i][j];
        }
        out << '\n';
    }
    out.close();
    
    // PROCESO DE DESENCRIPTACIÓN
    int originalSize, gridSize;
    vector<pair<int,int>> grilleFromFile = readGrilleFromFile("MENSAJE-CIFRADO.txt", originalSize, gridSize);
    vector<vector<char>> encryptedMatrix = readEncryptedMatrix("MENSAJE-CIFRADO.txt", gridSize);
    string decryptedMessage = decryptWithCardanGrille(encryptedMatrix, grilleFromFile, gridSize);
    decryptedMessage = decryptedMessage.substr(0, originalSize);
    ofstream outDecrypted("MENSAJE_DESCIFRADO.txt");
    outDecrypted << decryptedMessage;
    outDecrypted.close();

    return 0;
}